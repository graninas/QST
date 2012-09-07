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

#include "ut_sqlvalue.h"

typedef SqlValue T;

Q_DECLARE_METATYPE(SqlValue)
Q_DECLARE_METATYPE(QVariant)
Q_DECLARE_METATYPE(Functor)
Q_DECLARE_METATYPE(FuzzyBraces)

ut_SqlValue::ut_SqlValue()
{
}

/*!
	\class ut_SqlValue
	\brief Модульный тест для класса SqlValue. См. ut_sqlvalue.cpp .
*/


//! Тестирование isValid.
void ut_SqlValue::isValid()
{
	QFETCH(SqlValue, value);
	QFETCH(bool, result);

	QCOMPARE(value.isValid(), result);
}

void ut_SqlValue::isValid_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<bool>("result");

// Конструктор 1
	QTest::newRow("1")	<< T()					<< false;
	QTest::newRow("2")	<< T()					<< false;
	QTest::newRow("3")	<< T(QVariant::Int)		<< true;
	QTest::newRow("4")	<< T(QString())			<< true;
	QTest::newRow("5")	<< T(QDate())			<< true;
	QTest::newRow("6")	<< T(QString("1s"))		<< true;
	QTest::newRow("7")	<< T(true)				<< true;
	QTest::newRow("8")	<< T(false)				<< true;
	QTest::newRow("9")	<< T(10)				<< true;
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< true;
	QTest::newRow("11")	<< T(nt_null)			<< true;

// Конструктор 2
	QTest::newRow("A1")	<< T(T(), fo_none)				<< false;
	QTest::newRow("A2")	<< T(T(QVariant()), fo_none)	<< false;
	QTest::newRow("A3")	<< T(T(QVariant::Int), fo_none)	<< true;
	QTest::newRow("A4")	<< T(T(QString()), fo_none)		<< true;
	QTest::newRow("A5")	<< T(T(QString("1s")), fo_none)	<< true;
	QTest::newRow("A6")	<< T(T(true), fo_none)			<< true;
	QTest::newRow("A7")	<< T(T(10), fo_none)			<< true;
	QTest::newRow("A8")	<< T(T(nt_null), fo_none)		<< true;

// Конструктор 3
	QTest::newRow("B1")	<< T(T())				<< false;
	QTest::newRow("B2")	<< T(T(QVariant()))		<< false;
	QTest::newRow("B3")	<< T(T(QVariant::Int))	<< true;
	QTest::newRow("B4")	<< T(T(QString()))		<< true;
	QTest::newRow("B5")	<< T(T(QString("1s")))	<< true;
	QTest::newRow("B6")	<< T(T(true))			<< true;
	QTest::newRow("B7")	<< T(T(10))				<< true;

	// Конструктор 4
	QTest::newRow("C1")	<< T(nt_null)	<< true;
}

//! Тестирование isNull.

void ut_SqlValue::isNull()
{
	QFETCH(SqlValue, value);
	QFETCH(bool, result);

	QCOMPARE(value.isNull(), result);
}

void ut_SqlValue::isNull_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<bool>("result");

// Конструктор 1
	QTest::newRow("1")	<< T()					<< true;
	QTest::newRow("2")	<< T(QVariant())		<< true;
	QTest::newRow("3")	<< T(QVariant::Int)		<< true;
	QTest::newRow("4")	<< T(QString())			<< true;
	QTest::newRow("5")	<< T(QDate())			<< true;
	QTest::newRow("6")	<< T(QString("1s"))		<< false;
	QTest::newRow("6a")	<< T(QString(""))		<< false;
	QTest::newRow("7")	<< T(true)				<< false;
	QTest::newRow("8")	<< T(false)				<< false;
	QTest::newRow("9")	<< T(10)				<< false;
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< false;

// Конструктор 2
	QTest::newRow("A1")	<< T(T(), fo_none)				<< true;
	QTest::newRow("A2")	<< T(T(QVariant()), fo_none)	<< true;
	QTest::newRow("A3")	<< T(T(QVariant::Int), fo_none)	<< true;
	QTest::newRow("A4")	<< T(T(QString()), fo_none)		<< true;
	QTest::newRow("A5")	<< T(T(QString("1s")), fo_none)	<< false;
	QTest::newRow("A6")	<< T(T(true), fo_none)			<< false;
	QTest::newRow("A7")	<< T(T(10), fo_none)			<< false;

// Конструктор 3
	QTest::newRow("B1")	<< T(T())				<< true;
	QTest::newRow("B2")	<< T(T(QVariant()))		<< true;
	QTest::newRow("B3")	<< T(T(QVariant::Int))	<< true;
	QTest::newRow("B4")	<< T(T(QString()))		<< true;
	QTest::newRow("B5")	<< T(T(QString("1s")))	<< false;
	QTest::newRow("B6")	<< T(T(true))			<< false;
	QTest::newRow("B7")	<< T(T(10))				<< false;

// Конструктор 4
	QTest::newRow("C1")	<< T(nt_null)	<< true;
}

//! Тестирование value.

void ut_SqlValue::value()
{
	QFETCH(SqlValue, value);
	QFETCH(QVariant, result);

	QCOMPARE(value.value(), result);
}

void ut_SqlValue::value_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<QVariant>("result");

// Конструктор 1
	QTest::newRow("1")	<< T()					<< QVariant();
	QTest::newRow("2")	<< T(QVariant())		<< QVariant();
	QTest::newRow("3")	<< T(QVariant::Int)		<< QVariant(QVariant::Int);
	QTest::newRow("4")	<< T(QString())			<< QVariant(QString());
	QTest::newRow("5")	<< T(QDate())			<< QVariant(QDate());
	QTest::newRow("6")	<< T(QString("1s"))		<< QVariant(QString("1s"));
	QTest::newRow("6a")	<< T(QString(""))		<< QVariant(QString(""));
	QTest::newRow("7")	<< T(true)				<< QVariant(true);
	QTest::newRow("8")	<< T(false)				<< QVariant(false);
	QTest::newRow("9")	<< T(10)				<< QVariant(10);
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< QVariant(QDate(2010,12,13));

// Конструктор 2
	QTest::newRow("A1")	<< T(T(), fo_none)				<< QVariant();
	QTest::newRow("A2")	<< T(T(QVariant()), fo_none)	<< QVariant();
	QTest::newRow("A3")	<< T(T(QVariant::Int), fo_none)	<< QVariant(QVariant::Int);
	QTest::newRow("A4")	<< T(T(QString()), fo_none)		<< QVariant(QString());
	QTest::newRow("A5")	<< T(T(QString("1s")), fo_none)	<< QVariant(QString("1s"));
	QTest::newRow("A6")	<< T(T(true), fo_none)			<< QVariant(true);
	QTest::newRow("A7")	<< T(T(10), fo_none)			<< QVariant(10);

// Конструктор 3
	QTest::newRow("B1")	<< T(T())				<< QVariant();
	QTest::newRow("B2")	<< T(T(QVariant()))		<< QVariant();
	QTest::newRow("B3")	<< T(T(QVariant::Int))	<< QVariant(QVariant::Int);
	QTest::newRow("B4")	<< T(T(QString()))		<< QVariant(QString());
	QTest::newRow("B5")	<< T(T(QString("1s")))	<< QVariant(QString("1s"));
	QTest::newRow("B6")	<< T(T(true))			<< QVariant(true);
	QTest::newRow("B7")	<< T(T(10))				<< QVariant(10);

// Конструктор 4
	QTest::newRow("C1")	<< T(nt_null)	<< QVariant(QVariant::Int);
}

//! Тестирование functor.

void ut_SqlValue::functor()
{
	QFETCH(SqlValue, value);
	QFETCH(Functor, result);

	QCOMPARE(value.functor(), result);
}

void ut_SqlValue::functor_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<Functor>("result");

// Конструктор 1
	QTest::newRow("1")	<< T()					<< fo_none;
	QTest::newRow("2")	<< T(QVariant())		<< fo_none;
	QTest::newRow("3")	<< T(QVariant::Int)		<< fo_is_null;
	QTest::newRow("4")	<< T(QString())			<< fo_is_null;
	QTest::newRow("5")	<< T(QDate())			<< fo_is_null;
	QTest::newRow("6")	<< T(QString("1s"))		<< fo_like;
	QTest::newRow("6a")	<< T(QString(""))		<< fo_like;
	QTest::newRow("7")	<< T(true)				<< fo_equal;
	QTest::newRow("8")	<< T(false)				<< fo_equal;
	QTest::newRow("9")	<< T(10)				<< fo_equal;
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< fo_equal;

// Конструктор 1 (2)
	QTest::newRow("Q1")	<< T(QVariant::Int, fo_equal)				<< fo_equal;
	QTest::newRow("Q2")	<< T(QVariant::Int, fo_between)				<< fo_between;
	QTest::newRow("Q3")	<< T(QString(), fo_equal_or_null)			<< fo_equal_or_null;
	QTest::newRow("Q4")	<< T(QString(), fo_greater)					<< fo_greater;
	QTest::newRow("Q5")	<< T(QString("1s"), fo_greater_equal)		<< fo_greater_equal;
	QTest::newRow("Q6")	<< T(QString(""), fo_greater_equal_or_null)	<< fo_greater_equal_or_null;
	QTest::newRow("Q7")	<< T(true, fo_greater_or_null)				<< fo_greater_or_null;
	QTest::newRow("Q8")	<< T(false, fo_is_not_null)					<< fo_is_not_null;
	QTest::newRow("Q9")	<< T(10, fo_is_null)						<< fo_is_null;
	QTest::newRow("Q10")<< T(QDate(2010,12,13), fo_less)			<< fo_less;
	// Дальше перечислять все функторы не имеет смысла.

// Конструктор 2
	QTest::newRow("A1")	<< T(T(), fo_none)				<< fo_none;
	QTest::newRow("A2")	<< T(T(), fo_none)				<< fo_none;
	QTest::newRow("A3")	<< T(T(QVariant::Int), fo_none)	<< fo_none;
	QTest::newRow("A4")	<< T(T(QString()), fo_none)		<< fo_none;
	QTest::newRow("A5")	<< T(T(QString("1s")), fo_none)	<< fo_none;
	QTest::newRow("A6")	<< T(T(true), fo_none)			<< fo_none;
	QTest::newRow("A7")	<< T(T(10), fo_none)			<< fo_none;

// Конструктор 3
	QTest::newRow("B1")	<< T(T())				<< fo_none;
	QTest::newRow("B2")	<< T(T(QVariant()))		<< fo_none;
	QTest::newRow("B3")	<< T(T(QVariant::Int))	<< fo_is_null;
	QTest::newRow("B4")	<< T(T(QString()))		<< fo_is_null;
	QTest::newRow("B5")	<< T(T(QString("1s")))	<< fo_like;
	QTest::newRow("B6")	<< T(T(true))			<< fo_equal;
	QTest::newRow("B7")	<< T(T(10))				<< fo_equal;

// Конструктор 4
	QTest::newRow("C1")	<< T(nt_null)						<< fo_is_null;

// Конструктор 5
	QTest::newRow("X1")	<< T(T(), vbi_not_ignore)			<< fo_none;
	QTest::newRow("X2")	<< T(T(), vbi_ignore)				<< fo_none;
	QTest::newRow("X3")	<< T(T(QVariant()), vbi_not_ignore)	<< fo_none;
	QTest::newRow("X4")	<< T(T(QVariant()), vbi_ignore)		<< fo_none;
}

//! Тестирование braces.

void ut_SqlValue::braces()
{
	QFETCH(SqlValue, value);
	QFETCH(FuzzyBraces, result);

	QCOMPARE(value.braces(), result);
}

void ut_SqlValue::braces_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<FuzzyBraces>("result");

// Конструктор 1
	QTest::newRow("1")	<< T()					<< fb_none;
	QTest::newRow("2")	<< T(QVariant())		<< fb_none;
	QTest::newRow("3")	<< T(QVariant::Int)		<< fb_none;
	QTest::newRow("4")	<< T(QString())			<< fb_none;
	QTest::newRow("5")	<< T(QDate())			<< fb_none;
	QTest::newRow("6")	<< T(QString("1s"))		<< fb_both;
	QTest::newRow("6a")	<< T(QString(""))		<< fb_both;
	QTest::newRow("7")	<< T(true)				<< fb_none;
	QTest::newRow("8")	<< T(false)				<< fb_none;
	QTest::newRow("9")	<< T(10)				<< fb_none;
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< fb_none;
}


//! Тестирование isIgnoreBordering.

void ut_SqlValue::isIgnoreBordering()
{
	QFETCH(SqlValue, value);
	QFETCH(bool, result);

	QCOMPARE(value.isIgnoreBordering(), result);
}

void ut_SqlValue::isIgnoreBordering_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<bool>("result");

// Конструктор 1
	QTest::newRow("1")	<< T()					<< false;
	QTest::newRow("2")	<< T(QVariant())		<< false;
	QTest::newRow("3")	<< T(QVariant::Int)		<< false;
	QTest::newRow("4")	<< T(QString())			<< false;
	QTest::newRow("5")	<< T(QDate())			<< false;
	QTest::newRow("6")	<< T(QString("1s"))		<< false;
	QTest::newRow("6a")	<< T(QString(""))		<< false;
	QTest::newRow("7")	<< T(true)				<< false;
	QTest::newRow("8")	<< T(false)				<< false;
	QTest::newRow("9")	<< T(10)				<< false;
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< false;

// Конструктор 5
	QTest::newRow("X1")	<< T(T(), vbi_not_ignore)			<< false;
	QTest::newRow("X2")	<< T(T(), vbi_ignore)				<< true;
	QTest::newRow("X3")	<< T(T(QVariant()), vbi_not_ignore)	<< false;
	QTest::newRow("X4")	<< T(T(QVariant()), vbi_ignore)		<< true;
}

//! Тестирование toString с параметрами по умолчанию, а именно: vb_none, vfn_fill, vub_not.

void ut_SqlValue::toString()
{
	QFETCH(SqlValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(), result);
}

void ut_SqlValue::toString_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<QString>("result");

// Конструктор 1
	QTest::newRow("1")	<< T()					<< QString();
	QTest::newRow("2")	<< T(QVariant())		<< QString();
	QTest::newRow("3")	<< T(QVariant::Int)		<< QString("NULL");
	QTest::newRow("4")	<< T(QString())			<< QString("NULL");
	QTest::newRow("5")	<< T(QDate())			<< QString("NULL");
	QTest::newRow("6")	<< T(QString("1s"))		<< QString("1s");
	QTest::newRow("6a")	<< T(QString(""))		<< QString("");
	QTest::newRow("7")	<< T(true)				<< QString("true");
	QTest::newRow("8")	<< T(false)				<< QString("false");
	QTest::newRow("9")	<< T(10)				<< QString("10");
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< QString("13.12.2010");

// Конструктор 2
	QTest::newRow("A1")	<< T(T(), fo_none)				<< QString();
	QTest::newRow("A2")	<< T(T(), fo_none)				<< QString();
	QTest::newRow("A3")	<< T(T(QVariant::Int), fo_none)	<< QString("NULL");
	QTest::newRow("A4")	<< T(T(QString()), fo_none)		<< QString("NULL");
	QTest::newRow("A5")	<< T(T(QString("1s")), fo_none)	<< QString("1s");
	QTest::newRow("A6")	<< T(T(true), fo_none)			<< QString("true");
	QTest::newRow("A7")	<< T(T(10), fo_none)			<< QString("10");

// Конструктор 3
	QTest::newRow("B1")	<< T(T())				<< QString();
	QTest::newRow("B2")	<< T(T(QVariant()))		<< QString();
	QTest::newRow("B3")	<< T(T(QVariant::Int))	<< QString("NULL");
	QTest::newRow("B4")	<< T(T(QString()))		<< QString("NULL");
	QTest::newRow("B5")	<< T(T(QString("1s")))	<< QString("1s");
	QTest::newRow("B6")	<< T(T(true))			<< QString("true");
	QTest::newRow("B7")	<< T(T(10))				<< QString("10");

// Конструктор 4
	QTest::newRow("C1")	<< T(nt_null)	<< QString("NULL");
}

//! Тестирование toString1 с параметрами vb_bordered, vfn_fill, vub_not.

void ut_SqlValue::toString1()
{
	QFETCH(SqlValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(vb_bordered, vfn_fill, vub_not), result);
}

void ut_SqlValue::toString1_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< T()					<< QString();
	QTest::newRow("2")	<< T(QVariant())		<< QString();
	QTest::newRow("3")	<< T(QVariant::Int)		<< QString("'NULL'");
	QTest::newRow("4")	<< T(QString())			<< QString("'NULL'");
	QTest::newRow("5")	<< T(QDate())			<< QString("'NULL'");
	QTest::newRow("6")	<< T(QString("1s"))		<< QString("'1s'");
	QTest::newRow("6a")	<< T(QString(""))		<< QString("''");
	QTest::newRow("7")	<< T(true)				<< QString("'true'");
	QTest::newRow("8")	<< T(false)				<< QString("'false'");
	QTest::newRow("9")	<< T(10)				<< QString("'10'");
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< QString("'13.12.2010'");
}

//! Тестирование toString2 с параметрами vb_bordered, vfn_not, vub_not.

void ut_SqlValue::toString2()
{
	QFETCH(SqlValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(vb_none, vfn_not, vub_not), result);
}

void ut_SqlValue::toString2_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< T()					<< QString();
	QTest::newRow("2")	<< T(QVariant())		<< QString();
	QTest::newRow("3")	<< T(QVariant::Int)		<< QString("");
	QTest::newRow("4")	<< T(QString())			<< QString("");
	QTest::newRow("5")	<< T(QDate())			<< QString("");
	QTest::newRow("6")	<< T(QString("1s"))		<< QString("1s");
	QTest::newRow("6a")	<< T(QString(""))		<< QString("");
	QTest::newRow("7")	<< T(true)				<< QString("true");
	QTest::newRow("8")	<< T(false)				<< QString("false");
	QTest::newRow("9")	<< T(10)				<< QString("10");
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< QString("13.12.2010");
}


//! Тестирование toString3 с параметрами vb_bordered, vfn_not, vub_not.

void ut_SqlValue::toString3()
{
	QFETCH(SqlValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(vb_bordered, vfn_not, vub_not), result);
}

void ut_SqlValue::toString3_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< T()					<< QString();
	QTest::newRow("2")	<< T(QVariant())		<< QString();
	QTest::newRow("3")	<< T(QVariant::Int)		<< QString("''");
	QTest::newRow("4")	<< T(QString())			<< QString("''");
	QTest::newRow("5")	<< T(QDate())			<< QString("''");
	QTest::newRow("6")	<< T(QString("1s"))		<< QString("'1s'");
	QTest::newRow("6a")	<< T(QString(""))		<< QString("''");
	QTest::newRow("7")	<< T(true)				<< QString("'true'");
	QTest::newRow("8")	<< T(false)				<< QString("'false'");
	QTest::newRow("9")	<< T(10)				<< QString("'10'");
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< QString("'13.12.2010'");
}


// -------------------------------------------------------------------------- //

//! Тестирование toString4 с параметрами vb_none, vfn_fill, vub_use.

void ut_SqlValue::toString4()
{
	QFETCH(SqlValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(vb_none, vfn_fill, vub_use), result);
}

void ut_SqlValue::toString4_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< T()					<< QString();
	QTest::newRow("2")	<< T(QVariant())		<< QString();
	QTest::newRow("3")	<< T(QVariant::Int)		<< QString("%NULL%");
	QTest::newRow("4")	<< T(QString())			<< QString("%NULL%");
	QTest::newRow("5")	<< T(QDate())			<< QString("%NULL%");
	QTest::newRow("6")	<< T(QString("1s"))		<< QString("%1s%");
	QTest::newRow("6a")	<< T(QString(""))		<< QString("%%");
	QTest::newRow("7")	<< T(true)				<< QString("%true%");
	QTest::newRow("8")	<< T(false)				<< QString("%false%");
	QTest::newRow("9")	<< T(10)				<< QString("%10%");
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< QString("%13.12.2010%");
}

//! Тестирование toString5 с параметрами vb_bordered, vfn_fill, vub_use.

void ut_SqlValue::toString5()
{
	QFETCH(SqlValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(vb_bordered, vfn_fill, vub_use), result);
}

void ut_SqlValue::toString5_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< T()					<< QString();
	QTest::newRow("2")	<< T(QVariant())		<< QString();
	QTest::newRow("3")	<< T(QVariant::Int)		<< QString("'%NULL%'");
	QTest::newRow("4")	<< T(QString())			<< QString("'%NULL%'");
	QTest::newRow("5")	<< T(QDate())			<< QString("'%NULL%'");
	QTest::newRow("6")	<< T(QString("1s"))		<< QString("'%1s%'");
	QTest::newRow("6a")	<< T(QString(""))		<< QString("'%%'");
	QTest::newRow("7")	<< T(true)				<< QString("'%true%'");
	QTest::newRow("8")	<< T(false)				<< QString("'%false%'");
	QTest::newRow("9")	<< T(10)				<< QString("'%10%'");
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< QString("'%13.12.2010%'");
}

//! Тестирование toString6 с параметрами vb_none, vfn_not, vub_use.

void ut_SqlValue::toString6()
{
	QFETCH(SqlValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(vb_none, vfn_not, vub_use), result);
}

void ut_SqlValue::toString6_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< T()					<< QString();
	QTest::newRow("2")	<< T(QVariant())		<< QString();
	QTest::newRow("3")	<< T(QVariant::Int)		<< QString("%%");
	QTest::newRow("4")	<< T(QString())			<< QString("%%");
	QTest::newRow("5")	<< T(QDate())			<< QString("%%");
	QTest::newRow("6")	<< T(QString("1s"))		<< QString("%1s%");
	QTest::newRow("6a")	<< T(QString(""))		<< QString("%%");
	QTest::newRow("7")	<< T(true)				<< QString("%true%");
	QTest::newRow("8")	<< T(false)				<< QString("%false%");
	QTest::newRow("9")	<< T(10)				<< QString("%10%");
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< QString("%13.12.2010%");
}

//! Тестирование toString7 с параметрами vb_bordered, vfn_not, vub_use.

void ut_SqlValue::toString7()
{
	QFETCH(SqlValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(vb_bordered, vfn_not, vub_use), result);
}

void ut_SqlValue::toString7_data()
{
	QTest::addColumn<SqlValue>("value");
	QTest::addColumn<QString>("result");

	QTest::newRow("1")	<< T()					<< QString();
	QTest::newRow("2")	<< T(QVariant())		<< QString();
	QTest::newRow("3")	<< T(QVariant::Int)		<< QString("'%%'");
	QTest::newRow("4")	<< T(QString())			<< QString("'%%'");
	QTest::newRow("5")	<< T(QDate())			<< QString("'%%'");
	QTest::newRow("6")	<< T(QString("1s"))		<< QString("'%1s%'");
	QTest::newRow("6a")	<< T(QString(""))		<< QString("'%%'");
	QTest::newRow("7")	<< T(true)				<< QString("'%true%'");
	QTest::newRow("8")	<< T(false)				<< QString("'%false%'");
	QTest::newRow("9")	<< T(10)				<< QString("'%10%'");
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< QString("'%13.12.2010%'");
}
