#include "varint.h"

#include "blockbody.h"
#include "llvmunit.h"
#include "type.h"
#include <utility>

VIExpr::VIExpr(VarInt *vi) {
  this->vi = vi;
}

VIExpr::~VIExpr() {
}

llvm::Value *VIExpr::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  vi->alloca = new llvm::GlobalVariable(*u->module, vi->type == INT ? u->IntType() : u->StrType(), false, llvm::GlobalValue::ExternalLinkage, dynamic_cast<llvm::Constant *>(ChildValue(u, socks[0])));
  return NULL;
}

VSExpr::VSExpr(VarInt *vi) {
  this->vi = vi;
}

VSExpr::~VSExpr() {
}

llvm::Value *VSExpr::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  return u->builder->CreateStore(ChildValue(u, socks[0]), vi->alloca);
}

VVExpr::VVExpr(VarInt *vi) {
  this->vi = vi;
}

VVExpr::~VVExpr() {
}

llvm::Value *VVExpr::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  return u->builder->CreateLoad(vi->alloca);
}

VarInit::VarInit(BlockBody *body, VarInt *var) : Block(25, 15, body) {
  this->var = var;
  fflag = true;
}

VarInit::~VarInit() {
}

VarSet::VarSet(BlockBody *body, VarInt *var) : Proto(0, 0, body) {
  this->var = var;
}

VarSet::~VarSet() {
}

VarVal::VarVal(BlockBody *body, VarInt *var) : Proto(0, 0, body) {
  this->var = var;
}

VarVal::~VarVal() {
}

VarInt::VarInt(LLVMUnit *u, const char *name, DataType type) {
  this->name = name;
  this->type = type;
  
  std::vector<std::pair<const char *, std::vector<DataType>>> infos;
  infos.push_back({"val", {type}});
  init = new VarInit(new BlockBody((std::string(name) + ".init").c_str(), VOID, infos, new VIExpr(this)), this);
  set = new VarSet(new BlockBody((std::string(name) + ".set").c_str(), VOID, infos, new VSExpr(this)), this);
  infos.clear();
  val = new VarVal(new BlockBody((std::string(name) + ".val").c_str(), type, infos, new VVExpr(this)), this);
}

VarInt::~VarInt() {
  delete init->Body();
}
