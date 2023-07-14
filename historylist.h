#ifndef HISTORYLIST_H
#define HISTORYLIST_H
#include <QDateTime>
#include <QList>
#include <QSettings>
#include <QDataStream>

struct HLEntry { // history list entry
    QDateTime time;
    QString file;
    quint32 cursor;
    HLEntry() {}
    HLEntry(const QString& filename, quint32 cursor_position);
    inline bool operator<(const HLEntry& hle) const {return time > hle.time;}
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
    inline const HLEntry& get_latest() const {return data[0];}
    void update_now(const QString& file, quint32 cursor);
    HLEntry get_entry(const QString& filename);
    inline const HLEntry& get_entry(int idx) const { return data[idx]; }
    inline const int get_size() const { return data.size(); }
    ~HistoryList();
};

Q_DECLARE_METATYPE(HLEntry)
Q_DECLARE_METATYPE(QList<HLEntry>)

#endif // HISTORYLIST_H
