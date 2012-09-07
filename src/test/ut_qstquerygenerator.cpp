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

#include "ut_qstquerygenerator.h"

using namespace Qst;

Q_DECLARE_METATYPE(QstQueryGenerator)
Q_DECLARE_METATYPE(QstBatch)
Q_DECLARE_METATYPE(QueryType)

namespace QstTest
{


typedef QstQueryGenerator T;

/*!
	\class ut_QstQueryGenerator
	\brief Модульный тест для класса QstQueryGenerator.
*/

ut_QstQueryGenerator::ut_QstQueryGenerator()
{
}

void ut_QstQueryGenerator::initTestCase()
{
	m_b1 = QstBatch();

	m_b2.addSource("Source1");
	m_b2 << "Source2";

	m_b2.addField(QstField());

	m_b3.addSource("xSource");
	m_b3	<< QstField(RolePrimaryKey, "Field4", FieldVisible, "EngTitle", 100);

	m_b4.addSource("Tetha");
	m_b4	<< QstField("Field1", FieldInvisible, "Поле1", 20)
			<< QstField("Field4", FieldInvisible, "Поле2", 20);


	m_b5.addSource("vSomeSource");
	m_b5	<< QstField(RolePrimaryKey, "ID", FieldVisible)
			<< QstField(RoleParentKey, "Parent_ID", FieldVisible)
			<< QstField("Field1", FieldVisible, "Поле1", 100)
			<< QstField("Field2", FieldVisible, "Поле2", 50)
			<< QstField("Field3", FieldVisible, "Поле3", 100)

			<< QstField("Field1", QstValue(), PurposeWhere)
			<< QstField("Field2", QstValue(Null), PurposeWhere | PurposeInsert)
			<< QstField("Field3", QstValue(10, FunctorLess), PurposeWhere | PurposeSelect | PurposeUpdate)
			<< QstField("Field4", QstValue(QString("some string")), PurposeWhere | PurposeInsert)
			<< QstField("Field5", QstValue(QString("other string"), FunctorEqual, BracesRight), PurposeWhere)
			<< QstField(QstValue(10), PurposeParameter | PurposeInsert)	//PurposeInsert must be ignored.
			<< QstField(QstValue(QString("StringParameter")))
			<< QstField(QstValue(Null))
			<< QstField(QstValue())
			<< QstField(QstValue(QDate(2010, 4, 2)))
			;

	m_b6.addSource("vView");
	m_b6	<< QstField(RolePrimaryKey, "ID", FieldInvisible)
			<< QstField(RoleParentKey, "Parent_ID", FieldInvisible)
			<< QstField("Field1", FieldVisible, "ПолеX", 100)
			<< QstField("Field2", FieldInvisible, "ПолеY", 50)
			<< QstField("Field3", FieldVisible, "ПолеZ", 100)

			<< QstField("Field1", QstValue(QDate(1986, 7, 30)), PurposeInsert | PurposeUpdate | PurposeParameter)
			<< QstField("Field2", QstValue(Null), PurposeUpdate)
			<< QstField("Field3", QstValue(10, FunctorGreaterEqualOrNull), PurposeInsert | PurposeWhere | PurposeSelect | PurposeUpdate)
			<< QstField("Field4", QstValue(QDate(2010, 03, 20)), QstValue(QDate(2010, 04, 1)))
			<< QstField("Field5", QstValue(3.14, FunctorGreaterEqualOrNull), PurposeWhere | PurposeInsert)
			<< QstField("Field6", QstValue(true), PurposeWhere)
			;


	m_b7	<< "vView"
			<< QstField(RolePrimaryKey, "Key")
			<< QstField("Name", FieldVisible, "Наименование", 120)
			<< QstField("ID", QstValue());

	m_b8	<< "vView"
			<< QstField(RolePrimaryKey, "Key")
			<< QstField("Name", FieldVisible, "Наименование", 120)
			<< QstField("ID", QstValue(15));

	m_b9	<< "vView"
			<< QstField(RolePrimaryKey, "Key")
			<< QstField("Name", FieldVisible, "Наименование", 120)
			<< QstField("ID", QstValue())
			<< QstField("OrderDate", QstValue(), QstValue())
			;
}


void ut_QstQueryGenerator::query()
{
	QFETCH(QstQueryGenerator, gen);
	QFETCH(QString, result);

	QCOMPARE(gen.query(), result);
}

void ut_QstQueryGenerator::query_data()
{
	QTest::addColumn<QstQueryGenerator>("gen");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< 	T(m_b1, QuerySelect)	<<  QString();
	QTest::newRow("2")	<< 	T(m_b1, QueryInsert)	<<  QString();
	QTest::newRow("3")	<< 	T(m_b1, QueryExecute)	<<  QString();
	QTest::newRow("4")	<< 	T(m_b1, QueryUpdate)	<<  QString();
	QTest::newRow("5")	<< 	T(m_b1, QueryDelete)	<<  QString();

	QTest::newRow("7")	<< 	T(m_b2, QuerySelect)	<<  QString();
	QTest::newRow("8")	<< 	T(m_b2, QueryInsert)	<<  QString();
	QTest::newRow("9")	<< 	T(m_b2, QueryExecute)	<<  QString();
	QTest::newRow("10")	<< 	T(m_b2, QueryUpdate)	<<  QString();
	QTest::newRow("11")	<< 	T(m_b2, QueryDelete)	<<  QString();

	QTest::newRow("12")	<< 	T(m_b3, QuerySelect)	<<  QString("SELECT Field4 FROM xSource");
	QTest::newRow("13")	<< 	T(m_b3, QueryInsert)	<<  QString();
	QTest::newRow("14")	<< 	T(m_b3, QueryExecute)	<<  QString("EXEC xSource");
	QTest::newRow("15")	<< 	T(m_b3, QueryUpdate)	<<  QString();
	QTest::newRow("16")	<< 	T(m_b3, QueryDelete)	<<  QString("DELETE FROM xSource");

	QTest::newRow("17")	<< 	T(m_b4, QuerySelect)	<<  QString("SELECT Field1, Field4 FROM Tetha");
	QTest::newRow("18")	<< 	T(m_b4, QueryInsert)	<<  QString();
	QTest::newRow("19")	<< 	T(m_b4, QueryExecute)	<<  QString("EXEC Tetha");
	QTest::newRow("20")	<< 	T(m_b4, QueryUpdate)	<<  QString();
	QTest::newRow("21")	<< 	T(m_b4, QueryDelete)	<<  QString("DELETE FROM Tetha");

	QTest::newRow("22")	<< 	T(m_b5, QuerySelect)	<<  QString("SELECT ID, Parent_ID, Field1, Field2, Field3, Field3 FROM vSomeSource WHERE Field2 IS NULL AND Field3 < 10 AND Field4 LIKE '%some string%' AND Field5 = 'other string%'");
	QTest::newRow("23")	<< 	T(m_b5, QueryInsert)	<<  QString("INSERT INTO vSomeSource (Field2, Field4) VALUES (NULL, 'some string')");
	QTest::newRow("24")	<< 	T(m_b5, QueryExecute)	<<  QString("EXEC vSomeSource 10, 'StringParameter', NULL, NULL, '02.04.2010'");
	QTest::newRow("25")	<< 	T(m_b5, QueryUpdate)	<<  QString("UPDATE vSomeSource SET Field3 = 10 WHERE Field2 IS NULL AND Field3 < 10 AND Field4 LIKE '%some string%' AND Field5 = 'other string%'");
	QTest::newRow("26")	<< 	T(m_b5, QueryDelete)	<<  QString("DELETE FROM vSomeSource WHERE Field2 IS NULL AND Field3 < 10 AND Field4 LIKE '%some string%' AND Field5 = 'other string%'");

	QTest::newRow("27")	<< 	T(m_b6, QuerySelect)	<<  QString("SELECT Field1, Field3, ID, Parent_ID, Field2, Field3 FROM vView WHERE Field3 >= 10 OR Field3 IS NULL AND Field4 BETWEEN convert(datetime, '20.03.2010', 104) AND convert(datetime, '01.04.2010', 104) AND Field5 >= 3.14 OR Field5 IS NULL AND Field6 = true");
	QTest::newRow("28")	<< 	T(m_b6, QueryInsert)	<<  QString("INSERT INTO vView (Field1, Field3, Field5) VALUES (convert(datetime, '30.07.1986', 104), 10, 3.14)");
	QTest::newRow("29")	<< 	T(m_b6, QueryExecute)	<<  QString("EXEC vView '30.07.1986'");
	QTest::newRow("30")	<< 	T(m_b6, QueryUpdate)	<<  QString("UPDATE vView SET Field1 = convert(datetime, '30.07.1986', 104), Field2 = NULL, Field3 = 10 WHERE Field3 >= 10 OR Field3 IS NULL AND Field4 BETWEEN convert(datetime, '20.03.2010', 104) AND convert(datetime, '01.04.2010', 104) AND Field5 >= 3.14 OR Field5 IS NULL AND Field6 = true");
	QTest::newRow("31")	<< 	T(m_b6, QueryDelete)	<<  QString("DELETE FROM vView WHERE Field3 >= 10 OR Field3 IS NULL AND Field4 BETWEEN convert(datetime, '20.03.2010', 104) AND convert(datetime, '01.04.2010', 104) AND Field5 >= 3.14 OR Field5 IS NULL AND Field6 = true");

	QTest::newRow("32")	<< 	T(m_b7, QuerySelect)	<<  QString("SELECT Name, Key FROM vView");
	QTest::newRow("33")	<< 	T(m_b8, QuerySelect)	<<  QString("SELECT Name, Key FROM vView WHERE ID = 15");
	QTest::newRow("34")	<< 	T(m_b9, QuerySelect)	<<  QString("SELECT Name, Key FROM vView");

}

void ut_QstQueryGenerator::batch()
{
	QFETCH(QstQueryGenerator, gen);
	QFETCH(QstBatch, result);

	QCOMPARE(gen.batch(), result);
}

void ut_QstQueryGenerator::batch_data()
{
	QTest::addColumn<QstQueryGenerator>("gen");
	QTest::addColumn<QstBatch>("result");

	QTest::newRow("1")	<< 	T(m_b1, QuerySelect)	<<  QstBatch();
	QTest::newRow("2")	<< 	T(m_b2, QuerySelect)	<<  (QstBatch() << "Source1" << "Source2" << QstField());
	QTest::newRow("3")	<< 	T(m_b3, QuerySelect)	<<  (QstBatch()
														 << "xSource"
														 << QstField(RolePrimaryKey, "Field4", FieldVisible, "EngTitle", 100));

	QTest::newRow("4")	<< 	T(m_b4, QuerySelect)
			<<  (QstBatch() << "Tetha"
				<< QstField("Field1", FieldInvisible, "Поле1", 20)
				<< QstField("Field4", FieldInvisible, "Поле2", 20)
				);

	QTest::newRow("5")	<< 	T(m_b5, QuerySelect)
			<<  (QstBatch()
				 <<  "vSomeSource"
				<< QstField(RolePrimaryKey, "ID", FieldVisible)
				<< QstField(RoleParentKey, "Parent_ID", FieldVisible)
				<< QstField("Field1", FieldVisible, "Поле1", 100)
				<< QstField("Field2", FieldVisible, "Поле2", 50)
				<< QstField("Field3", FieldVisible, "Поле3", 100)
				<< QstField("Field1", QstValue(), PurposeWhere)
				<< QstField("Field2", QstValue(Null), PurposeWhere | PurposeInsert)
				<< QstField("Field3", QstValue(10, FunctorLess), PurposeWhere | PurposeSelect | PurposeUpdate)
				<< QstField("Field4", QstValue(QString("some string")), PurposeWhere | PurposeInsert)
				<< QstField("Field5", QstValue(QString("other string"), FunctorEqual, BracesRight), PurposeWhere)
				<< QstField(QstValue(10), PurposeParameter | PurposeInsert)
				<< QstField(QstValue(QString("StringParameter")))
				<< QstField(QstValue(Null))
				<< QstField(QstValue())
				<< QstField(QstValue(QDate(2010, 4, 2)))
				);

	QTest::newRow("6")	<< 	T(m_b6, QuerySelect)
			<<  (QstBatch()
				<<  "vView"
				<< QstField(RolePrimaryKey, "ID", FieldInvisible)
				<< QstField(RoleParentKey, "Parent_ID", FieldInvisible)
				<< QstField("Field1", FieldVisible, "ПолеX", 100)
				<< QstField("Field2", FieldInvisible, "ПолеY", 50)
				<< QstField("Field3", FieldVisible, "ПолеZ", 100)
				<< QstField("Field1", QstValue(QDate(1986, 7, 30)), PurposeInsert | PurposeUpdate | PurposeParameter)
				<< QstField("Field2", QstValue(Null), PurposeUpdate)
				<< QstField("Field3", QstValue(10, FunctorGreaterEqualOrNull), PurposeInsert | PurposeWhere | PurposeSelect | PurposeUpdate)
				<< QstField("Field4", QstValue(QDate(2010, 03, 20)), QstValue(QDate(2010, 04, 1)))
				<< QstField("Field5", QstValue(3.14, FunctorGreaterEqualOrNull), PurposeWhere | PurposeInsert)
				<< QstField("Field6", QstValue(true), PurposeWhere)
				);
}

void ut_QstQueryGenerator::queryType()
{
	QFETCH(QstQueryGenerator, gen);
	QFETCH(QueryType, result);

	QCOMPARE(gen.queryType(), result);
}

void ut_QstQueryGenerator::queryType_data()
{
	QTest::addColumn<QstQueryGenerator>("gen");
	QTest::addColumn<QueryType>("result");

	QTest::newRow("1")	<< 	T(m_b1, QuerySelect)	<<  QuerySelect;
	QTest::newRow("2")	<< 	T(m_b1, QueryInsert)	<<  QueryInsert;
	QTest::newRow("3")	<< 	T(m_b1, QueryExecute)	<<  QueryExecute;
	QTest::newRow("4")	<< 	T(m_b1, QueryUpdate)	<<  QueryUpdate;
	QTest::newRow("5")	<< 	T(m_b1, QueryDelete)	<<  QueryDelete;
}

void ut_QstQueryGenerator::isValid()
{
	QFETCH(QstQueryGenerator, gen);
	QFETCH(bool, result);

	QCOMPARE(gen.isValid(), result);
}

void ut_QstQueryGenerator::isValid_data()
{
	QTest::addColumn<QstQueryGenerator>("gen");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< 	T(m_b1, QuerySelect)	<<  false;
	QTest::newRow("2")	<< 	T(m_b2, QuerySelect)	<<  false;
	QTest::newRow("3")	<< 	T(m_b3, QuerySelect)	<<  true;
	QTest::newRow("4")	<< 	T(m_b4, QuerySelect)	<<  true;
	QTest::newRow("5")	<< 	T(m_b5, QuerySelect)	<<  true;

}


} // End of namespace QstTest
