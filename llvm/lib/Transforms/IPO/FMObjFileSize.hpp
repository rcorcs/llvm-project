
static void copyComdat(GlobalObject *Dst, const GlobalObject *Src) {
  const Comdat *SC = Src->getComdat();
  if (!SC)
    return;
  Comdat *DC = Dst->getParent()->getOrInsertComdat(SC->getName());
  DC->setSelectionKind(SC->getSelectionKind());
  Dst->setComdat(DC);
}


void CollectGlobalsInConst(const Constant *C, std::set<const GlobalObject*> &Globals, std::set<const Value*> NeededValues, std::set<const Value*> &Visited) {
  if (Visited.find(C)!=Visited.end()) return;
  Visited.insert(C);
  NeededValues.insert(C);

  if (auto *GO = dyn_cast<GlobalObject>(C)) {
    //errs() << "Found: ";
    //GO->dump();
    Globals.insert(GO);
  } //else C->dump();

  for (unsigned i = 0; i<C->getNumOperands(); i++) {
    if (auto *COp = dyn_cast<Constant>(C->getOperand(i))) CollectGlobalsInConst(COp, Globals, NeededValues, Visited);
  }

  if (auto *CStruct = dyn_cast<ConstantStruct>(C)) {
    StructType *STy = CStruct->getType();
    for (unsigned i = 0; i<STy->getNumElements(); i++) {
      CollectGlobalsInConst(C->getAggregateElement(i), Globals, NeededValues, Visited);
    }
  }
}

void CloneUsedGlobalsAcrossModule(const Function *F, const Module *M, Module *NewM, ValueToValueMapTy &VMap) {
  std::set<const Value*> NeededValues;

  std::set<const Value*> NewMappedValue;
  std::set<const GlobalObject*> Globals;

  NeededValues.insert(F);  
  for (const Instruction &I : instructions(F)) {
    for (unsigned i = 0; i<I.getNumOperands(); i++) {
      NeededValues.insert(I.getOperand(i));
      if (auto *GO = dyn_cast<GlobalObject>(I.getOperand(i))) {
	Globals.insert(GO);
        if (auto *C = dyn_cast<Constant>(GO)) {
          std::set<const Value*> Visited;
          CollectGlobalsInConst(C, Globals, NeededValues, Visited);
        }
	if (auto *GV = dyn_cast<GlobalVariable>(GO)) {
          if (auto *CInit = GV->getInitializer()) {
            std::set<const Value*> Visited;
            CollectGlobalsInConst(CInit, Globals, NeededValues, Visited);
          }
	}
      }
    }
    NeededValues.insert(&I);
  }

  bool AddedValues = true;
  while (AddedValues) {
    AddedValues = false;
    for (Module::const_global_iterator I = M->global_begin(), E = M->global_end();
       I != E; ++I) {

      bool FoundUse = false;
      for (const User *U : I->users()) {
        if (NeededValues.find(U)!=NeededValues.end() && NeededValues.find(&*I)==NeededValues.end()) { FoundUse = true; break; }
      }
      if (!FoundUse) continue;
      NeededValues.insert(&*I);
      Globals.insert(&*I);
      AddedValues = true;

      if (auto *C = dyn_cast<Constant>(&*I)) {
        std::set<const Value*> Visited;
        CollectGlobalsInConst(C, Globals, NeededValues, Visited);
      }
      if (auto *CInit = I->getInitializer()) {
        std::set<const Value*> Visited;
        CollectGlobalsInConst(CInit, Globals, NeededValues, Visited);
      }
    }

    // Loop over the functions in the module, making external functions as before
    for (const Function &F : *M) {
      if (VMap.find(&F)!=VMap.end()) continue;
      //if (NeededValues.find(&I)==NeededValues.end() || (&I)==F) continue;
      bool FoundUse = false;
      for (const User *U : F.users()) {
        if (NeededValues.find(U)!=NeededValues.end() && NeededValues.find(&F)==NeededValues.end()) { FoundUse = true; break; }
      }
      if (!FoundUse) continue;

      NeededValues.insert(&F);
      Globals.insert(&F);
      AddedValues = true;
    }
  }

  /*
  // Loop over all of the global variables, making corresponding globals in the
  // new module.  Here we add them to the VMap and to the new Module.  We
  // don't worry about attributes or initializers, they will come later.
  //
  for (Module::const_global_iterator I = M->global_begin(), E = M->global_end();
       I != E; ++I) {


    if (VMap.find(&*I)!=VMap.end()) continue;

    //if (UsedValues.find(&*I)==UsedValues.end()) continue;
    bool FoundUse = false;
    for (const User *U : I->users()) {
      if (NeededValues.find(U)!=NeededValues.end()) { FoundUse = true; break; }
    }
    if (!FoundUse) continue;
  */
  for (auto *GO : Globals) {
    if (auto *GV = dyn_cast<GlobalVariable>(GO)) {
      GlobalVariable *NewGV = new GlobalVariable(*NewM,
                                              GV->getValueType(),
                                              GV->isConstant(), GV->getLinkage(),
                                              (Constant*) nullptr, GV->getName(),
                                              (GlobalVariable*) nullptr,
                                              GV->getThreadLocalMode(),
                                              GV->getType()->getAddressSpace());
      NewGV->copyAttributesFrom(GV);
      VMap[GV] = NewGV;
      NewMappedValue.insert(GV);
    } else if (auto *F = dyn_cast<Function>(GO)) {
      Function *NF =
          Function::Create(cast<FunctionType>(F->getValueType()), F->getLinkage(),
                           //I.getAddressSpace(),
                           F->getName(), NewM);
      NF->copyAttributesFrom(F);
      NF->setLinkage(GlobalValue::LinkageTypes::ExternalLinkage);
      NF->setPersonalityFn(nullptr);
      VMap[F] = NF;
      NewMappedValue.insert(F);
    }
  }

  // Loop over the aliases in the module
  for (Module::const_alias_iterator I = M->alias_begin(), E = M->alias_end();
       I != E; ++I) {

    if (VMap.find(&*I)!=VMap.end()) continue;
    //if (NeededValues.find(&*I)==NeededValues.end()) continue;
    bool FoundUse = false;
    for (const User *U : I->users()) {
      if (NeededValues.find(U)!=NeededValues.end()) { FoundUse = true; break; }
    }
    if (!FoundUse) continue;

    auto *GA = GlobalAlias::create(I->getValueType(),
                                   I->getType()->getPointerAddressSpace(),
                                   I->getLinkage(), I->getName(), NewM);
    GA->copyAttributesFrom(&*I);
    VMap[&*I] = GA;
    NewMappedValue.insert(&*I);
  }

  // Now that all of the things that global variable initializer can refer to
  // have been created, loop through and copy the global variable referrers
  // over...  We also set the attributes on the global now.
  //
  for (Module::const_global_iterator I = M->global_begin(), E = M->global_end();
       I != E; ++I) {

    if (I->isDeclaration())
      continue;

    //if (VMap[&*I]==nullptr) continue;
    if (NewMappedValue.find(&*I)==NewMappedValue.end()) continue;

    GlobalVariable *GV = dyn_cast<GlobalVariable>(VMap[&*I]);


    if (I->hasInitializer())
      GV->setInitializer(MapValue(I->getInitializer(), VMap));

    SmallVector<std::pair<unsigned, MDNode *>, 1> MDs;
    I->getAllMetadata(MDs);
    for (auto MD : MDs)
      GV->addMetadata(MD.first,
                      *MapMetadata(MD.second, VMap, RF_MoveDistinctMDs));

    copyComdat(GV, &*I);
  }
}


void CloneFunctionAcrossModule(const Function *F, Module *M, ValueToValueMapTy &VMap, bool FunctionOnly=false) {

  if (!FunctionOnly) CloneUsedGlobalsAcrossModule(F,F->getParent(),M,VMap);

  Function *NewF = Function::Create(cast<FunctionType>(F->getValueType()), GlobalValue::LinkageTypes::ExternalLinkage, //F->getLinkage(),
                         //F->getAddressSpace(),
                         F->getName(), M);
  NewF->copyAttributesFrom(F);
  VMap[F] = NewF;

  Function::arg_iterator DestArg = NewF->arg_begin();
  for (Function::const_arg_iterator Arg = F->arg_begin(); Arg != F->arg_end();
         ++Arg) {
    DestArg->setName(Arg->getName());
    VMap[&*Arg] = &*DestArg++;
  }

  SmallVector<ReturnInst *, 8> Returns; // Ignore returns cloned.
  CloneFunctionInto(NewF, F, VMap, /*ModuleLevelChanges=*/true, Returns);

  NewF->setPersonalityFn(nullptr);
  //if (F->hasPersonalityFn())
  //  NewF->setPersonalityFn(MapValue(F->getPersonalityFn(), VMap));

  copyComdat(NewF, F);

  NewF->setUnnamedAddr( GlobalValue::UnnamedAddr::Local );
  NewF->setVisibility( GlobalValue::VisibilityTypes::DefaultVisibility );
  NewF->setLinkage(GlobalValue::LinkageTypes::ExternalLinkage);
  NewF->setDSOLocal(true);
}

void ExtractFunctionIntoFile(Module &M, std::string FName, std::string FilePath) {
  Function *F = M.getFunction(FName);
  if (F) {
      ValueToValueMapTy VMap;

      std::unique_ptr<Module> NewM =
      std::make_unique<Module>(M.getModuleIdentifier(), M.getContext());
      //std::unique_ptr<Module> NewM =
      //std::make_unique<Module>(M.getModuleIdentifier(), M.getContext());

      CloneFunctionAcrossModule(F,&*NewM,VMap);

      std::error_code EC;
      llvm::raw_fd_ostream OS(FilePath, EC, llvm::sys::fs::F_None);
      WriteBitcodeToFile(*NewM, OS);
      OS.flush();
  }
}

std::unique_ptr<Module> ExtractMultipleFunctionsIntoNewModule(std::vector<Function*> &Fs, Module &M) {
  std::unique_ptr<Module> NewM =
      std::make_unique<Module>(M.getModuleIdentifier(), M.getContext());
  //std::unique_ptr<Module> NewM = std::make_unique<Module>(M.getModuleIdentifier(), M.getContext());

  unsigned Count = 0;

  ValueToValueMapTy VMap;
  for (Function *F : Fs) {
    CloneFunctionAcrossModule(F,&*NewM,VMap);
    VMap[F]->setName( std::string("f")+std::to_string(Count++) );
    //errs() << "------------------------------------------------------------------\n";
    //NewM->dump();
    //errs() << "------------------------------------------------------------------\n";
  }

  return std::move(NewM);
}

static Optional<size_t> filesize(std::string filename)
{
    std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
    //std::ifstream::pos_type
    if (in.good())
      return Optional<size_t>(in.tellg());
    else
      return Optional<size_t>();
}

Optional<size_t> MeasureSize(std::vector<Function*> &Fs, Module &M, bool Timeout=true) {
  std::unique_ptr<Module> NewM = ExtractMultipleFunctionsIntoNewModule(Fs,M);
  //NewM->dump();
 
  std::string FilePathLL = TPrefix + std::string("extracted.ll");
  std::string FilePathObj = TPrefix + std::string("extracted.o");
  std::error_code EC;
  llvm::raw_fd_ostream OS(FilePathLL, EC, llvm::sys::fs::F_None);
  //WriteBitcodeToFile(*NewM, OS);
  //NewM->print(OS,false);
  OS << *NewM;
  OS.flush();

  std::remove(FilePathObj.c_str());

  std::string ClangPath = TestFM_ClangPATH;
  std::string Cmd = (Timeout?std::string("timeout -s KILL 5m "):std::string("")) +
	            ClangPath+std::string("  -x ir  ") + FilePathLL + std::string("  -Os -c -o  ") + FilePathObj;
  bool CompilationOK = !std::system(Cmd.c_str());

  std::ifstream builtObj(FilePathObj.c_str());
  if (CompilationOK && builtObj.good()) {
    std::string SizeFileTXT = TPrefix + std::string("extracted-size.txt");
    std::remove(SizeFileTXT.c_str());
    std::string CmdSize = std::string("size -d -A  ") + FilePathObj + std::string("  | grep text >  ") + SizeFileTXT;
    bool BadMeasurement = std::system(CmdSize.c_str());
    std::ifstream ifs(SizeFileTXT.c_str());
    if (BadMeasurement || ifs.bad())
      return Optional<size_t>();

    std::string Str;
    ifs >> Str;
    ifs >> Str;
    size_t Size = std::stoul(Str,nullptr,0);
    
    ifs.close();
    builtObj.close();
    return Optional<size_t>(Size);
  } else return Optional<size_t>();
}

Optional<size_t> MeasureSize(Module &M, std::string Name, bool Timeout=true) {
  std::string FilePathLL = TPrefix + std::string("extracted-") + Name + std::string("-") + std::to_string(TestCount) + std::string(".ll");
  std::string FilePathObj = TPrefix + std::string("extracted-") + Name + std::string("-") + std::to_string(TestCount) + std::string(".o");
  std::error_code EC;
  llvm::raw_fd_ostream OS(FilePathLL, EC, llvm::sys::fs::F_None);
  //WriteBitcodeToFile(*NewM, OS);
  //NewM->print(OS,false);
  OS << M;
  OS.flush();

  std::remove(FilePathObj.c_str());

  std::string ClangPath = TestFM_ClangPATH;
  std::string Cmd = (Timeout?std::string("timeout -s KILL 5m "):std::string("")) +
	            ClangPath+std::string("  -x ir  ") + FilePathLL + std::string("  -Os -c -o  ") + FilePathObj;
  bool CompilationOK = !std::system(Cmd.c_str());

  std::ifstream builtObj(FilePathObj.c_str());
  if (CompilationOK && builtObj.good()) {
    auto Size = filesize(FilePathObj);
    builtObj.close();
    return Size;
  } else return Optional<size_t>();

}

//Optional<size_t> SizeF1F2Opt = MeasureOriginalSize(M,Result,AlwaysPreserved,Options);
//Optional<size_t> SizeF12Opt = MeasureMergedSize(M,Result,AlwaysPreserved,Options);

bool canReplaceCallsWith(Function *F, FunctionMergeResult &MFR, const FunctionMergingOptions &Options) {

  Value *FuncId = MFR.getFunctionIdValue(F);
  Function *MergedF = MFR.getMergedFunction();

  unsigned CountUsers = 0;
  std::vector<CallBase *> Calls;
  for (User *U : F->users()) {
    CountUsers++;
    if (CallInst *CI = dyn_cast<CallInst>(U)) {
      if (CI->getCalledFunction() == F) {
        Calls.push_back(CI);
      }
    } else if (InvokeInst *II = dyn_cast<InvokeInst>(U)) {
      if (II->getCalledFunction() == F) {
        if (EnableSALSSA)
          Calls.push_back(II);
      }
    }
  }

  if (Calls.size()<CountUsers)
    return false;
  return true;

}

/*
Optional<size_t> MeasureOriginalSize(Module &M, FunctionMergeResult &Result, StringSet<> &AlwaysPreserved, const FunctionMergingOptions &Options={}, bool Timeout=true) {
  Function *F1 = Result.getFunctions().first;
  Function *F2 = Result.getFunctions().second;

  std::set<Function*> Fs;
  Fs.insert(F1);
  Fs.insert(F2);
  //Fs.insert(Result.getMergedFunction());

  for (User *U : F1->users()) {
    if (Instruction *I = dyn_cast<Instruction>(U)) {
      Fs.insert(dyn_cast<Function>(I->getParent()->getParent()));
    }
  }
  for (User *U : F2->users()) {
    if (Instruction *I = dyn_cast<Instruction>(U)) {
      Fs.insert(dyn_cast<Function>(I->getParent()->getParent()));
    }
  }

  //std::unique_ptr<Module> NewM = ExtractMultipleFunctionsIntoNewModule(Fs,M);

  //ValueToValueMapTy VMap;
  //std::unique_ptr<Module> NewM = CloneModule(M, VMap);

  std::unique_ptr<Module> NewM =
      std::make_unique<Module>(M.getModuleIdentifier(), M.getContext());

  int Count = 0;
  ValueToValueMapTy VMap;
  for (Function *F : Fs) {
    CloneFunctionAcrossModule(F,&*NewM,VMap);
    VMap[F]->setName( std::string("f")+std::to_string(Count++) );
  }

  return MeasureSize(*NewM,Timeout);
}

Optional<size_t> MeasureMergedSize(Module &M, FunctionMergeResult &Result, StringSet<> &AlwaysPreserved, const FunctionMergingOptions &Options={}, bool Timeout=true) {
  Function *F1 = Result.getFunctions().first;
  Function *F2 = Result.getFunctions().second;

  std::set<Function*> Fs;
  Fs.insert(F1);
  Fs.insert(F2);
  Fs.insert(Result.getMergedFunction());

  for (User *U : F1->users()) {
    if (Instruction *I = dyn_cast<Instruction>(U)) {
      Fs.insert(dyn_cast<Function>(I->getParent()->getParent()));
    }
  }
  for (User *U : F2->users()) {
    if (Instruction *I = dyn_cast<Instruction>(U)) {
      Fs.insert(dyn_cast<Function>(I->getParent()->getParent()));
    }
  }

  //std::unique_ptr<Module> NewM = ExtractMultipleFunctionsIntoNewModule(Fs,M);

  //ValueToValueMapTy VMap;
  //std::unique_ptr<Module> NewM = CloneModule(M, VMap);

  std::unique_ptr<Module> NewM =
      std::make_unique<Module>(M.getModuleIdentifier(), M.getContext());

  int Count = 0;
  ValueToValueMapTy VMap;
  for (Function *F : Fs) {
    CloneFunctionAcrossModule(F,&*NewM,VMap);
    VMap[F]->setName( std::string("f")+std::to_string(Count++) );
  }

  //return std::move(NewM);

  Function *NewF1 = dyn_cast<Function>(VMap[F1]);
  Function *NewF2 = dyn_cast<Function>(VMap[F2]);
  Function *NewMF = dyn_cast<Function>(VMap[Result.getMergedFunction()]);
  FunctionMergeResult NewResult(NewF1, NewF2, NewMF, Result.needUnifiedReturn());
  NewResult.setArgumentMapping(NewF1, Result.getArgumentMapping(F1));
  NewResult.setArgumentMapping(NewF2, Result.getArgumentMapping(F2));
  NewResult.setFunctionIdArgument(Result.hasFunctionIdArgument());

  //apply NewResults
  FunctionMerger Merger(NewM.get());
  Merger.updateCallGraph(NewResult, AlwaysPreserved, Options);

  return MeasureSize(*NewM,Timeout);
}
*/





bool MeasureMergedSize(unsigned &SizeF1F2, unsigned &SizeF12, Module &M, FunctionMergeResult &Result, StringSet<> &AlwaysPreserved, const FunctionMergingOptions &Options={}, bool Timeout=true) {

  Function *F1 = Result.getFunctions().first;
  Function *F2 = Result.getFunctions().second;

  std::set<const Function*> Fs;
  Fs.insert(F1);
  Fs.insert(F2);

  for (User *U : F1->users()) {
    if (Instruction *I = dyn_cast<Instruction>(U)) {
      Fs.insert(dyn_cast<Function>(I->getParent()->getParent()));
    }
  }
  for (User *U : F2->users()) {
    if (Instruction *I = dyn_cast<Instruction>(U)) {
      Fs.insert(dyn_cast<Function>(I->getParent()->getParent()));
    }
  }

  ValueToValueMapTy VMap;


  // First off, we need to create the new module.
  std::unique_ptr<Module> New =
      std::make_unique<Module>(M.getModuleIdentifier(), M.getContext());
  New->setSourceFileName(M.getSourceFileName());
  New->setDataLayout(M.getDataLayout());
  New->setTargetTriple(M.getTargetTriple());
  New->setModuleInlineAsm(M.getModuleInlineAsm());

  // Loop over all of the global variables, making corresponding globals in the
  // new module.  Here we add them to the VMap and to the new Module.  We
  // don't worry about attributes or initializers, they will come later.
  //
  for (Module::const_global_iterator I = M.global_begin(), E = M.global_end();
       I != E; ++I) {
    GlobalVariable *GV = new GlobalVariable(*New,
                                            I->getValueType(),
                                            I->isConstant(), I->getLinkage(),
                                            (Constant*) nullptr, I->getName(),
                                            (GlobalVariable*) nullptr,
                                            I->getThreadLocalMode(),
                                            I->getType()->getAddressSpace());
    GV->copyAttributesFrom(&*I);
    VMap[&*I] = GV;
  }

  // Loop over the functions in the module, making external functions as before
  for (const Function &I : M) {
    if ( (&I)==Result.getMergedFunction()) continue;
    Function *NF =
        Function::Create(cast<FunctionType>(I.getValueType()), I.getLinkage(),
                         I.getAddressSpace(), I.getName(), New.get());
    NF->copyAttributesFrom(&I);
    VMap[&I] = NF;
  }

  // Loop over the aliases in the module
  for (Module::const_alias_iterator I = M.alias_begin(), E = M.alias_end();
       I != E; ++I) {
    //if (!ShouldCloneDefinition(&*I)) {
      // An alias cannot act as an external reference, so we need to create
      // either a function or a global variable depending on the value type.
      // FIXME: Once pointee types are gone we can probably pick one or the
      // other.
      GlobalValue *GV;
      if (I->getValueType()->isFunctionTy())
        GV = Function::Create(cast<FunctionType>(I->getValueType()),
                              GlobalValue::ExternalLinkage,
                              I->getAddressSpace(), I->getName(), New.get());
      else
        GV = new GlobalVariable(
            *New, I->getValueType(), false, GlobalValue::ExternalLinkage,
            nullptr, I->getName(), nullptr,
            I->getThreadLocalMode(), I->getType()->getAddressSpace());
      VMap[&*I] = GV;
      // We do not copy attributes (mainly because copying between different
      // kinds of globals is forbidden), but this is generally not required for
      // correctness.
      continue;
    //}
    /*
    auto *GA = GlobalAlias::create(I->getValueType(),
                                   I->getType()->getPointerAddressSpace(),
                                   I->getLinkage(), I->getName(), New.get());
    GA->copyAttributesFrom(&*I);
    VMap[&*I] = GA;
    */
  }

  // Now that all of the things that global variable initializer can refer to
  // have been created, loop through and copy the global variable referrers
  // over...  We also set the attributes on the global now.
  //
  for (Module::const_global_iterator I = M.global_begin(), E = M.global_end();
       I != E; ++I) {
    if (I->isDeclaration())
      continue;

    GlobalVariable *GV = cast<GlobalVariable>(VMap[&*I]);
    /*if (!ShouldCloneDefinition(&*I)) {
      // Skip after setting the correct linkage for an external reference.
      GV->setLinkage(GlobalValue::ExternalLinkage);
      continue;
    }
    */
    if (I->hasInitializer())
      GV->setInitializer(MapValue(I->getInitializer(), VMap));

    SmallVector<std::pair<unsigned, MDNode *>, 1> MDs;
    I->getAllMetadata(MDs);
    for (auto MD : MDs)
      GV->addMetadata(MD.first,
                      *MapMetadata(MD.second, VMap, RF_MoveDistinctMDs));
    
    copyComdat(GV, &*I);
  }

  // Similarly, copy over function bodies now...
  //
  for (const Function &I : M) {
    if (I.isDeclaration())
      continue;

    if ( (&I)==Result.getMergedFunction()) continue;

    Function *F = cast<Function>(VMap[&I]);


    //if (!ShouldCloneDefinition(&I)) {
    if (Fs.count(&I)==0) {
      // Skip after setting the correct linkage for an external reference.
      F->setLinkage(GlobalValue::ExternalLinkage);
      // Personality function is not valid on a declaration.
      F->setPersonalityFn(nullptr);
      continue;
    }

    Function::arg_iterator DestI = F->arg_begin();
    for (Function::const_arg_iterator J = I.arg_begin(); J != I.arg_end();
         ++J) {
      DestI->setName(J->getName());
      VMap[&*J] = &*DestI++;
    }

    SmallVector<ReturnInst *, 8> Returns; // Ignore returns cloned.
    CloneFunctionInto(F, &I, VMap, /*ModuleLevelChanges=*/true, Returns);

    if (I.hasPersonalityFn())
      F->setPersonalityFn(MapValue(I.getPersonalityFn(), VMap));

    copyComdat(F, &I);
  }

  // And aliases
  /*
  for (Module::const_alias_iterator I = M.alias_begin(), E = M.alias_end();
       I != E; ++I) {
    // We already dealt with undefined aliases above.
    if (!ShouldCloneDefinition(&*I))
      continue;
    GlobalAlias *GA = cast<GlobalAlias>(VMap[&*I]);
    if (const Constant *C = I->getAliasee())
      GA->setAliasee(MapValue(C, VMap));
  }*/
  
  // And named metadata....
  const auto* LLVM_DBG_CU = M.getNamedMetadata("llvm.dbg.cu");
  for (Module::const_named_metadata_iterator I = M.named_metadata_begin(),
                                             E = M.named_metadata_end();
       I != E; ++I) {
    const NamedMDNode &NMD = *I;
    NamedMDNode *NewNMD = New->getOrInsertNamedMetadata(NMD.getName());
    if (&NMD == LLVM_DBG_CU) {
      // Do not insert duplicate operands.
      SmallPtrSet<const void*, 8> Visited;
      for (const auto* Operand : NewNMD->operands())
        Visited.insert(Operand);
      for (const auto* Operand : NMD.operands()) {
        auto* MappedOperand = MapMetadata(Operand, VMap);
        if (Visited.insert(MappedOperand).second)
          NewNMD->addOperand(MappedOperand);
      }
    } else
      for (unsigned i = 0, e = NMD.getNumOperands(); i != e; ++i)
        NewNMD->addOperand(MapMetadata(NMD.getOperand(i), VMap));
  }


  ///late cleanup
  for (Module::alias_iterator I = New->alias_begin(), E = New->alias_end();
       I != E;) {
    auto *V = &*I;
    I++;
    if ( V->getNumUses() == 0 ) V->eraseFromParent();
  }
  for (Module::global_iterator I = New->global_begin(), E = New->global_end();
       I != E;) {
    auto *V = &*I;
    I++;
    if ( V->getNumUses() == 0 ) V->eraseFromParent();
  }

  Optional<size_t> SizeF1F2Opt = MeasureSize(*New,"partial1",Timeout);

  //CloneFunctionAcrossModule(Result.getMergedFunction(),&*NewM,VMap, true);
  //VMap[Result.getMergedFunction()]->setName( std::string("f")+std::to_string(Count++) );

  {
    Function &I = *Result.getMergedFunction();

    Function *NF =
        Function::Create(cast<FunctionType>(I.getValueType()), I.getLinkage(),
                         I.getAddressSpace(), I.getName(), New.get());
    NF->copyAttributesFrom(&I);
    VMap[&I] = NF;

    Function *F = NF;//cast<Function>(VMap[&I]);

    /*
    if (!ShouldCloneDefinition(&I)) {
      // Skip after setting the correct linkage for an external reference.
      F->setLinkage(GlobalValue::ExternalLinkage);
      // Personality function is not valid on a declaration.
      F->setPersonalityFn(nullptr);
      continue;
    }
    */

    Function::arg_iterator DestI = F->arg_begin();
    for (Function::const_arg_iterator J = I.arg_begin(); J != I.arg_end();
         ++J) {
      DestI->setName(J->getName());
      VMap[&*J] = &*DestI++;
    }

    SmallVector<ReturnInst *, 8> Returns; // Ignore returns cloned.
    CloneFunctionInto(F, &I, VMap, /*ModuleLevelChanges=*/true, Returns);

    if (I.hasPersonalityFn())
      F->setPersonalityFn(MapValue(I.getPersonalityFn(), VMap));

    copyComdat(F, &I);

  }

  Function *NewF1 = dyn_cast<Function>(VMap[F1]);
  Function *NewF2 = dyn_cast<Function>(VMap[F2]);
  Function *NewMF = dyn_cast<Function>(VMap[Result.getMergedFunction()]);
  FunctionMergeResult NewResult(NewF1, NewF2, NewMF, Result.needUnifiedReturn());
  NewResult.setArgumentMapping(NewF1, Result.getArgumentMapping(F1));
  NewResult.setArgumentMapping(NewF2, Result.getArgumentMapping(F2));
  NewResult.setFunctionIdArgument(Result.hasFunctionIdArgument());

  //apply NewResults
  FunctionMerger Merger(New.get());
  Merger.updateCallGraph(NewResult, AlwaysPreserved, Options);

  Optional<size_t> SizeF12Opt = MeasureSize(*New, "partial2", Timeout);
  if (SizeF1F2Opt.hasValue() && SizeF12Opt.hasValue() && SizeF1F2Opt.getValue() && SizeF12Opt.getValue()) {
            SizeF1F2 = SizeF1F2Opt.getValue();
            SizeF12 = SizeF12Opt.getValue();
  } else {
    errs() << "Sizes: Could NOT Compute!\n";
    return false;
  }
  return true;


}

Optional<size_t> MeasureSize(Module &M, FunctionMergeResult &Result, StringSet<> &AlwaysPreserved, const FunctionMergingOptions &Options={}, bool Timeout=true) {

  ValueToValueMapTy VMap;
  std::unique_ptr<Module> NewM = CloneModule(M, VMap);

  Function *F1 = Result.getFunctions().first;
  Function *F2 = Result.getFunctions().second;

  Function *NewF1 = dyn_cast<Function>(VMap[F1]);
  Function *NewF2 = dyn_cast<Function>(VMap[F2]);
  Function *NewMF = dyn_cast<Function>(VMap[Result.getMergedFunction()]);
  FunctionMergeResult NewResult(NewF1, NewF2, NewMF, Result.needUnifiedReturn());
  NewResult.setArgumentMapping(NewF1, Result.getArgumentMapping(F1));
  NewResult.setArgumentMapping(NewF2, Result.getArgumentMapping(F2));
  NewResult.setFunctionIdArgument(Result.hasFunctionIdArgument());

  //apply NewResults
  FunctionMerger Merger(NewM.get());
  Merger.updateCallGraph(NewResult, AlwaysPreserved, Options);
  
  return MeasureSize(*NewM,"oracle",Timeout);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////


void ReplaceByMergedBody(bool IsFunc1, Function *F, Function *MF, Module *M, std::map<unsigned, unsigned> &ArgMap, bool HasIdArg, ValueToValueMapTy &VMap) {
  LLVMContext &Context = M->getContext();

  Value *FuncId = IsFunc1 ? ConstantInt::getTrue(IntegerType::get(Context,1))
                          : ConstantInt::getFalse(IntegerType::get(Context,1));

  F->deleteBody();
  BasicBlock *NewBB = BasicBlock::Create(Context, "", F);
  IRBuilder<> Builder(NewBB);

  std::vector<Value *> args;
  for (unsigned i = 0; i < MF->getFunctionType()->getNumParams(); i++) {
    args.push_back(nullptr);
  }

  if (HasIdArg) {
    args[0] = FuncId;
  }

  std::vector<Argument *> ArgsList;
  for (Argument &arg : F->args()) {
    ArgsList.push_back(&arg);
  }

  for (auto Pair : ArgMap) {
    args[Pair.second] = ArgsList[Pair.first];
  }

  for (unsigned i = 0; i < args.size(); i++) {
    if (args[i] == nullptr) {
      args[i] = UndefValue::get(MF->getFunctionType()->getParamType(i));
    }
  }

  CallInst *CI =
      (CallInst *)Builder.CreateCall(MF, ArrayRef<Value *>(args));
  CI->setTailCall();
  CI->setCallingConv(MF->getCallingConv());
  CI->setAttributes(MF->getAttributes());
  CI->setIsNoInline();

  if (F->getReturnType()->isVoidTy()) {
    Builder.CreateRetVoid();
  } else {
    Value *CastedV = CI;
    /*if (MFR.needUnifiedReturn()) {
      Value *AddrCI = Builder.CreateAlloca(CI->getType());
      Builder.CreateStore(CI,AddrCI);
      Value *CastedAddr = Builder.CreatePointerCast(AddrCI, PointerType::get(F->getReturnType(), DL->getAllocaAddrSpace()));
      CastedV = Builder.CreateLoad(CastedAddr);
    } else {
      CastedV = createCastIfNeeded(CI, F->getReturnType(), Builder, IntPtrTy, Options);
    }*/
    Builder.CreateRet(CastedV);
  }
  InlineFunctionInfo IFI;
  InlineFunction(*CI,IFI);
}

void ExportForAlive(Function *F, Module &M, FunctionMergeResult &Result, std::string Suffix, const FunctionMergingOptions &Options={}) {
  std::unique_ptr<Module> NewM =
      std::make_unique<Module>(M.getModuleIdentifier(), M.getContext());
  //std::unique_ptr<Module> NewM = std::make_unique<Module>(M.getModuleIdentifier(), M.getContext());

  ValueToValueMapTy VMap;

  CloneFunctionAcrossModule(F,&*NewM,VMap);

  Function *NewF = dyn_cast<Function>(VMap[F]);
  NewF->setName( std::string("__alive_func") );
  
  for (BasicBlock &BB : *NewF) {
    for (Instruction &I : BB) {
      SmallVector<std::pair<unsigned, MDNode *>, 8> MDs;
      I.getAllMetadata(MDs);
      for (std::pair<unsigned, MDNode *> MDPair : MDs) {
        I.setMetadata(MDPair.first, nullptr);
      }
    }
  }

  {
    std::string FilePath = std::string("/tmp/alive/func.")+Suffix+std::string(".ll");
    std::error_code EC;
    llvm::raw_fd_ostream OS(FilePath, EC, llvm::sys::fs::F_None);
    OS << *NewM;
    OS.flush();
  }

  CloneFunctionAcrossModule(Result.getMergedFunction(),&*NewM,VMap);
 
  Function *NewMF = dyn_cast<Function>(VMap[Result.getMergedFunction()]);

  for (BasicBlock &BB : *NewMF) {
    for (Instruction &I : BB) {
      SmallVector<std::pair<unsigned, MDNode *>, 8> MDs;
      I.getAllMetadata(MDs);
      for (std::pair<unsigned, MDNode *> MDPair : MDs) {
        I.setMetadata(MDPair.first, nullptr);
      }
    }
  }

  ReplaceByMergedBody(Result.getFunctions().first==F, NewF, NewMF, NewM.get(), Result.getArgumentMapping(F), Result.hasFunctionIdArgument(), VMap);
  NewMF->eraseFromParent();
  NewMF = NewF;

  {
    std::string FilePath = std::string("/tmp/alive/tr.func.")+Suffix+std::string(".ll");
    std::error_code EC;
    llvm::raw_fd_ostream OS(FilePath, EC, llvm::sys::fs::F_None);
    OS << *NewM;
    OS.flush();
  }

  CloneFunctionAcrossModule(F,&*NewM,VMap);
  NewF = dyn_cast<Function>(VMap[F]);
  //NewF->setName( std::string("__alive_func") );
  
  for (BasicBlock &BB : *NewF) {
    for (Instruction &I : BB) {
      SmallVector<std::pair<unsigned, MDNode *>, 8> MDs;
      I.getAllMetadata(MDs);
      for (std::pair<unsigned, MDNode *> MDPair : MDs) {
        I.setMetadata(MDPair.first, nullptr);
      }
    }
  }

}

void ExportForAlive(Module &M, FunctionMergeResult &Result, unsigned FileId, const FunctionMergingOptions &Options={}) {
  Function *F1 = Result.getFunctions().first;
  Function *F2 = Result.getFunctions().second;

  ExportForAlive(F1,M,Result,std::to_string(FileId)+std::string(".1"), Options);
  ExportForAlive(F2,M,Result,std::to_string(FileId)+std::string(".2"), Options);
}

/*

static bool simplifyInstructions(Function &F);
static bool simplifyCFG(Function &F);

bool ValidateEachMerge(Function *F, Module &M, FunctionMergeResult &Result, const FunctionMergingOptions &Options={}) {
  std::unique_ptr<Module> NewM =
      std::make_unique<Module>(M.getModuleIdentifier(), M.getContext());
  //std::unique_ptr<Module> NewM = std::make_unique<Module>(M.getModuleIdentifier(), M.getContext());

  ValueToValueMapTy VMap;

  CloneFunctionAcrossModule(F,&*NewM,VMap);

  Function *NewF = dyn_cast<Function>(VMap[F]);
  NewF->setName( std::string("__alive_func") );
  
  CloneFunctionAcrossModule(Result.getMergedFunction(),&*NewM,VMap);
 
  Function *NewMF = dyn_cast<Function>(VMap[Result.getMergedFunction()]);

  for (BasicBlock &BB : *NewMF) {
    for (Instruction &I : BB) {
      I.dropPoisonGeneratingFlags(); //TODO: NOT SURE IF THIS IS VALID
      SmallVector<std::pair<unsigned, MDNode *>, 8> MDs;
      I.getAllMetadata(MDs);
      for (std::pair<unsigned, MDNode *> MDPair : MDs) {
        I.setMetadata(MDPair.first, nullptr);
      }
      if (CallBase *CB = dyn_cast<CallBase>(&I)) {
        AttributeList AL;
        CB->setAttributes(AL);
      }
    }
  }

  ReplaceByMergedBody(Result.getFunctions().first==F, NewF, NewMF, NewM.get(), Result.getArgumentMapping(F), Result.hasFunctionIdArgument(), VMap);
  NewMF->eraseFromParent();
  NewMF = NewF;

  CloneFunctionAcrossModule(F,&*NewM,VMap);
  NewF = dyn_cast<Function>(VMap[F]);
  
  for (BasicBlock &BB : *NewF) {
    for (Instruction &I : BB) {
      I.dropPoisonGeneratingFlags(); //TODO: NOT SURE IF THIS IS VALID
      SmallVector<std::pair<unsigned, MDNode *>, 8> MDs;
      I.getAllMetadata(MDs);
      for (std::pair<unsigned, MDNode *> MDPair : MDs) {
        I.setMetadata(MDPair.first, nullptr);
      }
      if (CallBase *CB = dyn_cast<CallBase>(&I)) {
        AttributeList AL;
        CB->setAttributes(AL);
      }
    }
  }

  const int MaxTimeout = 10;
  int Timeout = MaxTimeout;
  bool Changed = false;
  do {
    for (BasicBlock &BB : *NewF) {
      Changed = Changed || SimplifyInstructionsInBlock(&BB);
    }
    //Changed = Changed || simplifyInstructions(*NewF);
    Changed = Changed || simplifyCFG(*NewF);
    Timeout--;
  } while (Changed && Timeout > 0);

  Timeout = MaxTimeout;
  Changed = false;
  do {
    for (BasicBlock &BB : *NewMF) {
      Changed = Changed || SimplifyInstructionsInBlock(&BB);
    }
    //Changed = Changed || simplifyInstructions(*NewMF);
    Changed = Changed || simplifyCFG(*NewMF);
    Timeout--;
  } while (Changed && Timeout > 0);

  GlobalNumberState GN;
  FunctionComparator FCmp(NewF, NewMF, &GN);

  if (FCmp.compare()) {
     errs() << "Non-matching functions\n";
     NewF->dump();
     NewMF->dump();
     return false;
  } else return true;
  //return FCmp.compare()==0; //if zero they are identical
}

bool ValidateMerge(Module &M, FunctionMergeResult &Result, const FunctionMergingOptions &Options={}) {
  Function *F1 = Result.getFunctions().first;
  Function *F2 = Result.getFunctions().second;

  if (!ValidateEachMerge(F1,M,Result,Options)) {
    errs() << "Function 1 Not Matching!!!!!!\n";
    return false;
  }
  if (!ValidateEachMerge(F2,M,Result,Options)) {
    errs() << "Function 2 Not Matching!!!!!!\n";
    return false;
  }
  return true;
}

*/

