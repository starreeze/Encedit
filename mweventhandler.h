#ifndef MWEVENTHANDLER_H
#define MWEVENTHANDLER_H
// event handlers for main window class
#include <QKeyEvent>

class MainWindow;

class MWEventHandler {
    MainWindow* object;
    bool ctrl_pressed = false, shift_pressed = false;
public:
    MWEventHandler(const MainWindow* p) : object(const_cast<MainWindow*>(p)) {}
    void key_press_handler(QKeyEvent* keyEvent);
    void key_release_handler(QKeyEvent* keyEvent);
    bool ctrl_key_handler(int key); // return if the pressed key takes effect
};

#endif // MWEVENTHANDLER_H
