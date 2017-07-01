#pragma once

#include "proto.h"
#include "type.h"
#include <vector>

class GUIManager;
class Ar;

class ArProto : public Proto {
public:
  ArProto(int x, int y, DataType type);
  virtual ~ArProto();

  void Drag(GUIManager *mng, int dx, int dy);
  void Release(GUIManager *mng, int rx, int ry);
  void DoubleClick(GUIManager *mng, int rx, int ry);
  void Input(GUIManager *mng, const char *str);

  DataType type;
  std::vector<Ar *> ars;
};
