#ifndef ARGS_H
#define ARGS_H

#include <QColor>

typedef const char* const LSTR;
LSTR status_file_name = "encedit-status.txt";
// 2021.7.24 Sat <weather>
LSTR default_title_regexp = "\\d{4}(\\.\\d{1,2}){2} \\w{3} .";
LSTR default_fontname = "Consolas";
constexpr int default_text_fontsize = 20, contents_fontsize = 12;
constexpr QRgb default_font_color = Qt::black, default_background_color = Qt::white;

#endif // ARGS_H
