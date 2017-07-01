#include "proto.h"

#include "block.h"
#include "blockbody.h"
#include "guimanager.h"
#include <QPainter>

Proto::Proto(int x, int y, BlockBody *body) : GUIObj(x, y, 81, 0) {
  this->body = body;
  inputIdx = -1;
  UpdateSize();
}

Proto::~Proto() {
  delete body;
}

void Proto::UpdateSize() {
  SetSize(W(), 17 + body->ArgNum() * 16);
}

void Proto::Draw(QPainter *p) const {
  DrawFrame(p);
  DrawPart(p);
  DrawName(p);
  DrawArgs(p);
}

void Proto::DrawPart(QPainter *p) const {
  p->drawLine(L() + 3, Y() + 16, R() - 3, Y() + 16);
  int partY = 8;
  for (auto *argInfo : body->ArgInfos()) {
    partY += 16;
    p->drawLine(L() + 4, Y() + partY, L() + 6, Y() + partY);
  }
}

void Proto::DrawName(QPainter *p) const {
  auto fm = p->fontMetrics();
  const auto *name = body->Name().c_str();
  p->drawText(X() + 40 - fm.width(name) / 2 + 1,
              Y() + 8 + fm.height() / 2 - 3, name);
}

void Proto::DrawArgs(QPainter *p) const {
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

void Proto::Drag(GUIManager *mng, int dx, int dy) {
  auto *block = new Block(X() + dx, Y() + dy, body);
  mng->objs.push_front(block);
  mng->blocks.push_back(block);
  mng->pointedObj = block;
}

void Proto::DoubleClick(GUIManager *mng, int rx, int ry) {
  if (ry < 17) {
    inputIdx = -1;
    mng->EnableTextBox(X(), Y() + (inputIdx + 1) * 16, body->Name().c_str());
  } else {
    inputIdx = (ry - 17) / 16;
    mng->EnableTextBox(X(), Y() + (inputIdx + 1) * 16, body->ArgInfos()[inputIdx]->Name().c_str());
  }
  mng->state = GUIManager::INPUT;
}

void Proto::Input(GUIManager *mng, const char *str) {
  if (inputIdx == -1) {
    body->SetName(str);
  } else {
    body->ArgInfos()[inputIdx]->SetName(str);
  }
  mng->DisableTextBox();
}
