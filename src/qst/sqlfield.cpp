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

#include "sqlfield.h"

#include <QObject>

namespace Sql
{
/*!
	\class SqlField
	\brief
	Класс предназначен для хранения значений SqlValue, содержит настройки
	для представлений (view), конвертируется в логически законченную единицу
	SQL-запроса (имя поля, фильтр, имя объекта и др.).

	\inmodule Sql

	Класс занимает центральное значение в системе DFD. SqlField - основная
	единица описателей, создаваемых в классах-хэндлерах. Набор (пакет, SqlBatch)
	из объектов SqlField может быть конвертирован в
	SQL-запрос с помощью классов SqlGen и SqlQueryComposer.

	Фактически, каждое поле SqlField представляет собой часть запроса SQL или
	настройку для генератора запросов.

	Пример.
	\code
	SqlBatch batch;
	batch.addSource("PeopleView");

	batch
		<< SqlField("ID", fv_invisible, fr_id)

		<< SqlField("Name", fv_visible, fr_none, "ФИО", 200)
		<< SqlField("Age", fv_visible, fr_none, "Возраст", 90)

		<< SqlField("Age", SqlValue(18, fo_greater), fp_where);
	\endcode

	Если сгенерировать SELECT-запрос, передав в генератор этот пакет,
	получим следующее:

	\code
	SELECT Name, Age FROM PeopleView WHERE Age > 18
	\endcode

	За дополнительными примерами полей обращаться в модульный тест ut_sqlfield.cpp .

	\attention В QST версии 0.4 класс SqlField переработан.
	Изменено его имя. Переименованы некоторые функции.

	\sa SqlBatch
*/

/*! Конструктор по умолчанию. */
SqlField::SqlField()
	:
	_name(QString()),
	_visibility(fv_none),
	_columnTitle(QString()),
	_columnWidth(0),
	_orientation(Qt::Horizontal),
	_role(fr_none),
	_purposes(fp_none)
{
}

/*! Конструктор для поля в секции SELECT

	columnTitle имеет тип const * char, переводится функцией QObject::tr(). */
SqlField::SqlField(const QString &name,
		 const SqlFieldVisibility &visibility,
		 const SqlFieldRoles &role,
		 const char *columnTitle,
		 const int &columnWidth,
		 const Qt::Orientation &titleOrientation)
	   :
	   _name(name),
	   _visibility(visibility),
	   _columnTitle(QObject::tr(columnTitle)),
	   _columnWidth(columnWidth),
	   _orientation(titleOrientation),
	   _role(role),
	   _purposes(fp_select)
{
	if (!strlen(columnTitle))
		_columnTitle = name;
}

/*! Конструктор для одинарного фильтра;	для поля в секциях INSERT/UPDATE;
	для параметров процедуры. */

SqlField::SqlField(const QString &name,
		 const SqlValue &value,
		 const SqlFieldPurposes purposes)
	   :
	   _name(name),
	   _visibility(fv_none),
	   _columnTitle(QString()),
	   _columnWidth(0),
	   _orientation(Qt::Horizontal),
	   _role(fr_none),
	   _purposes(purposes)
{
	_values.append(value);
}

/*! Конструктор для бинарного фильтра ("BETWEEN"). */
SqlField::SqlField(const QString &name,
			const SqlValue &value1,
			const SqlValue &value2,
			const SqlFieldPurposes purposes)	// Игнорируется.
	   :
	   _name(name),
	   _visibility(fv_none),
	   _columnTitle(QString()),
	   _columnWidth(0),
	   _orientation(Qt::Horizontal),
	   _role(fr_none),
	   _purposes(fp_where)
{
	_values.append(value1);
	_values.append(value2);
}

/*! Конструктор для параметра процедуры. */
SqlField::SqlField(const SqlValue &value,
				   const SqlFieldPurposes purposes) // Игнорируется.
	:
	_name(QString()),
	_visibility(fv_none),
	_columnTitle(QString()),
	_columnWidth(0),
	_orientation(Qt::Horizontal),
	_role(fr_none),
	_purposes(fp_parameter)
{
	_values.append(value);
}

/*! Конструктор для ORDER BY и GROUP BY. */
SqlField::SqlField(const QString &name,
		 const SqlFieldPurposes purposes)
	   :
	   _name(name),
	   _visibility(fv_none),
	   _columnTitle(QString()),
	   _columnWidth(0),
	   _orientation(Qt::Horizontal),
	   _role(fr_none),
	   _purposes(purposes)
{
}

/*! Возвращает имя поля.

	\sa setName()*/
QString SqlField::name() const
{
	return _name;
}

/*! Устанавливает имя поля. */
void SqlField::setName(const QString &name)
{
	_name = name;
}

/*! Устанавливает параметр видимости поля в fv_visible или fv_invisible.

	\sa SqlFieldVisibility, setVisibility(), isVisible(), visibility()*/
void SqlField::setVisible(const bool &visible)
{
	if (visible)
		_visibility = fv_visible;
	else
		_visibility = fv_invisible;
}

/*! Возвращает параметр видимости поля.

	\sa SqlFieldVisibility, setVisible(), isVisible(), setVisibility()*/
SqlFieldVisibility SqlField::visibility() const
{
	return _visibility;
}

/*! Устанавливает параметр видимости поля.

	\sa SqlFieldVisibility, visibility(), setVisible(), isVisible()*/
void SqlField::setVisibility(const SqlFieldVisibility &visibility)
{
	_visibility = visibility;
}

/*! Возвращает заголовок колонки. */
QString SqlField::columnTitle() const
{
	return _columnTitle;
}

/*! Устанавливает заголовок колонки. */
void SqlField::setColumnTitle(const QString &title)
{
	_columnTitle = title;
}

/*! Возвращает ширину колонки. */
int SqlField::columnWidth() const
{
	return _columnWidth;
}

/*! Устанавливает ширину колонки. */
void SqlField::setColumnWidth(const int &width)
{
	_columnWidth = width;
}

/*! Возвращает ориентацию заголовка колонки. */
Qt::Orientation SqlField::orientation() const
{
	return _orientation;
}

/*! Возвращает ориентацию заголовка колонки. */
void SqlField::setOrientation(const Qt::Orientation &orientation)
{
	_orientation = orientation;
}

/*! Возвращает флаги назначения поля.

	\sa SqlFieldPurposes */
SqlFieldPurposes	SqlField::purposes() const
{
	return _purposes;
}

/*! Возвращает роль поля.

	\sa SqlFieldRoles */
SqlFieldRoles	SqlField::role() const
{
	return _role;
}

/*! Возвращает значение, присвоенное полю.

	Так как у бинарного фильтра BETWEEN два параметра, в функции
	предусмотрен параметр BinaryValueOrder order.
	По умолчанию order = bvo_first, что соответствует первому параметру
	в бинарном фильтре и единственному параметру во всех остальных случаях.

	Если значение полю не присвоено, возвращает невалидный SqlValue.

	\sa SqlValue, BinaryValueOrder */
SqlValue SqlField::value(const BinaryValueOrder &order) const
{
#ifdef QT_DEBUG
#ifdef SQL_EXTENDED_DEBUG_MESSAGES
	if (!hasValue(order))
		qDebug() << "Warring! Trying to get unexisted value by SqlField::value() function.";
#endif
#endif

return _values.value((int)order, SqlValue());
}

/*! Устанавливает значение для поля.

	Так как у бинарного фильтра BETWEEN два параметра, в функции
	предусмотрен параметр BinaryValueOrder order.
	По умолчанию order = bvo_first, что соответствует первому параметру
	в бинарном фильтре и единственному параметру во всех остальных случаях.

	Если значение уже существует, оно заменяется на новое.

	\sa SqlValue, BinaryValueOrder */
void SqlField::setValue(const SqlValue & val,
						const BinaryValueOrder &order)
{
	if (_values.size() < (int)order)
		_values.resize((int)order);

	_values[(int)order] = val;
}

/*! Возвращает истину, если значение установлено, и ложь в противном случае.

	Так как у бинарного фильтра BETWEEN два параметра, в функции
	предусмотрен параметр BinaryValueOrder order.
	По умолчанию order = bvo_first, что соответствует первому параметру
	в бинарном фильтре и единственному параметру во всех остальных случаях.

	\sa SqlValue, BinaryValueOrder */
bool SqlField::hasValue(const BinaryValueOrder &order) const
{
	return ((int)order) < _values.size();
}

/*! Возвращает истину, если значение, присвоенное полю, является бинарным фильтром. */
bool SqlField::isBinaryFilter() const
{
	return (hasValue(bvo_second));
}

/*! Возвращает истину, если параметр видимости поля установлен в fv_visible.

	\sa SqlFieldVisibility, visibility(), setVisibility() setVisible()*/
bool SqlField::isVisible() const
{
	return _visibility == fv_visible;
}

/*! Возвращает истину, если параметр видимости поля установлен в fv_invisible.

  \sa SqlFieldVisibility, visibility(), setVisibility() setVisible()*/
bool SqlField::isInvisible() const
{
	return _visibility == fv_invisible;
}

/*! Возвращает истину, если параметр видимости поля установлен в fv_none.

  \sa SqlFieldVisibility, visibility(), setVisibility() setVisible()*/
bool SqlField::isNoneVisibility() const
{
	return _visibility == fv_none;
}

/*! Возвращает истину, если поле является первичным ключом или внешним ключом родителя.

	\sa SqlFieldRoles */
bool SqlField::isService() const
{
	return (_role == fr_id || _role == fr_parent);
}

/*! Возвращает истину, если установлен флаг назначения поля fp_select.

  \sa SqlFieldPurposes*/
bool SqlField::isSelectClauseField() const
{
	return (_purposes & fp_select);
}

/*! Возвращает истину, если установлен любой из следующих флагов назначения поля:
	- fp_group_by
	- fp_insert
	- fp_order_by
	- fp_parameter
	- fp_update
	- fp_where

  \sa SqlFieldPurposes */
bool SqlField::isStuffField() const
{
	return (_purposes & (fp_group_by | fp_insert | fp_order_by | fp_parameter | fp_update | fp_where));
}

/*! Возвращает истину, если все значения SqlValue валидны.

	Если ни одно значение не установлено, возвращает ложь.

	\sa SqlValue */
bool SqlField::isValuesValid() const
{
	if (_values.empty())
		return false;

	for (int i = 0; i < _values.size(); ++i)
		if (!_values[i].isValid())
			return false;
return true;
}

/*! Возвращает ложь в следующих случаях:
	- если имя поля пустое и значения SqlValue невалидны;
	- если назначение поля fp_all_valued, и значения SqlValue невалидны;
	- если назначение поля fp_has_field_name и имя поля пустое.

	Во всех остальных случаях возвращает истину. */
bool SqlField::isValid() const
{
	if (_name.isEmpty() && !isValuesValid())
		return false;

	if ((_purposes & fp_all_valued) && !isValuesValid())
		return false;

	if ((_purposes & fp_has_field_name) && _name.isEmpty())
		return false;

return true;
}

}
