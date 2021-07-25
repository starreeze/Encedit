#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encript.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->textEdit);
    setWindowTitle("EncEdit");
    showMaximized();
    ui->textEdit->setFont(QFont("Consolas", 20));
}

MainWindow::~MainWindow()
{
    QFile history(".history");
    if (history.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&history);
        out << mFilename;
        history.close();
    }
    close_current();
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
        else
            on_actionNew_triggered();
    }
}

void MainWindow::keyPressEvent(QKeyEvent *keyEvent)
{
    switch (keyEvent->key())
    {
    case Qt::Key_Control:
        ctrl_pressed = true;
        break;
    case Qt::Key_S:
        if (ctrl_pressed)
        {
            if (shift_pressed)
                on_actionSave_As_triggered();
            else
                on_actionSave_triggered();
        }
        break;
    case Qt::Key_N:
        if (ctrl_pressed)
            on_actionNew_triggered();
        break;
    case Qt::Key_O:
        if (ctrl_pressed)
            on_actionOpen_triggered();
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *keyEvent)
{
    switch (keyEvent->key())
    {
    case Qt::Key_Control:
        ctrl_pressed = false;
        break;
    case Qt::Key_Shift:
        shift_pressed = false;
        break;
    }
}

void MainWindow::display(QString filename)
{
    std::string debug = filename.toStdString();
    QFile sFile(filename);
    if (sFile.open(QFile::ReadOnly | QFile::Text))
    {
        set_filename(filename);
        QTextStream in(&sFile);
        QString text = in.readAll();
        debug = text.toStdString();
        sFile.close();
        decript(text);
        debug = text.toStdString();
        ui->textEdit->setPlainText(text);
        connection = connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_text_modified()));
    }
    else
        on_actionNew_triggered();
}

void MainWindow::on_actionNew_triggered()
{
    close_current();
    connection = connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_text_modified()));
    set_filename("");
    ui->textEdit->setPlainText("");
}

void MainWindow::on_actionOpen_triggered()
{
    QString file = QFileDialog::getOpenFileName(this, "Open a file");
    if (!file.isEmpty())
    {
        close_current();
        display(file);
    }
}

void MainWindow::on_actionSave_triggered()
{
    save_current();
}

void MainWindow::on_actionSave_As_triggered()
{
    QString file = QFileDialog::getSaveFileName(this, "Save as");
    if (!file.isEmpty())
    {
        set_filename(file);
        save_current(true);
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

void MainWindow::on_text_modified()
{
    set_dirty(true);
}

void MainWindow::set_filename(QString filename)
{
    printf("set filename %s\n", filename.toStdString().c_str());
    mFilename = filename;
    int i = filename.length();
    if (!i)
        filename = "new*";
    while (i && filename[--i] != '/')
        ;
    setWindowTitle(filename.mid(i + (filename[i] == '/')) + " - EncEdit");
}

void MainWindow::set_dirty(bool val)
{
    if (val == dirty)
        return;
    if (val)
    {
        dirty = true;
        QString title = windowTitle();
        if (title.back() != '*')
            setWindowTitle(title + "*");
    }
    else
    {
        dirty = false;
        QString title = windowTitle();
        if (title.back() == '*')
            setWindowTitle(title.mid(0, title.length() - 1));
    }
}

void MainWindow::close_current()
{
    save_current();
    disconnect(connection);
}

void MainWindow::save_current(bool saveClean)
{
    if (!dirty && !saveClean)
        return;
    QString text = ui->textEdit->toPlainText();
    QFile sFile(mFilename);
    if (sFile.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&sFile);
        encript(text);
        out << text;
        sFile.close();
        set_dirty(false);
    }
    else if (!text.isEmpty())
        on_actionSave_As_triggered();
    else
        set_dirty(false);
}
