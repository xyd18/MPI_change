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

Value* fromAlloca(Value *alloca);
Value* findMalloc(Value *v);

// Value* findNearestUser(Value *v, Instruction *inst){
//     // 找到 inst 前面最近的一条 StoreInst 或 CallInst

//     return null;
// }


Value* fromFunc(Function* func, int argsNum){
    errs() << "它的 User 有" << "\n";
    for(auto u = func->user_begin(); u != func->user_end(); u++){        
        if(isa<CallInst>(*u)){
            errs() << "是 CallInst的" << argsNum <<"个参数\n";
            CallInst* call_inst = cast<CallInst>(*u);
            Value* arg = call_inst->getArgOperand(argsNum);
            errs() << *arg << "\n"; // 第一个参数
            return findMalloc(arg);
        }
    }
}

Value* fromLoad(Value *load){
    return fromAlloca(load);
}

Value* fromAlloca2(Value *alloca){
    errs() << "它的 User 有" << "\n";
    for(auto u = alloca->user_begin(); u != alloca->user_end(); u++){
        u->printAsOperand(errs()); errs() << "\n";
        if(isa<LoadInst>(*u)){
            errs() << "是 LoadInst" << "\n";
            LoadInst* load_inst = cast<LoadInst>(*u);
            return fromLoad(load_inst);
        }
    }
}

Value* fromStore2(StoreInst *store){
    Value* vop = store->getPointerOperand();
    errs() << "其第二个操作数" << "\n";
    errs() << *vop << "\n";
    if(isa<AllocaInst>(*vop)){
        errs() << "是 AllocaInst" << "\n";
        return fromAlloca2(vop);
    }
}

Value* fromStore(StoreInst *store){
    Value* vop = store->getValueOperand();
    errs() << "其第一个操作数 类型是" << *(vop->getType()) << "\n";
    errs() << *vop << "\n";
    
    if(isa<CastInst>(*vop)){
        errs() << "是 CastInst" << "\n";
        Value* cv = cast<CastInst>(*vop).getOperand(0);
        errs() << "应该是malloc那条instruction了" << *cv << "\n"; // 第一个参数
        return cv;
    }else if(isa<CallInst>(*vop)){
        errs() << "是 CallInst" << "\n";
        Function* called = cast<CallInst>(vop)->getCalledFunction(); // Returns the function called, or null if this is an indirect function invocation.
        if(called->getName().contains(StringRef("malloc"))){
            errs() << "应该是malloc那条instruction了" << *vop << "\n"; // 第一个参数
            return vop;
        }else{
            errs() << "是 CallInst 调用的函数返回值" << "\n";
            return findMalloc(vop);
        }
    }
    else if(isa<Instruction>(*vop)){
        errs() << "不是 CastInst 也不是 CallInst" << "\n";
        bool isArg = false;
        Instruction *inst = cast<Instruction>(vop);
        Function* func = store->getFunction();
        errs() << "函数名称 " << func->getName() << "\n";
        int argsNum = 0;
        for(auto args = func->arg_begin(); args != func->arg_end(); args++)
            if(cast<Value>(vop) == cast<Value>(args)){
                isArg = true;
                errs() << cast<Value>(*args) << "\n";
                argsNum = args-func->arg_begin();
                errs() << argsNum << "\n";
            }
        if(isArg){
            errs() << "是函数参数,下面找该函数在哪里被调用的" << "\n";
            return fromFunc(func, argsNum);
        }else{
            errs() << "也不是函数参数" << "\n";
            while(isa<Instruction>(*vop)){
                Instruction* inst_vop = cast<Instruction>(vop);
                errs() << "其第一个操作数" << "\n";
                vop = inst_vop->getOperand(0);
                errs() << *vop << "\n"; // 第一个参数
                if(isa<AllocaInst>(*vop)){
                    errs() << "是 AllocaInst" << "\n";
                    return fromAlloca(vop);
                    break;
                }else if(isa<GlobalValue>(*vop)){
                    errs() << "是 GlobalValue" << "\n";
                    return fromAlloca(vop);
                    break;
                }
            }
        }
        
    }
}


Value* fromAlloca(Value *alloca){
    errs() << "它的 User 有" << "\n";
    for(auto u = alloca->user_begin(); u != alloca->user_end(); u++){
        u->printAsOperand(errs()); errs() << "\n";
        if(isa<StoreInst>(*u)){
            errs() << "是 StoreInst" << "\n";
            StoreInst* store_inst = cast<StoreInst>(*u);
            return fromStore(store_inst);
        }else if(isa<CallInst>(*u)){
            CallInst* calli = cast<CallInst>(*u);
            errs() << "是 CallInst 的某个参数?" << "\n";
            Function* called = calli->getCalledFunction(); // Returns the function called, or null if this is an indirect function invocation.
            errs() << "调用了" << called->getName() << "\n";
            if(called->getName().contains(StringRef("posix_memalign"))){
                errs() << "应该是 posix_memalign 那条instruction了" << *calli << "\n"; // 第一个参数
                return calli;
            }
            int argsNum = 0;
            for(auto args = calli->arg_begin(); args != calli->arg_end(); args++){
                if(isa<AllocaInst>(*args)){
                    if(cast<Value>(*args) == alloca){
                        argsNum = args-calli->arg_begin();
                        errs() <<"是第 " <<argsNum << "个参数\n";

                    }
                }
            }
            Argument* arg = called->arg_begin() + argsNum;
            for(auto u_arg = arg->user_begin(); u_arg != arg->user_end(); u_arg++){
                u_arg->printAsOperand(errs()); errs() << "\n";
                if(isa<StoreInst>(*u_arg)){
                    errs() << "是 StoreInst" << "\n";
                    StoreInst* store_inst = cast<StoreInst>(*u_arg);
                    Value* vop = store_inst->getPointerOperand();
                    errs() << "其第二个操作数" << "\n";
                    errs() << *vop << "\n";
                    if(store_inst->getValueOperand() == cast<Value>(arg)){
                        return fromStore2(store_inst);
                    }
                }
            }
            // errs() << "函数内容" << "\n";
            // // unsigned int number = u - calli->arg_begin();
            // // errs() << number << "\n";
            // for (BasicBlock & bb : *called)
            //     for (Instruction & i : bb) {
            //         errs() << i << "\n";
            //         if(auto storeinst = dyn_cast<StoreInst>(&i)){
            //             Value* vop = storeinst->getValueOperand();
            //             errs() << "其第一个操作数" << "\n";
            //             errs() << *vop << "\n";
            //             if(vop == alloca){
            //                 errs() << "yes" << "\n";
            //             }
            //         }
            //     }
        }
    }
}


int isFunctionArg(Value* v, Function* func){
    // 判断是不是函数参数，是第几个参数
    int argNum = 0;
    bool isArg = false;

    for(auto args = func->arg_begin(); args != func->arg_end(); args++){
        if(cast<Value>(v) == cast<Value>(args)){
            isArg = true;
            // errs() << cast<Value>(*args) << "\n";
            argNum = args - func->arg_begin();
            errs() << " 是函数 " << func->getName() << " 的第 "<< argNum << " 个参数\n";
            break;
        }
    }

    if(isArg)
        return argNum;
    else
        return -1;
}

Value* toI8Ptr(Value* v){
    // v : i8*
    if(isa<CallInst>(*v)){

    }else if(isa<LoadInst>(*v)){

    }else if(isa<CastInst>(*v) || isa<GetElementPtrInst>(*v)){

    }else{
        // 判断是不是当前函数的参数
        if(isa<Instruction>(*v)){
            Function* func = cast<Instruction>(v)->getFunction();
            int ArgNum = isFunctionArg(v, func);
            if(ArgNum == -1){
                // 不是
            }else{
                // 是

            }
        }else{

        }

    }
}


Value* findMalloc(Value *v){
    
    Value *op0 = v;
    if(isa<CallInst>(*op0)){
        CallInst* call_inst = cast<CallInst>(op0);
        Function* called = call_inst->getCalledFunction(); // Returns the function called, or null if this is an indirect function invocation.
        errs() << "是 CallInst 的返回值, CallInst 调用了" << called->getName() << "\n";
        if(called->getName().contains(StringRef("malloc"))){
            return op0;
        }
        for (BasicBlock & bb : *called){
            for (Instruction & i : bb) {
                errs() << i << "\n";
                if(auto inst = dyn_cast<ReturnInst>(&i)){
                    errs() << "是 ReturnInst 返回了" << "\n";
                    Value* rv = inst->getReturnValue();
                    errs() << *rv << "\n";
                    if(isa<LoadInst>(*rv)){
                        errs() << "是 LoadInst" << "\n";
                        Value* vv = cast<Instruction>(rv)->getOperand(0);
                        errs() << *vv << "\n";
                        errs() << *(vv->getType()) << "\n";
                        for(auto u = vv->user_begin(); u != vv->user_end(); u++){
                            u->printAsOperand(errs()); errs() << "\n";
                            if(isa<StoreInst>(*u)){
                                errs() << "是 StoreInst" << "\n";
                                StoreInst* store_inst = cast<StoreInst>(*u);
                                return fromStore(store_inst);
                            }
                        }
                    }else{
                        // 找找这个返回值与哪个参数或全局变量有关
                        bool isArg = false;
                        int argsNum = 0;
                        while(isa<Instruction>(*rv)){
                            Instruction* inst_rv = cast<Instruction>(rv);
                            rv = inst_rv->getOperand(0);
                            errs() << inst_rv->getOpcodeName() << "\n";
                            errs() << "其第一个操作数" << "\n";
                            errs() << *rv << "\n"; // 第一个参数
                            if(isa<AllocaInst>(*rv)){
                                errs() << "是 AllocaInst" << "\n";
                                errs() << "AllocaInst 的 User 有" << "\n";
                                for(auto u = rv->user_begin(); u != rv->user_end(); u++){
                                    u->printAsOperand(errs()); errs() << "\n";
                                    if(isa<StoreInst>(*u)){
                                        errs() << "是 StoreInst" << "\n";
                                        StoreInst* store_inst = cast<StoreInst>(*u);
                                        Value* vop = store_inst->getValueOperand();
                                        errs() << "其第一个操作数" << "\n";
                                        errs() << *vop << "\n";
                                        // 判断是不是函数参数
                                        Function* func = store_inst->getFunction();
                                        argsNum = isFunctionArg(vop, func);
                                        if(argsNum != -1){
                                            isArg = true;
                                            break;
                                        }
                                    }
                                }
                                if(isArg)
                                    break;
                            }
                        }
                        if(isArg){
                            Value* realV = call_inst->getArgOperand(argsNum);
                            errs() << "参数是 " << *realV << "\n";
                            if(isa<LoadInst>(*realV)){
                                errs() << "是 LoadInst" << "\n";
                                Value* vv = cast<Instruction>(realV)->getOperand(0);
                                errs() << *vv << "\n";
                                errs() << *(vv->getType()) << "\n";
                                for(auto u = vv->user_begin(); u != vv->user_end(); u++){
                                    u->printAsOperand(errs()); errs() << "\n";
                                    if(isa<StoreInst>(*u)){
                                        errs() << "是 StoreInst" << "\n";
                                        StoreInst* store_inst = cast<StoreInst>(*u);
                                        bool yes = true;
                                        if(store_inst->getParent() == cast<Instruction>(realV)->getParent()){
                                            BasicBlock* parent = store_inst->getParent();
                                            unsigned Order = 0;
                                            for (Instruction &I : *parent){
                                                if(&I == store_inst){
                                                    yes = true;
                                                    break;
                                                }else if(&I == cast<Instruction>(realV)){
                                                    yes = false;
                                                    errs() << "不是这个 Store!!!" << "\n";
                                                    break;
                                                }
                                            }
                                        }
                                        if(yes){
                                            return fromStore(store_inst);
                                        }else{
                                            errs() << "不是这个 Store!!!" << "\n";
                                            continue;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
            return op0; // 瞎写的！之后改!
        }
    }else{
        while(isa<Instruction>(*op0)){
            errs() << "是 Instruction 且不是 CallInst" << "\n";
            Instruction* inst_op0 = cast<Instruction>(op0);
            op0 = inst_op0->getOperand(0);
            errs() << "其第一个操作数 类型是" << *(op0->getType()) << "\n";
            errs() << *op0 << "\n"; // 第一个操作数
            if(isa<AllocaInst>(*op0)){
                errs() << "是 AllocaInst" << "\n";
                return fromAlloca(op0);
            }
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
                    if (auto inst = dyn_cast<CallInst>(&i)) { //使用dyn_cast函数来判断指令是callInst
                        Function* called = inst->getCalledFunction(); // Returns the function called, or null if this is an indirect function invocation.
						if(called->getName().contains(StringRef("MPI_Send"))){
                            errs() << "\n";
                            inst->printAsOperand(errs()); errs() << " [调用函数]" << called->getName() << "\n";    // 函数名称
                            Value *op0 = inst->getArgOperand(0); // MPI_Send第一个参数
                            errs() << "MPI_Send第一个参数" << " 类型是" << *(op0->getType()) << "\n";
                            errs() << *op0 << "\n";
                            Value* mallocValue = findMalloc(op0);
                            // TODO: 需要换成另一个函数
                        }else if(called->getName().contains(StringRef("malloc"))){
                            // errs() << "\n" << called->getName() << "\n";    // 函数名称
                            // inst->printAsOperand(errs()); errs() << "\n";   //编号
                        }
		            }
                }
    // write back IR .bc file
    errs() << "\nverifying...\n";
    verifyModule(*m, &errs());
    WriteBitcodeToFile(m.get(), outs());
    return 0;
}