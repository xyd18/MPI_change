#include "llvm/Support/TargetSelect.h"
#include "llvm/Bitcode/BitcodeReader.h"
#include "llvm/Bitcode/BitcodeWriter.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/InstrTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/SourceMgr.h"
#include "llvm/Support/MemoryBuffer.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <string>
#include <map>
#include <vector>
#include <sstream>
#include <fstream>

#include "common.h"

using namespace std;
using namespace llvm;


Value* fromAlloca(Value *alloca){
    errs() << "它的 User 有" << "\n";
    for(auto u = alloca->user_begin(); u != alloca->user_end(); u++){
        u->printAsOperand(errs()); errs() << "\n";
        if(isa<StoreInst>(*u)){
            errs() << "是 StoreInst" << "\n";
            StoreInst* store_inst = cast<StoreInst>(*u);
            Value* vop = store_inst->getValueOperand();
            errs() << "其第一个操作数" << "\n";
            errs() << *vop << "\n";
            if(isa<CastInst>(*vop)){
                errs() << "是 CastInst" << "\n";
                Value* cv = cast<CastInst>(*vop).getOperand(0);
                errs() << "应该是malloc那条instruction了" << *cv << "\n"; // 第一个参数
                return cv;
            }else if(isa<Instruction>(*vop)){
                while(isa<Instruction>(*vop)){
                    errs() << "不是 CastInst" << "\n";
                    Instruction* inst_vop = cast<Instruction>(vop);
                    errs() << "其第一个操作数" << "\n";
                    vop = inst_vop->getOperand(0);
                    errs() << *vop << "\n"; // 第一个参数
                    if(isa<AllocaInst>(*vop)){
                        errs() << "是 AllocaInst" << "\n";
                        return fromAlloca(vop);
                        break;
                    }
                }
            } 
        }
    }
}

Value* findMalloc(Value *v){
    Value *op0 = v;
    while(isa<Instruction>(*op0)){
        errs() << "是 Instruction" << "\n";
        Instruction* inst_op0 = cast<Instruction>(op0);
        op0 = inst_op0->getOperand(0);
        errs() << "其第一个操作数" << "\n";
        errs() << *op0 << "\n"; // 第一个参数
        if(isa<AllocaInst>(*op0)){
            errs() << "是 AllocaInst" << "\n";
            return fromAlloca(op0);
        }
    }
}

// main process
ExitOnError ExitOnErr;
int main(int argc, char** argv) {
    // prepare LLVM
    InitializeNativeTarget();
    InitializeNativeTargetAsmPrinter();
    ExitOnErr.setBanner(string(argv[0]) + "error: ");
    // handle input error
    if (argc <= 1) {
        errs() << "Usage: <" << argv[0] << "> target.bc\n";
        return -1;
    }
    // handle IR .bc file
    unique_ptr<MemoryBuffer> mb = ExitOnErr(errorOrToExpected(MemoryBuffer::getFile(argv[1])));
    LLVMContext context;
    unique_ptr<Module> m = ExitOnErr(parseBitcodeFile(*mb.get(), context));
    errs() << "\nprocessing...\n";
    // vector< pair<Instruction *, Instruction *> > toDel;
    // GlobalVariable * gvar_ivt = new GlobalVariable(*m, Type::getInt64Ty(context), false, GlobalValue::ExternalLinkage, 0, "g__ivt");
    for (Function & f : (*m))
        if (!f.isDeclaration())
            for (BasicBlock & bb : f)
                for (Instruction & i : bb) {
                    // if (isa<CallInst>(&i)) { //判断指令是callInst
                    if (auto inst = dyn_cast<CallInst>(&i)) { //使用dyn_cast函数来判断指令是callInst
                        Function* called = inst->getCalledFunction(); // Returns the function called, or null if this is an indirect function invocation.
						if(called->getName().contains(StringRef("MPI_Send"))){
                            errs() << "\n";
                            inst->printAsOperand(errs());
                            errs() << " [调用函数]" << called->getName() << "\n";    // 函数名称
                            errs() << "MPI_Send第一个参数" << "\n";
                            Value *op0 = inst->getArgOperand(0);
                            errs() << *op0 << "\n"; // 第一个参数
                            // errs() << *(op0->getType()) << "\n";
                            Value* mallocValue = findMalloc(op0);
                        }else if(called->getName().contains(StringRef("malloc"))){
                            errs() << "\n" << called->getName() << "\n";    // 函数名称
                            inst->printAsOperand(errs()); errs() << "\n";   //编号
                        }
		            }
                }
    // write back IR .bc file
    errs() << "\nverifying...\n";
    verifyModule(*m, &errs());
    WriteBitcodeToFile(m.get(), outs());
    return 0;
}