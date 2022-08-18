#include "io.h"

static QString get_line_str_arg(QTextStream& in) {
    QString arg = in.readLine();
    if (arg.size() && arg.back() == '\n')
        arg = arg.mid(0, arg.length() - 1);
    return arg;
}

QString FileIo::read()
{
    QFile file(file_path);
    file.open(QFile::ReadOnly);
    QByteArray content = file.readAll();
    file.close();
    file_text = decrypt(content, key);
    return file_text;
}

void FileIo::write(const QString &text, bool rewrite_all)
{
    QFile file(file_path);
    file.open(QFile::WriteOnly | QFile::Append);
    if (rewrite_all)
        file.write(encrypt(text, key));
    else {
        int i = 0, original_size = file_text.size(), new_size = text.size();
        for (; i < qMin(new_size, original_size) && file_text[i] == text[i]; ++i);
        file_text.resize(i);
        file_text.append(text.midRef(i));
        i -= i % 4;
        auto modified = encrypt(text.mid(i), key, i / 4);
        i *= 2;
        file.resize(i);
        file.seek(i);
        file.write(modified);
    }
    file.close();
}

void load_conf(QTextStream &in, Config &conf, FileIo &io)
{
    io.file_path = get_line_str_arg(in);
    conf.font_name = get_line_str_arg(in);
    conf.title_regexp = get_line_str_arg(in);
    in >> conf.cursor_pos >> conf.font_size >> conf.font_color >> conf.background_color;
    if (!conf.font_size)
        conf.font_size = default_text_fontsize;
    if (!conf.font_name.size())
        conf.font_name = default_fontname;
    if (!conf.title_regexp.size())
        conf.title_regexp = default_title_regexp;
    if (!conf.font_color)
        conf.font_color = default_font_color;
    if (!conf.background_color)
        conf.background_color = default_background_color;
}

void save_conf(QTextStream &out, const Config &conf, const FileIo &io)
{
    out << io.file_path << '\n'
        << conf.font_name << '\n'
        << conf.title_regexp << '\n'
        << conf.cursor_pos << '\n'
        << conf.font_size << '\n'
        << conf.font_color << '\n'
        << conf.background_color;
}

QString color2str(QRgb color)
{
     return QString("%1, %2, %3").arg(QColor(color).red()).arg(QColor(color).green()).arg(QColor(color).blue());
}
