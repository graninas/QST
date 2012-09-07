/****************************************************************************
** QST 0.4.2a release
** Copyright (C) 2010 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Test module of the QsT SQL Tools.
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

#include "ut_qstabstractmodelhandler.h"

using namespace Qst;

Q_DECLARE_METATYPE(QstTest::TestHandler)
Q_DECLARE_METATYPE(QstValue)

namespace QstTest
{

	/*!
		\class ut_QstAbstractModelHandler
		\brief Модульный тест для класса QstAbstractModelHandler.
	*/

ut_QstAbstractModelHandler::ut_QstAbstractModelHandler()
{
}

void ut_QstAbstractModelHandler::initTestCase()
{
	m_h1.setValue(ID_VALUE, QVariant());
	m_h2.setValue(ID_VALUE, QVariant(15));

	m_h3.setValue("DateBegin", QVariant(QDate(2010, 10, 10)));
	m_h3.setValue("DateEnd", QVariant(QDate(2010, 10, 16)));
}

void ut_QstAbstractModelHandler::cleanupTestCase()
{

}

void ut_QstAbstractModelHandler::value()
{
	QFETCH(TestHandler, handler);
	QFETCH(QstValue, result);

	QCOMPARE(handler.value(ID_VALUE), result);
}

void ut_QstAbstractModelHandler::value_data()
{
	QTest::addColumn<TestHandler>("handler");
	QTest::addColumn<QstValue>("result");

	QTest::newRow("1")	<< 	m_h1	<<  QstValue();
	QTest::newRow("2")	<< 	m_h2	<<	QstValue(15);
	QTest::newRow("3")	<< 	m_h3	<<  QstValue();
	QTest::newRow("4")	<< 	m_h4	<<  QstValue();
}

void ut_QstAbstractModelHandler::generateQuery1()
{
	QFETCH(TestHandler, handler);
	QFETCH(QString, result);

	QCOMPARE(handler.generateQuery(QuerySelect, TEST1), result);
}

void ut_QstAbstractModelHandler::generateQuery1_data()
{
	QTest::addColumn<TestHandler>("handler");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< 	m_h1	<<  QString("SELECT Name, ID FROM table1");
	QTest::newRow("2")	<< 	m_h2	<<	QString("SELECT Name, ID FROM table1 WHERE ID = 15");
	QTest::newRow("3")	<< 	m_h3	<<  QString("SELECT Name, ID FROM table1 WHERE OrderDate BETWEEN convert(datetime, '10.10.2010', 104) AND convert(datetime, '16.10.2010', 104)");
	QTest::newRow("4")	<< 	m_h4	<<  QString("SELECT Name, ID FROM table1");
}


} // End of namespace QstTest
