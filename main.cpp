#include "directory.h"
#include "config.h"
#include <QApplication>

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    QFile history(".history");
    Config config;
    if (history.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&history);
        in >> config;
        history.close();
    }
    else {
        config.file_path = "diary.enc";
        config.font_size = 20;
        config.password = 0;
    }
    Directory wdir(config.file_path, config.password);
    wdir.show();
    // MainWindow w;
    // w.receive_args(argc, argv);
    // w.show();
    return a.exec();
    // save after exec before return
}
