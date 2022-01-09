#include "directory.h"
#include "ui_directory.h"
#include "mainwindow.h"
#include <QStringListModel>

Directory::Directory(const QString& filename, quint64 key, QWidget* parent) : QMainWindow(parent), ui(new Ui::Directory), io(filename, key) {
    ui->setupUi(this);
    auto slm = new QStringListModel(io.get_mata(), this);
    ui->listView->setModel(slm);
    setCentralWidget(ui->listView);
    connect(ui->listView, &QAbstractItemView::clicked, this, &Directory::on_entry_clicked);
}

Directory::~Directory() {
    delete ui;
}

void Directory::on_entry_clicked(const QModelIndex& idx) {
    // TODO fontsize history
    auto wedit = new MainWindow(idx.row(), io.get_entry(idx.row()), 20, &io, this);
    wedit->show();
}

void Directory::on_actionFile_triggered() {

}

void Directory::on_actionEntry_triggered() {
    // TODO fontsize history
    auto wedit = new MainWindow(-1, {}, 20, &io, this);
    wedit->show();
    connect(wedit, &MainWindow::entry_closed, this, [&]() {
        auto slm = new QStringListModel(io.get_mata(), this);
        ui->listView->setModel(slm);
    });
}
