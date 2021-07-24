#include "encript.h"
#include <random>

void encript(QString &s)
{
	srand(13570684);
	for (int i = 0; i < s.length(); ++i)
		s[i] = (QChar)(s[i].unicode() + rand());
}

void decript(QString &s)
{
	srand(13570684);
	for (int i = 0; i < s.length(); ++i)
		s[i] = (QChar)(s[i].unicode() - rand());
}