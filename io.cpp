#include "io.h"

QString FileIo::read() {
    QFile file(file_path);
    file.open(QFile::ReadOnly);
    QByteArray content = file.readAll();
    file.close();
    return decrypt(content, key);
}

void FileIo::write(const QString& text, bool rewrite_all) {
    QFile file(file_path);
    file.open(QFile::WriteOnly | QFile::Append);
    if (rewrite_all)
        file.write(encrypt(text, key));
    else {
        int i = 0, original_size = file_text.size(), new_size = text.size();
        for (; i < qMin(new_size, original_size) && file_text[i] == text[i]; ++i);
        file_text.resize(i);
        const QString& new_text = text.mid(i);
        file_text.append(new_text);
        i -= i % 4;
        auto modified = encrypt(new_text, key, i / 4);
        i *= 2;
        file.resize(i);
        file.seek(i);
        file.write(modified);
    }
    file.close();
}

QString color2str(QRgb color) {
    return QString("%1, %2, %3").arg(QColor(color).red()).arg(QColor(color).green()).arg(QColor(color).blue());
}
