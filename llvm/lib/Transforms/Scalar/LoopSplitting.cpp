#include "llvm/ADT/Statistic.h"
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/Analysis/ScalarEvolution.h"
#include "llvm/Analysis/ScalarEvolutionExpressions.h"
#include "llvm/Transforms/Utils/ScalarEvolutionExpander.h"
#include "llvm/IR/Dominators.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/Pass.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/Transforms/Utils/LoopUtils.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include "llvm/Transforms/Scalar.h"
#include <typeinfo>

using namespace llvm;

#define DEBUG_TYPE "hello"

namespace {
    struct SplitInfo {
        Value *point;
        Value *iterationValue;
        Value *isTrueAtPoint;
        Value *nextValue;
        Value *isTrueAtNextPoint;
        CmpInst *comparison;
        int order;
        
        SplitInfo(Value *point, Value *iterationValue, Value *isTrueAtPoint, Value *nextValue, Value *isTrueAtNextPoint, CmpInst *comparison, int order) {
            this->point = point;
            this->isTrueAtPoint = isTrueAtPoint;
            this->comparison = comparison;
            this->iterationValue = iterationValue;
            this->nextValue = nextValue;
            this->isTrueAtNextPoint = isTrueAtNextPoint;
            this->order = order;

        }
    };
    // Hello2 - The second implementation with getAnalysisUsage implemented.
    struct LoopSplitting : public FunctionPass {
        static char ID; // Pass identification, replacement for typeid
    private:
        SCEVExpander *expander;
        
    public:
        LoopSplitting() : FunctionPass(ID) {}
        
        Value * calculateIntersectionPoint(const SCEVAddRecExpr *first,
                                          const SCEVAddRecExpr *second,
                                          CmpInst *compare,
                                          IRBuilder<> *builder) {
            auto predicate = compare->getPredicate();
            assert(first->isAffine() && second->isAffine() && "SCEVs must be affine");
            assert((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_ULT || predicate == CmpInst::ICMP_ULE ||
                  predicate == CmpInst::ICMP_SGT || predicate == CmpInst::ICMP_SGE || predicate == CmpInst::ICMP_SLT ||
                  predicate == CmpInst::ICMP_SLE) &&
                "We only treat integer inequalities for now");
            const auto b1 = this->expander->expandCodeFor(first->getOperand(0),
                                                          first->getOperand(1)->getType(),
                                                          compare);
            const auto a1 = this->expander->expandCodeFor(first->getOperand(1),
                                                          first->getOperand(0)->getType(),
                                                          compare);
            const auto b2 = this->expander->expandCodeFor(second->getOperand(0),
                                                          second->getOperand(1)->getType(),
                                                          compare);
            const auto a2 = this->expander->expandCodeFor(second->getOperand(1),
                                                          second->getOperand(0)->getType(),
                                                          compare);

            const auto b = builder->CreateSub(b2, b1, "b");    // b2 - b1
            const auto a = builder->CreateSub(a1, a2, "a");    // a1 - a2
            const auto div = builder->CreateSDiv(b, a, "div"); // b / a
            return div;

        }

        
        Value * calculateIntersectionPoint(const SCEVAddRecExpr *add,
                                          Value *constant,
                                          CmpInst *compare,
                                          IRBuilder<> &builder) {
            
            auto predicate = compare->getPredicate();
            assert(add->isAffine() && "SCEVs must be affine");
            assert((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_ULT ||
                    predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SGT ||
                    predicate == CmpInst::ICMP_SGE || predicate == CmpInst::ICMP_SLT ||
                    predicate == CmpInst::ICMP_SLE || predicate == CmpInst::ICMP_UGT) &&
                   "We only treat integer inequalities for now");
            const auto b = this->expander->expandCodeFor(add->getOperand(0),
                                                         constant->getType(),
                                                         compare->getParent()->getFirstNonPHI());
            const auto a = this->expander->expandCodeFor(add->getOperand(1),
                                                         constant->getType(),
                                                         builder.GetInsertBlock()->getFirstNonPHI());

            const auto y = constant;
            
            // Como y = ax + b
            // colocamos como y a constante e calculamos x a partir disso:
            // Ou seja: x = (y - b) / a

            const auto temp1 = builder.CreateSub(y, b, "yMinusB"); // y - b
            const auto x = builder.CreateSDiv(temp1, a, "x"); // (y - b) / a

            return x;
        }
        
  

        bool isExiting(const Loop &L, BasicBlock *BB) {
            bool exiting = L.isLoopExiting(BB);
            for (Loop *loop : L.getSubLoops()) {
                exiting = isExiting(*loop, BB) || exiting;
            }
            return exiting;
        }
        
        std::set<CmpInst *> getLoopComparisons(LoopInfo &LI, const Loop &L) {
            std::set<CmpInst *> AllCmps;
            
            for (BasicBlock *BB : L.blocks()) {
                for (Instruction &I : *BB) {
                    if (SelectInst *Sel = dyn_cast<SelectInst>(&I)) {
                        if (CmpInst *Cmp = dyn_cast<CmpInst>(Sel->getCondition()))
                            AllCmps.insert(Cmp);
                    }
                }
                
                bool exiting = false;
                for (Loop *loop : LI) {
                    exiting = isExiting(*loop, BB);
                }
                if (!LI.getLoopFor(BB)->isLoopExiting(BB)) {
                    BranchInst *Br = dyn_cast<BranchInst>(BB->getTerminator());
                    if (Br && Br->isConditional()) {
                        if (CmpInst *Cmp = dyn_cast<CmpInst>(Br->getCondition())) {
                            AllCmps.insert(Cmp);
                        }
                    }
                }
            }
            
            return AllCmps;
        }
        
        std::set<CmpInst *> collectCandidateInstructions(LoopInfo &LI, Loop &loop) {
            auto comparisons = std::set<CmpInst *>();
            for (Loop *subloop : loop.getSubLoops()) {
                auto subLoopComparisons = collectCandidateInstructions(LI, *subloop);
                comparisons.insert(subLoopComparisons.begin(), subLoopComparisons.end());
            }
            auto loopComparisons = getLoopComparisons(LI, loop);
            comparisons.insert(loopComparisons.begin(), loopComparisons.end());
            return comparisons;
        }
        
        std::set<CmpInst *> collectCandidateInstructions(LoopInfo &LI) {
            auto allComparisons = std::set<CmpInst *>();
            for (Loop *loop : LI) {
                auto loopComparisons = collectCandidateInstructions(LI, *loop);
                allComparisons.insert(loopComparisons.begin(), loopComparisons.end());
            }
            return allComparisons;
        }
        
        bool isAffineAndConstantComparison(ScalarEvolution &SE, CmpInst &comparison) {
            auto first = SE.getSCEV(comparison.getOperand(0));
            auto second = SE.getSCEV(comparison.getOperand(1));
            if ((first->getSCEVType() == scConstant || second->getSCEVType() == scConstant) &&
                (first->getSCEVType() == scAddRecExpr || second->getSCEVType() == scAddRecExpr)) {
                auto *addRec = dyn_cast<SCEVAddRecExpr>(first);
                if (addRec == nullptr) {
                    addRec = dyn_cast<SCEVAddRecExpr>(second);
                }
                return addRec->isAffine();
            }
        
            return false;
        }
        
        bool isBothAffineComparison(ScalarEvolution &SE, CmpInst &comparison) {
            auto first = SE.getSCEV(comparison.getOperand(0));
            auto second = SE.getSCEV(comparison.getOperand(1));
            if (first->getSCEVType() == scAddRecExpr && second->getSCEVType() == scAddRecExpr) {
                auto *firstAddRec = cast<SCEVAddRecExpr>(first);
                auto *secondAddRec = cast<SCEVAddRecExpr>(second);
                return firstAddRec->isAffine() && secondAddRec->isAffine();
            }
            return false;
        }
        
        Loop * getLoop(CmpInst *comparison, LoopInfo &LI, ScalarEvolution &SE) {

            if (isAffineAndConstantComparison(SE, *comparison)) {
                const SCEVAddRecExpr *affine;
                auto first = SE.getSCEV(comparison->getOperand(0));
                auto second = SE.getSCEV(comparison->getOperand(1));
                
                if (first->getSCEVType() == scConstant) {
                    affine = cast<SCEVAddRecExpr>(second);
                } else {
                    affine = cast<SCEVAddRecExpr>(first);
                }
                auto loop = affine->getLoop(); // const reference, find it in hierarchy :(
                auto tempLoop = LI.getLoopFor(comparison->getParent());

                while (true) {
                    if (loop == tempLoop) {
                        return tempLoop;
                    }
                    
                    tempLoop = tempLoop->getParentLoop();
                }

            } else if (isBothAffineComparison(SE, *comparison)) {
                auto firstAffine = cast<SCEVAddRecExpr>(SE.getSCEV(comparison->getOperand(0)));
                auto secondAffine = cast<SCEVAddRecExpr>(SE.getSCEV(comparison->getOperand(1)));
                
                auto firstLoop = firstAffine->getLoop();
                auto secondLoop = secondAffine->getLoop();
                auto tempLoop = LI.getLoopFor(comparison->getParent());
                while (true) {
                    if (firstLoop == tempLoop || secondLoop == tempLoop) {
                        return tempLoop;
                    }
                    tempLoop = tempLoop->getParentLoop();
                }
            }
            return nullptr;


        }
        
        
        SplitInfo *splitInforForAddRecAndConstant(ScalarEvolution &SE,
                                                  LoopInfo &LI,
                                                  CmpInst *instruction,
                                                  const SCEVAddRecExpr *affine,
                                                  Value *constant,
                                                  bool firstIsConstant,
                                                  Instruction *constantInstruction = nullptr) {
            constant->print(errs());
            errs() << "\n";
            auto loop = getLoop(instruction, LI, SE);
            
            auto insertionPoint = loop->getLoopPreheader()->getTerminator();
            IRBuilder<> builder(insertionPoint);
            
            if (constantInstruction != nullptr)
                builder.Insert(constantInstruction);
                
            auto value = calculateIntersectionPoint(affine, constant, instruction, builder);
            auto scevValue = SE.getSCEV(value);
            auto iterationScev = affine->evaluateAtIteration(scevValue, SE);
            auto iterationValue = expander->expandCodeFor(iterationScev, constant->getType(), insertionPoint);
            
            auto nextScevValue = SE.getAddExpr(scevValue, SE.getOne(value->getType()));
            auto nextIterationScev = affine->evaluateAtIteration(nextScevValue, SE);
            auto nextIterationValue = expander->expandCodeFor(nextIterationScev,
                                                              constant->getType(), insertionPoint);
            
            Value *v = builder.CreateICmp(instruction->getPredicate(),
                                          firstIsConstant ? constant : iterationValue,
                                          firstIsConstant ? iterationValue : constant);
            Value *nextPoint = builder.CreateICmp(instruction->getPredicate(),
                                                  firstIsConstant ? constant : nextIterationValue,
                                                  firstIsConstant ? nextIterationValue : constant);
            
            auto loopBranch = cast<BranchInst>(loop->getExitingBlock()->getTerminator());
            assert(loopBranch->isConditional() && "Expected to be conditional!");
            CmpInst *exitCmp = cast<CmpInst>(loopBranch->getCondition());

            auto loopScev = cast<SCEVAddRecExpr>(SE.getSCEV(exitCmp->getOperand(0)));
            auto loopEndValue = expander->expandCodeFor(loopScev->evaluateAtIteration(scevValue, SE), constant->getType(), insertionPoint);

            auto valueAfterLoop = expander->expandCodeFor(loopScev->evaluateAtIteration(nextScevValue, SE), constant->getType(), insertionPoint);
            auto splitInfo = new SplitInfo(value, loopEndValue, v, valueAfterLoop, nextPoint, instruction, firstIsConstant ? 0 : 1);
            return splitInfo;
        }
        std::vector<SplitInfo> comparisonToSplitInfo(ScalarEvolution &SE,
                                                     LoopInfo &LI,
                                                     std::set<CmpInst *> &comparisons) {
            auto splitInfoVector = std::vector<SplitInfo>();
            for (CmpInst *instruction : comparisons) {
                if (isAffineAndConstantComparison(SE, *instruction)) {
                    auto first = SE.getSCEV(instruction->getOperand(0));
                    auto second = SE.getSCEV(instruction->getOperand(1));
                    
                    const SCEVConstant *constant;
                    const SCEVAddRecExpr *affine;
                    bool firstIsConstant = false;
                    if (first->getSCEVType() == scConstant) {
                        constant = cast<SCEVConstant>(first);
                        affine = cast<SCEVAddRecExpr>(second);
                        firstIsConstant = true;
                    } else {
                        constant = cast<SCEVConstant>(second);
                        affine = cast<SCEVAddRecExpr>(first);
                    }
                    
                    auto splitInfo = splitInforForAddRecAndConstant(SE,
                                                                    LI,
                                                                    instruction,
                                                                    affine,
                                                                    constant->getValue(),
                                                                    firstIsConstant);
                    splitInfoVector.push_back(*splitInfo);
                } else if (isBothAffineComparison(SE, *instruction)) {
                    auto firstAffine = cast<SCEVAddRecExpr>(SE.getSCEV(instruction->getOperand(0)));
                    auto secondAffine = cast<SCEVAddRecExpr>(SE.getSCEV(instruction->getOperand(1)));
                    if (firstAffine->getLoop() == secondAffine->getLoop()) {
                        auto loop = getLoop(instruction, LI, SE);
                        auto insertionPoint = loop->getLoopPreheader()->getFirstNonPHI();
                        IRBuilder<> builder(insertionPoint);
                        
                        auto value = calculateIntersectionPoint(firstAffine, secondAffine, instruction, &builder);
                        auto scevValue = SE.getSCEV(value);

                        auto firstIterationScev = firstAffine->evaluateAtIteration(scevValue, SE);
                        auto firstIterationValue = expander->expandCodeFor(firstIterationScev, value->getType(), insertionPoint);

                        auto secondIterationScev = secondAffine->evaluateAtIteration(scevValue, SE);
                        auto secondIterationValue = expander->expandCodeFor(secondIterationScev, value->getType(), insertionPoint);

                        auto nextScevValue = SE.getAddExpr(scevValue, SE.getOne(value->getType()));

                        auto firstNextIterationScev = firstAffine->evaluateAtIteration(nextScevValue, SE);

                        auto firstNextIterationValue = expander->expandCodeFor(firstNextIterationScev,
                                                                               value->getType(),
                                                                               insertionPoint);

                        auto secondNextIterationScev = secondAffine->evaluateAtIteration(nextScevValue, SE);

                        auto secondNextIterationValue = expander->expandCodeFor(secondNextIterationScev,
                                                                          value->getType(), insertionPoint);

                        Value *v = builder.CreateICmp(instruction->getPredicate(), firstIterationValue, secondIterationValue);

                        Value *nextPoint = builder.CreateICmp(instruction->getPredicate(), firstNextIterationValue, secondNextIterationValue);

                        auto loopBranch = cast<BranchInst>(loop->getExitingBlock()->getTerminator());
                        assert(loopBranch->isConditional() && "Expected to be conditional!");
                        CmpInst *exitCmp = cast<CmpInst>(loopBranch->getCondition());
                        auto loopScev = cast<SCEVAddRecExpr>(SE.getSCEV(exitCmp->getOperand(0)));
                        auto loopEndValue = expander->expandCodeFor(loopScev->evaluateAtIteration(scevValue, SE), value->getType(), insertionPoint);

                        auto valueAfterLoop = expander->expandCodeFor(loopScev->evaluateAtIteration(nextScevValue, SE), value->getType(), insertionPoint);
                        auto splitInfo = SplitInfo(value, loopEndValue, v, valueAfterLoop, nextPoint, instruction, 0);
                        splitInfoVector.push_back(splitInfo);
                    } else {
                        auto firstLoop = firstAffine->getLoop();
                        auto secondLoop = secondAffine->getLoop();
                        
                        if (firstLoop->contains(secondLoop)) {
                            SplitInfo *splitInfo =splitInfo = splitInforForAddRecAndConstant(SE, LI, instruction, secondAffine, instruction->getOperand(0), true);
                            splitInfoVector.push_back(*splitInfo);
                        } else if (secondLoop->contains(firstLoop)) {
                            SplitInfo *splitInfo=  splitInforForAddRecAndConstant(SE, LI, instruction, firstAffine, instruction->getOperand(1), false);
                            splitInfoVector.push_back(*splitInfo);
                        }
                    }

                }
            }
            return splitInfoVector;
        }
        
        Loop *cloneLoop(Function *F, Loop *L, LoopInfo *LI, DominatorTree &DT, const Twine &NameSuffix,
                ValueToValueMapTy &VMap) {
    
        // original preheader of the loop
            const auto PreHeader = L->getLoopPreheader();
            
            // keep track of the original predecessors
            std::set<BasicBlock *> AllPredecessors;
            for (auto PredIt = pred_begin(PreHeader), E = pred_end(PreHeader);
                 PredIt != E; PredIt++)
                AllPredecessors.insert(*PredIt);
            
            BasicBlock *ExitBlock = L->getExitBlock();
            
            
            SmallVector<BasicBlock *, 8> Blocks;
            
            const auto ClonedLoop = cloneLoopWithPreheader(PreHeader, PreHeader, L, VMap, NameSuffix, LI, &DT, Blocks);
            VMap[ExitBlock] = PreHeader; // chain: cloned loop -> original loop
            remapInstructionsInBlocks(Blocks, VMap);
            
            // remap original predecessors to the cloned loop
            for (BasicBlock *PredBB : AllPredecessors) {
                Instruction *TI = PredBB->getTerminator();
                for (unsigned i = 0; i < TI->getNumOperands(); i++) {
                    if (TI->getOperand(i) == PreHeader)
                        TI->setOperand(i, ClonedLoop->getLoopPreheader());
                }
            }
            
            return ClonedLoop;
        }

        void splitLoop(ScalarEvolution &SE, Function *F, DominatorTree &DT, Loop *loop, LoopInfo *LI, SplitInfo *info) {
            const CmpInst::Predicate predicate = info->comparison->getPredicate();
            assert((predicate == CmpInst::ICMP_UGE || predicate == CmpInst::ICMP_ULT ||
                    predicate == CmpInst::ICMP_ULE || predicate == CmpInst::ICMP_SGT ||
                    predicate == CmpInst::ICMP_SGE || predicate == CmpInst::ICMP_SLT ||
                    predicate == CmpInst::ICMP_SLE || predicate == CmpInst::ICMP_UGT) &&
                   "We only treat integer inequalities for now");

            // só funciona para < e >
            info->comparison->setOperand(0, ConstantInt::getFalse(F->getContext()));
            info->comparison->setOperand(1, info->isTrueAtNextPoint);
            auto comparisonPredicate = info->comparison->getPredicate();
            info->comparison->setPredicate(ICmpInst::ICMP_EQ);
            
            ValueToValueMapTy VMap;
            auto clonedLoop = cloneLoop(F, loop, LI, DT, "FirstLoop", VMap);
            info->comparison->setOperand(0, ConstantInt::getTrue(F->getContext()));
            BranchInst *ClonedBr = cast<BranchInst>(clonedLoop->getExitingBlock()->getTerminator());
            assert(ClonedBr->isConditional() && "Expected to be conditional!");
            
            for (PHINode &PHI : loop->getHeader()->phis()) {
                PHINode *ClonedPHI = dyn_cast<PHINode>(VMap[&PHI]);

                Value *LastValue = ClonedPHI;
                if (clonedLoop->getExitingBlock() == clonedLoop->getLoopLatch()) {
                    LastValue =
                    ClonedPHI->getIncomingValueForBlock(clonedLoop->getLoopLatch());
                } else
                    assert(clonedLoop->getExitingBlock() == clonedLoop->getHeader() &&
                           "Expected exiting block to be the loop header!");

                PHI.setIncomingValue(PHI.getBasicBlockIndex(loop->getLoopPreheader()),
                                     LastValue);
            }
            IRBuilder<> builder(clonedLoop->getHeader()->getFirstNonPHI());
            
            if (CmpInst *ExitCmp = dyn_cast<CmpInst>(ClonedBr->getCondition())) {
                Value *iterationValue;
                Value *nextIterationValue;
                if (VMap[info->iterationValue]) {
                    iterationValue = VMap[info->iterationValue];
                } else {
                    iterationValue = info->iterationValue;
                }
                
                if (VMap[info->nextValue]) {
                    nextIterationValue = VMap[info->nextValue];
                } else {
                    nextIterationValue = info->nextValue;
                }

                auto first = info->order == 0 ? iterationValue : nextIterationValue;
                auto second = info->order == 0 ? nextIterationValue : iterationValue;
                
                Value *isTrueAtPoint = VMap[info->isTrueAtPoint];
                if (isTrueAtPoint == nullptr) {
                    isTrueAtPoint = info->isTrueAtPoint;
                }
                if (comparisonPredicate == CmpInst::ICMP_SGT || comparisonPredicate == CmpInst::ICMP_SGE) {
                    auto sel = builder.CreateSelect(isTrueAtPoint, second, first);
                    ExitCmp->setOperand(1, sel);
                } else {
                    auto sel = builder.CreateSelect(isTrueAtPoint, first, second);
                    ExitCmp->setOperand(1, sel);
                }
            }
        }
        
        bool runOnFunction(Function &F) override {
            auto &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
            auto &SE = getAnalysis<ScalarEvolutionWrapperPass>().getSE();
            auto &DT = getAnalysis<DominatorTreeWrapperPass>().getDomTree();
            this->expander = new SCEVExpander(SE, DataLayout(F.getParent()), "name");

	    errs() << "Running Loop Splitting\n";
            while (true) {
                auto allComparisons = collectCandidateInstructions(LI);
                auto splitInfo = comparisonToSplitInfo(SE, LI, allComparisons);
                if (splitInfo.size() == 0) break;
                for (auto splitInfo : splitInfo) {
	            errs() << "Splitting Loop\n";
                    auto loop = getLoop(splitInfo.comparison, LI, SE);
                    splitLoop(SE, &F, DT, loop, &LI, &splitInfo);
                }
                
            }
	    errs() << "Done Loop Splitting\n";

            return true;
        }
        
        void getAnalysisUsage(AnalysisUsage &AU) const override {
            AU.setPreservesAll();
            getLoopAnalysisUsage(AU);
        }
        

    };
}

char LoopSplitting::ID = 0;
static RegisterPass<LoopSplitting>
Y("loopsplit", "Hello World Pass (with getAnalysisUsage implemented)");

Pass *llvm::createLoopSplittingPass() {
  return new LoopSplitting;
}

