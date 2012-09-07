/****************************************************************************
** QST 0.3c beta
** Copyright (C) 2010 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the QsT SQL Tools.
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

#include <QTest>
#include "test/ut_sqlvalue.h"
#include "test/ut_sqlfield.h"
#include "test/ut_sqlbatch.h"
#include "test/ut_sqlgen.h"

#include "qst/cyrillic.h"

#include "qst/sqlvalue.h"

int main(int argc, char *argv[])
{
	Cyrillic cyr("UTF-8");	//Заодно тестируется и этот класс :)

	QCoreApplication app(argc, argv);
	int res;

	ut_SqlValue tc_SqlValue;
		res = QTest::qExec(&tc_SqlValue, argc, argv);
		Q_ASSERT(res == 0);

	ut_SqlField tc_SqlField;
		res = QTest::qExec(&tc_SqlField, argc, argv);
		Q_ASSERT(res == 0);

	ut_SqlBatch tc_SqlBatch;
		res = QTest::qExec(&tc_SqlBatch, argc, argv);
		Q_ASSERT(res == 0);

	ut_SqlGen tc_SqlGen;
		res = QTest::qExec(&tc_SqlGen, argc, argv);
		Q_ASSERT(res == 0);


	return res;
}


