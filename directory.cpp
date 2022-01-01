#include "directory.h"
#include "ui_directory.h"

Directory::Directory(QWidget* parent) : QMainWindow(parent), ui(new Ui::Directory) {
    ui->setupUi(this);
}

Directory::~Directory() {
    delete ui;
}
