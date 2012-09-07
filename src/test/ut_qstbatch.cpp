/****************************************************************************
** QST 0.4.2a beta
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

#include "ut_qstbatch.h"

using namespace Qst;

typedef QstBatch T;

Q_DECLARE_METATYPE(QstBatch)
Q_DECLARE_METATYPE(QstValue)
Q_DECLARE_METATYPE(QstField)
Q_DECLARE_METATYPE(QstFieldVector)

namespace QstTest
{


/*!
	\class ut_QstBatch
	\brief Модульный тест для класса QstBatch.
*/

ut_QstBatch::ut_QstBatch()
{
}

void ut_QstBatch::initTestCase()
{
	m_b1 = QstBatch();

	m_b2.addSource("Source1");
	m_b2 << "Source2";
	m_b2.addField(QstField());


	m_b3.addSource("xSource");
	m_b3	<< QstField(RolePrimaryKey, "Field4", FieldVisible, "EngTitle", 100)	;

	m_b4.addSource("Tetha");
	m_b4	<< QstField("Field1", FieldInvisible, "РусЗаголовок", 20)
			<< QstField("Field4", FieldInvisible, "РусЗаголовок2", 20);


	m_b5.addSource("vSomeSource");
	m_b5	<< QstField(RolePrimaryKey, "ID", FieldVisible)
			<< QstField(RoleParentKey, "Parent_ID", FieldVisible)
			<< QstField("Field1", FieldVisible, "Поле1", 100)
			<< QstField("Field2", FieldVisible, "Поле2", 50)
			<< QstField("Field3", FieldVisible, "Поле3", 100)

			<< QstField("Field1", QstValue(), PurposeWhere)
			<< QstField("Field2", QstValue(Null), PurposeWhere | PurposeInsert)
			<< QstField("Field3", QstValue(10, FunctorLess), PurposeWhere | PurposeSelect)
			<< QstField("Field4", QstValue(QString("some string")), PurposeWhere)
			<< QstField("Field5", QstValue(QString("other string"), FunctorEqual, BracesRight), PurposeWhere)
			;

	m_b6.addSource("vView");
	m_b6	<< QstField(RolePrimaryKey, "ID", FieldInvisible)
			<< QstField(RoleParentKey, "Parent_ID", FieldInvisible)
			<< QstField("Field1", FieldVisible, "ПолеX", 100)
			<< QstField("Field2", FieldInvisible, "ПолеY", 50)
			<< QstField("Field3", FieldVisible, "ПолеZ", 100)

			<< QstField("Field4", QstValue(QDate(2010, 03, 20)),
						QstValue(QDate(2010, 04, 1)))
			<< QstField("Field5", QstValue(3.14, FunctorGreaterEqualOrNull), PurposeWhere | PurposeInsert)
			<< QstField("Field6", QstValue(true), PurposeWhere)
			;
}

void ut_QstBatch::sources()
{
	QFETCH(QstBatch, batch);
	QFETCH(QStringList, result);

	QCOMPARE(batch.sources(), result);
}

void ut_QstBatch::sources_data()
{
	QTest::addColumn<QstBatch>("batch");
	QTest::addColumn<QStringList>("result");

	QTest::newRow("1")	<< 	m_b1	<<  QStringList();
	QTest::newRow("2")	<< 	m_b2	<< (QStringList() << "Source1" << "Source2");
	QTest::newRow("3")	<< 	m_b3	<< (QStringList() << "xSource");
	QTest::newRow("4")	<< 	m_b4	<< (QStringList() << "Tetha");
	QTest::newRow("5")	<< 	m_b5	<< (QStringList() << "vSomeSource");
	QTest::newRow("6")	<< 	m_b6	<< (QStringList() << "vView");
}

void ut_QstBatch::selectClauseFields()
{
	QFETCH(QstBatch, batch);
	QFETCH(QstFieldVector, result);

	QCOMPARE(batch.fields(PurposeSelect), result);
}

void ut_QstBatch::selectClauseFields_data()
{
	QTest::addColumn<QstBatch>("batch");
	QTest::addColumn<QstFieldVector>("result");

	QTest::newRow("1")	<< 	m_b1	<< QstFieldVector();
	QTest::newRow("2")	<< 	m_b2	<< QstFieldVector();
	QTest::newRow("3")	<< 	m_b3	<< (QstFieldVector()
										<< QstField(RolePrimaryKey, "Field4", FieldVisible, "EngTitle", 100)
										);

	QTest::newRow("4")	<< 	m_b4	<< (QstFieldVector()
										<< QstField("Field1", FieldInvisible, "РусЗаголовок", 20)
										<< QstField("Field4", FieldInvisible, "РусЗаголовок2", 20)
										);

	QTest::newRow("5")	<< 	m_b5	<< (QstFieldVector()
										<< QstField(RolePrimaryKey, "ID", FieldVisible)
										<< QstField(RoleParentKey, "Parent_ID", FieldVisible)
										<< QstField("Field1", FieldVisible, "Поле1", 100)
										<< QstField("Field2", FieldVisible, "Поле2", 50)
										<< QstField("Field3", FieldVisible, "Поле3", 100)
										);

	QTest::newRow("6")	<< 	m_b6	<< (QstFieldVector()
										<< QstField("Field1", FieldVisible, "ПолеX", 100)
										<< QstField("Field3", FieldVisible, "ПолеZ", 100)
										<< QstField(RolePrimaryKey, "ID", FieldInvisible)
										<< QstField(RoleParentKey, "Parent_ID", FieldInvisible)
										<< QstField("Field2", FieldInvisible, "ПолеY", 50)
										);
}

void ut_QstBatch::stuffFields()
{
	QFETCH(QstBatch, batch);
	QFETCH(QstFieldVector, result);

	QCOMPARE(batch.fields(PurposeAllButSelect_Mask), result);
}

void ut_QstBatch::stuffFields_data()
{
	QTest::addColumn<QstBatch>("batch");
	QTest::addColumn<QstFieldVector>("result");

	QTest::newRow("1")	<< 	m_b1	<< QstFieldVector();
	QTest::newRow("2")	<< 	m_b2	<< (QstFieldVector());
	QTest::newRow("3")	<< 	m_b3	<< (QstFieldVector());

	QTest::newRow("4")	<< 	m_b4	<< (QstFieldVector());

	QTest::newRow("5")	<< 	m_b5	<< (QstFieldVector()
										<< QstField("Field1", QstValue(), PurposeWhere)
										<< QstField("Field2", QstValue(Null), PurposeWhere | PurposeInsert)
										<< QstField("Field3", QstValue(10, FunctorLess), PurposeWhere | PurposeSelect)
										<< QstField("Field4", QstValue(QString("some string")), PurposeWhere)
										<< QstField("Field5", QstValue(QString("other string"), FunctorEqual, BracesRight), PurposeWhere)
										);

	QTest::newRow("6")	<< 	m_b6	<< (QstFieldVector()
										<< QstField("Field4", QstValue(QDate(2010, 03, 20)),
													QstValue(QDate(2010, 04, 1)))
										<< QstField("Field5", QstValue(3.14, FunctorGreaterEqualOrNull), PurposeWhere | PurposeInsert)
										<< QstField("Field6", QstValue(true), PurposeWhere)
										);
}

void ut_QstBatch::fields()
{
	QFETCH(QstBatch, batch);
	QFETCH(QstFieldVector, result);

	QCOMPARE(batch.fields(), result);
}

void ut_QstBatch::fields_data()
{
	QTest::addColumn<QstBatch>("batch");
	QTest::addColumn<QstFieldVector>("result");

	QTest::newRow("1")	<< 	m_b1	<< QstFieldVector();
	QTest::newRow("2")	<< 	m_b2	<< (QstFieldVector()
										<< QstField());

	QTest::newRow("3")	<< 	m_b3	<< (QstFieldVector()
										<< QstField(RolePrimaryKey, "Field4", FieldVisible, "EngTitle", 100));

	QTest::newRow("4")	<< 	m_b4	<< (QstFieldVector()
										<< QstField("Field1", FieldInvisible, "РусЗаголовок", 20)
										<< QstField("Field4", FieldInvisible, "РусЗаголовок2", 20));

	QTest::newRow("5")	<< 	m_b5	<< (QstFieldVector()
										<< QstField(RolePrimaryKey, "ID", FieldVisible)
										<< QstField(RoleParentKey, "Parent_ID", FieldVisible)
										<< QstField("Field1", FieldVisible, "Поле1", 100)
										<< QstField("Field2", FieldVisible, "Поле2", 50)
										<< QstField("Field3", FieldVisible, "Поле3", 100)

										<< QstField("Field1", QstValue(), PurposeWhere)
										<< QstField("Field2", QstValue(Null), PurposeWhere | PurposeInsert)
										<< QstField("Field3", QstValue(10, FunctorLess), PurposeWhere | PurposeSelect)
										<< QstField("Field4", QstValue(QString("some string")), PurposeWhere)
										<< QstField("Field5", QstValue(QString("other string"), FunctorEqual, BracesRight), PurposeWhere)
										);

	QTest::newRow("6")	<< 	m_b6	<< (QstFieldVector()
										<< QstField(RolePrimaryKey, "ID", FieldInvisible)
										<< QstField(RoleParentKey, "Parent_ID", FieldInvisible)
										<< QstField("Field1", FieldVisible, "ПолеX", 100)
										<< QstField("Field2", FieldInvisible, "ПолеY", 50)
										<< QstField("Field3", FieldVisible, "ПолеZ", 100)

										<< QstField("Field4", QstValue(QDate(2010, 03, 20)),
													QstValue(QDate(2010, 04, 1)))
										<< QstField("Field5", QstValue(3.14, FunctorGreaterEqualOrNull), PurposeWhere | PurposeInsert)
										<< QstField("Field6", QstValue(true), PurposeWhere)
										);
}


void ut_QstBatch::columnIndex1()
{
	QFETCH(QstBatch, batch);
	QFETCH(int, result);

	QCOMPARE(batch.columnIndex("Field1"), result);
}

void ut_QstBatch::columnIndex1_data()
{
	QTest::addColumn<QstBatch>("batch");
	QTest::addColumn<int>("result");

	QTest::newRow("1")	<< 	m_b1	<< -1;
	QTest::newRow("2")	<< 	m_b2	<< -1;
	QTest::newRow("3")	<< 	m_b3	<< -1;
	QTest::newRow("4")	<< 	m_b4	<< 0;
	QTest::newRow("5")	<< 	m_b5	<< 2;
	QTest::newRow("6")	<< 	m_b6	<< 0;
}

void ut_QstBatch::columnIndex2()
{
	QFETCH(QstBatch, batch);
	QFETCH(int, result);

	QCOMPARE(batch.columnIndex(RolePrimaryKey), result);
}

void ut_QstBatch::columnIndex2_data()
{
	QTest::addColumn<QstBatch>("batch");
	QTest::addColumn<int>("result");

	QTest::newRow("1")	<< 	m_b1	<< -1;
	QTest::newRow("2")	<< 	m_b2	<< -1;
	QTest::newRow("3")	<< 	m_b3	<< 0;
	QTest::newRow("4")	<< 	m_b4	<< -1;
	QTest::newRow("5")	<< 	m_b5	<< 0;
	QTest::newRow("6")	<< 	m_b6	<< 2;
}

void ut_QstBatch::isValid()
{
	QFETCH(QstBatch, batch);
	QFETCH(bool, result);

	QCOMPARE(batch.isValid(), result);
}

void ut_QstBatch::isValid_data()
{
	QTest::addColumn<QstBatch>("batch");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< 	m_b1	<< false;
	QTest::newRow("2")	<< 	m_b2	<< false;
	QTest::newRow("3")	<< 	m_b3	<< true;
	QTest::newRow("4")	<< 	m_b4	<< true;
	QTest::newRow("5")	<< 	m_b5	<< true;
	QTest::newRow("6")	<< 	m_b6	<< true;
}

} // End of namespace QstTest
