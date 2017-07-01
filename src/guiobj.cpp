#include "guiobj.h"

#include "guimanager.h"
#include <QPainter>

GUIObj::GUIObj(int x, int y, int w, int h) {
  SetPos(x, y);
  SetSize(w, h);
}

GUIObj::~GUIObj() {
}

int GUIObj::X() const {
  return x;
}

int GUIObj::Y() const {
  return y;
}

int GUIObj::W() const {
  return w;
}

int GUIObj::H() const {
  return h;
}

int GUIObj::L() const {
  return x;
}

int GUIObj::T() const {
  return y;
}

int GUIObj::R() const {
  return x + w - 1;
}

int GUIObj::B() const {
  return y + h - 1;
}

void GUIObj::SetPos(int x, int y) {
  this->x = x;
  this->y = y;
}

void GUIObj::SetSize(int w, int h) {
  this->w = w;
  this->h = h;
}

void GUIObj::Move(int dx, int dy) {
  SetPos(x + dx, y + dy);
}

bool GUIObj::IsPointed(int x, int y) const {
  return x >= L() && x <= R() && y >= T() && y <= B();
}

void GUIObj::Draw(QPainter *p) const {
  DrawFrame(p);
}

void GUIObj::DrawFrame(QPainter *p) const {
  p->drawLine(L(), T(), R(), T());
  p->drawLine(R(), T(), R(), B());
  p->drawLine(R(), B(), L(), B());
  p->drawLine(L(), B(), L(), T());
}

void GUIObj::Click(GUIManager *mng, int rx, int ry) {
}

void GUIObj::Drag(GUIManager *mng, int dx, int dy) {
}

void GUIObj::Release(GUIManager *mng, int rx, int ry) {
}

void GUIObj::DoubleClick(GUIManager *mng, int rx, int ry) {
}

void GUIObj::Input(GUIManager *mng, const char *str) {
}
