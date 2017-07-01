#pragma once

#include "guiobj.h"
#include <list>
#include <vector>

class ArgInfo;
class Block;
class BlockBody;
class GUIManager;
class QPainter;

class ArgSocket {
public:
  ArgSocket(Block *owner, ArgInfo *info);
  virtual ~ArgSocket();

  int X() const;
  int Y() const;

  void Link(Block *src);
  void Delink();

  Block *Owner();
  ArgInfo *Info();
  Block *Src();

private:
  Block *owner;
  ArgInfo *info;
  Block *src;
};

class Block : public GUIObj {
public:
  Block(int x, int y, BlockBody *body);
  virtual ~Block();

  void UpdateSize();

  int SinkX() const;
  int SinkY() const;

  BlockBody *Body();
  virtual std::vector<ArgSocket *> &Socks();
  ArgSocket *DstSock();
  std::list<Block *>::iterator *ProcIt();

  void SetDstSock(ArgSocket *dstSock);

  void Link(ArgSocket *dstSock);
  void Delink();

  void MoveWithChildren(int dx, int dy);

  void Draw(QPainter *p) const;
  void DrawPFrame(QPainter *p) const;
  void DrawPart(QPainter *p) const;
  virtual void DrawName(QPainter *p) const;
  void DrawArgs(QPainter *p) const;
  void DrawArrow(QPainter *p) const;

  void Click(GUIManager *mng, int rx, int ry);
  void Drag(GUIManager *mng, int dx, int dy);
  virtual void Release(GUIManager *mng, int rx, int ry);
  virtual void Input(GUIManager *mng, const char *str);
  void DoubleClick(GUIManager *mng, int rx, int ry);

  void InsertToProcs(std::list<Block *> &procs);
  void AutoLink(std::list<Block *> &blocks);

  bool pflag;
  bool fflag;
  //private:
  BlockBody *body;
  std::vector<ArgSocket *> socks;
  ArgSocket *dstSock;
  std::list<Block *>::iterator *procIt;
};
