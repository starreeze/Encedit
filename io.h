#ifndef FILEIO_H
#define FILEIO_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include "crypto.h"
#include "args.h"

struct Config
{
    QString font_name = default_fontname, title_regexp = default_title_regexp;
    int cursor_pos = 0, font_size = default_text_fontsize;
    QRgb font_color = default_font_color, background_color = default_background_color;
};


struct FileIo
{
    QString file_text, file_path;
    uint64_t key;
    FileIo() {}
    FileIo(uint64_t crypt_key, const QString& filename):
        file_path(filename), key(crypt_key) {}
    QString read();
    void write(const QString& text, bool rewrite_all = false);
};


QString color2str(QRgb color);

void load_conf(QTextStream& in, Config& conf, FileIo& io);
void save_conf(QTextStream& out, const Config& conf, const FileIo& io);

#endif // FILEIO_H
