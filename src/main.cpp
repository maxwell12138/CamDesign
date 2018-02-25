#include "mainwindow.h"
#include <QApplication>
//#include "rpn.h"
//#include <string>
//#include <iostream>
//#include
using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
