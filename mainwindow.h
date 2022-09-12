#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFileDialog>
#include <QKeyEvent>
#include <QSettings>
#include "indexmodel.h"
#include "io.h"
#include "historylist.h"
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    void receive_args(int argc, char* argv[]);
    void post_show();
    void keyPressEvent(QKeyEvent* keyEvent);
    void keyReleaseEvent(QKeyEvent* keyEvent);
//private slots:
    // void auto_save();

private:
    Ui::MainWindow* ui;
    FileIo io;
    QSettings* setting;
    HistoryList* history_list;
    bool dirty = false, ctrl_pressed = false, shift_pressed = false;
    QMetaObject::Connection text_connection;
    // QTimer* timer;
    IndexModel index;
    void set_filename(QString filename);
    void set_cursor_pos(int pos);
    bool handle_ctrl_key(int key); // whether is handled
    void display(QString filename, bool updateFilename = true, bool encrypt = true);
    void set_dirty(bool val = true);
    void close_current();
    void save_current(bool saveClean = false);
    void update_index(const QString& text, const QString& regexp = "");
    void update_style();
    void create_setting();

    void on_actionNew_triggered();
    bool on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionCut_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_text_modified();
    void on_listWidget_clicked(const QModelIndex& index);
};

#endif // MAINWINDOW_H
