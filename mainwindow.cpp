#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "encript.h"
#include <QInputDialog>
#include <QMessageBox>

MainWindow::MainWindow(int entryIndex, const QString& content, int font, Io* io, QWidget* parent) :
    entryIdx(entryIndex), fontSize(font), pio(io), QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    setCentralWidget(ui->textEdit);
    ui->textEdit->setFont(QFont("Consolas", font));
    ui->textEdit->setText(content);
    text_connection = connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_text_modified()));
    // timer = new QTimer(this);
    // timer->setInterval(60000);
    // timer->callOnTimeout(this, &MainWindow::auto_save);
}

MainWindow::~MainWindow() {
    // QFile history(".history");
    // if (history.open(QFile::WriteOnly | QFile::Text)) {
    //     QTextStream out(&history);
    //     config.cursor_pos = ui->textEdit->textCursor().position();
    //     out << config;
    //     history.close();
    // }
    // close_current();
    delete ui;
}

// void MainWindow::receive_args(int argc, char* argv[]) {
//     // config and history
//     if (argc == 2)
//         display(argv[1]);
//     else {
//         QFile history(".history");
//         if (history.open(QFile::ReadOnly | QFile::Text)) {
//             QTextStream in(&history);
//             in >> config;
//             history.close();
//             // auto-save
//             if (QFile::exists(".autosave") &&
//                 QMessageBox::question(this, "Retrive", "Your document was not saved before an unexpected shutdown. Retrive your document?") == QMessageBox::Yes
//                 ) {
//                 set_filename("");
//                 display(".autosave", false);
//                 set_dirty(true);
//             }
//             else
//                 display(config.file_path);
//             auto cursor = ui->textEdit->textCursor();
//             cursor.setPosition(config.cursor_pos);
//             ui->textEdit->setTextCursor(cursor);
//             ui->textEdit->setFont(QFont("Consolas", fontSize));
//         }
//         else
//             on_actionNew_triggered();
//     }
// }

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
            // if (shift_pressed)
            //     on_actionSave_As_triggered();
            // else
            on_actionSave_triggered();
        }
        break;
    // case Qt::Key_N:
    //     if (ctrl_pressed)
    //         on_actionNew_triggered();
    //     break;
    // case Qt::Key_O:
    //     if (ctrl_pressed)
    //         on_actionOpen_triggered();
    //     break;
    case Qt::Key_Equal:
        if (ctrl_pressed) {
            fontSize += 2;
            ui->textEdit->setFont(QFont("Consolas", fontSize));
        }
        break;
    case Qt::Key_Minus:
        if (ctrl_pressed) {
            fontSize -= 2;
            ui->textEdit->setFont(QFont("Consolas", fontSize));
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

// void MainWindow::display(QString filename, bool updateFilename) {
//     QFile sFile(filename);
//     if (sFile.open(QFile::ReadOnly)) {
//         if (updateFilename)
//             set_filename(filename);
//         QByteArray content = sFile.readAll();
//         sFile.close();
//         bool ok; QString password;
//         password = QInputDialog::getText(this, "password", QString("Password for encripted document %1:").arg(config.file_path), QLineEdit::Password, "", &ok);
//         if (ok) {
//             config.password = password.toULong();
//             QString text = decript(content, config.password);
//             ui->textEdit->setPlainText(text);
//             text_connection = connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_text_modified()));
//             timer->start();
//             return;
//         }
//     }
//     on_actionNew_triggered();
// }

// void MainWindow::on_actionNew_triggered() {
//     close_current();
//     text_connection = connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(on_text_modified()));
//     timer->start();
//     set_filename("");
//     ui->textEdit->setPlainText("");
//     set_dirty(true);
// }

// void MainWindow::on_actionOpen_triggered() {
//     QString file = QFileDialog::getOpenFileName(this, "Open a file", "", "Encripted text files(*.enc)");
//     if (!file.isEmpty()) {
//         close_current();
//         display(file);
//     }
// }

void MainWindow::on_actionSave_triggered() {
    save_current();
}

// void MainWindow::on_actionSave_As_triggered() {
//     QString file = QFileDialog::getSaveFileName(this, "Save as", "", "Encripted text files (*.enc)");
//     if (!file.isEmpty()) {
//         set_filename(file);
//         QString confirm_password, password;
//         while (true) {
//             password = QInputDialog::getText(this, "password", QString("A digit password to encript document %1:").arg(config.file_path), QLineEdit::Password);
//             confirm_password = QInputDialog::getText(this, "password", "Please confirm your password:", QLineEdit::Password);
//             if (password != confirm_password)
//                 QMessageBox::warning(this, "password", "Password mismatch!");
//             else
//                 break;
//         }
//         config.password = password.toULong();
//         save_current(true);
//     }
// }

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
//     if (!dirty) return;
//     QString text = ui->textEdit->toPlainText();
//     QFile sFile(".autosave");
//     if (sFile.open(QFile::WriteOnly)) {
//         sFile.write(encript(text, config.password));
//         sFile.close();
//     }
//     else
//         QMessageBox::critical(this, "Failure", "Cannot write file; please check permissions.");
// }

// void MainWindow::set_filename(QString filename) {
//     config.file_path = filename;
//     int i = filename.length();
//     if (!i)
//         filename = "new*";
//     while (i && filename[--i] != '/')
//         ;
//     setWindowTitle(filename.mid(i + (filename[i] == '/')) + " - EncEdit");
// }

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

// void MainWindow::close_current() {
//     disconnect(text_connection); timer->stop();
//     if (dirty && !ui->textEdit->toPlainText().isEmpty()) {
//         auto response = QMessageBox::question(this, "Save", "Save your document?");
//         if (response == QMessageBox::Yes)
//             save_current();
//     }
//     set_dirty(false);
//     QFile autosave(".autosave");
//     autosave.remove();
// }

void MainWindow::save_current(bool saveClean) {
    if (!dirty && !saveClean)
        return;
    pio->save_entry(ui->textEdit->toPlainText(), entryIdx);
    set_dirty(false);
    // QFile sFile(config.file_path);
    // if (sFile.open(QFile::WriteOnly)) {
    //     sFile.write(encript(text, config.password));
    //     sFile.close();
    //     set_dirty(false);
    // }
    // else
    //     on_actionSave_As_triggered();
}
