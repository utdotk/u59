#include "expr.h"

#include "block.h"
#include "blockbody.h"
#include "llvmunit.h"
#include "type.h"

#include <cstdlib>
#include <iostream>

class ArgChan : public Expression {
public:
  ArgChan(llvm::Argument *arg) {
    this->arg = arg;
  }
  virtual ~ArgChan() {
  }

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
    return arg;
  }

  llvm::Argument *arg;
};

Expression::Expression() {
}

Expression::~Expression() {
}

llvm::Type *Yeah(DataType dt, LLVMUnit *u) {
  return dt == INT ? u->IntType() : dt == STRING ? u->StrType() : u->VoidType();
}

int KazoeMax(ArgSocket *sock) {
  if (sock->Src() == NULL) {
    return 1;
  }
  int sum = -(int)(sock->Info()->Type()->ArgTypes().size());
  for (auto *s : sock->Src()->Socks()) {
    sum +=  KazoeMax(s);
  }
  return sum;
}

void Kuttsuke(ArgSocket *sock, std::vector<Block *> &mouiya, int *idx) {
  for (auto *s : sock->Src()->Socks()) {
    if (s->Src() == NULL) {
      s->Link(mouiya[*idx]);
      (*idx)++;
      if (*idx == (int)(mouiya.size())) {
	return;
      }
    }
  }
  for (auto *s : sock->Src()->Socks()) {
    if (KazoeMax(s) != 0) {
      Kuttsuke(s, mouiya, idx);
    }
  }
}

llvm::Value *Expression::ChildValue(LLVMUnit *u, ArgSocket *sock) {
  if (sock->Info()->Type()->RetType() == VOID || !sock->Info()->Type()->ArgTypes().empty()) {
    auto *nowBB = u->builder->GetInsertBlock();

    std::vector<llvm::Type *> argTypes;
    for (auto dt : sock->Info()->Type()->ArgTypes()) {
      argTypes.push_back(Yeah(dt, u));
    }
    auto *f = u->NewFunc("", Yeah(sock->Info()->Type()->RetType(), u), argTypes, false);
    u->SwitchBB(u->NewBB("entry", f));

    std::vector<Block *> mouiya;
    for (auto &arg : f->args()) {
      std::vector<std::pair<const char *, std::vector<DataType>>> info;
      mouiya.push_back(new Block(-100, -100, new BlockBody("", DUMMY, info, new ArgChan(&arg))));
    }
    int idx = 0;
    Kuttsuke(sock, mouiya, &idx);

    if (sock->Info()->Type()->RetType() != VOID) {
      u->builder->CreateRet(sock->Src()->Body()->Expr()->LLVMValue(u, sock->Src()->Socks()));
    } else {
      sock->Src()->Body()->Expr()->LLVMValue(u, sock->Src()->Socks());
      u->builder->CreateRetVoid();
    }

    for (auto *m : mouiya) {
      m->Delink();
      delete m;
    }
    u->SwitchBB(nowBB);
    return f;
  }
  return sock->Src()->Body()->Expr()->LLVMValue(u, sock->Src()->Socks());
}

/*
llvm::Value *Expression::ChildValue(LLVMUnit *u, ArgSocket *sock) {
  return sock->Src()->Body()->Expr()->LLVMValue(u, sock->Src()->Socks());
}
*/

Integer::Integer() {
}

Integer::~Integer() {
}

llvm::Value *Integer::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  return llvm::ConstantInt::get(*u->context, llvm::APInt(32, *(int *)(socks[0])));
}

String::String() {
}

String::~String() {
}

llvm::Value *String::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  return u->builder->CreateGlobalStringPtr((const char *)socks[0]);
}

Operation::Operation(char opcode) {
  this->opcode = opcode;
}

Operation::~Operation() {
}

llvm::Value *AAAA(LLVMUnit *u, llvm::Value *v) {
  return u->builder->CreateZExtOrTrunc(v, u->IntType());
}

llvm::Value *BBBB(LLVMUnit *u, llvm::Value *v) {
  return u->builder->CreateZExtOrTrunc(v, llvm::Type::getInt1Ty(*u->context));
}

llvm::Value *Operation::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  auto *l = ChildValue(u, socks[0]);
  auto *r = ChildValue(u, socks[1]);

  switch (opcode) {
  case '+':
    return u->builder->CreateAdd(l, r);
  case '-':
    return u->builder->CreateSub(l, r);
  case '*':
    return u->builder->CreateMul(l, r);
  case '/':
    return u->builder->CreateSDiv(l, r);
  case '%':
    return u->builder->CreateSRem(l, r);
  case '=':
    return AAAA(u, u->builder->CreateICmpEQ(l, r));
  case '<':
    return AAAA(u, u->builder->CreateICmpSLT(l, r));
  case 's':
    return AAAA(u, u->builder->CreateICmpSLE(l, r));
  }
  return NULL;
}

llvm::Value *Print::IntStr = NULL;
llvm::Value *Print::StrStr = NULL;
llvm::Function *Print::Func = NULL;
llvm::Function *Print::ScanFunc = NULL;

void Print::Setup(LLVMUnit *u) {
  IntStr = u->builder->CreateGlobalStringPtr("%d\n");
  StrStr = u->builder->CreateGlobalStringPtr("%s\n");
  std::vector<llvm::Type *> argTypes({u->StrType()});
  Func = u->DecFunc("printf", u->IntType(), argTypes, true);
  ScanFunc = u->DecFunc("scanf", u->IntType(), argTypes, true);
}

Print::Print(DataType type) {
  this->type = type;
}

Print::~Print() {
}

llvm::Value *Print::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  std::vector<llvm::Value *> argVals;
  if (type == INT) {
    argVals.push_back(IntStr);
  } else if (type == STRING) {
    argVals.push_back(StrStr);
  }
  argVals.push_back(ChildValue(u, socks[0]));
  return u->builder->CreateCall(Func, argVals);
}

Delay::Delay() {
}

Delay::~Delay() {
}

llvm::Value *Delay::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  u->builder->CreateCall(ChildValue(u, socks[0]));
  u->builder->CreateCall(ChildValue(u, socks[1]));
  return NULL;
}

If::If() {
}

If::~If() {
}

llvm::Value *If::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  auto *nowBB = u->builder->GetInsertBlock();
  auto *nowFunc = nowBB->getParent();
  std::vector<llvm::Type *> argTypes;

  /*
  auto *thenFunc = u->NewFunc("", u->VoidType(), argTypes, false);
  auto *thenFBB = u->NewBB("", thenFunc);
  u->SwitchBB(thenFBB);
  ChildValue(u, socks[1]);
  u->builder->CreateRetVoid();

  auto *elseFunc = u->NewFunc("", u->VoidType(), argTypes, false);
  auto *elseFBB = u->NewBB("", elseFunc);
  u->SwitchBB(elseFBB);
  ChildValue(u, socks[2]);
  u->builder->CreateRetVoid();
  */

  //u->SwitchBB(nowBB);
  auto *condVal = BBBB(u, ChildValue(u, socks[0]));
  auto *thenBB = u->NewBB("then", nowFunc);
  auto *elseBB = llvm::BasicBlock::Create(*u->context, "else");
  auto *margeBB = llvm::BasicBlock::Create(*u->context, "ifcont");
  u->builder->CreateCondBr(condVal, thenBB, elseBB);

  std::vector<llvm::Value *> argVals;

  u->SwitchBB(thenBB);
  u->builder->CreateCall(ChildValue(u, socks[1]));
  //auto *thenVal = u->builder->CreateCall(thenFunc, argVals);
  u->builder->CreateBr(margeBB);
  thenBB = u->builder->GetInsertBlock();

  nowFunc->getBasicBlockList().push_back(elseBB);
  u->SwitchBB(elseBB);
  u->builder->CreateCall(ChildValue(u, socks[2]));
  //auto *elseVal = u->builder->CreateCall(elseFunc, argVals);
  u->builder->CreateBr(margeBB);
  elseBB = u->builder->GetInsertBlock();

  nowFunc->getBasicBlockList().push_back(margeBB);
  u->SwitchBB(margeBB);

  return NULL;
}

IfVal::IfVal(DataType dt) {
  this->dt = dt;
}

IfVal::~IfVal() {
}

llvm::Value *IfVal::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  auto *nowBB = u->builder->GetInsertBlock();
  auto *nowFunc = nowBB->getParent();
  std::vector<llvm::Type *> argTypes;

  /*
  auto *thenFunc = u->NewFunc("", dt == INT ? u->IntType() : u->StrType(), argTypes, false);
  auto *thenFBB = u->NewBB("", thenFunc);
  u->SwitchBB(thenFBB);
  u->builder->CreateRet(ChildValue(u, socks[1]));

  auto *elseFunc = u->NewFunc("", dt == INT ? u->IntType() : u->StrType(), argTypes, false);
  auto *elseFBB = u->NewBB("", elseFunc);
  u->SwitchBB(elseFBB);
  u->builder->CreateRet(ChildValue(u, socks[2]));
  */

  //u->SwitchBB(nowBB);
  auto *condVal = BBBB(u, ChildValue(u, socks[0]));
  auto *thenBB = u->NewBB("then", nowFunc);
  auto *elseBB = llvm::BasicBlock::Create(*u->context, "else");
  auto *margeBB = llvm::BasicBlock::Create(*u->context, "ifcont");
  u->builder->CreateCondBr(condVal, thenBB, elseBB);

  std::vector<llvm::Value *> argVals;

  u->SwitchBB(thenBB);
  auto *thenVal = ChildValue(u, socks[1]);//u->builder->CreateCall(thenFunc, argVals);
  u->builder->CreateBr(margeBB);
  thenBB = u->builder->GetInsertBlock();

  nowFunc->getBasicBlockList().push_back(elseBB);
  u->SwitchBB(elseBB);
  auto *elseVal = ChildValue(u, socks[2]);//u->builder->CreateCall(elseFunc, argVals);
  u->builder->CreateBr(margeBB);
  elseBB = u->builder->GetInsertBlock();

  nowFunc->getBasicBlockList().push_back(margeBB);
  u->SwitchBB(margeBB);
  
  auto *PN = u->builder->CreatePHI(dt == INT ? u->IntType() : u->StrType(), 2, "");
  PN->addIncoming(thenVal, thenBB);
  PN->addIncoming(elseVal, elseBB);
  return PN;
}

Loop::Loop() {
}

Loop::~Loop() {
}

llvm::Value *Loop::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  auto *nowBB = u->builder->GetInsertBlock();
  auto *nowFunc = nowBB->getParent();
  std::vector<llvm::Type *> argTypes;

  auto *condVal = BBBB(u, ChildValue(u, socks[0]));
  auto *loopBB = u->NewBB("loop", nowFunc);
  auto *afterloopBB = llvm::BasicBlock::Create(*u->context, "afterloop");
  u->builder->CreateCondBr(condVal, loopBB, afterloopBB);

  u->SwitchBB(loopBB);
  u->builder->CreateCall(ChildValue(u, socks[1]));
  u->builder->CreateCondBr(BBBB(u, ChildValue(u, socks[0])), loopBB, afterloopBB);
  loopBB = u->builder->GetInsertBlock();

  nowFunc->getBasicBlockList().push_back(afterloopBB);
  u->SwitchBB(afterloopBB);

  return NULL;
}

/*
Scan::Scan(DataType type) {
  this->type = type;
}

Scan::~Scan() {
}

llvm::Value *Scan::LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
  std::vector<llvm::Value *> argVals;
  if (type == INT) {
    argVals.push_back(IntStr);
  } else if (type == STRING) {
    argVals.push_back(StrStr);
  }
  argVals.push_back(socks[0]->Src()->Body()->Expr());
  return u->builder->CreateCall(Func, argVals);
}
*/
