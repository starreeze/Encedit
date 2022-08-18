#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QTextStream>
#include "args.h"

struct Config
{
    QString file_path, font_name = default_fontname, title_regexp = default_title_regexp;
    int cursor_pos = 0, font_size = default_text_fontsize;
	uint64_t password;
};

QTextStream& operator<<(QTextStream& out, const Config& conf);
QTextStream& operator>>(QTextStream& in, Config& conf);

#endif
