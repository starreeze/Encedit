#include "args.h"
#include "historylist.h"
#include <QList>
#include <QFont>

const QVector<QPair<QString, QVariant>> default_config {
    {"history", QVariant::fromValue(QList<HLEntry>())},
    {"font", QVariant::fromValue(QFont(default_text_fontname, default_text_fontsize))},
    {"title_regexp", QString(default_title_regexp)},
    {"font_color", default_font_color},
    {"background_color", default_background_color},
    {"window_size", QVariant::fromValue(default_window_size)},
    {"window_pos", QVariant::fromValue(default_window_pos)},
    {"splitter_state", QVariant()},
    {"max_autosave_size", QVariant::fromValue(default_max_autosave_filesize_kb)},
    {"autosave_interval", QVariant::fromValue(default_autosave_interval_s * 1000)},
    {"sidebar", true},
    {"frameless", false},
};
