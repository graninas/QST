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

#include "ut_sqlbatch.h"

typedef SqlBatch T;

Q_DECLARE_METATYPE(SqlBatch)
Q_DECLARE_METATYPE(SqlValue)
Q_DECLARE_METATYPE(SqlField)
Q_DECLARE_METATYPE(SqlFieldsVector)

/*!
	\class ut_SqlBatch
	\brief Модульный тест для класса SqlBatch. См. ut_sqlbatch.cpp .
*/

ut_SqlBatch::ut_SqlBatch()
{
}

void ut_SqlBatch::initTestCase()
{
	m_b1 = SqlBatch();

	m_b2.addSource("Source1");
	m_b2 << "Source2";

	m_b2.addField(SqlField());


	m_b3.addSource("xSource");
	m_b3	<< SqlField("Field4", fv_visible, fr_id, "EngTitle", 100)	;

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
			<< SqlField("Field3", SqlValue(10, fo_less), fp_where | fp_select)
			<< SqlField("Field4", SqlValue(QString("some string")), fp_where)
			<< SqlField("Field5", SqlValue(QString("other string"), fo_equal, fb_right), fp_where)
			;

	m_b6.addSource("vView");
	m_b6	<< SqlField("ID", fv_invisible, fr_id)
			<< SqlField("Parent_ID", fv_invisible, fr_parent)
			<< SqlField("Field1", fv_visible, fr_none, "ПолеX", 100)
			<< SqlField("Field2", fv_invisible, fr_none, "ПолеY", 50)
			<< SqlField("Field3", fv_visible, fr_none, "ПолеZ", 100)

			<< SqlField("Field4", SqlValue(QDate(2010, 03, 20)),
						SqlValue(QDate(2010, 04, 1)))
			<< SqlField("Field5", SqlValue(3.14, fo_greater_equal_or_null), fp_where | fp_insert)
			<< SqlField("Field6", SqlValue(true), fp_where)
			;



//	qDebug() << "Enumerating selectClauseFields of m_b5.";
//	for (int i = 0; i < m_b5.selectClauseFields().size(); ++i)
//	{
//		qDebug() << (m_b5.selectClauseFields())[i].name();
//	}
//
//	qDebug() << "Enumerating stuffFields of m_b5.";
//	for (int i = 0; i < m_b5.stuffFields().size(); ++i)
//	{
//		qDebug() << (m_b5.stuffFields())[i].name();
//	}
//
//		qDebug() << "Enumerating fields of m_b5.";
//		for (int i = 0; i < m_b5.fields().size(); ++i)
//		{
//			qDebug() << (m_b5.fields())[i].name();
//		}
//
//
//	qDebug() << "Enumerating selectClauseFields of m_b6.";
//	for (int i = 0; i < m_b6.selectClauseFields().size(); ++i)
//	{
//		qDebug() << (m_b6.selectClauseFields())[i].name();
//	}
//
//	qDebug() << "Enumerating stuffFields of m_b6.";
//	for (int i = 0; i < m_b6.stuffFields().size(); ++i)
//	{
//		qDebug() << (m_b6.stuffFields())[i].name();
//	}

}

void ut_SqlBatch::sources()
{
	QFETCH(SqlBatch, batch);
	QFETCH(QStringList, result);

	QCOMPARE(batch.sources(), result);
}

void ut_SqlBatch::sources_data()
{
	QTest::addColumn<SqlBatch>("batch");
	QTest::addColumn<QStringList>("result");

	QTest::newRow("1")	<< 	m_b1	<<  QStringList();
	QTest::newRow("2")	<< 	m_b2	<< (QStringList() << "Source1" << "Source2");
	QTest::newRow("3")	<< 	m_b3	<< (QStringList() << "xSource");
	QTest::newRow("4")	<< 	m_b4	<< (QStringList() << "Tetha");
	QTest::newRow("5")	<< 	m_b5	<< (QStringList() << "vSomeSource");
	QTest::newRow("6")	<< 	m_b6	<< (QStringList() << "vView");
}

void ut_SqlBatch::selectClauseFields()
{
	QFETCH(SqlBatch, batch);
	QFETCH(SqlFieldsVector, result);

	QCOMPARE(batch.selectClauseFields(), result);
}

void ut_SqlBatch::selectClauseFields_data()
{
	QTest::addColumn<SqlBatch>("batch");
	QTest::addColumn<SqlFieldsVector>("result");

	QTest::newRow("1")	<< 	m_b1	<< SqlFieldsVector();
	QTest::newRow("2")	<< 	m_b2	<< (SqlFieldsVector());
	QTest::newRow("3")	<< 	m_b3	<< (SqlFieldsVector()
										<< SqlField("Field4", fv_visible, fr_id, "EngTitle", 100)
										);

	QTest::newRow("4")	<< 	m_b4	<< (SqlFieldsVector()
										<< SqlField("Field1", fv_invisible, fr_none, "РусЗаголовок", 20)
										<< SqlField("Field4", fv_invisible, fr_none, "РусЗаголовок2", 20)
										);

	QTest::newRow("5")	<< 	m_b5	<< (SqlFieldsVector()
										<< SqlField("ID", fv_visible, fr_id)
										<< SqlField("Parent_ID", fv_visible, fr_parent)
										<< SqlField("Field1", fv_visible, fr_none, "Поле1", 100)
										<< SqlField("Field2", fv_visible, fr_none, "Поле2", 50)
										<< SqlField("Field3", fv_visible, fr_none, "Поле3", 100)
										);

	QTest::newRow("6")	<< 	m_b6	<< (SqlFieldsVector()
										<< SqlField("Field1", fv_visible, fr_none, "ПолеX", 100)
										<< SqlField("Field3", fv_visible, fr_none, "ПолеZ", 100)
										<< SqlField("ID", fv_invisible, fr_id)
										<< SqlField("Parent_ID", fv_invisible, fr_parent)
										<< SqlField("Field2", fv_invisible, fr_none, "ПолеY", 50)
										);
}

void ut_SqlBatch::stuffFields()
{
	QFETCH(SqlBatch, batch);
	QFETCH(SqlFieldsVector, result);

	QCOMPARE(batch.stuffFields(), result);
}

void ut_SqlBatch::stuffFields_data()
{
	QTest::addColumn<SqlBatch>("batch");
	QTest::addColumn<SqlFieldsVector>("result");

	QTest::newRow("1")	<< 	m_b1	<< SqlFieldsVector();
	QTest::newRow("2")	<< 	m_b2	<< (SqlFieldsVector());
	QTest::newRow("3")	<< 	m_b3	<< (SqlFieldsVector());

	QTest::newRow("4")	<< 	m_b4	<< (SqlFieldsVector());

	QTest::newRow("5")	<< 	m_b5	<< (SqlFieldsVector()
										<< SqlField("Field1", SqlValue(), fp_where)
										<< SqlField("Field2", SqlValue(nt_null), fp_where | fp_insert)
										<< SqlField("Field3", SqlValue(10, fo_less), fp_where | fp_select)
										<< SqlField("Field4", SqlValue(QString("some string")), fp_where)
										<< SqlField("Field5", SqlValue(QString("other string"), fo_equal, fb_right), fp_where)
										);

	QTest::newRow("6")	<< 	m_b6	<< (SqlFieldsVector()
										<< SqlField("Field4", SqlValue(QDate(2010, 03, 20)),
													SqlValue(QDate(2010, 04, 1)))
										<< SqlField("Field5", SqlValue(3.14, fo_greater_equal_or_null), fp_where | fp_insert)
										<< SqlField("Field6", SqlValue(true), fp_where)
										);
}

void ut_SqlBatch::fields()
{
	QFETCH(SqlBatch, batch);
	QFETCH(SqlFieldsVector, result);

	QCOMPARE(batch.fields(), result);
}

void ut_SqlBatch::fields_data()
{
	QTest::addColumn<SqlBatch>("batch");
	QTest::addColumn<SqlFieldsVector>("result");

	QTest::newRow("1")	<< 	m_b1	<< SqlFieldsVector();
	QTest::newRow("2")	<< 	m_b2	<< (SqlFieldsVector()
										<< SqlField());

	QTest::newRow("3")	<< 	m_b3	<< (SqlFieldsVector()
										<< SqlField("Field4", fv_visible, fr_id, "EngTitle", 100));

	QTest::newRow("4")	<< 	m_b4	<< (SqlFieldsVector()
										<< SqlField("Field1", fv_invisible, fr_none, "РусЗаголовок", 20)
										<< SqlField("Field4", fv_invisible, fr_none, "РусЗаголовок2", 20));

	QTest::newRow("5")	<< 	m_b5	<< (SqlFieldsVector()
										<< SqlField("ID", fv_visible, fr_id)
										<< SqlField("Parent_ID", fv_visible, fr_parent)
										<< SqlField("Field1", fv_visible, fr_none, "Поле1", 100)
										<< SqlField("Field2", fv_visible, fr_none, "Поле2", 50)
										<< SqlField("Field3", fv_visible, fr_none, "Поле3", 100)

										<< SqlField("Field1", SqlValue(), fp_where)
										<< SqlField("Field2", SqlValue(nt_null), fp_where | fp_insert)
										<< SqlField("Field3", SqlValue(10, fo_less), fp_where | fp_select)
										<< SqlField("Field4", SqlValue(QString("some string")), fp_where)
										<< SqlField("Field5", SqlValue(QString("other string"), fo_equal, fb_right), fp_where)
										);

	QTest::newRow("6")	<< 	m_b6	<< (SqlFieldsVector()
										<< SqlField("ID", fv_invisible, fr_id)
										<< SqlField("Parent_ID", fv_invisible, fr_parent)
										<< SqlField("Field1", fv_visible, fr_none, "ПолеX", 100)
										<< SqlField("Field2", fv_invisible, fr_none, "ПолеY", 50)
										<< SqlField("Field3", fv_visible, fr_none, "ПолеZ", 100)

										<< SqlField("Field4", SqlValue(QDate(2010, 03, 20)),
													SqlValue(QDate(2010, 04, 1)))
										<< SqlField("Field5", SqlValue(3.14, fo_greater_equal_or_null), fp_where | fp_insert)
										<< SqlField("Field6", SqlValue(true), fp_where)
										);
}


void ut_SqlBatch::columnIndex1()
{
	QFETCH(SqlBatch, batch);
	QFETCH(int, result);

	QCOMPARE(batch.columnIndex("Field1"), result);
}

void ut_SqlBatch::columnIndex1_data()
{
	QTest::addColumn<SqlBatch>("batch");
	QTest::addColumn<int>("result");

	QTest::newRow("1")	<< 	m_b1	<< -1;
	QTest::newRow("2")	<< 	m_b2	<< -1;
	QTest::newRow("3")	<< 	m_b3	<< -1;
	QTest::newRow("4")	<< 	m_b4	<< 0;
	QTest::newRow("5")	<< 	m_b5	<< 2;
	QTest::newRow("6")	<< 	m_b6	<< 0;
}

void ut_SqlBatch::columnIndex2()
{
	QFETCH(SqlBatch, batch);
	QFETCH(int, result);

	QCOMPARE(batch.columnIndex(fr_id), result);
}

void ut_SqlBatch::columnIndex2_data()
{
	QTest::addColumn<SqlBatch>("batch");
	QTest::addColumn<int>("result");

	QTest::newRow("1")	<< 	m_b1	<< -1;
	QTest::newRow("2")	<< 	m_b2	<< -1;
	QTest::newRow("3")	<< 	m_b3	<< 0;
	QTest::newRow("4")	<< 	m_b4	<< -1;
	QTest::newRow("5")	<< 	m_b5	<< 0;
	QTest::newRow("6")	<< 	m_b6	<< 2;
}

void ut_SqlBatch::isValid()
{
	QFETCH(SqlBatch, batch);
	QFETCH(bool, result);

	QCOMPARE(batch.isValid(), result);
}

void ut_SqlBatch::isValid_data()
{
	QTest::addColumn<SqlBatch>("batch");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< 	m_b1	<< false;
	QTest::newRow("2")	<< 	m_b2	<< false;
	QTest::newRow("3")	<< 	m_b3	<< true;
	QTest::newRow("4")	<< 	m_b4	<< true;
	QTest::newRow("5")	<< 	m_b5	<< true;
	QTest::newRow("6")	<< 	m_b6	<< true;
}




