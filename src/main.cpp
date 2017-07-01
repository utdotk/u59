#include "window.h"
#include <QApplication>
#include <QtGui>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);
  QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());

  Window w("u59", 1024, 768);
  w.show();

  return app.exec();
}
