#include "guimanager.h"

#include "guiobj.h"
#include "protomanager.h"
#include <QLineEdit>
#include <QPainter>
#include <QWidget>

#include "block.h"
#include "blockbody.h"
#include "expr.h"
#include "llvmunit.h"
#include "proto.h"
#include "type.h"

#include <iostream>

llvm::Type *Henkan(DataType dt, LLVMUnit *u) {
  return dt == INT ? u->IntType() : dt == STRING ? u->StrType() : u->VoidType();
}

class ArgMan : public Expression {
public:
  ArgMan(llvm::Argument *arg) {
    this->arg = arg;
  }
  virtual ~ArgMan() {
  }

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
    return arg;
  }

  llvm::Argument *arg;
};

class ArgFuncMan : public Expression {
public:
  ArgFuncMan(llvm::Argument *arg) {//, llvm::FunctionType *ft) {
    this->arg = arg;
    //this->ft = ft;
  }
  virtual ~ArgFuncMan() {
  }

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
    std::vector<llvm::Value *> argVals;
    for (auto *sock : socks) {
      argVals.push_back(ChildValue(u, sock));
    }
    return u->builder->CreateCall(arg, argVals);
  }

  llvm::Argument *arg;
  //llvm::FunctionType *ft;
};

class FunFun {
public:
  FunFun(Block *root, BlockBody *newBody, LLVMUnit *u) {
    this->root = root;
    baban = newBody->RetType();
    //std::vector<llvm::Type *> argTypes;
    for (auto *info : newBody->ArgInfos()) {
      if (info->Type()->RetType() == VOID ||
	  !info->Type()->ArgTypes().empty()) {
	std::vector<llvm::Type *> ats;
	for (auto at : info->Type()->ArgTypes()) {
	  ats.push_back(Henkan(at, u));
	}
	auto *fttt = llvm::FunctionType::get(Henkan(info->Type()->RetType(), u),
					     ats, false);
	argTypes.push_back(fttt->getPointerTo());
      } else {
	argTypes.push_back(Henkan(info->Type()->RetType(), u));
      }
    }
    ft = llvm::FunctionType::get(Henkan(newBody->RetType(), u), argTypes, false);
  }
  virtual ~FunFun() {
  }
  void Gen(LLVMUnit *u) {
    f = llvm::Function::Create(ft, llvm::Function::ExternalLinkage, "", u->module);

    int i = 0;
    auto argit = f->arg_begin();
    for (auto *argMan : argMen) {
      auto &argg = *argit;
      auto *arex = isVal[i] ? static_cast<Expression *>(new ArgMan(&argg)) : static_cast<Expression *>(new ArgFuncMan(&argg));
      argMan->Body()->expr = arex;
      i++;
      argit++;
    }
    
    auto *nowBB = u->builder->GetInsertBlock();
    u->SwitchBB(u->NewBB("entry", f));
    if (baban != VOID) {
      u->builder->CreateRet(root->Body()->Expr()->LLVMValue(u, root->Socks()));
    } else {
      root->Body()->Expr()->LLVMValue(u, root->Socks());
      u->builder->CreateRetVoid();
    }
    u->SwitchBB(nowBB);
  }
  DataType baban;
  Block *root;
  llvm::FunctionType *ft;
  llvm::Function *f;
  std::vector<llvm::Type *> argTypes;
  std::vector<Block *> argMen;
  std::vector<bool> isVal;
};

GUIManager::GUIManager(QWidget *owner) {
  this->owner = owner;
  state = DEFAULT;
  pointedObj = NULL;
  lastX = 0;
  lastY = 0;

  InitMain();

  textBox = new QLineEdit(owner);
  textBox->resize(81, 17);
  textBox->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  auto font = textBox->font();
  font.setPointSize(font.pointSize() - 1);
  textBox->setFont(font);
  DisableTextBox();

  stdPM = new ProtoManager(25, 63, 600);
  stdPM->CreateSTD();
  for (auto *proto : stdPM->Protos()) {
    objs.push_back(proto);
  }
}

GUIManager::~GUIManager() {
  for (auto *obj : objs) {
    delete obj;
  }
  for (auto *ff : ffs) {
    delete ff;
  }
  delete stdPM;
}

void GUIManager::InitMain() {
  unit = new LLVMUnit("u59", &llvm::getGlobalContext());
  std::vector<llvm::Type *> argTypes;
  mainFunc = unit->NewFunc("main", unit->VoidType(), argTypes, false);
  mainBB = unit->NewBB("entry", mainFunc);
  unit->SwitchBB(mainBB);
  Print::Setup(unit);
}

void GUIManager::PressKey(int key) {
  if (state == DEFAULT && key == 'R') {
    for (auto *proc : procs) {
      if (proc->fflag) {
	proc->Body()->Expr()->LLVMValue(unit, proc->Socks());
      }
    }
    for (auto *ff : ffs) {
      ff->Gen(unit);
    }
    for (auto *proc : procs) {
      if (!proc->fflag) {
	proc->Body()->Expr()->LLVMValue(unit, proc->Socks());
      }
    }
    unit->builder->CreateRetVoid();
    system("echo '' > a.ll");
    unit->module->dump();
    delete unit;
    InitMain();
    system("lli a.ll");
  }
  if (state == DEFAULT && key == Qt::Key_Return) {
    Matome();
    Update();
  }
  if (state == INPUT && key == Qt::Key_Return) {
    state = DEFAULT;
    pointedObj->Input(this, textBox->text().toStdString().c_str());
    Update();
  }
}

void GUIManager::PressMouse(int x, int y) {
  if (state == INPUT) {
    DisableTextBox();
    state = DEFAULT;
  }
  if (state == DEFAULT) {
    pointedObj = PointedObj(x, y);
    if (pointedObj != NULL) {
      state = HOLD;
      pointedObj->Click(this, x - pointedObj->X(), y - pointedObj->Y());
    }
    Update();
  }
  lastX = x;
  lastY = y;
}

void GUIManager::MoveMouse(int x, int y) {
  if (state == HOLD) {
    pointedObj->Drag(this, x - lastX, y - lastY);
    lastX = x;
    lastY = y;
    Update();
  }
}

void GUIManager::ReleaseMouse(int x, int y) {
  if (state == HOLD) {
    state = DEFAULT;
    pointedObj->Release(this, x - pointedObj->X(), y - pointedObj->Y());
    Update();
  }
}

void GUIManager::DoubleClick(int x, int y) {
  if (state == DEFAULT) {
    pointedObj = PointedObj(x, y);
    if (pointedObj != NULL) {
      state = HOLD;
      pointedObj->DoubleClick(this, x - pointedObj->X(), y - pointedObj->Y());
    }
    Update();
  }
}

void GUIManager::Draw() {
  QPainter p(owner);
  auto font = p.font();
  font.setPointSize(font.pointSize() - 1);
  p.setFont(font);

  stdPM->UpdateProtos();
  for (auto *obj : objs) {
    obj->Draw(&p);
  }

  if (!procs.empty()) {
    p.drawLine(895, 0, 895, procs.front()->T());
    for (auto it = ++procs.begin(); it != procs.end(); it++) {
      auto prev = --it;
      p.drawLine(895, (*prev)->B(), 895, (*++it)->T());
    }
    p.drawLine(895, procs.back()->B(), 895, 767);
  } else {
    p.drawLine(895, 0, 895, 767);
  }
}

void GUIManager::Update() {
  owner->update();
}

GUIObj *GUIManager::PointedObj(int x, int y) {
  for (auto *obj : objs) {
    if (obj->IsPointed(x, y)) {
      return obj;
    }
  }
  return NULL;
}

void GUIManager::EnableTextBox(int x, int y, const char *str) {
  textBox->setText(str);
  textBox->move(x, y);
  textBox->setEnabled(true);
  textBox->setVisible(true);
}

void GUIManager::DisableTextBox() {
  textBox->setEnabled(false);
  textBox->setVisible(false);
}

/*
class ArgExpr : public Expr {
};

class ArgFuncExpr : public Expr {
};
*/

class EXXX : public Expression {
public:
  EXXX(FunFun *ff) {
    this->ff = ff;
  }
  virtual ~EXXX() {
  }

  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
    std::vector<llvm::Value *> argVals;
    for (auto *sock : socks) {
      argVals.push_back(ChildValue(u, sock));
    }
    return u->builder->CreateCall(ff->f, argVals);
  }

  FunFun *ff;
};

class ArgBlock : public Block {
public:
  ArgBlock(BlockBody *body) : Block(0, 0, body) {
  }
  virtual ~ArgBlock();
};

void DFS2(ArgSocket *sock, FuncType *ft) {
  if (sock->Src()->pflag) {
    ft->ArgTypes().push_back(sock->Src()->Body()->RetType());
    return;
  }
  for (auto *s : sock->Src()->Socks()) {
    if (s->Src() != NULL) {
      DFS2(s, ft);
    }
  }
}

FuncType *AvavaType(ArgSocket *sock) {
  FuncType *ft = new FuncType(DUMMY);
  if (sock->Src() == NULL || !sock->Src()->pflag) {
    ft->retType = sock->Info()->Type()->RetType();
    for (auto dt : sock->Info()->Type()->ArgTypes()) {
      ft->ArgTypes().push_back(dt);
    }
    if (sock->Src() == NULL) {
      return ft;
    }
    DFS2(sock, ft);
  }
  return ft;
}

void PrintFT(FuncType *ft) {
  std::cout << "rt:" << ft->RetType() << std::endl;
  for (auto at : ft->ArgTypes()) {
    std::cout << "at:" << at << std::endl;
  }
}

void TsunageMatsuri(ArgSocket *sock, Block *argBlock, int *idx) {
  if (sock->Src()->pflag) {
    argBlock->Socks()[*idx]->Link(sock->Src());
    (*idx)++;
    return;
  }
  for (auto *s : sock->Src()->Socks()) {
    if (s->Src() != NULL) {
      TsunageMatsuri(s, argBlock, idx);
    }
  }
}

class ProPro : public Expression {
public:
  ProPro() {
  }
  virtual ~ProPro() {
  }
  llvm::Value *LLVMValue(LLVMUnit *u, const std::vector<ArgSocket *> &socks) {
    for (auto *sock : socks) {
      sock->Src()->Body()->Expr()->LLVMValue(u, sock->Src()->Socks());
    }
    return NULL;
  }
};

int cnt = 0;

void GUIManager::Matome() {
  // pflag ga tatta block wo atsumeru
  std::vector<Block *> pbs;
  MakePBS(pbs);

  // root list wo tsukuru
  std::vector<Block *> rts;
  cnt = 0;
  MakeRTS(pbs, rts);

  // pb wo narabe kaeru
  std::vector<Block *> pbs2;
  MakePBS2(rts, pbs2);

  if (rts.size() != 1 || cnt < (int)pbs2.size() - 1) {
    std::vector<std::pair<const char *, std::vector<DataType>>> info;
    auto it = *rts[0]->procIt;
    for (; it != *rts.back()->procIt; it++) {
      info.push_back({"proc", {VOID}});
    }
    info.push_back({"proc", {VOID}});
    auto *fb = new Block(pbs2.back()->X(), pbs2.back()->Y(), new BlockBody("", VOID, info, new ProPro));
    fb->pflag = true;
    int yeah = 0;
    for (it = *rts[0]->procIt; it != *rts.back()->procIt; it++) {
      (*it)->Link(fb->Socks()[yeah]);
      yeah++;
    }
    (*it)->Link(fb->Socks()[yeah]);
    pbs2.push_back(fb);
  }

  std::vector<FuncType *> avavas;
  for (auto *pb2 : pbs2) {
    for (auto *sock : pb2->Socks()) {
      avavas.push_back(AvavaType(sock));
    }
  }
  
  auto *root = pbs2.back();
  
  std::vector<std::pair<const char *, std::vector<DataType>>> infofo;
  for (auto *avava : avavas) {
    if (avava->RetType() != DUMMY) {
      std::vector<DataType> gyaaa;
      gyaaa.push_back(avava->RetType());
      for (auto dt : avava->ArgTypes()) {
	gyaaa.push_back(dt);
      }
      infofo.push_back({"", gyaaa});
    }
  }
  auto *bb = new BlockBody("Module", root->Body()->RetType(), infofo, NULL);
  int ididx1 = 0;
  int ididx2 = 0;
  for (auto *pb2 : pbs2) {
    for (auto *sock : pb2->Socks()) {
      if (avavas[ididx1]->RetType() != DUMMY) {
	bb->ArgInfos()[ididx2]->SetName(sock->Info()->Name().c_str());
	ididx2++;
      }
      ididx1++;
    }
  }
  
  auto *newBlock = new Block(root->X(), root->Y(), bb);
  blocks.push_back(newBlock);
  auto *ff = new FunFun(root, bb, unit);
  ffs.push_back(ff);
  bb->expr = new EXXX(ff);
  
  newBlock->Link(root->DstSock());
  objs.push_back(newBlock);
  objs.push_back(stdPM->NewProto(bb));
  int idxx = 0;
  for (auto *pb2 : pbs2) {
    if (pb2->procIt != NULL) {
      procs.erase(*pb2->procIt);
    }
    pb2->SetPos(-100, -100);
  }
  
  int i = 0;
  for (auto *pb2 : pbs2) {
    for (auto *sock : pb2->Socks()) {
      if (avavas[i]->RetType() != DUMMY) {
	std::vector<std::pair<const char *, std::vector<DataType>>> info;
	for (auto atatata : avavas[i]->ArgTypes()) {
	  info.push_back({"", {atatata}});
	}
	auto *argBlock = new Block(-100, -100, new BlockBody("", avavas[i]->RetType(), info, NULL));
	ff->argMen.push_back(argBlock);
	ff->isVal.push_back(avavas[i]->RetType() != VOID && avavas[i]->ArgTypes().empty());
	objs.push_back(argBlock);
	int idx = 0;
	if (sock->Src() != NULL) {
	  TsunageMatsuri(sock, argBlock, &idx);
	} else {
	  idx++;
	}
	newBlock->Socks()[idxx]->Link(sock->Src());
	idxx++;
	sock->Link(argBlock);
      }
      i++;
    }
  }
  
  for (auto *pb2 : pbs2) {
    pb2->pflag = false;
  }

  /*
  ff->Gen(unit);
  unit->module->dump();
  */
}

void GUIManager::MakePBS(std::vector<Block *> &pbs) {
  for (auto *block : blocks) {
    if (block->pflag) {
      pbs.push_back(block);
    }
  }
}

void GUIManager::MakeRTS(std::vector<Block *> &pbs, std::vector<Block *> &rts) {
  for (auto *pb : pbs) {
    bool flg = false;
    auto *tmpb = pb;
    while (tmpb->ProcIt() == NULL) {
      tmpb = tmpb->DstSock()->Owner();
      if (!flg && tmpb->pflag) {
	cnt++;
	flg = true;
      }
    }
    bool find = false;
    for (auto *rt : rts) {
      if (rt == tmpb) {
	find = true;
	break;
      }
    }
    if (!find) {
      rts.push_back(tmpb);
    }
  }
  for (int i = 0; i < rts.size(); i++) {
    for (int j = rts.size() - 1; j > i; j--) {
      if (rts[j]->Y() < rts[j - 1]->Y()) {
	auto *tmp = rts[j];
	rts[j] = rts[j - 1];
	rts[j - 1] = tmp;
      }
    }
  }
}

void DFSSSS(Block *pb, std::vector<Block *> &pbs2) {
  for (auto *sock : pb->Socks()) {
    if (sock->Src() != NULL) {
      DFSSSS(sock->Src(), pbs2);
    }
  }
  if (pb->pflag) {
    pbs2.push_back(pb);
  }
}

void GUIManager::MakePBS2(std::vector<Block *> &rts, std::vector<Block *> &pbs2) {
  for (auto *pb : rts) {
    DFSSSS(pb, pbs2);
  }
}
