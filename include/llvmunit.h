#pragma once

#include "llvm.h"
#include <vector>

class LLVMUnit {
public:
  LLVMUnit(const char *moduleName, llvm::LLVMContext *context);
  virtual ~LLVMUnit();

  llvm::Function *NewFunc(const char *name, llvm::Type *retType,
			  std::vector<llvm::Type *> &argTypes, bool isVar);
  llvm::Function *DecFunc(const char *name, llvm::Type *retType,
			  std::vector<llvm::Type *> &argTypes, bool isVar);

  llvm::BasicBlock *NewBB(const char *name, llvm::Function *func);
  void SwitchBB(llvm::BasicBlock *bb);

  llvm::Type *IntType();
  llvm::Type *StrType();
  llvm::Type *VoidType();

  llvm::Module *module;
  llvm::IRBuilder<> *builder;
  llvm::LLVMContext *context;

private:
  llvm::Type *voidType;
  llvm::Type *intType;
  llvm::Type *strType;
};
