/****************************************************************************
** QST 0.3c beta
** Copyright (C) 2010 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Sql module of the QsT SQL Tools.
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

#include "sqlvalue.h"

#include <QDate>
#include "sqlgenconstants.h"


namespace Sql
{

/*!
	\class SqlValue
	\brief
	Класс предназначен для хранения любых значений, функторов сравнения и
	"нечетких скобок" (для строковых типов). Используется при создании
	описателей к запросам.

	\inmodule Sql

	Класс хранит любое значение, помещая его в QVariant. С помощью
	нескольких конструкторов можно создать значение нужного типа, а затем
	получить строку этого значения с помощью настраиваемой функции toString().

	\code
	SqlValue value1(10, fo_equal);
	SqlValue value2("Строка");

	value1.isValid();	// true
	value1.isNull();	// false
	value2.isValid();	// true
	value2.isNull();	// false

	value1.toString();	// QString("10")
	value1.toString(vb_bordered);	// QString("'10'")
	value2.toString(vb_bordered);	// QString("'Строка'")
	value2.toString(vb_bordered, vfn_not, vub_use);	// QString("'%Строка%'")
	\endcode

	Класс может быть невалидным и нулевым в том же смысле, что и QVariant.

	\code
	SqlValue invalidValue1;

	invalidValue1.isValid();	// false
	invalidValue1.isNull();		// true
	invalidValue1.toString();	// QString();
	invalidValue1.value();	// QVariant();


	SqlValue invalidValue2 = SqlValue(QVariant());

	invalidValue2.isValid();	// false
	invalidValue2.isNull();		// true
	invalidValue2.toString();	// QString();
	invalidValue2.value();	// QVariant();
	\endcode

	\code
	SqlValue nullValue1(nt_null);

	nullValue1.isValid();	// true
	nullValue1.isNull();	// true
	nullValue1.toString();	// QString("NULL")
	nullValue1.toString(vb_bordered);	// QString("'NULL'")


	SqlValue nullValue2 = SqlValue(QVariant(QVariant::Int));

	nullValue2.isValid();	// true
	nullValue2.isNull();	// true
	nullValue2.toString();	// QString("NULL")
	nullValue2.toString(vb_bordered);	// QString("'NULL'")
	\endcode

	Здесь вместо QVariant::Int может быть любой тип, определенный в QVariant.


	Строки и даты, помещенные в SqlValue, обрабатываются особым образом.

	\code
	SqlValue strValue("Hello, world!", fo_like, fb_left);

	strValue.toString(vb_none, vfn_not, vub_use);	// QString("%Hello, world!")
	strValue.toString(vb_bordered, vfn_not, vub_not);	// QString("'Hello, world!'")


	SqlValue dateValue(QDate(2010, 12, 13));

	dateValue.toString();	// QString("13.12.2010")
	dateValue.toString(vb_bordered);	// QString("'13.12.2010'")
	\endcode

	Формат даты можно поменять с помощью константы DATE_FORMAT в sqlgenconstants.h.


	Если установлен SQL_VALUE_SET_DEFAULT_FUNCTORS, то для разных типов передаваемого значения
	назначаются свои функторы сравнения при условии, что значение не нулевое.

	QString, QByteArray:	functor = fo_like, braces = fb_both

	Другие главные типы:	functor = fo_equal.

	Функторы сравнения используются затем при формировании запроса. Например,
	следующее поле с SqlValue для секции WHERE будет обработано компоновщиком так:

	Участок кода:
	...
	SqlField("FieldName", SqlValue(10, fo_greater_equal), fp_where)
	...

	Запрос:
	... WHERE ... FieldName >= 10

	Еще пример.

	Участок кода:
	...
	SqlField("Name", SqlValue("Ната"), fp_where)
	...

	Запрос:
	... WHERE ... Name LIKE '%Ната%'

	За дополнительными примерами использования SqlValue обращаться в
	модульные тесты ut_sqlvalue.cpp, ut_sqlfield.cpp, ut_sqlbatch.cpp,
	ut_sqlgen.cpp .

	\sa SqlField, AbstractModelHandler, SqlValue::toString(), SqlBatch
*/


/*! Основной конструктор. Позволяет создать SqlValue из QVariant-значения.

	QVariant при создании SqlValue можно опустить. Следующие две конструкции эквивалентны:

	\code
	SqlValue val = SqlValue(QVariant(3.14));
	SqlValue val = SqlValue(3.14);
	\endcode


	\attention Если установлен SQL_VALUE_SET_DEFAULT_FUNCTORS, то
	для разных типов передаваемого значения назначаются свои функторы сравнения
	при условии, что значение не нулевое.
	QString, QByteArray:	functor = fo_like, braces = fb_both
	Другие главные типы:	functor = fo_equal.
  */
SqlValue::SqlValue(const QVariant &value,
				   const Functor &functor,
				   const FuzzyBraces &braces)
		  :
		  _value(value),
		  _functor(functor),
		  _braces(braces),
		  _ignore(vbi_not_ignore)
{
#ifdef SQL_VALUE_SET_DEFAULT_FUNCTORS
	if (_value.isValid())

		if (_functor == fo_none)
		{
		if (_value.isNull())
			_functor = fo_is_null;
		else

			if (_value.type() == QVariant::ByteArray
				||
				_value.type() == QVariant::String)
			{
					_functor = fo_like;
					_braces  = fb_both;
			}
			else
				if (_value.type() == QVariant::Double
					||
					_value.type() == QVariant::Date
					||
					_value.type() == QVariant::Time)
				{
					_functor = fo_equal;
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
					_functor = fo_equal;
				}
			else
			{
				Q_ASSERT_X(false, "Main constructor", "Type not identified.");
			}
		}
#endif
}

/*! Конструктор, позволяющий создать объкт SqlValue из другого объекта с
	изменением функтора и нечетких скобок. */
SqlValue::SqlValue(const SqlValue &other,
				   const Functor &functor,
				   const FuzzyBraces &braces)
		   :
		   _value(other.value()),
		   _functor(functor),
		   _braces(braces),
		   _ignore(vbi_not_ignore)
{
}

/*! Конструктор копирования. */
SqlValue::SqlValue(const SqlValue &other)
	   :
	   _value(other.value()),
	   _functor(other.functor()),
	   _braces(other.braces()),
	   _ignore(other.borderIgnoring())
{
}

/*! QVariant-like конструктор для создания нулевого SqlValue. */
SqlValue::SqlValue(const NullType &null)
	:
	_value(QVariant::Int),
	_functor(fo_is_null),
	_braces(fb_none),
	_ignore(vbi_not_ignore)
{
}

/*! Конструктор, позволяющий обойти обрамление значения при компоновке UPDATE/SET в SqlQueryComposer. */
SqlValue::SqlValue(const SqlValue &other,
				   const SqlValueBorderIgnoring &ignore)
		:
		_value(other.value()),
		_functor(other.functor()),
		_braces(other.braces()),
		_ignore(ignore)
{
}

/*! Конструктор, позволяющий обойти обрамление значения при компоновке UPDATE/SET в SqlQueryComposer. */
SqlValue::SqlValue(const QVariant &value,
		 const SqlValueBorderIgnoring &ignore,
		 const Functor &functor,
		 const FuzzyBraces &braces)
	   :
	   _value(value),
	   _functor(functor),
	   _braces(braces),
	   _ignore(ignore)
{
}

// ------------------------------------------------------------------------ //

/*! Возвращает сохраненное QVariant-значение. */
QVariant SqlValue::value() const
{
	return _value;
}

/*! Возвращает функтор сравнения. */
Functor SqlValue::functor() const
{
	return _functor;
}

/*! Возвращает нечеткие скобки. */
FuzzyBraces SqlValue::braces() const
{
	return _braces;
}

/*! Возвращает флаг игнорирования обрамления. */
SqlValueBorderIgnoring SqlValue::borderIgnoring() const
{
	return _ignore;
}

/*! Переводит значение в тип QString с учетом типа этого значения и переданных
	в toString() параметров.

	Описание возвращаемых значений можно найти в модульном тесте
	к классу SqlValue.

	\sa ut_SqlValue */
QString SqlValue::toString(const SqlValueBordering &bordering,
						   const SqlValueFillNull &fillSqlNull,
						   const SqlValueUseBraces &useBraces) const
{
	if (!_value.isValid())
	{
		return _toInvalidString(bordering, fillSqlNull, useBraces);
	}

	if (_value.isNull())
	{
		return _toEmptyString(bordering, fillSqlNull, useBraces);
	}

	if (_value.type() == QVariant::Date)
	{
		return _toDateString(_value.toDate(), bordering, useBraces);
	}

return _toString(_value.toString(), bordering, useBraces);
}

/*! Возвращает ссылку на QVariant-значение. */
QVariant &SqlValue::rValue()
{
	return _value;
}

/*! Устанавливает QVariant-значение. */
void SqlValue::setValue(const QVariant &value)
{
	_value = value;
}

/*! Устанавливает функтор сравнения. */
void SqlValue::setFunctor(const Functor &functor)
{
	_functor = functor;
}

/*! Устанавливает "нечеткие скобки". */
void SqlValue::setBraces(const FuzzyBraces &braces)
{
	_braces = braces;
}

/*! Возвращает истину, если QVariant-значение нулевое.

	\sa ut_SqlValue::isNull()*/
bool SqlValue::isNull() const
{
	return _value.isNull();
}

/*! Возвращает истину, если QVariant-значение нулевое.

	\sa ut_SqlValue::isValid()*/
bool SqlValue::isValid() const
{

	return _value.isValid();
}

/*! Возвращает истину, если установлен флаг игнорирования обрамления. */
bool SqlValue::isIgnoreBordering() const
{
	return _ignore == vbi_ignore;
}

QString SqlValue::_toString(const QString &strVal,
							const SqlValueBordering &bordering,
							const SqlValueUseBraces &useBraces) const
{
	QString templ = _bracesToStringTemplate(useBraces).arg(strVal);

	if (bordering == vb_bordered)
		return QString("'") + templ + QString("'");

return templ;
}

QString SqlValue::_toDateString(const QDate &date,
								const SqlValueBordering &bordering,
								const SqlValueUseBraces &useBraces) const
{
	if (bordering == vb_bordered)
	{
		QString bracedDate = _bracesToStringTemplate(useBraces).arg(date.toString(DATE_FORMAT));
		return "'" + bracedDate + "'";
	}
	else
		return _bracesToStringTemplate(useBraces).arg(date.toString(DATE_FORMAT));
}

QString SqlValue::_toEmptyString(const SqlValueBordering &bordering,
								 const SqlValueFillNull &fillSqlNull,
								 const SqlValueUseBraces &useBraces) const
{
	if (bordering == vb_bordered)
		return _borderedEmptyString(fillSqlNull, useBraces);
	else
		return _notBorderedEmptyString(fillSqlNull, useBraces);

return QString();
}

QString SqlValue::_toInvalidString(const SqlValueBordering &bordering,
								   const SqlValueFillNull &fillSqlNull,
								   const SqlValueUseBraces &useBraces) const
{
	return QString();
}

QString SqlValue::_bracesToStringTemplate(const SqlValueUseBraces &useBraces) const
{
	if (useBraces == vub_use)
		switch(_braces)
		{
			case fb_left:	return QString("%%1");
			case fb_right:	return QString("%1%");
			case fb_both:	return QString("%%1%");

#ifdef SQL_USE_BRACES_PRIORITY_GREATER_THEN_NONE_BRACES
			case fb_none:	return QString("%%1%");
#else
			case fb_none:	return QString("%1");
#endif
		};

return QString("%1");
}


QString SqlValue::_borderedEmptyString(const SqlValueFillNull &fillSqlNull,
									   const SqlValueUseBraces &useBraces) const
{
	if (fillSqlNull == vfn_fill)
		return _borderedNullFilledEmptyString(useBraces);
	else
		return _borderedNotNullFilledEmptyString(useBraces);
}

QString SqlValue::_notBorderedEmptyString(const SqlValueFillNull &fillSqlNull,
										  const SqlValueUseBraces &useBraces) const
{
	if (fillSqlNull == vfn_fill)
		return _notBorderedNullFilledEmptyString(useBraces);
	else
		return _notBorderedNotNullFilledEmptyString(useBraces);
}


QString SqlValue::_borderedNullFilledEmptyString(const SqlValueUseBraces &useBraces) const
{
	QString bracedString = _bracesToStringTemplate(useBraces).arg(SQL_NULL_VALUE);
	return "'" + bracedString + "'";
}

QString SqlValue::_borderedNotNullFilledEmptyString(const SqlValueUseBraces &useBraces) const
{
	return "'" + _bracesToStringTemplate(useBraces).arg("") + "'";
}

QString SqlValue::_notBorderedNullFilledEmptyString(const SqlValueUseBraces &useBraces) const
{
	return _bracesToStringTemplate(useBraces).arg(SQL_NULL_VALUE);
}

QString SqlValue::_notBorderedNotNullFilledEmptyString(const SqlValueUseBraces &useBraces) const
{
	return _bracesToStringTemplate(useBraces).arg("");
}

}
