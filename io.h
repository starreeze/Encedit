#ifndef FILEIO_H
#define FILEIO_H
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QVariant>
#include <QSettings>
#include <QDateTime>
#include <QColor>
// all operations interacting with disk IO
// text file save & load with encryption; configuration management

class MainWindow;

QByteArray encrypt(const QString& s, uint64_t key, int skip_times = 0);
QString decrypt(const QByteArray& s, uint64_t key);

class FileIo {
public:
    QString buffer, file_path;
    FileIo() {}
    FileIo(uint64_t crypt_key, const QString& filename) :
        file_path(filename), key(crypt_key) {}
    QString read();
    // return if actually write something
    bool write(const QString& text, bool rewrite_all = false);
    void update_key(uint64_t crypt_key);
private:
    uint64_t key;
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
