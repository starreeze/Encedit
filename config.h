#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QTextStream>

struct Config
{
	QString file_path;
	int cursor_pos = 0, font_size = 20;
	Config() {}
	Config(const QString &current_file_path, int cursor_position, int font_point_size);
};

QTextStream &operator<<(QTextStream &out, const Config &conf);
QTextStream &operator>>(QTextStream &in, Config &conf);

#endif
