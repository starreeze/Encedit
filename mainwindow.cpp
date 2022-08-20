#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QFontDialog>
#include <QColorDialog>
#include <QFont>
#include <QColor>
#include <QDir>
#include <QStyle>
#include <QStandardPaths>
#include <QMessageBox>
#include <QTimer>
#include <QStandardItemModel>
#include <QPlainTextEdit>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    ui->listWidget->setFont(QFont(default_fontname, contents_fontsize));
    ui->centralWidget->setLayout(ui->horizontalLayout);
    ui->textEdit->setFocus();
    QString appDataDirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir appDataDir(appDataDirPath);
    appDataDir.mkpath(".");
    status_path = appDataDirPath + "/" + status_file_name;
    QFile status_file(status_path);
    if (!status_file.exists()) {
        status_file.open(QIODevice::WriteOnly);
        status_file.close();
    }
    // timer = new QTimer(this);
    // timer->setInterval(60000);
    // timer->callOnTimeout(this, &MainWindow::auto_save);
}

MainWindow::~MainWindow() {
    QFile status_file(status_path);
    if (status_file.open(QFile::WriteOnly | QFile::Text)) {
        QTextStream out(&status_file);
        config.cursor_pos = ui->textEdit->textCursor().position();
        save_conf(out, config, io);
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
        QFile status(status_path);
        if (status.open(QFile::ReadOnly | QFile::Text)) {
            QTextStream in(&status);
            load_conf(in, config, io);
            status.close();
            // auto-save
//            if (QFile::exists(".autosave") &&
//                QMessageBox::question(this, "Retrive", "Your document was not saved before an unexpected shutdown. Retrive your document?") == QMessageBox::Yes
//                ) {
//                set_filename("");
//                display(".autosave", false);
//                set_dirty(true);
//            }
//            else
            display(io.file_path);
            auto cursor = ui->textEdit->textCursor();
            cursor.setPosition(config.cursor_pos);
            ui->textEdit->setTextCursor(cursor);
            ui->textEdit->setFont(QFont(config.font_name, config.font_size));
            update_style();
        }
        else
            on_actionNew_triggered();
    }
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent) {
    if (ui->textEdit->isReadOnly()) {
        ui->textEdit->clear();
        ui->textEdit->setReadOnly(false);
        set_dirty(true);
        text_connection = connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_text_modified()));
    }
    switch (keyEvent->key()) {
    case Qt::Key_Control:
        ctrl_pressed = true;
        break;
    case Qt::Key_Shift:
        shift_pressed = true;
        break;
    case Qt::Key_H:
        QMessageBox::information(this, "shotcut help", help_text);
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
    case Qt::Key_K:
        if (ctrl_pressed) {
            if (ui->listWidget->isHidden())
                ui->listWidget->show();
            else    ui->listWidget->hide();
        }
        break;
    case Qt::Key_B:
        if (windowFlags().testFlag(Qt::FramelessWindowHint)) {
            hide();
            setWindowFlag(Qt::FramelessWindowHint, false);
            show();
        }
        else {
            hide();
            setWindowFlag(Qt::FramelessWindowHint);
            show();
        }
        break;
    case Qt::Key_L:
        config.font_color = QColorDialog::getColor(Qt::black, this, "Font color").rgb();
        config.background_color = QColorDialog::getColor(Qt::white, this, "Background color").rgb();
        update_style();
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

void MainWindow::display(QString filename, bool updateFilename, bool encrypt) {
    if (!QFile::exists(filename)) {
        on_actionNew_triggered();
        return;
    }
    if (updateFilename)
        set_filename(filename);
    bool ok; QString password;
    password = QInputDialog::getText(this, "password", QString("Password for document %1 (leave it blank if this is a plain text document):").arg(io.file_path), QLineEdit::Password, "", &ok);
    if (ok) {
        io.key = password.toULong();
        io.file_path = filename;
        QString text = io.read();
        ui->textEdit->setPlainText(text);
        update_index(text);
        text_connection = connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_text_modified()));
    }
    else    on_actionNew_triggered();
}

void MainWindow::on_actionNew_triggered() {
    close_current();
    // timer->start();
    set_filename("");
    ui->textEdit->setPlainText(QString(additional_welcome_message) + help_text + dismiss_reminder);
    ui->textEdit->setReadOnly(true);
//    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_dismiss_key_pressed()));
}

void MainWindow::on_actionOpen_triggered() {
    QString file = QFileDialog::getOpenFileName(this, "Open a file", "", "Text files(*.enc *.txt)");
    if (!file.isEmpty()) {
        close_current();
        display(file);
    }
}

void MainWindow::on_actionSave_triggered() {
    save_current();
}

void MainWindow::on_actionSave_As_triggered() {
    QString file = QFileDialog::getSaveFileName(this, "Save as", "my.enc", "Text files(*.enc *.txt)");
    if (!file.isEmpty()) {
        set_filename(file);
        if (file.endsWith(".txt"))
            io.key = 0;
        else {
            QString confirm_password, password;
            while (true) {
                bool ok;
                password = QInputDialog::getText(this, "password", QString("A digit password to encript document %1 (leave it blank if you don't want encryption):").arg(file), QLineEdit::Password, "", &ok);
                if (!ok)    return;
                if (password.isEmpty())
                    break;
                confirm_password = QInputDialog::getText(this, "password", "Please confirm your password:", QLineEdit::Password, "", &ok);
                if (!ok)    return;
                if (password != confirm_password)
                    QMessageBox::warning(this, "password", "Password mismatch!");
                else
                    break;
            }
            io.key = password.toULong();
        }
        QFile _file(file);
        _file.open(QIODevice::WriteOnly);
        _file.close();
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
    io.file_path = filename;
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
    if (!QFile::exists(io.file_path))
        on_actionSave_As_triggered();
    io.write(ui->textEdit->toPlainText(), saveClean);
    set_dirty(false);
    update_index(ui->textEdit->toPlainText());
}

void MainWindow::update_index(const QString& text, const QString& regexp) {
    index.load(text, regexp.size() ? regexp : config.title_regexp);
    ui->listWidget->clear();
    auto& strl = index.string_list();
    ui->listWidget->addItems(strl);
    connect(ui->listWidget, &QAbstractItemView::clicked, this, &MainWindow::on_listWidget_clicked);
}

void MainWindow::update_style() {
    QString style = QString("{background-color: rgb(%1); color: rgb(%2);}").arg(color2str(config.background_color)).arg(color2str(config.font_color));
    ui->textEdit->setStyleSheet("QPlainTextEdit " + style);
    ui->listWidget->setStyleSheet("QListWidget " + style);
}

void MainWindow::on_listWidget_clicked(const QModelIndex& idx) {
    auto cursor = ui->textEdit->textCursor();
    cursor.setPosition(ui->textEdit->toPlainText().size());
    ui->textEdit->setTextCursor(cursor);
    cursor.setPosition(index[idx]);
    ui->textEdit->setTextCursor(cursor);
}
