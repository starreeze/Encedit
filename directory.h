#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <QMainWindow>

namespace Ui {
class Directory;
}

class Directory : public QMainWindow
{
    Q_OBJECT

public:
    explicit Directory(QWidget *parent = nullptr);
    ~Directory();

private:
    Ui::Directory *ui;
};

#endif // DIRECTORY_H
