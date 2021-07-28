#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encript.h"
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->textEdit);
    setWindowTitle("EncEdit");
    ui->textEdit->setFont(QFont("Consolas", config.font_size));
}

MainWindow::~MainWindow()
{
    QFile history(".history");
    if (history.open(QFile::WriteOnly | QFile::Text))
    {
        QTextStream out(&history);
        config.cursor_pos = ui->textEdit->textCursor().position();
        out << config;
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
            in >> config;
            history.close();
            display(config.file_path);
            auto cursor = ui->textEdit->textCursor();
            cursor.setPosition(config.cursor_pos);
            ui->textEdit->setTextCursor(cursor);
            ui->textEdit->setFont(QFont("Consolas", config.font_size));
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
    case Qt::Key_Equal:
        if (ctrl_pressed)
        {
            config.font_size += 2;
            ui->textEdit->setFont(QFont("Consolas", config.font_size));
        }
        break;
    case Qt::Key_Minus:
        if (ctrl_pressed)
        {
            config.font_size -= 2;
            ui->textEdit->setFont(QFont("Consolas", config.font_size));
        }
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
    if (sFile.open(QFile::ReadOnly))
    {
        set_filename(filename);
        QByteArray content = sFile.readAll();
        sFile.close();
        bool ok;
        config.password = QInputDialog::getInt(this, "password", QString("Please enter digit password for encripted document%1:").arg(config.file_path), 0, -2147483648, 2147483647, 1, &ok);
        if (ok)
        {
            QString text = decript(content, config.password);
            ui->textEdit->setPlainText(text);
            connection = connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_text_modified()));
        }
        else
            on_actionNew_triggered();
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
    QString file = QFileDialog::getOpenFileName(this, "Open a file", "", "Encripted text files (*.enc)");
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
    QString file = QFileDialog::getSaveFileName(this, "Save as", "", "Encripted text files (*.enc)");
    if (!file.isEmpty())
    {
        set_filename(file);
        int confirm_password;
        while (true)
        {
            config.password = QInputDialog::getInt(this, "password", QString("Please enter a digit password to encript document %1:").arg(config.file_path));
            confirm_password = QInputDialog::getInt(this, "password", "Please confirm your password:");
            if (config.password != confirm_password)
                QMessageBox::warning(this, "password", "Password mismatch!");
            else
                break;
        }
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
    config.file_path = filename;
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
    QFile sFile(config.file_path);
    if (sFile.open(QFile::WriteOnly))
    {
        sFile.write(encript(text, config.password));
        sFile.close();
        set_dirty(false);
    }
    else if (!text.isEmpty())
        on_actionSave_As_triggered();
    else
        set_dirty(false);
}
