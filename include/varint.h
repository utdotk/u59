#pragma once

#include "block.h"
#include "proto.h"
#include "expr.h"
#include <vector>

class VarInt;

class VIExpr : public Expression {
public:
  VIExpr(VarInt *vi);
  virtual ~VIExpr();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);

  VarInt *vi;
};

class VSExpr : public Expression {
public:
  VSExpr(VarInt *vi);
  virtual ~VSExpr();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);

  VarInt *vi;
};

class VVExpr : public Expression {
public:
  VVExpr(VarInt *vi);
  virtual ~VVExpr();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);

  VarInt *vi;
};

class VarInit : public Block {
public:
  VarInit(BlockBody *body, VarInt *var);
  virtual ~VarInit();

  VarInt *var;
};

class VarSet : public Proto {
public:
  VarSet(BlockBody *body, VarInt *var);
  virtual ~VarSet();

  VarInt *var;
};

class VarVal : public Proto {
public:
  VarVal(BlockBody *body, VarInt *var);
  virtual ~VarVal();

  VarInt *var;
};

class VarInt {
public:
  VarInt(LLVMUnit *u, const char *name, DataType type);
  virtual ~VarInt();

  DataType type;
  const char *name;
  VarInit *init;
  VarSet *set;
  VarVal *val;
  llvm::Value *alloca;
};
