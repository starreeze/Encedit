#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QFileDialog>
#include <QKeyEvent>
#include "indexmodel.h"
#include "io.h"
#include "args.h"
namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();
    void receive_args(int argc, char* argv[]);
    void keyPressEvent(QKeyEvent* keyEvent);
    void keyReleaseEvent(QKeyEvent* keyEvent);
private slots:
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_As_triggered();
    void on_actionCopy_triggered();
    void on_actionPaste_triggered();
    void on_actionCut_triggered();
    void on_actionUndo_triggered();
    void on_actionRedo_triggered();
    void on_text_modified();
    // void auto_save();
    void on_listWidget_clicked(const QModelIndex& index);

private:
    Ui::MainWindow* ui;
    FileIo io;
    Config config;
    bool dirty = false, ctrl_pressed = false, shift_pressed = false;
    QMetaObject::Connection text_connection;
    // QTimer* timer;
    IndexModel index;
    QString status_path;
    void set_filename(QString filename);
    void display(QString filename, bool updateFilename = true, bool encrypt = true);
    void set_dirty(bool val = true);
    void close_current();
    void save_current(bool saveClean = false);
    void update_index(const QString& text, const QString& regexp = "");
    void update_style();
};

#endif // MAINWINDOW_H
