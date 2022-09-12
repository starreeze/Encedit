#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QException>
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
#include <QDebug>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    create_setting();
    history_list = new HistoryList(setting);
    ui->centralWidget->setLayout(ui->horizontalLayout);
    ui->textEdit->setFocus();
    move(setting->value("window_pos").value<QPoint>());
    resize(setting->value("window_size").value<QSize>());
    setWindowFlag(Qt::FramelessWindowHint, setting->value("frameless").toBool());
    ui->listWidget->setHidden(!setting->value("sidebar").toBool());
    // timer = new QTimer(this);
    // timer->setInterval(60000);
    // timer->callOnTimeout(this, &MainWindow::auto_save);
}

MainWindow::~MainWindow() {
    close_current();
    setting->setValue("window_pos", QVariant::fromValue(pos()));
    setting->setValue("window_size", QVariant::fromValue(size()));
    setting->setValue("frameless", windowFlags().testFlag(Qt::FramelessWindowHint));
    setting->setValue("sidebar", !ui->listWidget->isHidden());
    delete history_list;
    delete ui;
}

void MainWindow::receive_args(int argc, char* argv[]) {
    // config and history
    if (argc == 2)
        display(argv[1]);
    else {
        if (history_list->repr().size()) { // not empty history
            // auto-save
//            if (QFile::exists(".autosave") &&
//                QMessageBox::question(this, "Retrive", "Your document was not saved before an unexpected shutdown. Retrive your document?") == QMessageBox::Yes
//                ) {
//                set_filename("");
//                display(".autosave", false);
//                set_dirty(true);
//            }
//            else
            display(history_list->get_latest().file);
            update_style();
        }
        else
            on_actionNew_triggered();
    }
}

void MainWindow::post_show() {
    auto _font = setting->value("font").value<QFont>();
    ui->textEdit->setFont(_font);
    ui->listWidget->setFont(QFont(contents_fontname, contents_fontsize));
}

void MainWindow::keyPressEvent(QKeyEvent* keyEvent) {
    using namespace Qt;
    auto key = keyEvent->key();
    if (ui->textEdit->isReadOnly() && key != Key_Control && key != Key_Shift && key != Key_Alt && key != Key_CapsLock && key != Key_Super_L) {
        if (ctrl_pressed) {
            if (key >= Key_1 && key <= Key_9) {
                try { // index out of range
                    auto history_entry = history_list->get_entry(key - Key_1);
                    display(history_entry.file);
                    ui->textEdit->setReadOnly(false);
                }
                catch (QException) { /* do nothing to wait for next input */ }
            }
            else if (handle_ctrl_key(key))
                ui->textEdit->setReadOnly(key != Key_O);
            return;
        }
        ui->textEdit->clear();
        set_dirty(true);
        text_connection = connect(ui->textEdit, &QPlainTextEdit::textChanged, this, &MainWindow::on_text_modified);
        ui->textEdit->setReadOnly(false);
        return;
    }
    if (key == Key_Control)
        ctrl_pressed = true;
    else if (key == Key_Shift)
        shift_pressed = true;
    else if (ctrl_pressed)
        handle_ctrl_key(key);
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

void MainWindow::display(QString filepath, bool updateFilename, bool encrypt) {
    if (!QFile::exists(filepath)) {
        on_actionNew_triggered();
        return;
    }
    bool ok; QString password;
    password = QInputDialog::getText(this, "password", QString("Password for document %1 (leave it blank if this is a plain text document):").arg(filepath), QLineEdit::Password, "", &ok);
    if (ok) {
        if (updateFilename)
            set_filename(filepath);
        io.key = password.toULong();
        io.file_path = filepath;
        QString text = io.read();
        ui->textEdit->setPlainText(text);
        int cursor_pos = history_list->get_entry(filepath).cursor;
        if (cursor_pos)
            set_cursor_pos(cursor_pos);
        update_index(text);
        text_connection = connect(ui->textEdit, &QPlainTextEdit::textChanged, this, &MainWindow::on_text_modified);
    }
    else    on_actionNew_triggered();
}

void MainWindow::on_actionNew_triggered() {
    close_current();
    // timer->start();
    set_filename("");
    const QString& history_repr = history_list->repr();
    if (history_repr.isEmpty())
        ui->textEdit->setPlainText(QString(additional_welcome_message) + help_text_full + dismiss_reminder);
    else
        ui->textEdit->setPlainText(QString(history_prompt) + history_repr + help_text_short + dismiss_reminder);
    ui->textEdit->setReadOnly(true);
}

bool MainWindow::on_actionOpen_triggered() {
    QString file = QFileDialog::getOpenFileName(this, "Open a file", "", "Text files(*.enc *.txt)");
    if (!file.isEmpty()) {
        close_current();
        display(file);
        return true;
    }
    return false;
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

void MainWindow::set_cursor_pos(int pos) {
    auto cursor = ui->textEdit->textCursor();
    cursor.setPosition(pos);
    ui->textEdit->setTextCursor(cursor);
}

bool MainWindow::handle_ctrl_key(int key) {
    using namespace Qt;
    switch (key) {
        // reserved: ACDKSXZ
    case Key_H:
        QMessageBox::information(this, "shotcut help", help_text_full);
        return true;
    case Key_S:
        if (shift_pressed)
            on_actionSave_As_triggered();
        else
            on_actionSave_triggered();
        return true;
    case Key_N:
        on_actionNew_triggered();
        return true;
    case Key_O:
        return on_actionOpen_triggered();
    case Key_F: {
        bool ok;
        QFont font = QFontDialog::getFont(&ok, ui->textEdit->font(), this);
        if (ok) {
            setting->setValue("font", font);
            ui->textEdit->setFont(font);
        }
        return ok;
    }
    case Key_R: {
        bool ok;
        QString regexp = QInputDialog::getText(this, "title regexp", QString("Enter an regexp to match titles in your passage for contents to display:"), QLineEdit::Normal, setting->value("title_regexp").toString(), &ok);
        if (ok) {
            setting->setValue("title_regexp", regexp);
            update_index(ui->textEdit->toPlainText(), regexp);
        }
        return ok;
    }
    case Key_T:
        if (ui->listWidget->isHidden())
            ui->listWidget->show();
        else    ui->listWidget->hide();
        return true;
    case Key_B:
        hide();
        setWindowFlag(FramelessWindowHint, !windowFlags().testFlag(FramelessWindowHint));
        show();
        return true;
    case Key_L: {
        QColor selected = QColorDialog::getColor(black, this, "Font color");
        if (selected.isValid()) {
            setting->setValue("font_color", selected.rgb());
            selected = QColorDialog::getColor(white, this, "Background color");
            if (selected.isValid()) {
                setting->setValue("background_color", selected.rgb());
                update_style();
                return true;
            }
        }
        return false;
    }
    case Key_Left:
        move(pos().x() - 1, pos().y());
        return true;
    case Key_Up:
        move(pos().x(), pos().y() - 1);
        return true;
    case Key_Right:
        move(pos().x() + 1, pos().y());
        return true;
    case Key_Down:
        move(pos().x(), pos().y() + 1);
        return true;
    }
    return false;
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
    if (io.file_path.size())
        history_list->update_now(io.file_path, ui->textEdit->textCursor().position());
//    QFile autosave(".autosave");
//    autosave.remove();
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
    index.load(text, regexp.size() ? regexp : setting->value("title_regexp").toString());
    ui->listWidget->clear();
    auto& strl = index.string_list();
    ui->listWidget->addItems(strl);
    connect(ui->listWidget, &QAbstractItemView::clicked, this, &MainWindow::on_listWidget_clicked);
}

void MainWindow::update_style() {
    QString style = QString("{background-color: rgb(%1); color: rgb(%2);}").arg(color2str(setting->value("background_color").toInt())).arg(color2str(setting->value("font_color").toInt()));
    ui->textEdit->setStyleSheet("QPlainTextEdit " + style);
    ui->listWidget->setStyleSheet("QListWidget " + style);
}

void MainWindow::create_setting() {
    QString appDataDirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    appDataDirPath = appDataDirPath.mid(0, appDataDirPath.lastIndexOf('/'));
    appDataDirPath = appDataDirPath.mid(0, appDataDirPath.lastIndexOf('/'));
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, appDataDirPath);
    QSettings::setDefaultFormat(QSettings::IniFormat);
    setting = new QSettings(this);
    for (const auto& conf : default_config)
        if (!setting->contains(conf.first))
            setting->setValue(conf.first, conf.second);
}

void MainWindow::on_listWidget_clicked(const QModelIndex& idx) {
    auto cursor = ui->textEdit->textCursor();
    cursor.setPosition(ui->textEdit->toPlainText().size());
    ui->textEdit->setTextCursor(cursor);
    cursor.setPosition(index[idx]);
    ui->textEdit->setTextCursor(cursor);
}
