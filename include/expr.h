#pragma once

#include "llvm.h"
#include <vector>

class ArgSocket;
class LLVMUnit;
enum DataType : int;

class Expression {
public:
  Expression();
  virtual ~Expression();

  llvm::Value *ChildValue(LLVMUnit *u, ArgSocket *sock);

  virtual llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) = 0;
};

class Integer : public Expression {
public:
  Integer();
  virtual ~Integer();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);
};

class String : public Expression {
public:
  String();
  virtual ~String();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);
};

class Operation : public Expression {
public:
  Operation(char opcode);
  virtual ~Operation();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);

private:
  char opcode;
};

class Print : public Expression {
public:
  static llvm::Value *IntStr;
  static llvm::Value *StrStr;
  static llvm::Function *Func;
  static llvm::Function *ScanFunc;
  static void Setup(LLVMUnit *u);

  Print(DataType type);
  virtual ~Print();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);

private:
  DataType type;
};

class Delay : public Expression {
public:
  Delay();
  virtual ~Delay();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);
};

class If : public Expression {
public:
  If();
  virtual ~If();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);
};

class IfVal : public Expression {
public:
  IfVal(DataType dt);
  virtual ~IfVal();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);

  DataType dt;
};

class Loop : public Expression {
public:
  Loop();
  virtual ~Loop();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);
};

/*
class Scan : public Expression {
public:
  Scan(DataType type);
  virtual ~Scan();

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks);

private:
  DataType type;
};
*/
