#include "constint.h"

#include "blockbody.h"
#include "expr.h"
#include "guimanager.h"
#include "type.h"
#include <QPainter>

ConstInt::ConstInt(int x, int y, BlockBody *body) : Block(x, y, body) {
  valStr = "ConstInt";
  sumi = false;
}

std::vector<ArgSocket *> &ConstInt::Socks() {
  return hack;
}

void ConstInt::DrawName(QPainter *p) const {
  auto fm = p->fontMetrics();
  const auto *name = valStr.c_str();
  p->drawText(X() + 40 - fm.width(name) / 2 + 1,
              Y() + 8 + fm.height() / 2 - 2, name);
}

void ConstInt::Release(GUIManager *mng, int rx, int ry) {
  if (sumi) {
    return;
  }
  sumi = true;
  hack.push_back((ArgSocket *)(new int(0)));
  hack.resize(0);
  valStr = "0";
  mng->EnableTextBox(X(), Y(), "");
  mng->state = GUIManager::INPUT;
}

void ConstInt::Input(GUIManager *mng, const char *str) {
  int val = std::strtol(str, NULL, 10);
  valStr = std::to_string(val);
  hack.push_back((ArgSocket *)(new int(val)));
  hack.resize(0);
  mng->DisableTextBox();
}

ConstStr::ConstStr(int x, int y, BlockBody *body) : Block(x, y, body) {
  valStr = "ConstStr";
  sumi = false;
}

std::vector<ArgSocket *> &ConstStr::Socks() {
  return hack;
}

void ConstStr::DrawName(QPainter *p) const {
  auto fm = p->fontMetrics();
  std::string s = sumi ? "\"" : "";
  s += valStr;
  s += sumi ? "\"" : "";
  const auto *name = s.c_str();
  p->drawText(X() + 40 - fm.width(name) / 2 + 1,
              Y() + 8 + fm.height() / 2 - 2, name);
}

void ConstStr::Release(GUIManager *mng, int rx, int ry) {
  if (sumi) {
    return;
  }
  sumi = true;
  hack.push_back((ArgSocket *)(new int(0)));
  hack.resize(0);
  valStr = "";
  mng->EnableTextBox(X(), Y(), "");
  mng->state = GUIManager::INPUT;
}

void ConstStr::Input(GUIManager *mng, const char *str) {
  valStr = str;
  hack.push_back((ArgSocket *)(valStr.c_str()));
  hack.resize(0);
  mng->DisableTextBox();
}
