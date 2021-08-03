#include "encript.h"

uint next;
uint myrand() {
	return next = next * 1103515245 + 12345;
}

QByteArray encript(QString& s, int key) {
	next = key;
	if (s.length() % 2)
		s.append('\n');
	QByteArray res(s.length() * 2, 0);
	for (int i = 0; i < s.length() / 2; ++i) {
		uint word = s[2 * i].unicode();
		word |= (uint)s[2 * i + 1].unicode() << 16;
		word ^= myrand();
		res[4 * i] = word;
		res[4 * i + 1] = word >> 8;
		res[4 * i + 2] = word >> 16;
		res[4 * i + 3] = word >> 24;
	}
	return res;
}

QString decript(const QByteArray& s, int key) {
	next = key;
	QString res(s.length() / 2, 0);
	for (int i = 0; i < res.length() / 2; ++i) {
		uint word = ((s[4 * i] & 0xff) | (s[4 * i + 1] & 0xff) << 8 | (s[4 * i + 2] & 0xff) << 16 | (s[4 * i + 3] & 0xff) << 24) ^ myrand();
		res[2 * i] = word;
		res[2 * i + 1] = word >> 16;
	}
	return res;
}