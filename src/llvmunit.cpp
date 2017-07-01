#include "llvmunit.h"

LLVMUnit::LLVMUnit(const char *moduleName, llvm::LLVMContext *context) {
  module = new llvm::Module(moduleName, *context);
  builder = new llvm::IRBuilder<>(*context);
  this->context = context;

  intType = llvm::Type::getInt32Ty(*context);
  strType = llvm::Type::getInt8Ty(*context)->getPointerTo();
  voidType = llvm::Type::getVoidTy(*context);
}

LLVMUnit::~LLVMUnit() {
  delete module;
  delete builder;
}

llvm::Function *LLVMUnit::NewFunc(const char *name, llvm::Type *retType,
				  std::vector<llvm::Type *> &argTypes, bool isVar) {
  return llvm::Function::Create(llvm::FunctionType::get(retType, argTypes, isVar),
				llvm::Function::ExternalLinkage, name, module);
}

llvm::Function *LLVMUnit::DecFunc(const char *name, llvm::Type *retType,
				  std::vector<llvm::Type *> &argTypes, bool isVar) {
  module->getOrInsertFunction(name, llvm::FunctionType::get(retType, argTypes, isVar));
  return module->getFunction(name);
}

llvm::BasicBlock *LLVMUnit::NewBB(const char *name, llvm::Function *func) {
  return llvm::BasicBlock::Create(*context, name, func);
}

void LLVMUnit::SwitchBB(llvm::BasicBlock *bb) {
  builder->SetInsertPoint(bb);
}

llvm::Type *LLVMUnit::VoidType() {
  return voidType;
}

llvm::Type *LLVMUnit::IntType() {
  return intType;
}

llvm::Type *LLVMUnit::StrType() {
  return strType;
}
