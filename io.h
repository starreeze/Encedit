#ifndef FILEIO_H
#define FILEIO_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include "crypto.h"
#include "args.h"

struct FileIo
{
    QString file_text, file_path;
    uint64_t key;
    FileIo() {}
    FileIo(uint64_t crypt_key, const QString& filename) :
        file_path(filename), key(crypt_key) {}
    QString read();
    void write(const QString& text, bool rewrite_all = false);
};

QString color2str(QRgb color);
#endif // FILEIO_H
