#include "blockbody.h"

#include "expr.h"
#include "llvmunit.h"
#include "type.h"

ArgInfo::ArgInfo(BlockBody *owner, int idx, const char *name, FuncType *type) {
  this->owner = owner;
  this->idx = idx;
  this->name = name;
  this->type = type;
}

ArgInfo::~ArgInfo() {
  delete type;
}

BlockBody *ArgInfo::Owner() {
  return owner;
}

int ArgInfo::Idx() const {
  return idx;
}

std::string &ArgInfo::Name() {
  return name;
}

FuncType *ArgInfo::Type() {
  return type;
}

void ArgInfo::SetName(const char *name) {
  this->name = name;
}

BlockBody::BlockBody(const char *name, DataType retType,
		     const std::vector<std::pair<const char *, std::vector<DataType>>> &info,
		     Expression *expr) {
  this->name = name;
  this->retType = retType;
  Set(info);
  this->expr = expr;
}

BlockBody::~BlockBody() {
  for (auto *argInfo : argInfos) {
    delete argInfo;
  }
  delete expr;
}

std::string &BlockBody::Name() {
  return name;
}

DataType BlockBody::RetType() const {
  return retType;
}

std::vector<ArgInfo *> &BlockBody::ArgInfos() {
  return argInfos;
}

int BlockBody::ArgNum() const {
  return (int)argInfos.size();
}

Expression *BlockBody::Expr() {
  return expr;
}

void BlockBody::SetName(const char *name) {
  this->name = name;
}

void BlockBody::Set(const std::vector<std::pair<const char *, std::vector<DataType>>> &infos) {
  for (auto *argInfo : argInfos) {
    delete argInfo;
  }
  argInfos.clear();

  for (int i = 0; i < infos.size(); i++) {
    auto *type = new FuncType(infos[i].second[0]);
    for (int j = 1; j < infos[i].second.size(); j++) {
      type->ArgTypes().push_back(infos[i].second[j]);
    }
    argInfos.push_back(new ArgInfo(this, i, infos[i].first, type));
  }
}
