#pragma once

#include "block.h"
#include "proto.h"
#include "expr.h"
#include <vector>

class Ar;

class ArSizeExpr : public Expression {
public:
  ArSizeExpr(Ar *ar);
  virtual ~ArSizeExpr();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);

  Ar *ar;
};

class ArInitExpr : public Expression {
public:
  ArInitExpr(Ar *ar);
  virtual ~ArInitExpr();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);

  Ar *ar;
};

class ArSetExpr : public Expression {
public:
  ArSetExpr(Ar *ar);
  virtual ~ArSetExpr();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);

  Ar *ar;
};

class ArValExpr : public Expression {
public:
  ArValExpr(Ar *ar);
  virtual ~ArValExpr();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);

  Ar *ar;
};

class ArInit : public Block {
public:
  ArInit(BlockBody *body, Ar *ar);
  virtual ~ArInit();

  Ar *ar;
};

class ArSize : public Proto {
public:
  ArSize(BlockBody *body, Ar *var);
  virtual ~ArSize();

  Ar *ar;
};

class ArSet : public Proto {
public:
  ArSet(BlockBody *body, Ar *var);
  virtual ~ArSet();

  Ar *ar;
};

class ArVal : public Proto {
public:
  ArVal(BlockBody *body, Ar *var);
  virtual ~ArVal();

  Ar *ar;
};

class Ar {
public:
  Ar(LLVMUnit *u, const char *name, DataType type);
  virtual ~Ar();

  DataType type;
  const char *name;
  ArInit *init;
  ArSize *size;
  ArSet *set;
  ArVal *val;
  llvm::Value *var;
  llvm::Value *sizeVal;
};
