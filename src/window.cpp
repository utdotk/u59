#include "window.h"

#include "guimanager.h"
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintEvent>

Window::Window(const char *title, int w, int h) {
  mng = new GUIManager(this);
  setWindowTitle(title);
  resize(w, h);
}

Window::~Window() {
  delete mng;
}

void Window::keyPressEvent(QKeyEvent *event) {
  mng->PressKey(event->key());
}

void Window::mouseDoubleClickEvent(QMouseEvent *event) {
  mng->DoubleClick(event->x(), event->y());
}

void Window::mouseMoveEvent(QMouseEvent *event) {
  mng->MoveMouse(event->x(), event->y());
}

void Window::mousePressEvent(QMouseEvent *event) {
  mng->PressMouse(event->x(), event->y());
}

void Window::mouseReleaseEvent(QMouseEvent *event) {
  mng->ReleaseMouse(event->x(), event->y());
}

void Window::paintEvent(QPaintEvent *event) {
  mng->Draw();
}
