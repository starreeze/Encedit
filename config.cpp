#include "config.h"

QTextStream &operator<<(QTextStream &out, const Config &conf)
{
	out << conf.file_path << '\n'
		<< conf.cursor_pos << '\n'
		<< conf.font_size;
	return out;
}

QTextStream &operator>>(QTextStream &in, Config &conf)
{
	QString path = in.readLine();
	if (path.back() == '\n')
		path = path.mid(0, path.length() - 1);
	conf.file_path = path;
	in >> conf.cursor_pos >> conf.font_size;
	if (!conf.font_size)
		conf.font_size = 20;
	return in;
}