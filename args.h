#ifndef ARGS_H
#define ARGS_H

#include <QColor>
#include <QVariant>
#include <QVector>
#include <QSize>
#include <QPoint>

typedef const char* const LSTR;

constexpr int default_text_fontsize = 18, contents_fontsize = 12, max_history_entry_num = 9;
LSTR default_text_fontname = "Microsoft YaHei";
LSTR contents_fontname = "Consolas";

constexpr QRgb default_font_color = qRgb(0,0,0), default_background_color = qRgb(255,255,255);

constexpr QSize default_window_size(1600, 900);
constexpr QPoint default_window_pos(100, 100);

constexpr bool default_sidebar_status = true, default_frameless_status = false;

LSTR org_name = "Xsy";
LSTR app_name = "Encedit";
LSTR version_string = "v2.4";

// 2021.7.24 Sat <weather>
LSTR default_title_regexp = "\\d{4}(\\.\\d{1,2}){2} \\w{3} .";

LSTR time_format = "yyyy.MM.dd hh:mm";

LSTR additional_welcome_message =
"Welcome to encedit!\n"
"The following content is the shotcut you should know.\n"
"But don't worry if you can't remember;\n"
"you can always press Ctrl-H to show this help.\n";

LSTR help_text_full =
"Ctrl - <NUM>: open a recent file;\n"
"Ctrl - Alt - <ARROW_KEY>: adjust window position;\n"
"Ctrl - A: select all;\n"
"Ctrl - C: copy;\n"
"Ctrl - X: cut;\n"
"Ctrl - V: paste;\n"
"Ctrl - Z: undo;\n"
"Ctrl - Shift - Z <OR> Ctrl - Y: redo;\n"
"Ctrl - S: save current file;\n"
"Ctrl - Shift - S: save as;\n"
"Ctrl - N: new file;\n"
"Ctrl - O: open file;\n"
"Ctrl - F: select font family and size;\n"
"Ctrl - L: select font color and background color;\n"
"Ctrl - R: set index match rule(regular expression);\n"
"Ctrl - T: toggle index(side widget) view;\n"
"Ctrl - B: toggle frameless mode.";

LSTR help_text_short = "\nPress Ctrl - H to show more help.\n";
LSTR dismiss_reminder = "\nTo start with a blank new file, press any key to dismiss this message...\n";
LSTR history_prompt = "Recent files (press Crtl - <NUM> to open the corresponding file):\n";

extern const QVector<QPair<QString, QVariant>> default_config;

#endif // ARGS_H
