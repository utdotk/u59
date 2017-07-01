#pragma once

#include "proto.h"
#include "type.h"

class CIProto : public Proto {
public:
  CIProto(int x, int y, DataType type);
  virtual ~CIProto();

  void Drag(GUIManager *mng, int dx, int dy);
  void DoubleClick(GUIManager *mng, int rx, int ry);
  void Input(GUIManager *mng, const char *str);

  DataType type;
};
