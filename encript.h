#ifndef ENCRIPT_H
#define ENCRIPT_H
#include <QString>

QByteArray encript(QString& s, uint64_t key);

QString decript(const QByteArray& s, uint64_t key);

#endif
