#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFileDialog>
#include <QKeyEvent>
#include <QSplitter>
#include "indexmodel.h"
#include "io.h"
namespace Ui {
    class MainWindow;
}

class MWEventHandler;

class MainWindow : public QMainWindow {
    Q_OBJECT;
    friend class MWEventHandler;
    friend class MWSettings;
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void receive_args(int argc, char* argv[]);
    void post_show();
    void keyPressEvent(QKeyEvent* keyEvent);
    void keyReleaseEvent(QKeyEvent* keyEvent);

private:
    Ui::MainWindow* ui;
    FileIo std_file;
    FileIo autosave_file = { 0, autosave_filepath };
    MWSettings* setting;
    HistoryList* history_list;
    bool dirty = false;
    QMetaObject::Connection text_connection;
    QTimer* timer;
    IndexModel index;
    QSplitter* splitter;
    MWEventHandler* event_handler;
    // update std_file name and window title
    void set_filename(QString filename);
    void set_cursor_pos(int pos);
    // request password input, read text and then display
    void display(QString filename, bool updateFilename = true);
    void set_dirty(bool val = true);
    void close_current();
    void save_current(bool saveClean = false);
    void update_index(const QString& text, const QString& regexp = "");
    void update_style();
    void create_setting();
    void log(const QString& message, int timeout = statusbar_message_timeout_s);

    void on_actionNew_triggered();
    bool on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
private slots:
    void on_text_modified();
    void auto_save();
    void on_listWidget_clicked(const QModelIndex& index);
};

#endif // MAINWINDOW_H
