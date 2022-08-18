#include "config.h"

static QString get_line_str_arg(QTextStream& in) {
	QString arg = in.readLine();
    if (arg.size() && arg.back() == '\n')
        arg = arg.mid(0, arg.length() - 1);
	return arg;
}

QTextStream& operator<<(QTextStream& out, const Config& conf) {
	out << conf.file_path << '\n'
		<< conf.font_name << '\n'
        << conf.title_regexp << '\n'
		<< conf.cursor_pos << '\n'
		<< conf.font_size;
	return out;
}

QTextStream& operator>>(QTextStream& in, Config& conf) {
	conf.file_path = get_line_str_arg(in);
	conf.font_name = get_line_str_arg(in);
    conf.title_regexp = get_line_str_arg(in);
	in >> conf.cursor_pos >> conf.font_size;
	if (!conf.font_size)
        conf.font_size = default_text_fontsize;
    if (!conf.font_name.size())
        conf.font_name = default_fontname;
    if (!conf.title_regexp.size())
        conf.title_regexp = default_title_regexp;
    return in;
}
