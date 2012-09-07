/****************************************************************************
** QST 0.4.2a rc
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

#include "ut_qstdefaultitemnameextractor.h"

using namespace Qst;

typedef QstDefaultItemNameExtractor T;

Q_DECLARE_METATYPE(QstDefaultItemNameExtractor)

namespace QstTest
{

	/*!
		\class ut_QstDefaultItemNameExtractor
		\brief Модульный тест для класса QstDefaultItemNameExtractor.
	*/

ut_QstDefaultItemNameExtractor::ut_QstDefaultItemNameExtractor()
{
}


void ut_QstDefaultItemNameExtractor::extractItemName()
{
	QFETCH(QstDefaultItemNameExtractor, value);
	QFETCH(QString, result);

	QCOMPARE(value.extractItemName(), result);
}

void ut_QstDefaultItemNameExtractor::extractItemName_data()
{
	QTest::addColumn<QstDefaultItemNameExtractor>("value");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< T()						<< QString();
	QTest::newRow("2")	<< T("ID")					<< QString("ID");
	QTest::newRow("3")	<< T("[ID]")				<< QString("ID");
	QTest::newRow("4")	<< T("([ID])")				<< QString("ID");
	QTest::newRow("5")	<< T("[(ID)]")				<< QString("ID");
	QTest::newRow("6")	<< T("count(ID)")			<< QString("ID");
	QTest::newRow("7")	<< T("key as ID")			<< QString("ID");
	QTest::newRow("8")	<< T("key AS ID")			<< QString("ID");
	QTest::newRow("9")	<< T("count(key) AS ID")	<< QString("ID");
	QTest::newRow("10")	<< T("count([key]) AS ID")	<< QString("ID");
	QTest::newRow("11")	<< T("max(ID)")				<< QString("ID");
	QTest::newRow("12")	<< T("min(ID)")				<< QString("ID");
	QTest::newRow("13")	<< T("sum(ID)")				<< QString("ID");
	QTest::newRow("14")	<< T("FieldName alias")		<< QString("alias");
	QTest::newRow("15")	<< T("count(key) alias")	<< QString("alias");
	QTest::newRow("16")	<< T("FieldName [alias]")	<< QString("alias");
	QTest::newRow("17")	<< T("count(key)  [alias]")	<< QString("alias");
}

} // End of namespace QstTest
