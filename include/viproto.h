#pragma once

#include "proto.h"
#include "type.h"
#include <vector>

class GUIManager;
class VarInt;

class VIProto : public Proto {
public:
  VIProto(int x, int y, DataType type);
  virtual ~VIProto();

  void Drag(GUIManager *mng, int dx, int dy);
  void Release(GUIManager *mng, int rx, int ry);
  void DoubleClick(GUIManager *mng, int rx, int ry);
  void Input(GUIManager *mng, const char *str);

  DataType type;
  std::vector<VarInt *> vis;
};
