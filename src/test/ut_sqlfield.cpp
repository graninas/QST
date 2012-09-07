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

#include "ut_sqlfield.h"

typedef SqlField T;

Q_DECLARE_METATYPE(SqlField)
Q_DECLARE_METATYPE(SqlValue)
Q_DECLARE_METATYPE(SqlFieldVisibility)
Q_DECLARE_METATYPE(Qt::Orientation)
Q_DECLARE_METATYPE(SqlFieldPurpose)
Q_DECLARE_METATYPE(SqlFieldPurposes)
Q_DECLARE_METATYPE(SqlFieldRoles)


/*!
	\class ut_SqlField
	\brief Модульный тест для класса SqlField. См. ut_sqlfield.cpp .
*/


ut_SqlField::ut_SqlField()
{
}


void ut_SqlField::name()
{
	QFETCH(SqlField, value);
	QFETCH(QString, result);

	QCOMPARE(value.name(), result);
}

void ut_SqlField::name_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< T()				<< QString();
	QTest::newRow("2")	<< T("SomeName")	<< QString("SomeName");
	QTest::newRow("3")	<< T(SqlValue())	<< QString();
}

void ut_SqlField::visibility()
{
	QFETCH(SqlField, value);
	QFETCH(SqlFieldVisibility, result);

	QCOMPARE(value.visibility(), result);
}

void ut_SqlField::visibility_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<SqlFieldVisibility>("result");

	QTest::newRow("1")	<< T()										<< fv_none;
	QTest::newRow("2")	<< T("SomeName")							<< fv_visible;
	QTest::newRow("3")	<< T("SomeName", fv_visible)				<< fv_visible;
	QTest::newRow("4")	<< T("SomeName", fv_invisible)				<< fv_invisible;
	QTest::newRow("5")	<< T("SomeName", SqlValue())				<< fv_none;
	QTest::newRow("6")	<< T("SomeName", SqlValue(), SqlValue())	<< fv_none;
	QTest::newRow("7")	<< T(SqlValue())							<< fv_none;
	QTest::newRow("8")	<< T("SomeName", fp_where)					<< fv_none;
}

void ut_SqlField::columnTitle()
{
	QFETCH(SqlField, value);
	QFETCH(QString, result);

	QCOMPARE(value.columnTitle(), result);
}

void ut_SqlField::columnTitle_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< T()												<< QString(tr(""));
	QTest::newRow("2")	<< T("SomeName")									<< QString(tr("SomeName"));
	QTest::newRow("3")	<< T("SomeName", fv_visible, fr_none, "EngTitle")	<< QString(tr("EngTitle"));
	QTest::newRow("4")	<< T("SomeName", fv_invisible, fr_none, "РусНазв")	<< QString(tr("РусНазв"));
	QTest::newRow("5")	<< T("SomeName", fv_invisible, fr_none, "РусEng")	<< QString(tr("РусEng"));
	QTest::newRow("6")	<< T("SomeName", SqlValue())						<< QString(tr(""));
	QTest::newRow("7")	<< T("SomeName", SqlValue(), SqlValue())			<< QString(tr(""));
	QTest::newRow("8")	<< T(SqlValue())									<< QString(tr(""));
	QTest::newRow("9")	<< T("SomeName", fp_where)							<< QString(tr(""));
}

void ut_SqlField::orientation()
{
	QFETCH(SqlField, value);
	QFETCH(Qt::Orientation, result);

	QCOMPARE(value.orientation(), result);
}

void ut_SqlField::orientation_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<Qt::Orientation>("result");

	QTest::newRow("1")	<< T()													<< Qt::Horizontal;
	QTest::newRow("2")	<< T("SomeName")										<< Qt::Horizontal;
	QTest::newRow("3")	<< T("sn", fv_visible, fr_none, "et", 0, Qt::Vertical)	<< Qt::Vertical;
	QTest::newRow("6")	<< T("SomeName", SqlValue())							<< Qt::Horizontal;
	QTest::newRow("7")	<< T("SomeName", SqlValue(), SqlValue())				<< Qt::Horizontal;
	QTest::newRow("8")	<< T(SqlValue())										<< Qt::Horizontal;
	QTest::newRow("9")	<< T("SomeName", fp_where)								<< Qt::Horizontal;
}

void ut_SqlField::purposes()
{
	QFETCH(SqlField, value);
	QFETCH(SqlFieldPurposes, result);

	QCOMPARE(value.purposes(), result);
}

void ut_SqlField::purposes_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<SqlFieldPurposes>("result");

	QTest::newRow("1")	<< T()												<< (fp_none | fp_none);
	QTest::newRow("2")	<< T(QString("SN"))									<< (fp_select | fp_select);
	QTest::newRow("3")	<< T("SN")											<< (fp_select | fp_select);
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)						<< (fp_select | fp_select);
	QTest::newRow("5")	<< T("SN", SqlValue())								<< (fp_all_valued | fp_all_valued);
	QTest::newRow("6")	<< T("SN", SqlValue())								<< (fp_where | fp_insert | fp_update | fp_parameter);
	QTest::newRow("7")	<< T("SN", SqlValue(), fp_modifying)				<< (fp_insert | fp_update);
	QTest::newRow("8")	<< T("SN", SqlValue(), fp_insert)					<< (fp_insert | fp_insert);
	QTest::newRow("9")	<< T("SN", SqlValue(), SqlValue())					<< (fp_where | fp_where);
	QTest::newRow("10")	<< T("SN", SqlValue(), SqlValue(), fp_all_valued)	<< (fp_where | fp_where);
	QTest::newRow("11")	<< T(SqlValue())									<< (fp_parameter | fp_parameter);
	QTest::newRow("12")	<< T(SqlValue(), fp_all_valued)						<< (fp_parameter | fp_parameter);
	QTest::newRow("13")	<< T("SN", fp_all_valued)							<< (fp_where | fp_insert | fp_update | fp_parameter);
	QTest::newRow("14")	<< T("SN", fp_order_by)								<< (fp_order_by | fp_order_by);
	QTest::newRow("15")	<< T("SN", fp_order_by | fp_group_by)				<< (fp_order_by | fp_group_by);
}

void ut_SqlField::role()
{
	QFETCH(SqlField, value);
	QFETCH(SqlFieldRoles, result);

	QCOMPARE(value.role(), result);
}

void ut_SqlField::role_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<SqlFieldRoles>("result");

	QTest::newRow("1")	<< T()												<< (fr_none);
	QTest::newRow("2")	<< T(QString("SN"))									<< (fr_none);
	QTest::newRow("3")	<< T("SN")											<< (fr_none);
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)						<< (fr_none);
	QTest::newRow("5")	<< T("SN", fv_visible, fr_id)						<< (fr_id);
	QTest::newRow("6")	<< T("SN", fv_visible, fr_parent)					<< (fr_parent);
	QTest::newRow("7")	<< T("SN", SqlValue())								<< (fr_none);
	QTest::newRow("8")	<< T(SqlValue())									<< (fr_none);
	QTest::newRow("9")	<< T("SN", fp_all_valued)							<< (fr_none);
}

void ut_SqlField::value()
{
	QFETCH(SqlField, value);
	QFETCH(SqlValue, result);

	QCOMPARE(value.value(), result);
}

void ut_SqlField::value_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<SqlValue>("result");

	QTest::newRow("1")	<< T()												<< SqlValue();
	QTest::newRow("2")	<< T(QString("SN"))									<< SqlValue();
	QTest::newRow("3")	<< T("SN")											<< SqlValue();
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)						<< SqlValue();
	QTest::newRow("5")	<< T("SN", SqlValue())								<< SqlValue();
	QTest::newRow("6")	<< T("SN", SqlValue(QVariant()))					<< SqlValue(QVariant());
	QTest::newRow("7")	<< T("SN", SqlValue(QVariant("dfd")))				<< SqlValue(QVariant("dfd"));
	QTest::newRow("8")	<< T("SN", SqlValue("1"), SqlValue("2"))			<< SqlValue("1");
	QTest::newRow("9")	<< T("SN", SqlValue(), SqlValue("2"))				<< SqlValue();
	QTest::newRow("10")	<< T(SqlValue())									<< SqlValue();
	QTest::newRow("11")	<< T(SqlValue(QDate(10,1,22)))						<< SqlValue(QDate(10,1,22));
	QTest::newRow("12")	<< T("SN", fp_all_valued)							<< SqlValue();
}

void ut_SqlField::value1()
{
	QFETCH(SqlField, value);
	QFETCH(SqlValue, result);

	QCOMPARE(value.value(bvo_first), result);
}

void ut_SqlField::value1_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<SqlValue>("result");

	QTest::newRow("1")	<< T()												<< SqlValue();
	QTest::newRow("2")	<< T(QString("SN"))									<< SqlValue();
	QTest::newRow("3")	<< T("SN")											<< SqlValue();
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)						<< SqlValue();
	QTest::newRow("5")	<< T("SN", SqlValue())								<< SqlValue();
	QTest::newRow("6")	<< T("SN", SqlValue(QVariant()))					<< SqlValue();
	QTest::newRow("7")	<< T("SN", SqlValue(QVariant("dfd")))				<< SqlValue("dfd");
	QTest::newRow("8")	<< T("SN", SqlValue("1"), SqlValue("2"))			<< SqlValue("1");
	QTest::newRow("9")	<< T("SN", SqlValue(), SqlValue("2"))				<< SqlValue();
	QTest::newRow("10")	<< T(SqlValue())									<< SqlValue();
	QTest::newRow("11")	<< T(SqlValue(QDate(10,1,22)))						<< SqlValue(QDate(10,1,22));
	QTest::newRow("12")	<< T("SN", fp_all_valued)							<< SqlValue();
}


void ut_SqlField::value2()
{
	QFETCH(SqlField, value);
	QFETCH(SqlValue, result);

	QCOMPARE(value.value(bvo_second), result);
}

void ut_SqlField::value2_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<SqlValue>("result");

	QTest::newRow("1")	<< T()												<< SqlValue();
	QTest::newRow("2")	<< T(QString("SN"))									<< SqlValue();
	QTest::newRow("3")	<< T("SN")											<< SqlValue();
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)						<< SqlValue();
	QTest::newRow("5")	<< T("SN", SqlValue())								<< SqlValue();
	QTest::newRow("6")	<< T("SN", SqlValue(QVariant()))					<< SqlValue();
	QTest::newRow("7")	<< T("SN", SqlValue(QVariant("dfd")))				<< SqlValue();
	QTest::newRow("8")	<< T("SN", SqlValue("1"), SqlValue("2"))			<< SqlValue("2");
	QTest::newRow("9")	<< T("SN", SqlValue(), SqlValue("2"))				<< SqlValue("2");
	QTest::newRow("10")	<< T(SqlValue())									<< SqlValue();
	QTest::newRow("11")	<< T(SqlValue(QDate(10,1,22)))						<< SqlValue();
	QTest::newRow("12")	<< T("SN", fp_all_valued)							<< SqlValue();
}

void ut_SqlField::hasValue()
{
	QFETCH(SqlField, value);
	QFETCH(bool, result);

	QCOMPARE(value.hasValue(), result);
}

void ut_SqlField::hasValue_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()												<< false;
	QTest::newRow("2")	<< T(QString("SN"))									<< false;
	QTest::newRow("3")	<< T("SN")											<< false;
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)						<< false;
	QTest::newRow("5")	<< T("SN", SqlValue())								<< true;
	QTest::newRow("6")	<< T("SN", SqlValue(QVariant()))					<< true;
	QTest::newRow("7")	<< T("SN", SqlValue(QVariant("dfd")))				<< true;
	QTest::newRow("8")	<< T("SN", SqlValue("1"), SqlValue("2"))			<< true;
	QTest::newRow("9")	<< T("SN", SqlValue(), SqlValue("2"))				<< true;
	QTest::newRow("10")	<< T(SqlValue())									<< true;
	QTest::newRow("11")	<< T(SqlValue(QDate(10,1,22)))						<< true;
	QTest::newRow("12")	<< T("SN", fp_all_valued)							<< false;
}

void ut_SqlField::hasValue1()
{
	QFETCH(SqlField, value);
	QFETCH(bool, result);

	QCOMPARE(value.hasValue(bvo_first), result);
}

void ut_SqlField::hasValue1_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()												<< false;
	QTest::newRow("2")	<< T(QString("SN"))									<< false;
	QTest::newRow("3")	<< T("SN")											<< false;
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)						<< false;
	QTest::newRow("5")	<< T("SN", SqlValue())								<< true;
	QTest::newRow("6")	<< T("SN", SqlValue(QVariant()))					<< true;
	QTest::newRow("7")	<< T("SN", SqlValue(QVariant("dfd")))				<< true;
	QTest::newRow("8")	<< T("SN", SqlValue("1"), SqlValue("2"))			<< true;
	QTest::newRow("9")	<< T("SN", SqlValue(), SqlValue("2"))				<< true;
	QTest::newRow("10")	<< T(SqlValue())									<< true;
	QTest::newRow("11")	<< T(SqlValue(QDate(10,1,22)))						<< true;
	QTest::newRow("12")	<< T("SN", fp_all_valued)							<< false;
}

void ut_SqlField::hasValue2()
{
	QFETCH(SqlField, value);
	QFETCH(bool, result);

	QCOMPARE(value.hasValue(bvo_second), result);
}

void ut_SqlField::hasValue2_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()												<< false;
	QTest::newRow("2")	<< T(QString("SN"))									<< false;
	QTest::newRow("3")	<< T("SN")											<< false;
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)						<< false;
	QTest::newRow("5")	<< T("SN", SqlValue())								<< false;
	QTest::newRow("6")	<< T("SN", SqlValue(QVariant()))					<< false;
	QTest::newRow("7")	<< T("SN", SqlValue(QVariant("dfd")))				<< false;
	QTest::newRow("8")	<< T("SN", SqlValue("1"), SqlValue("2"))			<< true;
	QTest::newRow("9")	<< T("SN", SqlValue(), SqlValue("2"))				<< true;
	QTest::newRow("10")	<< T(SqlValue())									<< false;
	QTest::newRow("11")	<< T(SqlValue(QDate(10,1,22)))						<< false;
	QTest::newRow("12")	<< T("SN", fp_all_valued)							<< false;
}

void ut_SqlField::isBinaryFilter()
{
	QFETCH(SqlField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isBinaryFilter(), result);
}

void ut_SqlField::isBinaryFilter_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()												<< false;
	QTest::newRow("2")	<< T(QString("SN"))									<< false;
	QTest::newRow("3")	<< T("SN")											<< false;
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)						<< false;
	QTest::newRow("5")	<< T("SN", SqlValue())								<< false;
	QTest::newRow("6")	<< T("SN", SqlValue(QVariant()))					<< false;
	QTest::newRow("7")	<< T("SN", SqlValue(QVariant("dfd")))				<< false;
	QTest::newRow("8")	<< T("SN", SqlValue("1"), SqlValue("2"))			<< true;
	QTest::newRow("9")	<< T("SN", SqlValue(), SqlValue("2"))				<< true;
	QTest::newRow("10")	<< T(SqlValue())									<< false;
	QTest::newRow("11")	<< T(SqlValue(QDate(10,1,22)))						<< false;
	QTest::newRow("12")	<< T("SN", fp_all_valued)							<< false;
}

void ut_SqlField::isVisible()
{
	QFETCH(SqlField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isVisible(), result);
}

void ut_SqlField::isVisible_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< false;
	QTest::newRow("2")	<< T("SomeName")							<< true;
	QTest::newRow("3")	<< T("SomeName", fv_visible)				<< true;
	QTest::newRow("4")	<< T("SomeName", fv_invisible)				<< false;
	QTest::newRow("5")	<< T("SomeName", SqlValue())				<< false;
	QTest::newRow("6")	<< T("SomeName", SqlValue(), SqlValue())	<< false;
	QTest::newRow("7")	<< T(SqlValue())							<< false;
	QTest::newRow("8")	<< T("SomeName", fp_where)					<< false;
}


void ut_SqlField::isInvisible()
{
	QFETCH(SqlField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isInvisible(), result);
}

void ut_SqlField::isInvisible_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< false;
	QTest::newRow("2")	<< T("SomeName")							<< false;
	QTest::newRow("3")	<< T("SomeName", fv_visible)				<< false;
	QTest::newRow("4")	<< T("SomeName", fv_invisible)				<< true;
	QTest::newRow("5")	<< T("SomeName", SqlValue())				<< false;
	QTest::newRow("6")	<< T("SomeName", SqlValue(), SqlValue())	<< false;
	QTest::newRow("7")	<< T(SqlValue())							<< false;
	QTest::newRow("8")	<< T("SomeName", fp_where)					<< false;
}

void ut_SqlField::isNoneVisibility()
{
	QFETCH(SqlField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isNoneVisibility(), result);
}

void ut_SqlField::isNoneVisibility_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< true;
	QTest::newRow("2")	<< T("SomeName")							<< false;
	QTest::newRow("3")	<< T("SomeName", fv_visible)				<< false;
	QTest::newRow("4")	<< T("SomeName", fv_invisible)				<< false;
	QTest::newRow("5")	<< T("SomeName", SqlValue())				<< true;
	QTest::newRow("6")	<< T("SomeName", SqlValue(), SqlValue())	<< true;
	QTest::newRow("7")	<< T(SqlValue())							<< true;
	QTest::newRow("8")	<< T("SomeName", fp_where)					<< true;
}

void ut_SqlField::isService()
{
	QFETCH(SqlField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isService(), result);
}

void ut_SqlField::isService_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()												<< false;
	QTest::newRow("2")	<< T(QString("SN"))									<< false;
	QTest::newRow("3")	<< T("SN")											<< false;
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)						<< false;
	QTest::newRow("5")	<< T("SN", fv_visible, fr_id)						<< true;
	QTest::newRow("6")	<< T("SN", fv_visible, fr_parent)					<< true;
	QTest::newRow("7")	<< T("SN", SqlValue())								<< false;
	QTest::newRow("8")	<< T("SN", SqlValue(), SqlValue("2"))				<< false;
	QTest::newRow("9")	<< T(SqlValue())									<< false;
	QTest::newRow("10")	<< T("SN", fp_all_valued)							<< false;
}

void ut_SqlField::isSelectClauseField()
{
	QFETCH(SqlField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isSelectClauseField(), result);
}

void ut_SqlField::isSelectClauseField_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()												<< false;
	QTest::newRow("2")	<< T(QString("SN"))									<< true;
	QTest::newRow("3")	<< T("SN")											<< true;
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)						<< true;
	QTest::newRow("5")	<< T("SN", fv_visible, fr_id)						<< true;
	QTest::newRow("6")	<< T("SN", fv_visible, fr_parent)					<< true;
	QTest::newRow("7")	<< T("SN", SqlValue())								<< false;
	QTest::newRow("8")	<< T("SN", SqlValue(), SqlValue("2"))				<< false;
	QTest::newRow("9")	<< T(SqlValue())									<< false;
	QTest::newRow("10")	<< T("SN", fp_all_valued)							<< false;
}

void ut_SqlField::isStuffField()
{
	QFETCH(SqlField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isStuffField(), result);
}

void ut_SqlField::isStuffField_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()												<< false;
	QTest::newRow("2")	<< T(QString("SN"))									<< false;
	QTest::newRow("3")	<< T("SN")											<< false;
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)						<< false;
	QTest::newRow("5")	<< T("SN", fv_visible, fr_id)						<< false;
	QTest::newRow("6")	<< T("SN", fv_visible, fr_parent)					<< false;
	QTest::newRow("7")	<< T("SN", SqlValue())								<< true;
	QTest::newRow("8")	<< T("SN", SqlValue(), SqlValue("2"))				<< true;
	QTest::newRow("9")	<< T(SqlValue())									<< true;
	QTest::newRow("10")	<< T("SN", fp_all_valued)							<< true;
}


void ut_SqlField::isValuesValid()
{
	QFETCH(SqlField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isValuesValid(), result);
}

void ut_SqlField::isValuesValid_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< false;
	QTest::newRow("2")	<< T(QString("SN"))							<< false;
	QTest::newRow("3")	<< T("SN")									<< false;
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)				<< false;
	QTest::newRow("5")	<< T("SN", SqlValue())						<< false;
	QTest::newRow("6")	<< T("SN", SqlValue(QVariant()))			<< false;
	QTest::newRow("7")	<< T("SN", SqlValue(QVariant("dfd")))		<< true;
	QTest::newRow("8")	<< T("SN", SqlValue("1"), SqlValue("2"))	<< true;
	QTest::newRow("9")	<< T("SN", SqlValue(), SqlValue("2"))		<< false;
	QTest::newRow("10")	<< T(SqlValue())							<< false;
	QTest::newRow("11")	<< T(SqlValue(QDate(10,1,22)))				<< true;
	QTest::newRow("12")	<< T("SN", fp_all_valued)					<< false;
}

void ut_SqlField::isValid()
{
	QFETCH(SqlField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isValid(), result);
}

void ut_SqlField::isValid_data()
{
	QTest::addColumn<SqlField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< false;
	QTest::newRow("2")	<< T(QString("SN"))							<< true;
	QTest::newRow("3")	<< T("SN")									<< true;
	QTest::newRow("4")	<< T("SN", fv_visible, fr_none)				<< true;
	QTest::newRow("5")	<< T("SN", SqlValue())						<< false;
	QTest::newRow("6")	<< T("SN", SqlValue(nt_null))				<< true;
	QTest::newRow("7")	<< T("SN", SqlValue(QVariant()))			<< false;
	QTest::newRow("8")	<< T("SN", SqlValue(QVariant("dfd")))		<< true;
	QTest::newRow("9")	<< T("SN", SqlValue("1"), SqlValue("2"))	<< true;
	QTest::newRow("10")	<< T("SN", SqlValue(), SqlValue("2"))		<< false;
	QTest::newRow("11")	<< T(SqlValue())							<< false;
	QTest::newRow("12")	<< T(SqlValue(QDate(10,1,22)))				<< true;
	QTest::newRow("13")	<< T("SN", fp_all_valued)					<< false;
	QTest::newRow("14")	<< T(QString(), SqlValue(3.14))				<< false;
	QTest::newRow("15")	<< T(QString("SN"), SqlValue())				<< false;

}
