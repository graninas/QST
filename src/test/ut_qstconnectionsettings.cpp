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

#include "ut_qstconnectionsettings.h"

using namespace Qst;

Q_DECLARE_METATYPE(QstConnectionSettings)

namespace QstTest
{

typedef QstConnectionSettings T;

/*!
	\class ut_QstConnectionSettings
	\brief Модульный тест для класса QstConnectionSettings.
*/

ut_QstConnectionSettings::ut_QstConnectionSettings()
{
}

void ut_QstConnectionSettings::initTestCase()
{
	m_cs1 = QstConnectionSettings();

	m_cs2.setHostName("192.168.0.1");
	m_cs2.setDatabaseName("TestDB");
	m_cs2.setUserName("defaultUser");
	m_cs2.setPassword("defaultPassword");

	m_cs3.setHostName("hostName");
	m_cs3.setDatabaseName("employee");
	m_cs3.setUserName("joe");
	m_cs3.setPassword("123");
	m_cs3.setPort(5432);
}

void ut_QstConnectionSettings::hostName()
{
	QFETCH(QstConnectionSettings, settings);
	QFETCH(QString, result);

	QCOMPARE(settings.hostName(), result);
}

void ut_QstConnectionSettings::hostName_data()
{
	QTest::addColumn<QstConnectionSettings>("settings");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< 	m_cs1	<<  QString("");
	QTest::newRow("2")	<< 	m_cs2	<<  QString("192.168.0.1");
	QTest::newRow("3")	<< 	m_cs3	<<  QString("hostName");
}

void ut_QstConnectionSettings::port()
{
	QFETCH(QstConnectionSettings, settings);
	QFETCH(int, result);

	QCOMPARE(settings.port(), result);
}

void ut_QstConnectionSettings::port_data()
{
	QTest::addColumn<QstConnectionSettings>("settings");
	QTest::addColumn<int>("result");

	QTest::newRow("1")	<< 	m_cs1	<<  -1;
	QTest::newRow("2")	<< 	m_cs2	<<  -1;
	QTest::newRow("3")	<< 	m_cs3	<<  5432;
}

void ut_QstConnectionSettings::databaseName()
{
	QFETCH(QstConnectionSettings, settings);
	QFETCH(QString, result);

	QCOMPARE(settings.databaseName(), result);
}

void ut_QstConnectionSettings::databaseName_data()
{
	QTest::addColumn<QstConnectionSettings>("settings");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< 	m_cs1	<<  "";
	QTest::newRow("2")	<< 	m_cs2	<<  "TestDB";
	QTest::newRow("3")	<< 	m_cs3	<<  "employee";
}

void ut_QstConnectionSettings::userName()
{
	QFETCH(QstConnectionSettings, settings);
	QFETCH(QString, result);

	QCOMPARE(settings.userName(), result);
}

void ut_QstConnectionSettings::userName_data()
{
	QTest::addColumn<QstConnectionSettings>("settings");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< 	m_cs1	<<  "";
	QTest::newRow("2")	<< 	m_cs2	<<  "defaultUser";
	QTest::newRow("3")	<< 	m_cs3	<<  "joe";
}

void ut_QstConnectionSettings::password()
{
	QFETCH(QstConnectionSettings, settings);
	QFETCH(QString, result);

	QCOMPARE(settings.password(), result);
}

void ut_QstConnectionSettings::password_data()
{
	QTest::addColumn<QstConnectionSettings>("settings");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< 	m_cs1	<<  "";
	QTest::newRow("2")	<< 	m_cs2	<<  "defaultPassword";
	QTest::newRow("3")	<< 	m_cs3	<<  "123";
}

void ut_QstConnectionSettings::isNull()
{
	QFETCH(QstConnectionSettings, settings);
	QFETCH(bool, result);

	QCOMPARE(settings.isNull(), result);
}

void ut_QstConnectionSettings::isNull_data()
{
	QTest::addColumn<QstConnectionSettings>("settings");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< 	m_cs1	<<  true;
	QTest::newRow("2")	<< 	m_cs2	<<  false;
	QTest::newRow("3")	<< 	m_cs3	<<  false;
}

} // End of namespace QstTest
