#include "arproto.h"

#include "blockbody.h"
#include "guimanager.h"
#include "type.h"
#include "ar.h"
#include "protomanager.h"

ArProto::ArProto(int x, int y, DataType type) : Proto(x, y, new BlockBody(type == INT ? "IntArray" : "StrArray", type, {}, NULL)) {
  this->type = type;
}

ArProto::~ArProto() {
  for (auto *ar : ars) {
    delete ar;
  }
}

void ArProto::Drag(GUIManager *mng, int dx, int dy) {
}

void ArProto::Release(GUIManager *mng, int rx, int ry) {
  mng->EnableTextBox(X(), Y(), "");
  mng->state = GUIManager::INPUT;
}

void ArProto::DoubleClick(GUIManager *mng, int rx, int ry) {
}

void ArProto::Input(GUIManager *mng, const char *str) {
  auto *ar = new Ar(mng->unit, str, type);
  ars.push_back(ar);
  mng->objs.push_front(ar->init);
  mng->blocks.push_back(ar->init);

  mng->objs.push_front(ar->size);
  mng->stdPM->AddProto(ar->size);
  mng->objs.push_front(ar->val);
  mng->stdPM->AddProto(ar->val);
  mng->objs.push_front(ar->set);
  mng->stdPM->AddProto(ar->set);
  mng->DisableTextBox();
}
