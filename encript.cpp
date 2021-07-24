#include "encript.h"

int key = 13570684;

void encript(QString &s)
{
	for (int i = 0; i < s.length(); ++i)
		s[i] = (QChar)(s[i].unicode() + (i + key));
}

void decript(QString &s)
{
	for (int i = 0; i < s.length(); ++i)
		s[i] = (QChar)(s[i].unicode() - (i + key));
}