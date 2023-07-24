#ifndef FILEIO_H
#define FILEIO_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <QSettings>
#include <QDateTime>
#include <QColor>
#include "aes.h"
#include "args.h"
// all operations interacting with disk IO
// text file save & load with encryption; configuration management

class MainWindow;

class Cipher {
    QByteArray key;
    QAESEncryption aes_cipher = { QAESEncryption::Aes(aes_type), QAESEncryption::ECB };
public:
    inline Cipher() {}
    inline Cipher(const QString& crypt_key) { if (!crypt_key.isEmpty()) update_key(crypt_key); }
    inline QByteArray encrypt(const QString& s);
    inline QString decrypt(const QByteArray& s);
    void update_key(const QString& crypt_key);
};

class FileIo {
    Cipher cipher;
public:
    QString buffer, file_path;
    FileIo() {}
    FileIo(const QString& crypt_key, const QString& filename) : file_path(filename), cipher(crypt_key) {}
    QString read();
    // return if actually write something
    bool write(const QString& text, bool rewrite_all = false);
    void update_key(const QString& crypt_key);
};

class MWSettings : public QSettings {
    Q_OBJECT
public:
    MWSettings(const MainWindow* parent);
    void save();
};

struct HLEntry { // history list entry
    QDateTime time;
    QString file;
    quint32 cursor;
    HLEntry() {}
    HLEntry(const QString& filename, quint32 cursor_position);
    inline bool operator<(const HLEntry& hle) const { return time > hle.time; }
    QString repr() const;
};

inline QDataStream& operator<<(QDataStream& out, const HLEntry& v) {
    return out << v.time << v.cursor << v.file;
}

inline QDataStream& operator>>(QDataStream& in, HLEntry& v) {
    return in >> v.time >> v.cursor >> v.file;
}

class HistoryList {
    QList<HLEntry> data;
    QSettings* setting;
public:
    HistoryList(QSettings* settings);
    QString repr(); // sort before return
    inline const HLEntry& get_latest() const { return data[0]; }
    void update_now(const QString& file, quint32 cursor);
    HLEntry get_entry(const QString& filename);
    inline const HLEntry& get_entry(int idx) const { return data[idx]; }
    inline const int get_size() const { return data.size(); }
    ~HistoryList();
};

QString color2str(QRgb color);
#endif // FILEIO_H
