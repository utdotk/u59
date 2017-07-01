#pragma once

#include "block.h"

#include <string>
#include <vector>

class BlockBody;
class QPainter;

class ConstInt : public Block {
public:
  ConstInt(int x, int y, BlockBody *body);
  virtual ~ConstInt() {}

  std::vector<ArgSocket *> &Socks();

  void DrawName(QPainter *p) const;
  void Release(GUIManager *mng, int rx, int ry);
  void Input(GUIManager *mng, const char *str);

  
private:
  std::string valStr;
  std::vector<ArgSocket *> hack;
  bool sumi;
};

class ConstStr : public Block {
public:
  ConstStr(int x, int y, BlockBody *body);
  virtual ~ConstStr() {}

  std::vector<ArgSocket *> &Socks();

  void DrawName(QPainter *p) const;
  void Release(GUIManager *mng, int rx, int ry);
  void Input(GUIManager *mng, const char *str);
  
private:
  std::string valStr;
  std::vector<ArgSocket *> hack;
  bool sumi;
};
