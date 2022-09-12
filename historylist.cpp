#include "historylist.h"
#include "args.h"
#include <algorithm>

HLEntry::HLEntry(const QString &filename, quint32 cursor_position)
    : file(filename), cursor(cursor_position), time(QDateTime::currentDateTime()) {}

QString HLEntry::repr() const {
    return file + ": last accessed\t" + time.toString(time_format);
}

void HistoryList::insert_now(const QString &file, quint32 cursor)
{
    if (data.size() >= max_history_entry_num)
        data.pop_back();
    data.push_front({file, cursor});
}

HistoryList::HistoryList(QSettings *settings) : setting(settings), data(settings->value("history").value<QList<HLEntry>>())
{
//    auto history_list = settings->value("history").toList();
//    for (const QVariant& var: history_list)
//        data.append(var.value<HLEntry>());
}

QString HistoryList::repr()
{
    QString str;
    for (int i = 0; i < data.size(); ++i)
        str.append(QString("%1\t").arg(i + 1) + data[i].repr() + "\n");
    return str;
}

void HistoryList::update_now(const QString &file, quint32 cursor)
{
    if (cursor == 0)
        throw 0;
    HLEntry newEntry(file, cursor);
    for (int i=0; i<data.size(); ++i)
        if (data[i].file == newEntry.file) {
            data[i] = newEntry;
            std::sort(data.begin(), data.end());
            return;
        }
    insert_now(file, cursor);
}

HLEntry HistoryList::get_entry(const QString &filename)
{
    for (int i=0; i<data.size(); ++i)
        if (data[i].file == filename)
            return data[i];
    return {};
}

HistoryList::~HistoryList()
{
    setting->setValue("history", QVariant::fromValue(data));
}
