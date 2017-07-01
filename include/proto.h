#pragma once

#include "guiobj.h"

class BlockBody;
class GUIManager;
class QPainter;

class Proto : public GUIObj {
public:
  Proto(int x, int y, BlockBody *body);
  virtual ~Proto();
  void UpdateSize();

  virtual void Draw(QPainter *p) const;
  void DrawPart(QPainter *p) const;
  void DrawName(QPainter *p) const;
  void DrawArgs(QPainter *p) const;

  virtual void Drag(GUIManager *mng, int dx, int dy);
  virtual void DoubleClick(GUIManager *mng, int rx, int ry);
  virtual void Input(GUIManager *mng, const char *str);

  BlockBody *body;
  int inputIdx;
};
