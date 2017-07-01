#include "viproto.h"

#include "blockbody.h"
#include "guimanager.h"
#include "type.h"
#include "varint.h"
#include "protomanager.h"

VIProto::VIProto(int x, int y, DataType type) : Proto(x, y, new BlockBody(type == INT ? "IntVar" : "StrVar", type, {}, NULL)) {
  this->type = type;
}

VIProto::~VIProto() {
  for (auto *vi : vis) {
    delete vi;
  }
}

void VIProto::Drag(GUIManager *mng, int dx, int dy) {
}

void VIProto::Release(GUIManager *mng, int rx, int ry) {
  mng->EnableTextBox(X(), Y(), "");
  mng->state = GUIManager::INPUT;
}

void VIProto::DoubleClick(GUIManager *mng, int rx, int ry) {
}

void VIProto::Input(GUIManager *mng, const char *str) {
  auto *vi = new VarInt(mng->unit, str, type);
  vis.push_back(vi);
  mng->objs.push_front(vi->init);
  mng->blocks.push_back(vi->init);

  mng->objs.push_front(vi->val);
  mng->stdPM->AddProto(vi->val);
  mng->objs.push_front(vi->set);
  mng->stdPM->AddProto(vi->set);
  mng->DisableTextBox();
}
