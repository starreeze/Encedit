#include "io.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardPaths>
#include <QTimer>

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

bool FileIo::write(const QString& text, bool rewrite_all) {
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
        if (i == new_size)
            return false;
        i -= i % 4;
        auto modified = encrypt(text.mid(i), key, i / 4);
        file.open(QFile::WriteOnly | QFile::Append);
        file.resize(i * 2);
        file.seek(i * 2);
        file.write(modified);
    }
    file.close();
    return true;
}

void FileIo::update_key(uint64_t crypt_key) {
    key = crypt_key;
    buffer.clear();
}

QString color2str(QRgb color) {
    return QString("%1, %2, %3").arg(QColor(color).red()).arg(QColor(color).green()).arg(QColor(color).blue());
}

MWSettings::MWSettings(const MainWindow* mw) : QSettings(const_cast<MainWindow*>(mw)) {
    QString appDataDirPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    appDataDirPath = appDataDirPath.mid(0, appDataDirPath.lastIndexOf('/'));
    appDataDirPath = appDataDirPath.mid(0, appDataDirPath.lastIndexOf('/'));
    QSettings::setPath(QSettings::IniFormat, QSettings::UserScope, appDataDirPath);
    QSettings::setDefaultFormat(QSettings::IniFormat);
    for (const auto& conf : default_config)
        if (!contains(conf.first))
            setValue(conf.first, conf.second);
}

void MWSettings::save() {
    const MainWindow* mw = static_cast<const MainWindow *>(parent());
    setValue("window_pos", QVariant::fromValue(mw->pos()));
    setValue("window_size", QVariant::fromValue(mw->size()));
    setValue("frameless", mw->windowFlags().testFlag(Qt::FramelessWindowHint));
    setValue("sidebar", !mw->ui->listWidget->isHidden());
    setValue("statusbar", !mw->ui->statusBar->isHidden());
    setValue("splitter_state", QVariant::fromValue(mw->splitter->saveState()));
    setValue("autosave_interval", mw->timer->interval());
}

HLEntry::HLEntry(const QString& filename, quint32 cursor_position)
    : file(filename), cursor(cursor_position), time(QDateTime::currentDateTime()) {}

QString HLEntry::repr() const {
    return file + ": last accessed\t" + time.toString(history_time_format);
}

HistoryList::HistoryList(QSettings* settings) :
    setting(settings), data(settings->value("history").value<QList<HLEntry>>()) {}

QString HistoryList::repr() {
    QString str;
    for (int i = 0; i < data.size(); ++i)
        str.append(QString("%1\t").arg(i + 1) + data[i].repr() + "\n");
    return str;
}

void HistoryList::update_now(const QString& file, quint32 cursor) {
    if (file == autosave_filepath)
        return;
    HLEntry newEntry(file, cursor);
    for (int i = 0; i < data.size(); ++i)
        if (data[i].file == newEntry.file) { // if found, update
            data[i] = newEntry;
            std::sort(data.begin(), data.end());
            return;
        }
    // else, insert a new entry
    if (data.size() >= max_history_entry_num)
        data.pop_back();
    data.push_front({ file, cursor });
}

HLEntry HistoryList::get_entry(const QString& filename) {
    for (int i = 0; i < data.size(); ++i)
        if (data[i].file == filename)
            return data[i];
    return {};
}

HistoryList::~HistoryList() {
    setting->setValue("history", QVariant::fromValue(data));
}
