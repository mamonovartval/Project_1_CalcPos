#include "testmodules.h"
#include "mainwindow.h"
#include <QApplication>

const bool TEST = true;

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  if(TEST){
      TestModules();
    }

  handler::Control c;
  MainWindow w(nullptr, &c);

  return a.exec();
}
