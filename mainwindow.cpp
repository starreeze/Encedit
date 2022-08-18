#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encript.h"
#include <QInputDialog>
#include <QFontDialog>
#include <QFont>
#include <QDir>
#include <QStandardPaths>
#include <QMessageBox>
#include <QTimer>
#include <QStandardItemModel>
#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->listWidget->setFont(QFont(default_fontname, contents_fontsize));
    QString appDataDir = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir(appDataDir).mkpath(".");
    status_path = appDataDir + "/" + status_file_name;
    // timer = new QTimer(this);
    // timer->setInterval(60000);
    // timer->callOnTimeout(this, &MainWindow::auto_save);
}

MainWindow::~MainWindow() {
    QFile status_file(status_path);
    if (status_file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&status_file);
        config.cursor_pos = ui->textEdit->textCursor().position();
        out << config;
        status_file.close();
    }
    close_current();
    delete ui;
}

void MainWindow::receive_args(int argc, char* argv[]) {
    // config and history
    if (argc == 2)
        display(argv[1]);
    else {
        QFile history(status_path);
        if (history.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&history);
            in >> config;
            history.close();
            // auto-save
            if (QFile::exists(".autosave") &&
                QMessageBox::question(this, "Retrive", "Your document was not saved before an unexpected shutdown. Retrive your document?") == QMessageBox::Yes
                ) {
                set_filename("");
                display(".autosave", false);
                set_dirty(true);
            }
            else
                display(config.file_path);
            auto cursor = ui->textEdit->textCursor();
            cursor.setPosition(config.cursor_pos);
            ui->textEdit->setTextCursor(cursor);
            ui->textEdit->setFont(QFont(config.font_name, config.font_size));
        }
        else
            on_actionNew_triggered();
    }
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent) {
    switch (keyEvent->key()) {
    case Qt::Key_Control:
        ctrl_pressed = true;
        break;
    case Qt::Key_Shift:
        shift_pressed = true;
        break;
    case Qt::Key_S:
        if (ctrl_pressed) {
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
    case Qt::Key_F:
        if (ctrl_pressed) {
            bool ok;
            QFont font = QFontDialog::getFont(&ok, QFont(config.font_name, config.font_size), this);
            if (ok) {
                config.font_name = font.family();
                config.font_size = font.pointSize();
                ui->textEdit->setFont(font);
                ui->listWidget->setFont(QFont(config.font_name, contents_fontsize));
            }
        }
        break;
    case Qt::Key_R:
        if (ctrl_pressed) {
            bool ok;
            QString regexp = QInputDialog::getText(this, "title regexp", QString("Enter an regexp to match titles in your passage for contents to display:"), QLineEdit::Normal, config.title_regexp, &ok);
            if (ok) {
                config.title_regexp = regexp;
                update_index(ui->textEdit->toPlainText(), regexp);
            }
        }
        break;
    case Qt::Key_Equal:
        if (ctrl_pressed) {
            config.font_size += 2;
            ui->textEdit->setFont(QFont(config.font_name, config.font_size));
        }
        break;
    case Qt::Key_Minus:
        if (ctrl_pressed) {
            config.font_size -= 2;
            ui->textEdit->setFont(QFont(config.font_name, config.font_size));
        }
        break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent* keyEvent) {
    switch (keyEvent->key()) {
    case Qt::Key_Control:
        ctrl_pressed = false;
        break;
    case Qt::Key_Shift:
        shift_pressed = false;
        break;
    }
}

void MainWindow::display(QString filename, bool updateFilename) {
    QFile sFile(filename);
    if (sFile.open(QFile::ReadOnly)) {
        if (updateFilename)
            set_filename(filename);
        content = sFile.readAll();
        sFile.close();
        bool ok; QString password;
        password = QInputDialog::getText(this, "password", QString("Password for encripted document %1:").arg(config.file_path), QLineEdit::Password, "", &ok);
        if (ok) {
            config.password = password.toULong();
            QString text = decript(content, config.password);
            ui->textEdit->setPlainText(text);
            update_index(text);
            text_connection = connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_text_modified()));
            // timer->start();
            return;
        }
    }
    on_actionNew_triggered();
}

void MainWindow::on_actionNew_triggered() {
    close_current();
    text_connection = connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_text_modified()));
    // timer->start();
    set_filename("");
    ui->textEdit->setPlainText("");
    set_dirty(true);
}

void MainWindow::on_actionOpen_triggered() {
    QString file = QFileDialog::getOpenFileName(this, "Open a file", "", "Encripted text files(*.enc)");
    if (!file.isEmpty()) {
        close_current();
        display(file);
    }
}

void MainWindow::on_actionSave_triggered() {
    save_current();
}

void MainWindow::on_actionSave_As_triggered() {
    QString file = QFileDialog::getSaveFileName(this, "Save as", "", "Encripted text files(*.enc)");
    if (!file.isEmpty()) {
        set_filename(file);
        QString confirm_password, password;
        while (true) {
            password = QInputDialog::getText(this, "password", QString("A digit password to encript document %1:").arg(config.file_path), QLineEdit::Password);
            confirm_password = QInputDialog::getText(this, "password", "Please confirm your password:", QLineEdit::Password);
            if (password != confirm_password)
                QMessageBox::warning(this, "password", "Password mismatch!");
            else
                break;
        }
        config.password = password.toULong();
        save_current(true);
    }
}

void MainWindow::on_actionCopy_triggered() {
    ui->textEdit->copy();
}

void MainWindow::on_actionPaste_triggered() {
    ui->textEdit->paste();
}

void MainWindow::on_actionCut_triggered() {
    ui->textEdit->cut();
}

void MainWindow::on_actionUndo_triggered() {
    ui->textEdit->undo();
}

void MainWindow::on_actionRedo_triggered() {
    ui->textEdit->redo();
}

void MainWindow::on_text_modified() {
    set_dirty(true);
    // if (ui->textEdit->toPlainText()[ui->textEdit->textCursor().position() - 1] == '\n')
    //     ui->textEdit->insertPlainText("\t");
}

// void MainWindow::auto_save() {
//     QString text = ui->textEdit->toPlainText();
//     update_index(text);
    /* !autosave disabled to improve performance */
    // if (!dirty) return;
    // QFile sFile(".autosave");
    // if (sFile.open(QFile::WriteOnly)) {
    //     sFile.write(encript(text, config.password));
    //     sFile.close();
    // }
    // else
    //     QMessageBox::critical(this, "Failure", "Cannot write file; please check permissions.");
// }

void MainWindow::set_filename(QString filename) {
    config.file_path = filename;
    int i = filename.length();
    if (!i)   filename = "new*";
    while (i && filename[--i] != '/');
    setWindowTitle(filename.mid(i + (filename[i] == '/')) + " - EncEdit");
}

void MainWindow::set_dirty(bool val) {
    if (val == dirty)
        return;
    if (val) {
        dirty = true;
        QString title = windowTitle();
        if (title.back() != '*')
            setWindowTitle(title + "*");
    }
    else {
        dirty = false;
        QString title = windowTitle();
        if (title.back() == '*')
            setWindowTitle(title.mid(0, title.length() - 1));
    }
}

void MainWindow::close_current() {
    disconnect(text_connection); // timer->stop();
    if (dirty && !ui->textEdit->toPlainText().isEmpty()) {
        auto response = QMessageBox::question(this, "Save", "Save your document?");
        if (response == QMessageBox::Yes)
            save_current();
    }
    set_dirty(false);
    QFile autosave(".autosave");
    autosave.remove();
}

void MainWindow::save_current(bool saveClean) {
    if (!dirty && !saveClean)
        return;
    QFile sFile(config.file_path);
    if (sFile.open(QFile::WriteOnly | QFile::Append)) {
        QByteArray newContent = encript(ui->textEdit->toPlainText(), config.password);
        int i = 0, os = content.size(), ns = newContent.size();
        for (; i < qMin(ns, os) && content[i] == newContent[i]; ++i);
        auto modified = newContent.mid(i);
        sFile.resize(i);
        sFile.seek(i);
        sFile.write(modified);
        sFile.close();
        set_dirty(false);
        content = newContent;
        update_index(ui->textEdit->toPlainText());
    }
    else
        on_actionSave_As_triggered();
}

void MainWindow::update_index(const QString& text, const QString& regexp) {
    index.load(text, regexp);
    ui->listWidget->clear();
    auto& strl = index.string_list();
    ui->listWidget->addItems(strl);
    connect(ui->listWidget, &QAbstractItemView::clicked, this, &MainWindow::on_listWidget_clicked);
}

void MainWindow::on_listWidget_clicked(const QModelIndex& idx) {
    auto cursor = ui->textEdit->textCursor();
    cursor.setPosition(ui->textEdit->toPlainText().size());
    ui->textEdit->setTextCursor(cursor);
    cursor.setPosition(index[idx]);
    ui->textEdit->setTextCursor(cursor);
}
