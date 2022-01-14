#include "indexmodel.h"
#include <QRegularExpression>

void IndexModel::load(const QString& content) {
    indexList.clear(); indexPos.clear();
	// 2021.7.24 Sat <weather>
	QRegularExpression re("\\d{4,4}(\\.\\d{1,2}){2,2} \\w{3,3} .");
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
