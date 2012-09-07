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

#include "ut_qstvalue.h"

#include <QDebug>

using namespace Qst;

typedef QstValue T;

Q_DECLARE_METATYPE(QstValue)
Q_DECLARE_METATYPE(QVariant)
Q_DECLARE_METATYPE(CompareFunctor)
Q_DECLARE_METATYPE(FuzzyBraces)

namespace QstTest
{


ut_QstValue::ut_QstValue()
{
}

/*!
	\class ut_QstValue
	\brief Модульный тест для класса QstValue.
*/


//! Тестирование isValid.
void ut_QstValue::isValid()
{
	QFETCH(QstValue, value);
	QFETCH(bool, result);

	QCOMPARE(value.isValid(), result);
}

void ut_QstValue::isValid_data()
{
	QTest::addColumn<QstValue>("value");
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
	QTest::newRow("11")	<< T(Null)			<< true;

// Конструктор 2
	QTest::newRow("A1")	<< T(T(), FunctorNone)				<< false;
	QTest::newRow("A2")	<< T(T(QVariant()), FunctorNone)	<< false;
	QTest::newRow("A3")	<< T(T(QVariant::Int), FunctorNone)	<< true;
	QTest::newRow("A4")	<< T(T(QString()), FunctorNone)		<< true;
	QTest::newRow("A5")	<< T(T(QString("1s")), FunctorNone)	<< true;
	QTest::newRow("A6")	<< T(T(true), FunctorNone)			<< true;
	QTest::newRow("A7")	<< T(T(10), FunctorNone)			<< true;
	QTest::newRow("A8")	<< T(T(Null), FunctorNone)		<< true;

// Конструктор 3
	QTest::newRow("B1")	<< T(T())				<< false;
	QTest::newRow("B2")	<< T(T(QVariant()))		<< false;
	QTest::newRow("B3")	<< T(T(QVariant::Int))	<< true;
	QTest::newRow("B4")	<< T(T(QString()))		<< true;
	QTest::newRow("B5")	<< T(T(QString("1s")))	<< true;
	QTest::newRow("B6")	<< T(T(true))			<< true;
	QTest::newRow("B7")	<< T(T(10))				<< true;

	// Конструктор 4
	QTest::newRow("C1")	<< T(Null)	<< true;
}

//! Тестирование isNull.

void ut_QstValue::isNull()
{
	QFETCH(QstValue, value);
	QFETCH(bool, result);

	QCOMPARE(value.isNull(), result);
}

void ut_QstValue::isNull_data()
{
	QTest::addColumn<QstValue>("value");
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
	QTest::newRow("A1")	<< T(T(), FunctorNone)				<< true;
	QTest::newRow("A2")	<< T(T(QVariant()), FunctorNone)	<< true;
	QTest::newRow("A3")	<< T(T(QVariant::Int), FunctorNone)	<< true;
	QTest::newRow("A4")	<< T(T(QString()), FunctorNone)		<< true;
	QTest::newRow("A5")	<< T(T(QString("1s")), FunctorNone)	<< false;
	QTest::newRow("A6")	<< T(T(true), FunctorNone)			<< false;
	QTest::newRow("A7")	<< T(T(10), FunctorNone)			<< false;

// Конструктор 3
	QTest::newRow("B1")	<< T(T())				<< true;
	QTest::newRow("B2")	<< T(T(QVariant()))		<< true;
	QTest::newRow("B3")	<< T(T(QVariant::Int))	<< true;
	QTest::newRow("B4")	<< T(T(QString()))		<< true;
	QTest::newRow("B5")	<< T(T(QString("1s")))	<< false;
	QTest::newRow("B6")	<< T(T(true))			<< false;
	QTest::newRow("B7")	<< T(T(10))				<< false;

// Конструктор 4
	QTest::newRow("C1")	<< T(Null)	<< true;
}

//! Тестирование value.

void ut_QstValue::value()
{
	QFETCH(QstValue, value);
	QFETCH(QVariant, result);

	QCOMPARE(value.value(), result);
}

void ut_QstValue::value_data()
{
	QTest::addColumn<QstValue>("value");
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
	QTest::newRow("A1")	<< T(T(), FunctorNone)				<< QVariant();
	QTest::newRow("A2")	<< T(T(QVariant()), FunctorNone)	<< QVariant();
	QTest::newRow("A3")	<< T(T(QVariant::Int), FunctorNone)	<< QVariant(QVariant::Int);
	QTest::newRow("A4")	<< T(T(QString()), FunctorNone)		<< QVariant(QString());
	QTest::newRow("A5")	<< T(T(QString("1s")), FunctorNone)	<< QVariant(QString("1s"));
	QTest::newRow("A6")	<< T(T(true), FunctorNone)			<< QVariant(true);
	QTest::newRow("A7")	<< T(T(10), FunctorNone)			<< QVariant(10);

// Конструктор 3
	QTest::newRow("B1")	<< T(T())				<< QVariant();
	QTest::newRow("B2")	<< T(T(QVariant()))		<< QVariant();
	QTest::newRow("B3")	<< T(T(QVariant::Int))	<< QVariant(QVariant::Int);
	QTest::newRow("B4")	<< T(T(QString()))		<< QVariant(QString());
	QTest::newRow("B5")	<< T(T(QString("1s")))	<< QVariant(QString("1s"));
	QTest::newRow("B6")	<< T(T(true))			<< QVariant(true);
	QTest::newRow("B7")	<< T(T(10))				<< QVariant(10);

// Конструктор 4
	QTest::newRow("C1")	<< T(Null)	<< QVariant(QVariant::Int);
}

//! Тестирование functor.

void ut_QstValue::functor()
{
	QFETCH(QstValue, value);
	QFETCH(CompareFunctor, result);

	QCOMPARE(value.functor(), result);
}

void ut_QstValue::functor_data()
{
	QTest::addColumn<QstValue>("value");
	QTest::addColumn<CompareFunctor>("result");

// Конструктор 1
	QTest::newRow("1")	<< T()					<< FunctorNone;
	QTest::newRow("2")	<< T(QVariant())		<< FunctorNone;
	QTest::newRow("3")	<< T(QVariant::Int)		<< FunctorIsNull;
	QTest::newRow("4")	<< T(QString())			<< FunctorIsNull;
	QTest::newRow("5")	<< T(QDate())			<< FunctorIsNull;
	QTest::newRow("6")	<< T(QString("1s"))		<< FunctorLike;
	QTest::newRow("6a")	<< T(QString(""))		<< FunctorLike;
	QTest::newRow("7")	<< T(true)				<< FunctorEqual;
	QTest::newRow("8")	<< T(false)				<< FunctorEqual;
	QTest::newRow("9")	<< T(10)				<< FunctorEqual;
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< FunctorEqual;

// Конструктор 1 (2)
	QTest::newRow("Q1")	<< T(QVariant::Int, FunctorEqual)				<< FunctorEqual;
	QTest::newRow("Q2")	<< T(QVariant::Int, FunctorBetween)				<< FunctorBetween;
	QTest::newRow("Q3")	<< T(QString(), FunctorEqualOrNull)			<< FunctorEqualOrNull;
	QTest::newRow("Q4")	<< T(QString(), FunctorGreater)					<< FunctorGreater;
	QTest::newRow("Q5")	<< T(QString("1s"), FunctorGreaterEqual)		<< FunctorGreaterEqual;
	QTest::newRow("Q6")	<< T(QString(""), FunctorGreaterEqualOrNull)	<< FunctorGreaterEqualOrNull;
	QTest::newRow("Q7")	<< T(true, FunctorGreaterOrNull)				<< FunctorGreaterOrNull;
	QTest::newRow("Q8")	<< T(false, FunctorIsNotNull)					<< FunctorIsNotNull;
	QTest::newRow("Q9")	<< T(10, FunctorIsNull)						<< FunctorIsNull;
	QTest::newRow("Q10")<< T(QDate(2010,12,13), FunctorLess)			<< FunctorLess;
	// Дальше перечислять все функторы не имеет смысла.

// Конструктор 2
	QTest::newRow("A1")	<< T(T(), FunctorNone)				<< FunctorNone;
	QTest::newRow("A2")	<< T(T(), FunctorNone)				<< FunctorNone;
	QTest::newRow("A3")	<< T(T(QVariant::Int), FunctorNone)	<< FunctorNone;
	QTest::newRow("A4")	<< T(T(QString()), FunctorNone)		<< FunctorNone;
	QTest::newRow("A5")	<< T(T(QString("1s")), FunctorNone)	<< FunctorNone;
	QTest::newRow("A6")	<< T(T(true), FunctorNone)			<< FunctorNone;
	QTest::newRow("A7")	<< T(T(10), FunctorNone)			<< FunctorNone;

// Конструктор 3
	QTest::newRow("B1")	<< T(T())				<< FunctorNone;
	QTest::newRow("B2")	<< T(T(QVariant()))		<< FunctorNone;
	QTest::newRow("B3")	<< T(T(QVariant::Int))	<< FunctorIsNull;
	QTest::newRow("B4")	<< T(T(QString()))		<< FunctorIsNull;
	QTest::newRow("B5")	<< T(T(QString("1s")))	<< FunctorLike;
	QTest::newRow("B6")	<< T(T(true))			<< FunctorEqual;
	QTest::newRow("B7")	<< T(T(10))				<< FunctorEqual;

// Конструктор 4
	QTest::newRow("C1")	<< T(Null)				<< FunctorIsNull;
}

//! Тестирование braces.

void ut_QstValue::braces()
{
	QFETCH(QstValue, value);
	QFETCH(FuzzyBraces, result);

	QCOMPARE(value.braces(), result);
}

void ut_QstValue::braces_data()
{
	QTest::addColumn<QstValue>("value");
	QTest::addColumn<FuzzyBraces>("result");

// Конструктор 1
	QTest::newRow("1")	<< T()					<< BracesNone;
	QTest::newRow("2")	<< T(QVariant())		<< BracesNone;
	QTest::newRow("3")	<< T(QVariant::Int)		<< BracesNone;
	QTest::newRow("4")	<< T(QString())			<< BracesNone;
	QTest::newRow("5")	<< T(QDate())			<< BracesNone;
	QTest::newRow("6")	<< T(QString("1s"))		<< BracesBoth;
	QTest::newRow("6a")	<< T(QString(""))		<< BracesBoth;
	QTest::newRow("7")	<< T(true)				<< BracesNone;
	QTest::newRow("8")	<< T(false)				<< BracesNone;
	QTest::newRow("9")	<< T(10)				<< BracesNone;
	QTest::newRow("10")	<< T(QDate(2010,12,13))	<< BracesNone;
}


/** Тестирование toString с параметрами по умолчанию,
 а именно: ValueNotBordered, BracesNotUse, NullSubstitute,
 если QST_VALUE_NULL_SUBSTITUTE_BY_DEFAULT определен,

 и			ValueNotBordered, BracesNotUse, NullNotSubstitute,
 если не определен.
  */

void ut_QstValue::toString()
{
	QFETCH(QstValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(), result);
}

void ut_QstValue::toString_data()
{
	QTest::addColumn<QstValue>("value");
	QTest::addColumn<QString>("result");

#ifdef QST_VALUE_NULL_SUBSTITUTE_BY_DEFAULT

	qDebug() << "Null substituting by default is ON.";

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
	QTest::newRow("A1")	<< T(T(), FunctorNone)				<< QString();
	QTest::newRow("A2")	<< T(T(), FunctorNone)				<< QString();
	QTest::newRow("A3")	<< T(T(QVariant::Int), FunctorNone)	<< QString("NULL");
	QTest::newRow("A4")	<< T(T(QString()), FunctorNone)		<< QString("NULL");
	QTest::newRow("A5")	<< T(T(QString("1s")), FunctorNone)	<< QString("1s");
	QTest::newRow("A6")	<< T(T(true), FunctorNone)			<< QString("true");
	QTest::newRow("A7")	<< T(T(10), FunctorNone)			<< QString("10");

// Конструктор 3
	QTest::newRow("B1")	<< T(T())				<< QString();
	QTest::newRow("B2")	<< T(T(QVariant()))		<< QString();
	QTest::newRow("B3")	<< T(T(QVariant::Int))	<< QString("NULL");
	QTest::newRow("B4")	<< T(T(QString()))		<< QString("NULL");
	QTest::newRow("B5")	<< T(T(QString("1s")))	<< QString("1s");
	QTest::newRow("B6")	<< T(T(true))			<< QString("true");
	QTest::newRow("B7")	<< T(T(10))				<< QString("10");

// Конструктор 4
	QTest::newRow("C1")	<< T(Null)	<< QString("NULL");

#else

	qDebug() << "Null substituting by default is OFF.";

	// Конструктор 1
		QTest::newRow("1")	<< T()					<< QString();
		QTest::newRow("2")	<< T(QVariant())		<< QString();
		QTest::newRow("3")	<< T(QVariant::Int)		<< QString();
		QTest::newRow("4")	<< T(QString())			<< QString();
		QTest::newRow("5")	<< T(QDate())			<< QString();
		QTest::newRow("6")	<< T(QString("1s"))		<< QString("1s");
		QTest::newRow("6a")	<< T(QString(""))		<< QString("");
		QTest::newRow("7")	<< T(true)				<< QString("true");
		QTest::newRow("8")	<< T(false)				<< QString("false");
		QTest::newRow("9")	<< T(10)				<< QString("10");
		QTest::newRow("10")	<< T(QDate(2010,12,13))	<< QString("13.12.2010");

	// Конструктор 2
		QTest::newRow("A1")	<< T(T(), FunctorNone)				<< QString();
		QTest::newRow("A2")	<< T(T(), FunctorNone)				<< QString();
		QTest::newRow("A3")	<< T(T(QVariant::Int), FunctorNone)	<< QString();
		QTest::newRow("A4")	<< T(T(QString()), FunctorNone)		<< QString();
		QTest::newRow("A5")	<< T(T(QString("1s")), FunctorNone)	<< QString("1s");
		QTest::newRow("A6")	<< T(T(true), FunctorNone)			<< QString("true");
		QTest::newRow("A7")	<< T(T(10), FunctorNone)			<< QString("10");

	// Конструктор 3
		QTest::newRow("B1")	<< T(T())				<< QString();
		QTest::newRow("B2")	<< T(T(QVariant()))		<< QString();
		QTest::newRow("B3")	<< T(T(QVariant::Int))	<< QString();
		QTest::newRow("B4")	<< T(T(QString()))		<< QString();
		QTest::newRow("B5")	<< T(T(QString("1s")))	<< QString("1s");
		QTest::newRow("B6")	<< T(T(true))			<< QString("true");
		QTest::newRow("B7")	<< T(T(10))				<< QString("10");

	// Конструктор 4
		QTest::newRow("C1")	<< T(Null)	<< QString();

#endif
}

//! Тестирование toString1 с параметрами ValueBordered, NullSubstitute, BracesNotUse.

void ut_QstValue::toString1()
{
	QFETCH(QstValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(ValueBordered, BracesNotUse, NullSubstitute), result);
}

void ut_QstValue::toString1_data()
{
	QTest::addColumn<QstValue>("value");
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

//! Тестирование toString2 с параметрами ValueBordered, NullNotSubstitute, BracesNotUse.

void ut_QstValue::toString2()
{
	QFETCH(QstValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(ValueNotBordered, BracesNotUse, NullNotSubstitute), result);
}

void ut_QstValue::toString2_data()
{
	QTest::addColumn<QstValue>("value");
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


//! Тестирование toString3 с параметрами ValueBordered, BracesNotUse, NullNotSubstitute.

void ut_QstValue::toString3()
{
	QFETCH(QstValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(ValueBordered, BracesNotUse, NullNotSubstitute), result);
}

void ut_QstValue::toString3_data()
{
	QTest::addColumn<QstValue>("value");
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

//! Тестирование toString4 с параметрами ValueNotBordered, BracesUse, NullSubstitute.

void ut_QstValue::toString4()
{
	QFETCH(QstValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(ValueNotBordered, BracesUse, NullSubstitute), result);
}

void ut_QstValue::toString4_data()
{
	QTest::addColumn<QstValue>("value");
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

//! Тестирование toString5 с параметрами ValueBordered, BracesUse, NullSubstitute.

void ut_QstValue::toString5()
{
	QFETCH(QstValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(ValueBordered, BracesUse, NullSubstitute), result);
}

void ut_QstValue::toString5_data()
{
	QTest::addColumn<QstValue>("value");
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

//! Тестирование toString6 с параметрами ValueNotBordered, BracesUse, NullNotSubstitute.

void ut_QstValue::toString6()
{
	QFETCH(QstValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(ValueNotBordered, BracesUse, NullNotSubstitute), result);
}

void ut_QstValue::toString6_data()
{
	QTest::addColumn<QstValue>("value");
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

//! Тестирование toString7 с параметрами ValueBordered, BracesUse, NullNotSubstitute.

void ut_QstValue::toString7()
{
	QFETCH(QstValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(ValueBordered, BracesUse, NullNotSubstitute), result);
}

void ut_QstValue::toString7_data()
{
	QTest::addColumn<QstValue>("value");
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

//! Тестирование toString8 с параметрами ValueNotBordered, BracesNotUse, NullSubstitute.

void ut_QstValue::toString8()
{
	QFETCH(QstValue, value);
	QFETCH(QString, result);

	QCOMPARE(value.toString(ValueNotBordered, BracesNotUse, NullSubstitute), result);
}

void ut_QstValue::toString8_data()
{
	QTest::addColumn<QstValue>("value");
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
		QTest::newRow("A1")	<< T(T(), FunctorNone)				<< QString();
		QTest::newRow("A2")	<< T(T(), FunctorNone)				<< QString();
		QTest::newRow("A3")	<< T(T(QVariant::Int), FunctorNone)	<< QString("NULL");
		QTest::newRow("A4")	<< T(T(QString()), FunctorNone)		<< QString("NULL");
		QTest::newRow("A5")	<< T(T(QString("1s")), FunctorNone)	<< QString("1s");
		QTest::newRow("A6")	<< T(T(true), FunctorNone)			<< QString("true");
		QTest::newRow("A7")	<< T(T(10), FunctorNone)			<< QString("10");

	// Конструктор 3
		QTest::newRow("B1")	<< T(T())				<< QString();
		QTest::newRow("B2")	<< T(T(QVariant()))		<< QString();
		QTest::newRow("B3")	<< T(T(QVariant::Int))	<< QString("NULL");
		QTest::newRow("B4")	<< T(T(QString()))		<< QString("NULL");
		QTest::newRow("B5")	<< T(T(QString("1s")))	<< QString("1s");
		QTest::newRow("B6")	<< T(T(true))			<< QString("true");
		QTest::newRow("B7")	<< T(T(10))				<< QString("10");

	// Конструктор 4
		QTest::newRow("C1")	<< T(Null)	<< QString("NULL");}


} // End of namespace QstTest
