#include "block.h"

#include "blockbody.h"
#include "guimanager.h"
#include "type.h"
#include <QPainter>

#include <iostream>

ArgSocket::ArgSocket(Block *owner, ArgInfo *info) {
  this->owner = owner;
  this->info = info;
  src = NULL;
}

ArgSocket::~ArgSocket() {
  Delink();
}

int ArgSocket::X() const {
  return owner->X();
}

int ArgSocket::Y() const {
  return owner->Y() + 24 + 16 * info->Idx();
}

void ArgSocket::Link(Block *src) {
  Delink();
  if (src != NULL) {
    src->Delink();
    src->SetDstSock(this);
    this->src = src;
  }
}

void ArgSocket::Delink() {
  if (src != NULL) {
    src->SetDstSock(NULL);
    src = NULL;
  }
}

Block *ArgSocket::Owner() {
  return owner;
}

ArgInfo *ArgSocket::Info() {
  return info;
}

Block *ArgSocket::Src() {
  return src;
}

Block::Block(int x, int y, BlockBody *body) : GUIObj(x, y, 81, 0) {
  this->body = body;
  for (auto *argInfo : body->ArgInfos()) {
    socks.push_back(new ArgSocket(this, argInfo));
  }
  UpdateSize();
  dstSock = NULL;
  procIt = NULL;
  pflag = false;
  fflag = false;
}

Block::~Block() {
  Delink();
  for (auto *sock : socks) {
    delete sock;
  }
  delete procIt;
}

void Block::UpdateSize() {
  SetSize(W(), 17 + body->ArgNum() * 16);
}

int Block::SinkX() const {
  return R();
}

int Block::SinkY() const {
  return (T() + B()) / 2;
}

BlockBody *Block::Body() {
  return body;
}

std::vector<ArgSocket *> &Block::Socks() {
  return socks;
}

ArgSocket *Block::DstSock() {
  return dstSock;
}

std::list<Block *>::iterator *Block::ProcIt() {
  return procIt;
}

void Block::SetDstSock(ArgSocket *dstSock) {
  this->dstSock = dstSock;
}

void Block::Link(ArgSocket *dstSock) {
  if (dstSock != NULL) {
    dstSock->Link(this);
  }
}

void Block::Delink() {
  if (dstSock != NULL) {
    dstSock->Delink();
  }
}

void Block::MoveWithChildren(int dx, int dy) {
  Move(dx, dy);
  for (auto *sock : socks) {
    if (sock->Src() != NULL) {
      sock->Src()->MoveWithChildren(dx, dy);
    }
  }
}

void Block::Draw(QPainter *p) const {
  DrawFrame(p);
  if (pflag) {
    DrawPFrame(p);
  }
  DrawPart(p);
  DrawName(p);
  DrawArgs(p);
  DrawArrow(p);
}

void Block::DrawPFrame(QPainter *p) const {
  p->drawLine(L() - 2, T() - 2, R() + 2, T() - 2);
  p->drawLine(R() + 2, T() - 2, R() + 2, B() + 2);
  p->drawLine(R() + 2, B() + 2, L() - 2, B() + 2);
  p->drawLine(L() - 2, B() + 2, L() - 2, T() - 2);
}

void Block::DrawPart(QPainter *p) const {
  p->drawLine(L() + 3, Y() + 16, R() - 3, Y() + 16);
  int partY = 8;
  for (auto *argInfo : body->ArgInfos()) {
    partY += 16;
    p->drawLine(L() + 4, Y() + partY, L() + 6, Y() + partY);
  }
}

void Block::DrawName(QPainter *p) const {
  auto fm = p->fontMetrics();
  const auto *name = body->Name().c_str();
  p->drawText(X() + 40 - fm.width(name) / 2 + 1,
              Y() + 8 + fm.height() / 2 - 3, name);
}

void Block::DrawArgs(QPainter *p) const {
  auto fm = p->fontMetrics();
  int partY = Y();
  for (auto *argInfo : body->ArgInfos()) {
    partY += 16;
    auto fm = p->fontMetrics();
    const auto *name = argInfo->Name().c_str();
    p->drawText(X() + 40 - fm.width(name) / 2 + 1,
		partY + 8 + fm.height() / 2 - 3, name);
  }
}

void Block::DrawArrow(QPainter *p) const {
  if (dstSock != NULL) {
    int centerX = (SinkX() + dstSock->X()) / 2;
    p->drawLine(SinkX(), SinkY(), centerX, SinkY());
    p->drawLine(centerX, SinkY(), centerX, dstSock->Y());
    p->drawLine(centerX, dstSock->Y(), dstSock->X(), dstSock->Y());
  }
}

void Block::Click(GUIManager *mng, int rx, int ry) {
  if (procIt != NULL) {
    mng->procs.erase(*procIt);
    delete procIt;
    procIt = NULL;
  }
}

void Block::Drag(GUIManager *mng, int dx, int dy) {
  MoveWithChildren(dx, dy);
  AutoLink(mng->blocks);
}

void Block::Release(GUIManager *mng, int rx, int ry) {
  if (body->RetType() == VOID || body->RetType() == IFTYPE) {
    InsertToProcs(mng->procs);
  }
}

void Block::Input(GUIManager *mng, const char *str) {
}

void Block::DoubleClick(GUIManager *mng, int rx, int ry) {
  pflag = pflag ? false : true;
  mng->state = GUIManager::DEFAULT;
}

void Block::InsertToProcs(std::list<Block *> &procs) {
    if (IsPointed(895, Y())) {
      MoveWithChildren(895 - W() / 2 - X(), 0);
    auto it = procs.begin();
    while (it != procs.end()) {
      if (Y() < (*it)->Y()) {
	procIt = new std::list<Block *>::iterator(procs.insert(it, this));
	break;
      }
      it++;
    }
    if (it == procs.end()) {
      procIt = new std::list<Block *>::iterator(procs.insert(it, this));
    }
  }
}

void Block::AutoLink(std::list<Block *> &blocks) {
  Delink();
  int minR2 = 20000;
  for (auto *block : blocks) {
    for (auto *sock : block->Socks()) {
      if (sock->Info()->Type()->RetType() == body->RetType() &&
	  sock->Src() == NULL && sock->X() >= SinkX()) {
	int r2 = (SinkX() - sock->X()) * (SinkX() - sock->X()) +
	  (SinkY() - sock->Y()) * (SinkY() - sock->Y());
	if (r2 < minR2) {
	  sock->Link(this);
	  minR2 = r2;
	}
      }
    }
  }
}
