#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QtGui>
#include <QtCore>
#include <QFileDialog>
#include <QKeyEvent>
#include "config.h"
namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
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
    void auto_save();
private:
    Ui::MainWindow* ui;
    Config config;
    bool dirty = false, ctrl_pressed = false, shift_pressed = false;
    QMetaObject::Connection text_connection;
    QTimer* timer;
    QByteArray content;
    void set_filename(QString filename);
    void display(QString filename, bool updateFilename = true);
    void set_dirty(bool val);
    void close_current();
    void save_current(bool saveClean = false);
};

#endif // MAINWINDOW_H
