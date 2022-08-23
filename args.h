#ifndef ARGS_H
#define ARGS_H

#include <QColor>

typedef const char* const LSTR;
constexpr int default_text_fontsize = 18, contents_fontsize = 12;
constexpr QRgb default_font_color = qRgb(0,0,0), default_background_color = qRgb(255,255,255);
LSTR status_file_name = "encedit-status.txt";
// 2021.7.24 Sat <weather>
LSTR default_title_regexp = "\\d{4}(\\.\\d{1,2}){2} \\w{3} .";
LSTR default_fontname = "Consolas";

LSTR additional_welcome_message =
"Welcome to encedit!\n"
"The following content is the shotcut you should know.\n"
"But don't worry if you can't remember;\n"
"you can always press Ctrl-H to show this help.\n";

LSTR help_text =
"Ctrl - A: select all;\n"
"Ctrl - C: copy;\n"
"Ctrl - X: cut;\n"
"Ctrl - V: paste;\n"
"Ctrl - Z: undo;\n"
"Ctrl - Shift - Z: redo;\n"
"Ctrl - S: save current file;\n"
"Ctrl - Shift - S: save as;\n"
"Ctrl - N: new file;\n"
"Ctrl - O: open file;\n"
// "Ctrl - Shift - O: load file in plain text mode(skip decrytion)\n"
"Ctrl - F: select font family and size;\n"
"Ctrl - L: select font color and background color;\n"
"Ctrl - R: set index match rule(regular expression);\n"
"Ctrl - T: toggle index(side widget) view;\n"
"Ctrl - B: toggle frameless mode;\n"
"Ctrl - =: zoom in;\n"
"Ctrl - -: zoom out.";

LSTR dismiss_reminder = "\nTo start typing, press any key to dismiss this message...\n";

#endif // ARGS_H
