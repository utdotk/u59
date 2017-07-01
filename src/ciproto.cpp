#include "ciproto.h"

#include "blockbody.h"
#include "constint.h"
#include "expr.h"
#include "guimanager.h"
#include "type.h"

CIProto::CIProto(int x, int y, DataType type) : Proto(x, y, new BlockBody(type == INT ? "IntConst" : "StrConst", type, {}, type == INT ? (Expression *)(new Integer) : (Expression *)(new String))) {
  this->type = type;
}

CIProto::~CIProto() {
}

void CIProto::Drag(GUIManager *mng, int dx, int dy) {
  auto *block = type == INT ? (Block *)(new ConstInt(X() + dx, Y() + dy, body)) :
    (Block *)(new ConstStr(X() + dx, Y() + dy, body));
  mng->objs.push_front(block);
  mng->blocks.push_back(block);
  mng->pointedObj = block;
}

void CIProto::DoubleClick(GUIManager *mng, int rx, int ry) {
}

void CIProto::Input(GUIManager *mng, const char *str) {
}
