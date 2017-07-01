#pragma once

class GUIManager;
class QPainter;

class GUIObj {
public:
  GUIObj(int x, int y, int w, int h);
  virtual ~GUIObj();

  int X() const;
  int Y() const;
  int W() const;
  int H() const;
  int L() const;
  int T() const;
  int R() const;
  int B() const;
  void SetPos(int x, int y);
  void SetSize(int w, int h);
  void Move(int dx, int dy);
  bool IsPointed(int x, int y) const;

  virtual void Draw(QPainter *p) const;
  virtual void DrawFrame(QPainter *p) const;

  virtual void Click(GUIManager *mng, int rx, int ry);
  virtual void Drag(GUIManager *mng, int dx, int dy);
  virtual void Release(GUIManager *mng, int rx, int ry);
  virtual void DoubleClick(GUIManager *mng, int rx, int ry);
  virtual void Input(GUIManager *mng, const char *str);

private:
  int x;
  int y;
  int w;
  int h;
};
