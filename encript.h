#ifndef ENCRIPT_H
#define ENCRIPT_H
#include <QString>

QByteArray encript(const QString &s, int key);

QString decript(const QByteArray &s, int key);

#endif
