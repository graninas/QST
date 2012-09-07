/****************************************************************************
** QST 0.4.1 pre-alpha
** Copyright (C) 2010 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Qst module of the QsT SQL Tools.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 3 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 3 requirements
** will be met: http://www.gnu.org/licenses/lgpl.html.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL3 included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/licenses/gpl.html.
**
** If you have questions regarding the use of this file, please contact
** author (graninas@gmail.com).
**
****************************************************************************/

#include "qstdefaultitemnameextractor.h"

#include "qst/qstgenconstants.h"
#include <QRegExp>
#include <QStringList>

namespace Qst
{


QstDefaultItemNameExtractor::QstDefaultItemNameExtractor()
	:
	_str(QString())
{
}

QstDefaultItemNameExtractor::QstDefaultItemNameExtractor(const QString &str)
	:
	_str(str)
{
}

QString QstDefaultItemNameExtractor::extractItemName() const
{
	return extractItemName(_str);
}

QString QstDefaultItemNameExtractor::extractItemName(const QString &str) const
{
	if (str.isEmpty())
		return QString();

	QStringList splitted = str.split(" ", QString::SkipEmptyParts);
	QString		resString = splitted[splitted.count()-1];

	int pos = resString.indexOf( QString(" " + AS_KEYWORD + " "), 0, Qt::CaseInsensitive );

	if (pos != -1)
	{
		return _getFromBrackets(resString.mid(AS_KEYWORD.length() + 2 + pos));
	}
	else
	{
		return _getFromBrackets(resString);
	}

return resString;
}


QString QstDefaultItemNameExtractor::_getFromBrackets(const QString &str) const
{
	int startPos = str.indexOf("(");
	int endPos = str.indexOf(")");

	if (startPos != -1 && endPos != -1 && (endPos - startPos)>1)
	{
		return _getFromBrackets(str.mid(startPos + 1, endPos - startPos - 1));
	}

	startPos = str.indexOf("[");
	endPos = str.indexOf("]");

	if (startPos != -1 && endPos != -1 && (endPos - startPos)>1)
	{
		return _getFromBrackets(str.mid(startPos + 1, endPos - startPos - 1));
	}

	return str;
}

}
