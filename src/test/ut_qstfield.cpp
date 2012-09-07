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

#include "ut_qstfield.h"

using namespace Qst;

typedef QstField T;

Q_DECLARE_METATYPE(QstField)
Q_DECLARE_METATYPE(QstValue)
Q_DECLARE_METATYPE(FieldVisibility)
Q_DECLARE_METATYPE(Qt::Orientation)
Q_DECLARE_METATYPE(FieldPurpose)
Q_DECLARE_METATYPE(FieldPurposes)
Q_DECLARE_METATYPE(FieldRole)


namespace QstTest
{


	/*!
		\class ut_QstField
		\brief Модульный тест для класса QstField.
	*/


ut_QstField::ut_QstField()
{
}


void ut_QstField::name()
{
	QFETCH(QstField, value);
	QFETCH(QString, result);

	QCOMPARE(value.name(), result);
}

void ut_QstField::name_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< T()				<< QString();
	QTest::newRow("2")	<< T("SomeName")	<< QString("SomeName");
	QTest::newRow("3")	<< T(QstValue())	<< QString();
}

void ut_QstField::visibility()
{
	QFETCH(QstField, value);
	QFETCH(FieldVisibility, result);

	QCOMPARE(value.visibility(), result);
}

void ut_QstField::visibility_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<FieldVisibility>("result");

	QTest::newRow("1")	<< T()										<< VisibilityNone;
	QTest::newRow("2")	<< T("SomeName")							<< FieldVisible;
	QTest::newRow("3")	<< T("SomeName", FieldVisible)				<< FieldVisible;
	QTest::newRow("4")	<< T("SomeName", FieldInvisible)			<< FieldInvisible;
	QTest::newRow("5")	<< T("SomeName", QstValue())				<< VisibilityNone;
	QTest::newRow("6")	<< T("SomeName", QstValue(), QstValue())	<< VisibilityNone;
	QTest::newRow("7")	<< T(QstValue())							<< VisibilityNone;
	QTest::newRow("8")	<< T("SomeName", PurposeWhere)				<< VisibilityNone;
}

void ut_QstField::columnTitle()
{
	QFETCH(QstField, value);
	QFETCH(QString, result);

	QCOMPARE(value.columnTitle(), result);
}

void ut_QstField::columnTitle_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< T()										<< QString(tr(""));
	QTest::newRow("2")	<< T("SomeName")							<< QString(tr("SomeName"));
	QTest::newRow("3")	<< T("SomeName", FieldVisible, "EngTitle")	<< QString(tr("EngTitle"));
	QTest::newRow("4")	<< T("SomeName", FieldInvisible, "Р СѓСЃРќР°Р·РІ")	<< QString(tr("Р СѓСЃРќР°Р·РІ"));
	QTest::newRow("5")	<< T("SomeName", FieldInvisible, "Р СѓСЃEng")	<< QString(tr("Р СѓСЃEng"));
	QTest::newRow("6")	<< T("SomeName", QstValue())				<< QString(tr(""));
	QTest::newRow("7")	<< T("SomeName", QstValue(), QstValue())	<< QString(tr(""));
	QTest::newRow("8")	<< T(QstValue())							<< QString(tr(""));
	QTest::newRow("9")	<< T("SomeName", PurposeWhere)				<< QString(tr(""));
}

void ut_QstField::orientation()
{
	QFETCH(QstField, value);
	QFETCH(Qt::Orientation, result);

	QCOMPARE(value.orientation(), result);
}

void ut_QstField::orientation_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<Qt::Orientation>("result");

	QTest::newRow("1")	<< T()							<< Qt::Horizontal;
	QTest::newRow("2")	<< T("SomeName")				<< Qt::Horizontal;
	QTest::newRow("3")	<< T("sn",
							 FieldVisible,
							 "et", 0, Qt::Vertical)		<< Qt::Vertical;
	QTest::newRow("6")	<< T("SomeName",
							 QstValue())				<< Qt::Horizontal;
	QTest::newRow("7")	<< T("SomeName",
							 QstValue(),
							 QstValue())				<< Qt::Horizontal;
	QTest::newRow("8")	<< T(QstValue())				<< Qt::Horizontal;
	QTest::newRow("9")	<< T("SomeName", PurposeWhere)	<< Qt::Horizontal;
}

void ut_QstField::purposes()
{
	QFETCH(QstField, value);
	QFETCH(FieldPurposes, result);

	QCOMPARE(value.purposes(), result);
}

void ut_QstField::purposes_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<FieldPurposes>("result");

	QTest::newRow("1")	<< T()											<< (PurposeNone | PurposeNone);
	QTest::newRow("2")	<< T(QString("SN"))								<< (PurposeSelect | PurposeSelect);
	QTest::newRow("3")	<< T("SN")										<< (PurposeSelect | PurposeSelect);
	QTest::newRow("4")	<< T("SN", FieldVisible)						<< (PurposeSelect | PurposeSelect);
	QTest::newRow("5")	<< T("SN", QstValue())							<< (PurposeValued_Mask | PurposeValued_Mask);
	QTest::newRow("6")	<< T("SN", QstValue())							<< (PurposeWhere | PurposeInsert | PurposeUpdate | PurposeParameter);
	QTest::newRow("7")	<< T("SN", QstValue(), PurposeModifying_Mask)	<< (PurposeInsert | PurposeUpdate);
	QTest::newRow("8")	<< T("SN", QstValue(), PurposeInsert)			<< (PurposeInsert | PurposeInsert);
	QTest::newRow("9")	<< T("SN", QstValue(), QstValue())				<< (PurposeWhere | PurposeWhere);
	QTest::newRow("10")	<< T("SN",
							 QstValue(),
							 QstValue(),
							 PurposeValued_Mask)						<< (PurposeWhere | PurposeWhere);
	QTest::newRow("11")	<< T(QstValue())								<< (PurposeParameter | PurposeParameter);
	QTest::newRow("12")	<< T(QstValue(), PurposeValued_Mask)			<< (PurposeParameter | PurposeParameter);
	QTest::newRow("13")	<< T("SN", PurposeValued_Mask)				<< (PurposeWhere | PurposeInsert | PurposeUpdate | PurposeParameter);
	QTest::newRow("14")	<< T("SN", PurposeOrderBy)						<< (PurposeOrderBy | PurposeOrderBy);
	QTest::newRow("15")	<< T("SN", PurposeOrderBy | PurposeGroupBy)		<< (PurposeOrderBy | PurposeGroupBy);
}

void ut_QstField::role()
{
	QFETCH(QstField, value);
	QFETCH(FieldRole, result);

	QCOMPARE(value.role(), result);
}

void ut_QstField::role_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<FieldRole>("result");

	QTest::newRow("1")	<< T()										<< (RoleNone);
	QTest::newRow("2")	<< T(QString("SN"))							<< (RoleNone);
	QTest::newRow("3")	<< T("SN")									<< (RoleNone);
	QTest::newRow("4")	<< T("SN", FieldVisible)					<< (RoleNone);
	QTest::newRow("5")	<< T(RolePrimaryKey, "SN", FieldVisible)	<< (RolePrimaryKey);
	QTest::newRow("6")	<< T(RoleParentKey, "SN", FieldVisible)		<< (RoleParentKey);
	QTest::newRow("7")	<< T("SN", QstValue())						<< (RoleNone);
	QTest::newRow("8")	<< T(QstValue())							<< (RoleNone);
	QTest::newRow("9")	<< T("SN", PurposeValued_Mask)			<< (RoleNone);
}

void ut_QstField::value()
{
	QFETCH(QstField, value);
	QFETCH(QstValue, result);

	QCOMPARE(value.value(), result);
}

void ut_QstField::value_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<QstValue>("result");

	QTest::newRow("1")	<< T()										<< QstValue();
	QTest::newRow("2")	<< T(QString("SN"))							<< QstValue();
	QTest::newRow("3")	<< T("SN")									<< QstValue();
	QTest::newRow("4")	<< T("SN", FieldVisible)					<< QstValue();
	QTest::newRow("5")	<< T("SN", QstValue())						<< QstValue();
	QTest::newRow("6")	<< T("SN", QstValue(QVariant()))			<< QstValue(QVariant());
	QTest::newRow("7")	<< T("SN", QstValue(QVariant("dfd")))		<< QstValue(QVariant("dfd"));
	QTest::newRow("8")	<< T("SN", QstValue("1"), QstValue("2"))	<< QstValue("1");
	QTest::newRow("9")	<< T("SN", QstValue(), QstValue("2"))		<< QstValue(QVariant());
	QTest::newRow("10")	<< T(QstValue())							<< QstValue();
	QTest::newRow("11")	<< T(QstValue(QDate(10,1,22)))				<< QstValue(QDate(10,1,22));
	QTest::newRow("12")	<< T("SN", PurposeValued_Mask)			<< QstValue();
}

void ut_QstField::value1()
{
	QFETCH(QstField, value);
	QFETCH(QstValue, result);

	QCOMPARE(value.value(OrderFirst), result);
}

void ut_QstField::value1_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<QstValue>("result");

	QTest::newRow("1")	<< T()										<< QstValue();
	QTest::newRow("2")	<< T(QString("SN"))							<< QstValue();
	QTest::newRow("3")	<< T("SN")									<< QstValue();
	QTest::newRow("4")	<< T("SN", FieldVisible)					<< QstValue();
	QTest::newRow("5")	<< T("SN", QstValue())						<< QstValue();
	QTest::newRow("6")	<< T("SN", QstValue(QVariant()))			<< QstValue();
	QTest::newRow("7")	<< T("SN", QstValue(QVariant("dfd")))		<< QstValue("dfd");
	QTest::newRow("8")	<< T("SN", QstValue("1"), QstValue("2"))	<< QstValue("1");
	QTest::newRow("9")	<< T("SN", QstValue(), QstValue("2"))		<< QstValue();
	QTest::newRow("10")	<< T(QstValue())							<< QstValue();
	QTest::newRow("11")	<< T(QstValue(QDate(10,1,22)))				<< QstValue(QDate(10,1,22));
	QTest::newRow("12")	<< T("SN", PurposeValued_Mask)			<< QstValue();
}


void ut_QstField::value2()
{
	QFETCH(QstField, value);
	QFETCH(QstValue, result);

	QCOMPARE(value.value(OrderSecond), result);
}

void ut_QstField::value2_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<QstValue>("result");

	QTest::newRow("1")	<< T()										<< QstValue();
	QTest::newRow("2")	<< T(QString("SN"))							<< QstValue();
	QTest::newRow("3")	<< T("SN")									<< QstValue();
	QTest::newRow("4")	<< T("SN", FieldVisible)					<< QstValue();
	QTest::newRow("5")	<< T("SN", QstValue())						<< QstValue();
	QTest::newRow("6")	<< T("SN", QstValue(QVariant()))			<< QstValue();
	QTest::newRow("7")	<< T("SN", QstValue(QVariant("dfd")))		<< QstValue();
	QTest::newRow("8")	<< T("SN", QstValue("1"), QstValue("2"))	<< QstValue("2");
	QTest::newRow("9")	<< T("SN", QstValue(), QstValue("2"))		<< QstValue("2");
	QTest::newRow("10")	<< T(QstValue())							<< QstValue();
	QTest::newRow("11")	<< T(QstValue(QDate(10,1,22)))				<< QstValue();
	QTest::newRow("12")	<< T("SN", PurposeValued_Mask)			<< QstValue();
}

void ut_QstField::hasValue()
{
	QFETCH(QstField, value);
	QFETCH(bool, result);

	QCOMPARE(value.hasValue(), result);
}

void ut_QstField::hasValue_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< false;
	QTest::newRow("2")	<< T(QString("SN"))							<< false;
	QTest::newRow("3")	<< T("SN")									<< false;
	QTest::newRow("4")	<< T("SN", FieldVisible)					<< false;
	QTest::newRow("5")	<< T("SN", QstValue())						<< true;
	QTest::newRow("6")	<< T("SN", QstValue(QVariant()))			<< true;
	QTest::newRow("7")	<< T("SN", QstValue(QVariant("dfd")))		<< true;
	QTest::newRow("8")	<< T("SN", QstValue("1"), QstValue("2"))	<< true;
	QTest::newRow("9")	<< T("SN", QstValue(), QstValue("2"))		<< true;
	QTest::newRow("10")	<< T(QstValue())							<< true;
	QTest::newRow("11")	<< T(QstValue(QDate(10,1,22)))				<< true;
	QTest::newRow("12")	<< T("SN", PurposeValued_Mask)			<< false;
}

void ut_QstField::hasValue1()
{
	QFETCH(QstField, value);
	QFETCH(bool, result);

	QCOMPARE(value.hasValue(OrderFirst), result);
}

void ut_QstField::hasValue1_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< false;
	QTest::newRow("2")	<< T(QString("SN"))							<< false;
	QTest::newRow("3")	<< T("SN")									<< false;
	QTest::newRow("4")	<< T("SN", FieldVisible)					<< false;
	QTest::newRow("5")	<< T("SN", QstValue())						<< true;
	QTest::newRow("6")	<< T("SN", QstValue(QVariant()))			<< true;
	QTest::newRow("7")	<< T("SN", QstValue(QVariant("dfd")))		<< true;
	QTest::newRow("8")	<< T("SN", QstValue("1"), QstValue("2"))	<< true;
	QTest::newRow("9")	<< T("SN", QstValue(), QstValue("2"))		<< true;
	QTest::newRow("10")	<< T(QstValue())							<< true;
	QTest::newRow("11")	<< T(QstValue(QDate(10,1,22)))				<< true;
	QTest::newRow("12")	<< T("SN", PurposeValued_Mask)			<< false;
}

void ut_QstField::hasValue2()
{
	QFETCH(QstField, value);
	QFETCH(bool, result);

	QCOMPARE(value.hasValue(OrderSecond), result);
}

void ut_QstField::hasValue2_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< false;
	QTest::newRow("2")	<< T(QString("SN"))							<< false;
	QTest::newRow("3")	<< T("SN")									<< false;
	QTest::newRow("4")	<< T("SN", FieldVisible)					<< false;
	QTest::newRow("5")	<< T("SN", QstValue())						<< false;
	QTest::newRow("6")	<< T("SN", QstValue(QVariant()))			<< false;
	QTest::newRow("7")	<< T("SN", QstValue(QVariant("dfd")))		<< false;
	QTest::newRow("8")	<< T("SN", QstValue("1"), QstValue("2"))	<< true;
	QTest::newRow("9")	<< T("SN", QstValue(), QstValue("2"))		<< true;
	QTest::newRow("10")	<< T(QstValue())							<< false;
	QTest::newRow("11")	<< T(QstValue(QDate(10,1,22)))				<< false;
	QTest::newRow("12")	<< T("SN", PurposeValued_Mask)			<< false;
}

void ut_QstField::isBinaryFilter()
{
	QFETCH(QstField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isBinaryFilter(), result);
}

void ut_QstField::isBinaryFilter_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< false;
	QTest::newRow("2")	<< T(QString("SN"))							<< false;
	QTest::newRow("3")	<< T("SN")									<< false;
	QTest::newRow("4")	<< T("SN", FieldVisible)					<< false;
	QTest::newRow("5")	<< T("SN", QstValue())						<< false;
	QTest::newRow("6")	<< T("SN", QstValue(QVariant()))			<< false;
	QTest::newRow("7")	<< T("SN", QstValue(QVariant("dfd")))		<< false;
	QTest::newRow("8")	<< T("SN", QstValue("1"), QstValue("2"))	<< true;
	QTest::newRow("9")	<< T("SN", QstValue(), QstValue("2"))		<< true;
	QTest::newRow("10")	<< T(QstValue())							<< false;
	QTest::newRow("11")	<< T(QstValue(QDate(10,1,22)))				<< false;
	QTest::newRow("12")	<< T("SN", PurposeValued_Mask)			<< false;
}

void ut_QstField::isVisible()
{
	QFETCH(QstField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isVisible(), result);
}

void ut_QstField::isVisible_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< false;
	QTest::newRow("2")	<< T("SomeName")							<< true;
	QTest::newRow("3")	<< T("SomeName", FieldVisible)				<< true;
	QTest::newRow("4")	<< T("SomeName", FieldInvisible)			<< false;
	QTest::newRow("5")	<< T("SomeName", QstValue())				<< false;
	QTest::newRow("6")	<< T("SomeName", QstValue(), QstValue())	<< false;
	QTest::newRow("7")	<< T(QstValue())							<< false;
	QTest::newRow("8")	<< T("SomeName", PurposeWhere)				<< false;
}


void ut_QstField::isInvisible()
{
	QFETCH(QstField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isInvisible(), result);
}

void ut_QstField::isInvisible_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< false;
	QTest::newRow("2")	<< T("SomeName")							<< false;
	QTest::newRow("3")	<< T("SomeName", FieldVisible)				<< false;
	QTest::newRow("4")	<< T("SomeName", FieldInvisible)			<< true;
	QTest::newRow("5")	<< T("SomeName", QstValue())				<< false;
	QTest::newRow("6")	<< T("SomeName", QstValue(), QstValue())	<< false;
	QTest::newRow("7")	<< T(QstValue())							<< false;
	QTest::newRow("8")	<< T("SomeName", PurposeWhere)				<< false;
}

void ut_QstField::isNoneVisibility()
{
	QFETCH(QstField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isNoneVisibility(), result);
}

void ut_QstField::isNoneVisibility_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< true;
	QTest::newRow("2")	<< T("SomeName")							<< false;
	QTest::newRow("3")	<< T("SomeName", FieldVisible)				<< false;
	QTest::newRow("4")	<< T("SomeName", FieldInvisible)			<< false;
	QTest::newRow("5")	<< T("SomeName", QstValue())				<< true;
	QTest::newRow("6")	<< T("SomeName", QstValue(), QstValue())	<< true;
	QTest::newRow("7")	<< T(QstValue())							<< true;
	QTest::newRow("8")	<< T("SomeName", PurposeWhere)				<< true;
}

void ut_QstField::isService()
{
	QFETCH(QstField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isService(), result);
}

void ut_QstField::isService_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< false;
	QTest::newRow("2")	<< T(QString("SN"))							<< false;
	QTest::newRow("3")	<< T("SN")									<< false;
	QTest::newRow("4")	<< T("SN", FieldVisible)					<< false;
	QTest::newRow("5")	<< T(RoleParentKey, "SN", FieldVisible)		<< true;
	QTest::newRow("6")	<< T(RoleParentKey, "SN", FieldVisible)		<< true;
	QTest::newRow("7")	<< T("SN", QstValue())						<< false;
	QTest::newRow("8")	<< T("SN", QstValue(), QstValue("2"))		<< false;
	QTest::newRow("9")	<< T(QstValue())							<< false;
	QTest::newRow("10")	<< T("SN", PurposeValued_Mask)			<< false;
}

void ut_QstField::isValuesValid()
{
	QFETCH(QstField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isValuesValid(), result);
}

void ut_QstField::isValuesValid_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< false;
	QTest::newRow("2")	<< T(QString("SN"))							<< false;
	QTest::newRow("3")	<< T("SN")									<< false;
	QTest::newRow("4")	<< T("SN", FieldVisible)					<< false;
	QTest::newRow("5")	<< T("SN", QstValue())						<< false;
	QTest::newRow("6")	<< T("SN", QstValue(QVariant()))			<< false;
	QTest::newRow("7")	<< T("SN", QstValue(QVariant("dfd")))		<< true;
	QTest::newRow("8")	<< T("SN", QstValue("1"), QstValue("2"))	<< true;
	QTest::newRow("9")	<< T("SN", QstValue(), QstValue("2"))		<< false;
	QTest::newRow("10")	<< T(QstValue())							<< false;
	QTest::newRow("11")	<< T(QstValue(QDate(10,1,22)))				<< true;
	QTest::newRow("12")	<< T("SN", PurposeValued_Mask)			<< false;
}

void ut_QstField::isValid()
{
	QFETCH(QstField, value);
	QFETCH(bool, result);

	QCOMPARE(value.isValid(), result);
}

void ut_QstField::isValid_data()
{
	QTest::addColumn<QstField>("value");
	QTest::addColumn<bool>("result");

	QTest::newRow("1")	<< T()										<< false;
	QTest::newRow("2")	<< T(QString("SN"))							<< true;
	QTest::newRow("3")	<< T("SN")									<< true;
	QTest::newRow("4")	<< T("SN", FieldVisible)					<< true;
	QTest::newRow("5")	<< T("SN", QstValue())						<< false;
	QTest::newRow("6")	<< T("SN", QstValue(Null))					<< true;
	QTest::newRow("7")	<< T("SN", QstValue(QVariant()))			<< false;
	QTest::newRow("8")	<< T("SN", QstValue(QVariant("dfd")))		<< true;
	QTest::newRow("9")	<< T("SN", QstValue("1"), QstValue("2"))	<< true;
	QTest::newRow("10")	<< T("SN", QstValue(), QstValue("2"))		<< false;
	QTest::newRow("11")	<< T(QstValue())							<< false;
	QTest::newRow("12")	<< T(QstValue(QDate(10,1,22)))				<< true;
	QTest::newRow("13")	<< T("SN", PurposeValued_Mask)			<< false;
	QTest::newRow("14")	<< T(QString(), QstValue(3.14))				<< false;
	QTest::newRow("15")	<< T(QString("SN"), QstValue())				<< false;

}

} // End of namespace QstTest
