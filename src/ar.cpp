#include "ar.h"

#include "blockbody.h"
#include "llvmunit.h"
#include "type.h"

ArSizeExpr::ArSizeExpr(Ar *ar) {
  this->ar = ar;
}

ArSizeExpr::~ArSizeExpr() {
}

llvm::Value *ArSizeExpr::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  return ar->sizeVal;
}

ArInitExpr::ArInitExpr(Ar *ar) {
  this->ar = ar;
}

ArInitExpr::~ArInitExpr() {
}

llvm::Value *ArInitExpr::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  ar->sizeVal = ChildValue(u, socks[0]);
  auto *at = llvm::ArrayType::get(ar->type == INT ? u->IntType() : u->StrType(), dynamic_cast<llvm::ConstantInt *>(ar->sizeVal)->getSExtValue());
  auto *gv = new llvm::GlobalVariable(*u->module, at, false, llvm::GlobalValue::ExternalLinkage, 0);
  gv->setAlignment(4);
  auto* all0 = llvm::ConstantAggregateZero::get(at);
  gv->setInitializer(all0);
  ar->var = gv;
  return NULL;
}

ArSetExpr::ArSetExpr(Ar *ar) {
  this->ar = ar;
}

ArSetExpr::~ArSetExpr() {
}

llvm::Value *ArSetExpr::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  std::vector<llvm::Value *> idxVals;
  idxVals.push_back(llvm::ConstantInt::get(*u->context, llvm::APInt(32, 0)));
  idxVals.push_back(ChildValue(u, socks[0]));
  return u->builder->CreateStore(ChildValue(u, socks[1]), u->builder->CreateGEP(ar->var, idxVals));
}

ArValExpr::ArValExpr(Ar *ar) {
  this->ar = ar;
}

ArValExpr::~ArValExpr() {
}

llvm::Value *ArValExpr::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  std::vector<llvm::Value *> idxVals;
  idxVals.push_back(llvm::ConstantInt::get(*u->context, llvm::APInt(32, 0)));
  idxVals.push_back(ChildValue(u, socks[0]));
  return u->builder->CreateLoad(u->builder->CreateGEP(ar->var, idxVals));
}

ArInit::ArInit(BlockBody *body, Ar *ar) : Block(25, 15, body) {
  this->ar = ar;
  fflag = true;
}

ArInit::~ArInit() {
}

ArSize::ArSize(BlockBody *body, Ar *ar) : Proto(0, 0, body) {
  this->ar = ar;
}

ArSize::~ArSize() {
}

ArSet::ArSet(BlockBody *body, Ar *ar) : Proto(0, 0, body) {
  this->ar = ar;
}

ArSet::~ArSet() {
}

ArVal::ArVal(BlockBody *body, Ar *ar) : Proto(0, 0, body) {
  this->ar = ar;
}

ArVal::~ArVal() {
}

Ar::Ar(LLVMUnit *u, const char *name, DataType type) {
  this->name = name;
  this->type = type;
  
  std::vector<std::pair<const char *, std::vector<DataType>>> infos;
  size = new ArSize(new BlockBody((std::string(name) + ".size").c_str(), type, infos, new ArSizeExpr(this)), this);
  infos.push_back({"size", {INT}});
  init = new ArInit(new BlockBody((std::string(name) + ".init").c_str(), VOID, infos, new ArInitExpr(this)), this);
  infos.clear();
  infos.push_back({"index", {INT}});
  infos.push_back({"val", {type}});
  set = new ArSet(new BlockBody((std::string(name) + ".set").c_str(), VOID, infos, new ArSetExpr(this)), this);
  infos.clear();
  infos.push_back({"index", {INT}});
  val = new ArVal(new BlockBody((std::string(name) + ".val").c_str(), type, infos, new ArValExpr(this)), this);
}

Ar::~Ar() {
  delete init->Body();
}
