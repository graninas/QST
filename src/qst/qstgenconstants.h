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

#ifndef QSTGENCONSTANTS_H
#define QSTGENCONSTANTS_H

#include <QString>

const QString SQL_SELECT = " SELECT ";
const QString SQL_INSERT = " INSERT INTO ";
const QString SQL_UPDATE = " UPDATE ";
const QString SQL_DELETE = " DELETE FROM ";
const QString SQL_VALUES = " VALUES ";
const QString SQL_SET = " SET ";
const QString SQL_FROM	= " FROM ";
const QString SQL_WHERE	= " WHERE ";
const QString SQL_ORDER_BY = " ORDER BY ";
const QString SQL_GROUP_BY	= " GROUP BY ";
const QString SQL_HAVING	= " HAVING ";
const QString SQL_EXECUTE	= " EXEC ";

const QString STRING_TYPE_NAME = "QString";
const QString DATE_TYPE_NAME = "QDate";


const QString DATE_FORMAT = "dd.MM.yyyy";
const int		SQL_DATE_FORMAT = 104;
const QString DATE_CONVERT = QString("convert(datetime, %2, %1)").arg(SQL_DATE_FORMAT);

const QString SQL_NULL_VALUE = "NULL";

const QString AS_KEYWORD = "AS";

#endif // QSTGENCONSTANTS_H
