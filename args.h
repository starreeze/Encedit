#ifndef ARGS_H
#define ARGS_H

#include <QColor>
#include <QVariant>
#include <QVector>
#include <QSize>
#include <QPoint>

#define ENCODING utf-8

typedef const char* const LSTR;

constexpr int default_text_fontsize = 18, contents_fontsize = 12, max_history_entry_num = 9, default_max_autosave_filesize_kb = 1024, default_autosave_interval_s = 60, statusbar_message_timeout_s = 0, cursor_recover_delay_ms = 100, crypto_blocksize = 128, aes_type = 0;
LSTR default_text_fontname = "Microsoft YaHei";
LSTR contents_fontname = "Consolas";
LSTR autosave_filepath = ".autosave";

constexpr QRgb default_font_color = qRgb(0, 0, 0), default_background_color = qRgb(255, 255, 255);

constexpr QSize default_window_size(1600, 900);
constexpr QPoint default_window_pos(100, 100);

constexpr bool default_sidebar_status = true, default_frameless_status = false;

LSTR org_name = "Xsy";
LSTR app_name = "Encedit";
LSTR version_string = "v3.0";

// 2021.7.24 Sat <weather>
LSTR default_title_regexp = "\\d{4}(\\.\\d{1,2}){2} \\w{3} .";

LSTR history_time_format = "yyyy.MM.dd hh:mm";
LSTR status_time_format = "hh:mm:ss";

LSTR additional_welcome_message =
"Welcome to EncEdit!\n"
"The following content is the shortcut you should know.\n"
"But don't worry if you can't remember;\n"
"you can always press Ctrl-H to show this help.\n";

LSTR about_message =
"Project URL: github.com/starreeze/encedit\n"
"This is free software. License: GPLv3\n"
"Copyright (c) 2021~2023 by Starreeze";

LSTR help_text_full =
"Ctrl - E: about;\n"
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
"Ctrl - B: toggle frameless mode;\n"
"Ctrl - M: set autosave file size limit and interval;\n"
"Ctrl - =: zoom in;\n"
"Ctrl - -: zoom out.";

LSTR help_text_short = "\nPress Ctrl - H to show more help.\n";
LSTR dismiss_reminder = "\nTo start with a blank new file, press any key to dismiss this message...\n";
LSTR history_prompt = "Recent files (press Ctrl - <NUM> to open the corresponding file):\n";

extern const QVector<QPair<QString, QVariant>> default_config;

#endif // ARGS_H
