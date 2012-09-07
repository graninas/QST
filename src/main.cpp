/****************************************************************************
** QST 0.4.2a rc
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
** Alternatively, thi file may be used under the terms of the GNU
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
#include "test/ut_qstvalue.h"
#include "test/ut_qstfield.h"
#include "test/ut_qstbatch.h"
#include "test/ut_qstquerygenerator.h"
#include "test/ut_qstdefaultitemnameextractor.h"
#include "test/ut_qstabstractmodelhandler.h"

#include <QCoreApplication>

using namespace QstTest;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);

	int res;

	ut_QstValue tc_QstValue;
		res = QTest::qExec(&tc_QstValue, argc, argv);
		Q_ASSERT(res == 0);

	ut_QstField tc_QstField;
		res = QTest::qExec(&tc_QstField, argc, argv);
		Q_ASSERT(res == 0);

	ut_QstBatch tc_Batch;
		res = QTest::qExec(&tc_Batch, argc, argv);
		Q_ASSERT(res == 0);

	ut_QstQueryGenerator tc_QueryGenerator;
		res = QTest::qExec(&tc_QueryGenerator, argc, argv);
		Q_ASSERT(res == 0);

	ut_QstDefaultItemNameExtractor tc_DefaultItemNameExtractor;
		res = QTest::qExec(&tc_DefaultItemNameExtractor, argc, argv);
		Q_ASSERT(res == 0);

	ut_QstAbstractModelHandler tc_AbstractModelHandler;
	res = QTest::qExec(&tc_AbstractModelHandler, argc, argv);
		Q_ASSERT(res == 0);

	return res;
}


