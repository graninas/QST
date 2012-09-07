/****************************************************************************
** QST 0.4.2a beta
** Copyright (C) 2010 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Qst module of the QsT SQL Tools.
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

#include "qstvalue.h"

#include <QDate>
#include "qstgenconstants.h"

namespace Qst
{

	/*!
		\class QstValue
		\brief
		Хранит QVariant-значение, а так же функтор сравнения и
		"нечеткие скобки" (для строковых типов данных).
		Используется в DFD-описателях в качестве правой части условия:
		<левая часть> <правая часть:[Функтор, значение]>.

		\inmodule Qst
	*/

/*! Основной конструктор и конструктор по умолчанию.

	Если установлен флаг QST_VALUE_SET_DEFAULT_FUNCTORS, функтор и
	нечеткие скобки будут выставлены в зависимости от типа QVariant.
	В этом случае второй и третий параметры игнорируются.

	По умолчанию создает инвалидный QstValue. */
QstValue::QstValue(const QVariant &value,
				   const CompareFunctor &functor,
				   const FuzzyBraces &braces)
		  :
		  _value(value),
		  _functor(functor),
		  _braces(braces)
{
#ifdef QST_VALUE_SET_DEFAULT_FUNCTORS

	if (_value.isValid())

		if (_functor == FunctorNone)
		{
		if (_value.isNull())
			_functor = FunctorIsNull;
		else

			if (_value.type() == QVariant::ByteArray
				||
				_value.type() == QVariant::String)
			{
			_functor = FunctorLike;
			_braces  = BracesBoth;
		}
		else
			if (_value.type() == QVariant::Double
				||
				_value.type() == QVariant::Date
				||
				_value.type() == QVariant::Time)
			{
			_functor = FunctorEqual;
		}
		else
			if (_value.type() == QVariant::Int
				||
				_value.type() == QVariant::LongLong
				||
				_value.type() == QVariant::ULongLong
				||
				_value.type() == QVariant::Bool)
			{
			_functor = FunctorEqual;
		}
		else
		{
			Q_ASSERT_X(false, "Main constructor", "Type not identified.");
		}
	}
#endif
}

/*!
	Конструктор с расширенной функциональностью для создания фильтров
	на формах, где требуется точное и неточное совпадение строк.
	В отличие от основного конструктора создает QstValue только тогда, когда
	enable = true. В качестве enabled может быть передан параметр isChecked
	от QComboBox. */
QstValue::QstValue(const bool &enabled,
				   const QString &valueString,
				   const bool &fullStringMatch,
				   const CompareFunctor &functor,
				   const FuzzyBraces &braces,
				   const MatchPolicy &matchPolicy)
	   :
	   _value(valueString),
	   _functor(functor),
	   _braces(braces)
{
	if (!enabled)
	{
		_value = QVariant();
		_functor = FunctorNone;
		_braces = BracesNone;
	}
	else
	{
		_value = QVariant(valueString);

		switch (matchPolicy)
		{
		case MatchFlagDependent:
			if (fullStringMatch)
			{
				_functor = FunctorEqual;
				_braces = BracesNone;
			}
			else
			{
				_functor = functor;
				_braces	 = braces;
			}
			break;

		case MatchBracesDependent:
			_functor = FunctorLike;
			_braces	 = braces;
			break;

		case MatchFuzzy:
			_functor = FunctorLike;
			_braces  = BracesBoth;
			break;

		case MatchFull:
			_functor = FunctorEqual;
			_braces = BracesNone;
			break;

		default: Q_ASSERT(false);
		};
	}
}

/*! Создает QstValue на основе другого, при этом изменяет functor и braces. */
QstValue::QstValue(const QstValue &other,
				   const CompareFunctor &functor,
				   const FuzzyBraces &braces)
		   :
		   _value(other.value()),
		   _functor(functor),
		   _braces(braces)
{
}

/*! Конструктор копирования. */
QstValue::QstValue(const QstValue &other)
	   :
	   _value(other.value()),
	   _functor(other.functor()),
	   _braces(other.braces())
{
}

/*! QVariant-like конструктор для конструирования NULL-значений. */
QstValue::QstValue(const NullType &null)
	:
	_value(QVariant::Int),
	_functor(FunctorIsNull),
	_braces(BracesNone)
{
}

// ------------------------------------------------------------------------ //

/*! Возвращает QVariant-значение. */
QVariant QstValue::value() const
{
	return _value;
}

/*! Возвращает функтор сравнения. */
CompareFunctor QstValue::functor() const
{
	return _functor;
}

/*! Возвращает нечеткие скобки. */
FuzzyBraces QstValue::braces() const
{
	return _braces;
}

/*! Преобразует QVariant-зачение в строку.

	Результат преобразования зависит от параметров функции. */

QString QstValue::toString(const ValueBordering &bordering,
						   const FuzzyBracesUsage &useBraces,
						   const NullSubstitution &nullSubstitute) const
{
	if (!_value.isValid())
	{
		return _toInvalidString(bordering, nullSubstitute, useBraces);
	}

	if (_value.isNull())
	{
		return _toEmptyString(bordering, nullSubstitute, useBraces);
	}

	if (_value.type() == QVariant::Date)
	{
		return _toDateString(_value.toDate(), bordering, useBraces);
	}

return _toString(_value.toString(), bordering, useBraces);
}

/*! Устанавливает QVariant-значение. */
void QstValue::setValue(const QVariant &value)
{
	_value = value;
}

/*! Устанавливает фукнтор сравнения. */
void QstValue::setFunctor(const CompareFunctor &functor)
{
	_functor = functor;
}

/*! Устанавливает нечеткие скобки. */
void QstValue::setBraces(const FuzzyBraces &braces)
{
	_braces = braces;
}

/*! Возвращает true, если QVariant-значение нулевое. */
bool QstValue::isNull() const
{
	return _value.isNull();
}

/*! Возвращает true QVariant-значение валидно. */
bool QstValue::isValid() const
{

	return _value.isValid();
}

QString QstValue::_toString(const QString &strVal,
							const ValueBordering &bordering,
							const FuzzyBracesUsage &useBraces) const
{
	QString templ = _bracesToStringTemplate(useBraces).arg(strVal);

	if (bordering == ValueBordered)
		return QString("'") + templ + QString("'");

return templ;
}

QString QstValue::_toDateString(const QDate &date,
								const ValueBordering &bordering,
								const FuzzyBracesUsage &useBraces) const
{
	if (bordering == ValueBordered)
	{
		QString bracedDate = _bracesToStringTemplate(useBraces).arg(date.toString(DATE_FORMAT));
		return "'" + bracedDate + "'";
	}
	else
		return _bracesToStringTemplate(useBraces).arg(date.toString(DATE_FORMAT));
}

QString QstValue::_toEmptyString(const ValueBordering &bordering,
								 const NullSubstitution &fillSqlNull,
								 const FuzzyBracesUsage &useBraces) const
{
	if (bordering == ValueBordered)
		return _borderedEmptyString(fillSqlNull, useBraces);
	else
		return _notBorderedEmptyString(fillSqlNull, useBraces);

return QString();
}

QString QstValue::_toInvalidString(const ValueBordering &bordering,
								   const NullSubstitution &fillSqlNull,
								   const FuzzyBracesUsage &useBraces) const
{
	return QString();
}

QString QstValue::_bracesToStringTemplate(const FuzzyBracesUsage &useBraces) const
{
	if (useBraces == BracesUse)
		switch(_braces)
		{
			case BracesLeft:	return QString("%%1");
			case BracesRight:	return QString("%1%");
			case BracesBoth:	return QString("%%1%");
#ifdef QST_USE_BOTH_BRACES_WHEN_NONE_BRACES
			case BracesNone:	return QString("%%1%");
#else
			case BracesNone:	return QString("%1");
#endif
		};

return QString("%1");
}


QString QstValue::_borderedEmptyString(const NullSubstitution &fillSqlNull,
									   const FuzzyBracesUsage &useBraces) const
{
	if (fillSqlNull == NullSubstitute)
		return _borderedNullFilledEmptyString(useBraces);
	else
		return _borderedNotNullFilledEmptyString(useBraces);
}

QString QstValue::_notBorderedEmptyString(const NullSubstitution &fillSqlNull,
										  const FuzzyBracesUsage &useBraces) const
{
	if (fillSqlNull == NullSubstitute)
		return _notBorderedNullFilledEmptyString(useBraces);
	else
		return _notBorderedNotNullFilledEmptyString(useBraces);
}


QString QstValue::_borderedNullFilledEmptyString(const FuzzyBracesUsage &useBraces) const
{
	QString bracedString = _bracesToStringTemplate(useBraces).arg(SQL_NULL_VALUE);
	return "'" + bracedString + "'";
}

QString QstValue::_borderedNotNullFilledEmptyString(const FuzzyBracesUsage &useBraces) const
{
	return "'" + _bracesToStringTemplate(useBraces).arg("") + "'";
}

QString QstValue::_notBorderedNullFilledEmptyString(const FuzzyBracesUsage &useBraces) const
{
	return _bracesToStringTemplate(useBraces).arg(SQL_NULL_VALUE);
}

QString QstValue::_notBorderedNotNullFilledEmptyString(const FuzzyBracesUsage &useBraces) const
{
	return _bracesToStringTemplate(useBraces).arg("");
}

}
