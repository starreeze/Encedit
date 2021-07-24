#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encript.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(ui->textEdit);
    this->setWindowTitle("qPad");
    this->showMaximized();
    ui->textEdit->setFont(QFont("Consolas", 20));
}

MainWindow::~MainWindow()
{
    on_actionSave_triggered();
    QFile history(".history");
    if (history.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&history);
        out << mFilename;
        history.close();
    }
    delete ui;
}

void MainWindow::receive_args(int argc, char *argv[])
{
    if (argc == 2)
        display(argv[1]);
    else
    {
        QFile history(".history");
        if (history.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&history);
            QString filename = in.readAll();
            history.close();
            display(filename);
        }
    }
}

void MainWindow::display(QString filename)
{
    std::string debug = filename.toStdString();
    QFile sFile(filename);
    if (sFile.open(QFile::ReadOnly | QFile::Text))
    {
        mFilename = filename;
        int i = filename.length();
        while (i && filename[--i] != '/')
            ;
        this->setWindowTitle(filename.mid(i + 1) + " - qPad");
        QTextStream in(&sFile);
        QString text = in.readAll();
        debug = text.toStdString();
        sFile.close();
        decript(text);
        debug = text.toStdString();
        ui->textEdit->setPlainText(text);
    }
}

void MainWindow::on_actionNew_triggered()
{
    on_actionSave_triggered();
    mFilename = "";
    ui->textEdit->setPlainText("");
}

void MainWindow::on_actionOpen_triggered()
{
    on_actionSave_triggered();
    QString file = QFileDialog::getOpenFileName(this, "Open a file");
    if (!file.isEmpty())
        display(file);
}

void MainWindow::on_actionSave_triggered()
{
    QString text = ui->textEdit->toPlainText();
    QFile sFile(mFilename);
    if (sFile.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&sFile);
        encript(text);
        out << text;
        sFile.close();
    }
    else if (!text.isEmpty())
        on_actionSave_As_triggered();
}

void MainWindow::on_actionSave_As_triggered()
{
    QString file = QFileDialog::getSaveFileName(this, "Save as");
    if (!file.isEmpty())
    {
        mFilename = file;
        on_actionSave_triggered();
    }
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
}

void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered()
{
    ui->textEdit->redo();
}
