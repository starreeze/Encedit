#include "indexmodel.h"
#include <QRegularExpression>

void IndexModel::load(const QString& content, const QString& regexp) {
    indexList.clear(); indexPos.clear();
    QRegularExpression re(regexp);
	auto it = re.globalMatch(content);
	while (it.hasNext()) {
		QRegularExpressionMatch match = it.next();
		QString str = match.captured();
		int i = str.indexOf(' ');
		str.insert(i, QString(" ").repeated(12 - i));
		for (i = str.size() - 1; str[i] != ' '; --i);
		str.insert(i, "   ");
		indexList << str;
		indexPos << match.capturedStart();
	}
}
