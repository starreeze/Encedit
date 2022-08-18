#ifndef CRYPTO_H
#define CRYPTO_H
#include <QString>

QByteArray encrypt(const QString& s, uint64_t key, int skip_times = 0);

QString decrypt(const QByteArray& s, uint64_t key);

#endif
