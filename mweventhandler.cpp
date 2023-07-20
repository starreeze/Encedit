#include "mweventhandler.h"
#include "ui_mainwindow.h"
#include "mainwindow.h"
#include <QTimer>
#include <QMessageBox>
#include <QFontDialog>
#include <QInputDialog>
#include <QColorDialog>
using namespace Qt;

void MWEventHandler::key_press_handler(QKeyEvent* keyEvent) {
	int key = keyEvent->key();
	if (object->ui->textEdit->isReadOnly() && key != Key_Control && key != Key_Shift && key != Key_Alt && key != Key_CapsLock && key != Key_Super_L) {
		// it is displaying the welcome message on opening a new file
		if (ctrl_pressed) { // its a functional key
			ctrl_pressed = false; // a fallback scheme that won't allow ctrl + 2 keys to prevent key release problems
			if (key >= Key_1 && key <= Key_9) {
				if (key - Key_1 >= object->history_list->get_size())
					return;
				auto history_entry = object->history_list->get_entry(key - Key_1);
				if (QFile::exists(history_entry.file)) {
					object->display(history_entry.file);
					object->ui->textEdit->setReadOnly(false);
				}
				else
					QMessageBox::warning(object, "File not exists", "The file has been moved or deleted.");
			}
			else if (ctrl_key_handler(key))
				object->ui->textEdit->setReadOnly(key != Key_O);
			return;
		}
		// otherwise, start input of a new file
		// Do not change the std filename to leave it default ""
		object->ui->textEdit->clear();
		object->text_connection = object->connect(object->ui->textEdit, &QPlainTextEdit::textChanged, object, &MainWindow::on_text_modified);
		object->ui->textEdit->setReadOnly(false);
		object->timer->start();
		return;
	}
	if (key == Key_Control)
		ctrl_pressed = true;
	else if (key == Key_Shift)
		shift_pressed = true;
	else if (ctrl_pressed) {
		ctrl_key_handler(key);
		ctrl_pressed = false;
	}
}

void MWEventHandler::key_release_handler(QKeyEvent* keyEvent) {
	switch (keyEvent->key()) {
	case Key_Control:
		ctrl_pressed = false;
		break;
	case Key_Shift:
		shift_pressed = false;
		break;
	}
}

bool MWEventHandler::ctrl_key_handler(int key) {
	switch (key) {
		// reserved: ACDKSXZ
	case Key_H:
		QMessageBox::information(object, "shortcut help", help_text_full);
		return true;
	case Key_S:
		if (shift_pressed)
			object->on_actionSave_As_triggered();
		else
			object->on_actionSave_triggered();
		return true;
	case Key_N:
		object->on_actionNew_triggered();
		return true;
	case Key_O:
		return object->on_actionOpen_triggered();
	case Key_F: {
		bool ok;
		QFont font = QFontDialog::getFont(&ok, object->ui->textEdit->font(), object);
		if (ok) {
			object->setting->setValue("font", font);
			object->ui->textEdit->setFont(font);
		}
		return ok;
	}
	case Key_R: {
		bool ok;
		QString regexp = QInputDialog::getText(object, "title regexp", QString("Enter an regexp to match titles in your passage for contents to display:"), QLineEdit::Normal, object->setting->value("title_regexp").toString(), &ok);
		if (ok) {
			object->setting->setValue("title_regexp", regexp);
			object->update_index(object->ui->textEdit->toPlainText(), regexp);
		}
		return ok;
	}
	case Key_M: {
		bool ok_size, ok_interval;
		int max_autosave_size = QInputDialog::getInt(object, "max autosave size (KB)", "Enter max autosave filesize in KB; autosave will be disabled on larger file:", object->setting->value("max_autosave_size").toInt(), 0, 1024 * 1024, 1, &ok_size);
		if (ok_size)
			object->setting->setValue("max_autosave_size", QVariant::fromValue(max_autosave_size));
		int autosave_interval = QInputDialog::getInt(object, "autosave interval (second)", "Enter autosave interval in second", object->timer->interval() / 1000, 0, 3600, 1, &ok_interval);
		if (ok_interval)
			object->timer->setInterval(autosave_interval * 1000);
		return ok_size || ok_interval;
	}
	case Key_T:
		if (object->ui->listWidget->isHidden())
			object->ui->listWidget->show();
		else    object->ui->listWidget->hide();
		return true;
	case Key_B:
		object->hide();
		object->setWindowFlag(FramelessWindowHint, !object->windowFlags().testFlag(FramelessWindowHint));
		object->show();
		return true;
	case Key_L: {
		QColor selected = QColorDialog::getColor(black, object, "Font color");
		if (selected.isValid()) {
			object->setting->setValue("font_color", selected.rgb());
			selected = QColorDialog::getColor(white, object, "Background color");
			if (selected.isValid()) {
				object->setting->setValue("background_color", selected.rgb());
				object->update_style();
				return true;
			}
		}
		return false;
	}
	case Key_U:
		if (object->ui->statusBar->isHidden())
			object->ui->statusBar->show();
		else    object->ui->statusBar->hide();
		return true;
	case Key_Left:
		object->move(object->pos().x() - 1, object->pos().y());
		return true;
	case Key_Up:
		object->move(object->pos().x(), object->pos().y() - 1);
		return true;
	case Key_Right:
		object->move(object->pos().x() + 1, object->pos().y());
		return true;
	case Key_Down:
		object->move(object->pos().x(), object->pos().y() + 1);
		return true;
	case Key_Equal: {
		QFont font = object->ui->textEdit->font();
		font.setPointSize(font.pointSize() + 1);
		object->setting->setValue("font", font);
		object->ui->textEdit->setFont(font);
		return true;
	}
	case Key_Minus: {
		QFont font = object->ui->textEdit->font();
		font.setPointSize(font.pointSize() - 1);
		object->setting->setValue("font", font);
		object->ui->textEdit->setFont(font);
		return true;
	}
	}
	return false;
}
