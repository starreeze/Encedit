#ifndef CONFIG_H
#define CONFIG_H

#include <QString>
#include <QTextStream>

struct Config {
	QString file_path;
	int font_size = 20;
	uint64_t password;
};

QTextStream& operator<<(QTextStream& out, const Config& conf);
QTextStream& operator>>(QTextStream& in, Config& conf);

#endif
