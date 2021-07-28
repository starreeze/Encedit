#include "encript.h"

int next;
int rand()
{
	return next = next * 1103515245 + 12345;
}

QByteArray encript(const QString &s, int key)
{
	next = key;
	QByteArray res(s.length() * 2, 0);
	for (int i = 0; i < s.length(); ++i)
	{
		ushort word = s[i].unicode();
		word ^= rand();
		res[2 * i] = word;
		res[2 * i + 1] = word >> 8;
		// printf("%x -> %x, %x\n", word, char(res[2 * i]), char(res[2 * i + 1]));
	}
	return res;
}

QString decript(const QByteArray &s, int key)
{
	next = key;
	QString res(s.length() / 2, 0);
	for (int i = 0; i < res.length(); ++i)
	{
		res[i] = ((s[2 * i] & 0xff) | (s[2 * i + 1] & 0xff) << 8) ^ rand();
		// printf("%x, %x -> %x\n", char(s[2 * i]), char(s[2 * i + 1]), res[i].unicode());
	}
	return res;
}