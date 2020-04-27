
static void copyComdat(GlobalObject *Dst, const GlobalObject *Src) {
  const Comdat *SC = Src->getComdat();
  if (!SC)
    return;
  Comdat *DC = Dst->getParent()->getOrInsertComdat(SC->getName());
  DC->setSelectionKind(SC->getSelectionKind());
  Dst->setComdat(DC);
}


void CloneUsedGlobalsAcrossModule(Function *F, Module *M, Module *NewM, ValueToValueMapTy &VMap) {
  std::set<const Value*> NeededValues;

  std::set<const Value*> NewMappedValue;

  NeededValues.insert(F);  
  for (Instruction &I : instructions(F)) {
    for (unsigned i = 0; i<I.getNumOperands(); i++) {
      NeededValues.insert(I.getOperand(i));
    }
    NeededValues.insert(&I);
  }

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

    GlobalVariable *GV = new GlobalVariable(*NewM,
                                            I->getValueType(),
                                            I->isConstant(), I->getLinkage(),
                                            (Constant*) nullptr, I->getName(),
                                            (GlobalVariable*) nullptr,
                                            I->getThreadLocalMode(),
                                            I->getType()->getAddressSpace());
    GV->copyAttributesFrom(&*I);
    VMap[&*I] = GV;
    NewMappedValue.insert(&*I);
  }

  // Loop over the functions in the module, making external functions as before
  for (const Function &I : *M) {
    if (VMap.find(&I)!=VMap.end()) continue;
    //if (NeededValues.find(&I)==NeededValues.end() || (&I)==F) continue;
    bool FoundUse = false;
    for (const User *U : I.users()) {
      if (NeededValues.find(U)!=NeededValues.end()) { FoundUse = true; break; }
    }
    if (!FoundUse) continue;

    Function *NF =
        Function::Create(cast<FunctionType>(I.getValueType()), I.getLinkage(),
                         //I.getAddressSpace(),
                         I.getName(), NewM);
    NF->copyAttributesFrom(&I);
    NF->setLinkage(GlobalValue::LinkageTypes::ExternalLinkage);
    NF->setPersonalityFn(nullptr);
    VMap[&I] = NF;
    NewMappedValue.insert(&I);
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


void CloneFunctionAcrossModule(Function *F, Module *M, ValueToValueMapTy &VMap) {

  CloneUsedGlobalsAcrossModule(F,F->getParent(),M,VMap);

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

  if (F->hasPersonalityFn())
    NewF->setPersonalityFn(MapValue(F->getPersonalityFn(), VMap));

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
 
  std::string FilePath("/tmp/.tmp.ll");
  std::error_code EC;
  llvm::raw_fd_ostream OS(FilePath, EC, llvm::sys::fs::F_None);
  //WriteBitcodeToFile(*NewM, OS);
  //NewM->print(OS,false);
  OS << *NewM;
  OS.flush();

  std::remove("/tmp/.tmp.o");

  std::string ClangPath = "/home/rodrigo/salssa20/build/bin/clang";
  //std::string Cmd = std::string("rm /tmp/.tmp.o; ")+(Timeout?std::string("timeout -s KILL 2m "):std::string(""))+ClangPath+std::string(" -x ir /tmp/.tmp.ll -Os -c -o /tmp/.tmp.o");
  std::string Cmd = (Timeout?std::string("timeout -s KILL 5m "):std::string(""))+ClangPath+std::string(" -x ir /tmp/.tmp.ll -Os -c -o /tmp/.tmp.o");
  bool CompilationOK = !std::system(Cmd.c_str());

  std::ifstream builtObj("/tmp/.tmp.o");
  if (CompilationOK && builtObj.good()) {
    std::remove("/tmp/.size.txt");
    bool BadMeasurement = std::system("size -d -A /tmp/.tmp.o | grep text > /tmp/.size.txt");
    std::ifstream ifs("/tmp/.size.txt");
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
  //return filesize(std::string("/tmp/.tmp.o"));
}

Optional<size_t> MeasureSize(Module &M, bool Timeout=true) {
  std::string FilePath("/tmp/.tmp.ll");
  std::error_code EC;
  llvm::raw_fd_ostream OS(FilePath, EC, llvm::sys::fs::F_None);
  //WriteBitcodeToFile(*NewM, OS);
  //NewM->print(OS,false);
  OS << M;
  OS.flush();

  std::remove("/tmp/.tmp.o");

  std::string ClangPath = "/home/rodrigo/salssa20/build/bin/clang";
  std::string Cmd = (Timeout?std::string("timeout -s KILL 5m "):std::string(""))+ClangPath+std::string(" -x ir /tmp/.tmp.ll -Os -c -o /tmp/.tmp.o");
  bool CompilationOK = !std::system(Cmd.c_str());

  std::ifstream builtObj("/tmp/.tmp.o");
  if (CompilationOK && builtObj.good())
    return filesize(std::string("/tmp/.tmp.o"));
  else return Optional<size_t>();
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
  
  return MeasureSize(*NewM,Timeout);
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
  InlineFunction(CI,IFI);
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

