/****************************************************************************
** QST 0.3c beta
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

#include "ut_sqlgen.h"

Q_DECLARE_METATYPE(SqlGen)
Q_DECLARE_METATYPE(SqlBatch)
Q_DECLARE_METATYPE(SqlQueryTypes)

typedef SqlGen T;

/*!
	\class ut_SqlGen
	\brief Модульный тест для класса SqlGen. См. ut_sqlgen.cpp .
*/

ut_SqlGen::ut_SqlGen()
{
}

void ut_SqlGen::initTestCase()
{
	m_b1 = SqlBatch();

	m_b2.addSource("Source1");
	m_b2 << "Source2";

	m_b2.addField(SqlField());


	m_b3.addSource("xSource");
	m_b3	<< SqlField("Field4", fv_visible, fr_id, "EngTitle", 100);

	m_b4.addSource("Tetha");
	m_b4	<< SqlField("Field1", fv_invisible, fr_none, "РусЗаголовок", 20)
			<< SqlField("Field4", fv_invisible, fr_none, "РусЗаголовок2", 20);


	m_b5.addSource("vSomeSource");
	m_b5	<< SqlField("ID", fv_visible, fr_id)
			<< SqlField("Parent_ID", fv_visible, fr_parent)
			<< SqlField("Field1", fv_visible, fr_none, "Поле1", 100)
			<< SqlField("Field2", fv_visible, fr_none, "Поле2", 50)
			<< SqlField("Field3", fv_visible, fr_none, "Поле3", 100)

			<< SqlField("Field1", SqlValue(), fp_where)
			<< SqlField("Field2", SqlValue(nt_null), fp_where | fp_insert)
			<< SqlField("Field3", SqlValue(10, fo_less), fp_where | fp_select | fp_update)
			<< SqlField("Field4", SqlValue(QString("some string")), fp_where | fp_insert)
			<< SqlField("Field5", SqlValue(QString("other string"), fo_equal, fb_right), fp_where)
			<< SqlField(SqlValue(10), fp_parameter | fp_insert)	//fp_insert must be ignored.
			<< SqlField(SqlValue(QString("StringParameter")))
			<< SqlField(SqlValue(nt_null))
			<< SqlField(SqlValue())
			<< SqlField(SqlValue(QDate(2010, 4, 2)))
			;

	m_b6.addSource("vView");
	m_b6	<< SqlField("ID", fv_invisible, fr_id)
			<< SqlField("Parent_ID", fv_invisible, fr_parent)
			<< SqlField("Field1", fv_visible, fr_none, "ПолеX", 100)
			<< SqlField("Field2", fv_invisible, fr_none, "ПолеY", 50)
			<< SqlField("Field3", fv_visible, fr_none, "ПолеZ", 100)

			<< SqlField("Field1", SqlValue(QDate(1986, 7, 30)), fp_insert | fp_update | fp_parameter)
			<< SqlField("Field2", SqlValue(nt_null), fp_update)
			<< SqlField("Field3", SqlValue(10, fo_greater_equal_or_null), fp_insert | fp_where | fp_select | fp_update)
			<< SqlField("Field4", SqlValue(QDate(2010, 03, 20)), SqlValue(QDate(2010, 04, 1)))
			<< SqlField("Field5", SqlValue(3.14, fo_greater_equal_or_null), fp_where | fp_insert)
			<< SqlField("Field6", SqlValue(true), fp_where)
			;
}


void ut_SqlGen::query()
{
	QFETCH(SqlGen, gen);
	QFETCH(QString, result);

	QCOMPARE(gen.query(), result);
}

void ut_SqlGen::query_data()
{
	QTest::addColumn<SqlGen>("gen");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< 	T(m_b1, sql_select)		<<  QString();
	QTest::newRow("2")	<< 	T(m_b1, sql_insert)		<<  QString();
	QTest::newRow("3")	<< 	T(m_b1, sql_execute)	<<  QString();
	QTest::newRow("4")	<< 	T(m_b1, sql_update)		<<  QString();
	QTest::newRow("5")	<< 	T(m_b1, sql_delete)		<<  QString();

	QTest::newRow("7")	<< 	T(m_b2, sql_select)		<<  QString();
	QTest::newRow("8")	<< 	T(m_b2, sql_insert)		<<  QString();
	QTest::newRow("9")	<< 	T(m_b2, sql_execute)	<<  QString();
	QTest::newRow("10")	<< 	T(m_b2, sql_update)		<<  QString();
	QTest::newRow("11")	<< 	T(m_b2, sql_delete)		<<  QString();

	QTest::newRow("12")	<< 	T(m_b3, sql_select)		<<  QString("SELECT Field4 FROM xSource");
	QTest::newRow("13")	<< 	T(m_b3, sql_insert)		<<  QString();
	QTest::newRow("14")	<< 	T(m_b3, sql_execute)	<<  QString("EXEC xSource");
	QTest::newRow("15")	<< 	T(m_b3, sql_update)		<<  QString();
	QTest::newRow("16")	<< 	T(m_b3, sql_delete)		<<  QString("DELETE FROM xSource");

	QTest::newRow("17")	<< 	T(m_b4, sql_select)		<<  QString("SELECT Field1, Field4 FROM Tetha");
	QTest::newRow("18")	<< 	T(m_b4, sql_insert)		<<  QString();
	QTest::newRow("19")	<< 	T(m_b4, sql_execute)	<<  QString("EXEC Tetha");
	QTest::newRow("20")	<< 	T(m_b4, sql_update)		<<  QString();
	QTest::newRow("21")	<< 	T(m_b4, sql_delete)		<<  QString("DELETE FROM Tetha");

	QTest::newRow("22")	<< 	T(m_b5, sql_select)		<<  QString("SELECT ID, Parent_ID, Field1, Field2, Field3, Field3 FROM vSomeSource WHERE Field2 IS NULL AND Field3 < 10 AND Field4 LIKE '%some string%' AND Field5 = 'other string%'");
	QTest::newRow("23")	<< 	T(m_b5, sql_insert)		<<  QString("INSERT INTO vSomeSource (Field2, Field4) VALUES (NULL, 'some string')");
	QTest::newRow("24")	<< 	T(m_b5, sql_execute)	<<  QString("EXEC vSomeSource 10, 'StringParameter', NULL, NULL, '02.04.2010'");
	QTest::newRow("25")	<< 	T(m_b5, sql_update)		<<  QString("UPDATE vSomeSource SET Field3 = 10 WHERE Field2 IS NULL AND Field3 < 10 AND Field4 LIKE '%some string%' AND Field5 = 'other string%'");
	QTest::newRow("26")	<< 	T(m_b5, sql_delete)		<<  QString("DELETE FROM vSomeSource WHERE Field2 IS NULL AND Field3 < 10 AND Field4 LIKE '%some string%' AND Field5 = 'other string%'");

	QTest::newRow("27")	<< 	T(m_b6, sql_select)		<<  QString("SELECT Field1, Field3, ID, Parent_ID, Field2, Field3 FROM vView WHERE Field3 >= 10 OR Field3 IS NULL AND Field4 BETWEEN convert(datetime, '20.03.2010', 104) AND convert(datetime, '01.04.2010', 104) AND Field5 >= 3.14 OR Field5 IS NULL AND Field6 = true");
	QTest::newRow("28")	<< 	T(m_b6, sql_insert)		<<  QString("INSERT INTO vView (Field1, Field3, Field5) VALUES (convert(datetime, '30.07.1986', 104), 10, 3.14)");
	QTest::newRow("29")	<< 	T(m_b6, sql_execute)	<<  QString("EXEC vView '30.07.1986'");
	QTest::newRow("30")	<< 	T(m_b6, sql_update)		<<  QString("UPDATE vView SET Field1 = convert(datetime, '30.07.1986', 104), Field2 = NULL, Field3 = 10 WHERE Field3 >= 10 OR Field3 IS NULL AND Field4 BETWEEN convert(datetime, '20.03.2010', 104) AND convert(datetime, '01.04.2010', 104) AND Field5 >= 3.14 OR Field5 IS NULL AND Field6 = true");
	QTest::newRow("31")	<< 	T(m_b6, sql_delete)		<<  QString("DELETE FROM vView WHERE Field3 >= 10 OR Field3 IS NULL AND Field4 BETWEEN convert(datetime, '20.03.2010', 104) AND convert(datetime, '01.04.2010', 104) AND Field5 >= 3.14 OR Field5 IS NULL AND Field6 = true");
}

void ut_SqlGen::batch()
{
	QFETCH(SqlGen, gen);
	QFETCH(SqlBatch, result);

	QCOMPARE(gen.batch(), result);
}

void ut_SqlGen::batch_data()
{
	QTest::addColumn<SqlGen>("gen");
	QTest::addColumn<SqlBatch>("result");

	QTest::newRow("1")	<< 	T(m_b1, sql_select)		<<  SqlBatch();
	QTest::newRow("2")	<< 	T(m_b2, sql_select)		<<  (SqlBatch() << "Source1" << "Source2" << SqlField());
	QTest::newRow("3")	<< 	T(m_b3, sql_select)		<<  (SqlBatch() << "xSource" << SqlField("Field4", fv_visible, fr_id, "EngTitle", 100));

	QTest::newRow("4")	<< 	T(m_b4, sql_select)
			<<  (SqlBatch() << "Tetha"
				<< SqlField("Field1", fv_invisible, fr_none, "РусЗаголовок", 20)
				<< SqlField("Field4", fv_invisible, fr_none, "РусЗаголовок2", 20)
				);

	QTest::newRow("5")	<< 	T(m_b5, sql_select)
			<<  (SqlBatch()
				 <<  "vSomeSource"
				<< SqlField("ID", fv_visible, fr_id)
				<< SqlField("Parent_ID", fv_visible, fr_parent)
				<< SqlField("Field1", fv_visible, fr_none, "Поле1", 100)
				<< SqlField("Field2", fv_visible, fr_none, "Поле2", 50)
				<< SqlField("Field3", fv_visible, fr_none, "Поле3", 100)
				<< SqlField("Field1", SqlValue(), fp_where)
				<< SqlField("Field2", SqlValue(nt_null), fp_where | fp_insert)
				<< SqlField("Field3", SqlValue(10, fo_less), fp_where | fp_select | fp_update)
				<< SqlField("Field4", SqlValue(QString("some string")), fp_where | fp_insert)
				<< SqlField("Field5", SqlValue(QString("other string"), fo_equal, fb_right), fp_where)
				<< SqlField(SqlValue(10), fp_parameter | fp_insert)
				<< SqlField(SqlValue(QString("StringParameter")))
				<< SqlField(SqlValue(nt_null))
				<< SqlField(SqlValue())
				<< SqlField(SqlValue(QDate(2010, 4, 2)))
				);

	QTest::newRow("6")	<< 	T(m_b6, sql_select)
			<<  (SqlBatch()
				<<  "vView"
				<< SqlField("ID", fv_invisible, fr_id)
				<< SqlField("Parent_ID", fv_invisible, fr_parent)
				<< SqlField("Field1", fv_visible, fr_none, "ПолеX", 100)
				<< SqlField("Field2", fv_invisible, fr_none, "ПолеY", 50)
				<< SqlField("Field3", fv_visible, fr_none, "ПолеZ", 100)
				<< SqlField("Field1", SqlValue(QDate(1986, 7, 30)), fp_insert | fp_update | fp_parameter)
				<< SqlField("Field2", SqlValue(nt_null), fp_update)
				<< SqlField("Field3", SqlValue(10, fo_greater_equal_or_null), fp_insert | fp_where | fp_select | fp_update)
				<< SqlField("Field4", SqlValue(QDate(2010, 03, 20)), SqlValue(QDate(2010, 04, 1)))
				<< SqlField("Field5", SqlValue(3.14, fo_greater_equal_or_null), fp_where | fp_insert)
				<< SqlField("Field6", SqlValue(true), fp_where)
				);
}

void ut_SqlGen::queryType()
{
	QFETCH(SqlGen, gen);
	QFETCH(SqlQueryTypes, result);

	QCOMPARE(gen.queryType(), result);
}

void ut_SqlGen::queryType_data()
{
	QTest::addColumn<SqlGen>("gen");
	QTest::addColumn<SqlQueryTypes>("result");

	QTest::newRow("1")	<< 	T(m_b1, sql_select)		<<  sql_select;
	QTest::newRow("2")	<< 	T(m_b1, sql_insert)		<<  sql_insert;
	QTest::newRow("3")	<< 	T(m_b1, sql_execute)	<<  sql_execute;
	QTest::newRow("4")	<< 	T(m_b1, sql_update)		<<  sql_update;
	QTest::newRow("5")	<< 	T(m_b1, sql_delete)		<<  sql_delete;
}

void ut_SqlGen::isValid()
{
	QFETCH(SqlGen, gen);
	QFETCH(bool, result);

	QCOMPARE(gen.isValid(), result);
}

void ut_SqlGen::isValid_data()
{
	QTest::addColumn<SqlGen>("gen");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< 	T(m_b1, sql_select)		<<  false;
	QTest::newRow("2")	<< 	T(m_b2, sql_select)		<<  false;
	QTest::newRow("3")	<< 	T(m_b3, sql_select)		<<  true;
	QTest::newRow("4")	<< 	T(m_b4, sql_select)		<<  true;
	QTest::newRow("5")	<< 	T(m_b5, sql_select)		<<  true;

}


