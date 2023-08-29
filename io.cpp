#include "io.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardPaths>
#include <QTimer>

#if ENCODING == utf-8
constexpr auto encode_str = [](const QString& s) { return s.toUtf8();};
constexpr auto decode_str = [](const QByteArray& s) { return QString::fromUtf8(s);};
#endif

inline QByteArray Cipher::encrypt(const QByteArray& s) {
    return key.isEmpty() ? s : aes_cipher.encode(s, key);
}

inline QByteArray Cipher::decrypt(const QByteArray& s) {
    return key.isEmpty() ? s : aes_cipher.removePadding(aes_cipher.decode(s, key));
}

void Cipher::update_key(const QString& crypt_key) {
    constexpr int aes_size = (aes_type * 64 + 128) / 8;
    key = encode_str(crypt_key);
    auto append_size = aes_size - key.size();
    key.append(append_size < 0 ? 0 : append_size, 0).resize(aes_size);
}

QString FileIo::read() {
    QFile file(file_path);
    file.open(QFile::ReadOnly);
    QByteArray content = file.readAll();
    file.close();
    buffer = cipher.decrypt(content);
    return decode_str(buffer);
}

bool FileIo::write(const QString& text, bool rewrite_all) {
    QFile file(file_path);
    QByteArray encoded = encode_str(text);
    if (rewrite_all) {
        file.open(QFile::WriteOnly);
        file.write(cipher.encrypt(encoded));
    }
    else {
        int i = 0, original_size = buffer.size(), new_size = encoded.size();
        for (; i < qMin(new_size, original_size) && buffer[i] == encoded[i]; ++i);
        buffer.resize(i);
        buffer.append(encoded.mid(i));
        // special optimization for autosave performance, as most autosaves save nothing
        if (i == new_size && original_size == new_size)
            return false;
        i -= i % crypto_blocksize;
        auto modified = cipher.encrypt(encoded.mid(i));
        file.open(QFile::WriteOnly | QFile::Append);
        file.resize(i);
        file.seek(i);
        file.write(modified);
    }
    file.close();
    return true;
}

void FileIo::update_key(const QString& crypt_key) {
    cipher.update_key(crypt_key);
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
    const MainWindow* mw = static_cast<const MainWindow*>(parent());
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
