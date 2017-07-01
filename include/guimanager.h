#pragma once

#include "llvm.h"
#include <list>

class Block;
class GUIObj;
class LLVMUnit;
class ProtoManager;
class QLineEdit;
class QPainter;
class QWidget;
class FunFun;

class GUIManager {
public:
  GUIManager(QWidget *owner);
  virtual ~GUIManager();

  void InitMain();
  
  void PressKey(int key);
  void PressMouse(int x, int y);
  void MoveMouse(int x, int y);
  void ReleaseMouse(int x, int y);
  void DoubleClick(int x, int y);
  void Draw();
  void Update();

  GUIObj *PointedObj(int x, int y);
  void EnableTextBox(int x, int y, const char *str);
  void DisableTextBox();

  void Matome();
  void MakePBS(std::vector<Block *> &pbs);
  void MakeRTS(std::vector<Block *> &pbs, std::vector<Block *> &rts);
  void MakePBS2(std::vector<Block *> &pbs, std::vector<Block *> &pbs2);

  std::list<GUIObj *> objs;
  std::list<Block *> blocks;
  std::list<Block *> procs;
  std::list<FunFun *> ffs;

  ProtoManager *stdPM;

  enum State {
    DEFAULT,
    HOLD,
    INPUT
  };

  State state;
  GUIObj *pointedObj;

  int lastX;
  int lastY;

  QWidget *owner;
  QLineEdit *textBox;

  LLVMUnit *unit;
  llvm::Function *mainFunc;
  llvm::BasicBlock *mainBB;
};
