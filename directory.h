#ifndef DIRECTORY_H
#define DIRECTORY_H
#include "io.h"
#include <QMainWindow>

namespace Ui {
    class Directory;
}

class Directory : public QMainWindow {
    Q_OBJECT
public:
    Directory(const QString& filename, quint64 key, QWidget* parent = nullptr);
    ~Directory();
private:
    Ui::Directory* ui;
    Io io;
private slots:
    void on_entry_clicked(const QModelIndex& idx);
    void on_actionFile_triggered();
    void on_actionEntry_triggered();
};

#endif // DIRECTORY_H
