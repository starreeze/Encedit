#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.receive_args(argc, argv);
    w.show();
    return a.exec();
}
