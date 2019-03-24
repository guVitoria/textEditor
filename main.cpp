#include "mainwindow.h"
#include "menu.h"
#include <QApplication>
#include "reashimport.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //MainWindow w;
    Menu s;
    //ReashImport ptr;

    s.show();
    //ptr.show();
    return a.exec();
}
