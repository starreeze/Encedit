#include "io.h"

uint64_t next;
static uint64_t myrand() {
    return next = next * 1103515245 + 12345;
}

QByteArray encrypt(const QString& str, uint64_t key, int skip_times) {
    if (!key)
        return str.toUtf8();
    next = key;
    for (++skip_times; --skip_times; )
        myrand();
    QString s = str;
    while (s.length() % 4)
        s.append(' ');
    QByteArray res(s.length() * 2, 0);
    for (int i = 0; i < s.length() / 4; ++i) {
        uint64_t word = 0;
        for (int j = 0; j < 4; ++j)
            word |= (uint64_t)(s[4 * i + j].unicode()) << (16 * j);
        word ^= myrand();
        for (int j = 0; j < 8; ++j)
            res[8 * i + j] = (uint8_t)(word >> (8 * j));
    }
    return res;
}

QString decrypt(const QByteArray& s, uint64_t key) {
    if (!key)
        return QString::fromUtf8(s);
    if (s.isEmpty())
        return "";
    next = key;
    QString res;
    res.resize(s.length() / 2);
    for (int i = 0; i < res.length() / 4; ++i) {
        uint64_t word = 0;
        for (int j = 0; j < 8; ++j)
            word |= (uint64_t)(s[8 * i + j] & 0xff) << (8 * j);
        word ^= myrand();
        for (int j = 0; j < 4; ++j)
            res[4 * i + j] = (uint16_t)(word >> (16 * j));
    }
    int idx = res.length();
    while (res[--idx] == ' ');
    return res.mid(0, idx + 1);
}

QString FileIo::read() {
    QFile file(file_path);
    file.open(QFile::ReadOnly);
    QByteArray content = file.readAll();
    file.close();
    return buffer = decrypt(content, key);
}

void FileIo::write(const QString& text, bool rewrite_all) {
    QFile file(file_path);
    if (rewrite_all) {
        file.open(QFile::WriteOnly | QFile::Append);
        file.write(encrypt(text, key));
    }
    else {
        int i = 0, original_size = buffer.size(), new_size = text.size();
        for (; i < qMin(new_size, original_size) && buffer[i] == text[i]; ++i);
        buffer.resize(i);
        buffer.append(text.mid(i));
        i -= i % 4;
        auto modified = encrypt(text.mid(i), key, i / 4);
        if (modified.isEmpty())
            return;
        file.open(QFile::WriteOnly | QFile::Append);
        file.resize(i * 2);
        file.seek(i * 2);
        file.write(modified);
    }
    file.close();
}

void FileIo::update_key(uint64_t crypt_key) {
    key = crypt_key;
    buffer.clear();
}

QString color2str(QRgb color) {
    return QString("%1, %2, %3").arg(QColor(color).red()).arg(QColor(color).green()).arg(QColor(color).blue());
}
