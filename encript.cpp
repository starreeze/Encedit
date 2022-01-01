#include "encript.h"

uint64_t next;
uint64_t myrand() {
	return next = next * 1103515245 + 12345;
}

QByteArray encript(const QString& str, uint64_t key) {
	QString s(str);
	next = key;
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

QString decript(const QByteArray& s, uint64_t key) {
	next = key;
	QString res(s.length() / 2, 0);
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