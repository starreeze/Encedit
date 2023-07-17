#ifndef FILEIO_H
#define FILEIO_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include "args.h"

QByteArray encrypt(const QString& s, uint64_t key, int skip_times = 0);
QString decrypt(const QByteArray& s, uint64_t key);

class FileIo {
public:
    QString buffer, file_path;
    FileIo() {}
    FileIo(uint64_t crypt_key, const QString& filename) :
        file_path(filename), key(crypt_key) {}
    QString read();
    void write(const QString& text, bool rewrite_all = false);
    void update_key(uint64_t crypt_key);
private:
    uint64_t key;
};

QString color2str(QRgb color);
#endif // FILEIO_H
