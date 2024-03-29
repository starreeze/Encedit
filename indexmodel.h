#ifndef INDEXMODEL_H
#define INDEXMODEL_H
#include <QString>
#include <QStringList>
#include <QVector>
#include <QModelIndex>
#include "args.h"

class IndexModel {
    QStringList indexList;
    QVector<int> indexPos;
public:
    inline const QStringList& string_list() { return indexList; }
    inline int operator[](const QModelIndex& idx) { return indexPos[idx.row()]; }
    void load(const QString& content, const QString& regexp = default_title_regexp);
};

#endif // INDEXMODEL_H
