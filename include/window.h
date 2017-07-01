#pragma once

#include <QMainWindow>

class GUIManager;
class QKeyEvent;
class QMouseEvent;
class QPaintEvent;

class Window : public QMainWindow {
public:
  Window(const char *title, int w, int h);
  virtual ~Window();

protected:
  void keyPressEvent(QKeyEvent *event);
  void mouseDoubleClickEvent(QMouseEvent *event);
  void mouseMoveEvent(QMouseEvent *event);
  void mousePressEvent(QMouseEvent *event);
  void mouseReleaseEvent(QMouseEvent *event);
  void paintEvent(QPaintEvent *event);

private:
  GUIManager *mng;
};
