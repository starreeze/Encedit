#include "mainwindow.h"
#include "historylist.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaTypeStreamOperators<HLEntry>("HLEntry");
    qRegisterMetaTypeStreamOperators<QList<HLEntry>>("QList<HLEntry>");
    QApplication a(argc, argv);
    a.setOrganizationName(org_name);
    a.setApplicationName(app_name + QString("_") + version_string);
    a.setApplicationVersion(version_string);
    MainWindow w;
    w.receive_args(argc, argv);
    w.show();
    w.post_show();
    return a.exec();
}
