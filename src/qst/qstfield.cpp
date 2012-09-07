/****************************************************************************
** QST 0.4.2a release
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

#include "qstfield.h"

#include <QObject>

namespace Qst
{

	/*!
		\class QstField
		\brief
		Объекты класса QstField составляются в пакеты (QstBatch) и затем
		непосредственно участвуют в генерации SQL-запросов.
		Класс хранит имена полей для секций SELECT, ORDER BY, GROUP BY,
		хранит имя поля и значение для секций WHERE, INSERT INTO / VALUES,
		UPDATE / SET, а так же представляет	параметр хранимой процедуры.

		\inmodule Qst

		Каждый класс QstField имеет свое назначение (QstField::purposes()).
		Поле может быть предназначено для нескольких секций сразу:
		Qst::PurposeSelect | Qst::PurposeInsert,
		Qst::PurposeParameter | Qst::PurposeWhere, однако обычно у поля всего
		одно назначение. Чаще всего назначение задается по умолчанию и зависит
		от выбранного конструктора QstField.  Есть конструкторы только
		для секции SELECT, только для секции WHERE, только для
		параметра процедуры. Есть общий конструктор для секций
		INSERT/UPDATE, WHERE и для параметра процедуры.

		У поля есть имя - QstField::name(), котрое интерпретируется в зависимости
		от секции. Чаще всего оно представляет имя поля в БД.

		Те объекты QstField, которые предназначены для секции SELECT, могут
		быть снабжены дополнительной информацией, используемой при настройке
		представлений (QTableView, QTreeView, QListView, QComboBox):
			- видимость поля, невидимые поля отображаться не будут;
			- название колонки для этого поля (переводится функцией tr());
			- ширина колонки;
			- ориентация колонки (почти не используется).

		Подключаемые представления будут настраиваться в соответствии с этими
		параметрами.

		\attention в QComboBox и QListView отображается первое поле с
		параметром isVisible() == true.

		\sa QstAbstractModelHandler

		Поле может иметь роль ключевого: первичного ключа или ключа
		на родительскую запись (RolePrimaryKey, RoleParentKey).
		Для ключевых полей есть специальный конструктор.
		Может быть только по одному полю с каждой ролью.
	*/

/*! Конструктор по умолчанию. Создает инвалидный QstField. */
QstField::QstField()
	:
	_name(QString()),
	_visibility(VisibilityNone),
	_columnTitle(QString()),
	_columnWidth(0),
	_orientation(Qt::Horizontal),
	_role(RoleNone),
	_purposes(PurposeNone)
{
}

/*! Конструктор для служебных ключевых полей.
	const * char версия для параметра columnTitle. Переводится функцией tr.
*/
QstField::QstField(const FieldRole &role,
				  const QString &name,
				  const FieldVisibility &visibility,
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
	_purposes(PurposeSelect)
{
}

/*! Конструктор для поля в секции SELECT
	const * char версия для параметра columnTitle. Переводится функцией tr.
*/
QstField::QstField(const QString &name,
				   const FieldVisibility &visibility,
				   const char *columnTitle,
				   const int &columnWidth,
				   const Qt::Orientation &titleOrientation)
	   :
	   _name(name),
	   _visibility(visibility),
	   _columnTitle(QObject::tr(columnTitle)),
	   _columnWidth(columnWidth),
	   _orientation(titleOrientation),
	   _role(RoleNone),
	   _purposes(PurposeSelect)
{
	if (!strlen(columnTitle))
		_columnTitle = name;
}

/*! Конструктор для одинарного фильтра; для поля в секциях INSERT/UPDATE;
	для параметров процедуры. */
QstField::QstField(const QString &name,
				   const QstValue &value,
				   const FieldPurposes purposes)
	   :
	   _name(name),
	   _visibility(VisibilityNone),
	   _columnTitle(QString()),
	   _columnWidth(0),
	   _orientation(Qt::Horizontal),
	   _role(RoleNone),
	   _purposes(purposes)
{
	_values.append(value);
}

/*! Конструктор для бинарного фильтра ("BETWEEN"). */
QstField::QstField(const QString &name,
				   const QstValue &value1,
				   const QstValue &value2,
				   const FieldPurposes purposes)	// Игнорируется.
	   :
	   _name(name),
	   _visibility(VisibilityNone),
	   _columnTitle(QString()),
	   _columnWidth(0),
	   _orientation(Qt::Horizontal),
	   _role(RoleNone),
	   _purposes(PurposeWhere)
{
	_values.append(value1);
	_values.append(value2);
}

/*! Конструктор для параметра процедуры. */
QstField::QstField(const QstValue &value,
				   const FieldPurposes purposes) // Игнорируется.
	:
	_name(QString()),
	_visibility(VisibilityNone),
	_columnTitle(QString()),
	_columnWidth(0),
	_orientation(Qt::Horizontal),
	_role(RoleNone),
	_purposes(PurposeParameter)
{
	_values.append(value);
}

/*! Конструктор для ORDER BY и GROUP BY. */
QstField::QstField(const QString &name,
				   const FieldPurposes purposes)
	   :
	   _name(name),
	   _visibility(VisibilityNone),
	   _columnTitle(QString()),
	   _columnWidth(0),
	   _orientation(Qt::Horizontal),
	   _role(RoleNone),
	   _purposes(purposes)
{
}

/*! Возвращает имя поля. */
QString QstField::name() const
{
	return _name;
}

/*! Устанавливает имя поля. */
void QstField::setName(const QString &name)
{
	_name = name;
}

/*! Устанавливает, видимое ли поле (FieldVisible или FieldInvisible). */
void QstField::setVisible(const bool &visible)
{
	if (visible)
		_visibility = FieldVisible;
	else
		_visibility = FieldInvisible;
}

/*! Возвращает видимость поля. */
FieldVisibility QstField::visibility() const
{
	return _visibility;
}

/*! Устанавливает видимость поля. */
void QstField::setVisibility(const FieldVisibility &visibility)
{
	_visibility = visibility;
}

/*! Возвращает название колонки. */
QString QstField::columnTitle() const
{
	return _columnTitle;
}

/*! Устанавливает название колонки. */
void QstField::setColumnTitle(const QString &title)
{
	_columnTitle = title;
}

/*! Возвращает ширину колонки. */
int QstField::columnWidth() const
{
	return _columnWidth;
}

/*! Устанавливает ширину колонки. */
void QstField::setColumnWidth(const int &width)
{
	_columnWidth = width;
}

/*! Возвращает ориентацию колонки. */
Qt::Orientation QstField::orientation() const
{
	return _orientation;
}

/*! Устанавливает ориентацию колонки. */
void QstField::setOrientation(const Qt::Orientation &orientation)
{
	_orientation = orientation;
}

/*! Возвращает назначения поля. */
FieldPurposes	QstField::purposes() const
{
	return _purposes;
}

/*! Возвращает роль поля. */
FieldRole	QstField::role() const
{
	return _role;
}

/*! Возвращает значение поля. */
QstValue QstField::value(const BinaryValueOrder &order) const
{
#ifdef QT_DEBUG
#ifdef QST_EXTENDED_DEBUG_MESSAGES
	if (!hasValue(order))
		qDebug() << "Warring! Trying to get unexisted value by QstField::value() function.";
#endif
#endif

	return _values.value((int)order, QstValue());
}

/*! Устанавливает значение поля. */
void QstField::setValue(const QstValue & val, const BinaryValueOrder &order)
{
	if (_values.size() < (int)order)
		_values.resize((int)order);

	_values[(int)order] = val;
}

/*! Возвращает true, если значение поля установлено.

	Для бинарного фильтра BETWEEN может быть два значения.
	order указывает, наличие какого значения проверяется. */
bool QstField::hasValue(const BinaryValueOrder &order) const
{
	return ((int)order) < _values.size();
}

/*! Возвращает true, если поле - бинарный фильтр BETWEEN. */
bool QstField::isBinaryFilter() const
{
	return	hasValue(OrderSecond);
}

/*! Возвращает true, если видимость поля - FieldVisible. */
bool QstField::isVisible() const
{
	return _visibility == FieldVisible;
}

/*! Возвращает true, если видимость поля - FieldInvisible. */
bool QstField::isInvisible() const
{
	return _visibility == FieldInvisible;
}

/*! Возвращает true, если видимость поля - VisibilityNone. */
bool QstField::isNoneVisibility() const
{
	return _visibility == VisibilityNone;
}

/*! Возвращает true, если видимость поля - RolePrimaryKey или RoleParentKey. */
bool QstField::isService() const
{
	return (_role == RolePrimaryKey || _role == RoleParentKey);
}

/*! Возвращает true, если все значения валидны. */
bool QstField::isValuesValid() const
{
	if (_values.empty())
		return false;

	for (int i = 0; i < _values.size(); ++i)
		if (!_values[i].isValid())
			return false;
return true;
}

/*! Возвращает false в следующих случаях:
	- имя поля пустое и все значения инвалидны;
	- если назначение поля входит в PurposeValued_Mask и все значения инвалидны;
	- если назначение поля входитв PurposeHasFieldName_Mask и имя поля пустое. */
bool QstField::isValid() const
{
	if (_name.isEmpty() && !isValuesValid())
		return false;

	if ((_purposes & PurposeValued_Mask) && !isValuesValid())
		return false;

	if ((_purposes & PurposeHasFieldName_Mask) && _name.isEmpty())
		return false;

return true;
}

}
