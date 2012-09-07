/****************************************************************************
** QST 0.4.1 pre-alpha
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
// Конструктор по умолчанию.
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

// Конструктор для служебных ключевых полей.
// const * char версия для параметра columnTitle. Переводится
// функцией tr.
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

// Конструктор для поля в секции SELECT
// const * char версия для параметра columnTitle. Переводится
// функцией tr.
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

// Конструктор для одинарного фильтра;
// для поля в секциях INSERT/UPDATE;
// для параметров процедуры.
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

// Конструктор для бинарного фильтра ("BETWEEN").
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

// Конструктор для параметра процедуры.
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

// Конструктор для ORDER BY и GROUP BY.
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

QString QstField::name() const
{
	return _name;
}

void QstField::setName(const QString &name)
{
	_name = name;
}

void QstField::setVisible(const bool &visible)
{
	if (visible)
		_visibility = FieldVisible;
	else
		_visibility = FieldInvisible;
}

FieldVisibility QstField::visibility() const
{
	return _visibility;
}

void QstField::setVisibility(const FieldVisibility &visibility)
{
	_visibility = visibility;
}

QString QstField::columnTitle() const
{
	return _columnTitle;
}

void QstField::setColumnTitle(const QString &title)
{
	_columnTitle = title;
}

int QstField::columnWidth() const
{
	return _columnWidth;
}

void QstField::setColumnWidth(const int &width)
{
	_columnWidth = width;
}

Qt::Orientation QstField::orientation() const
{
	return _orientation;
}

void QstField::setOrientation(const Qt::Orientation &orientation)
{
	_orientation = orientation;
}

FieldPurposes	QstField::purposes() const
{
	return _purposes;
}

FieldRole	QstField::role() const
{
	return _role;
}

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

void QstField::setValue(const QstValue & val, const BinaryValueOrder &order)
{
	if (_values.size() < (int)order)
		_values.resize((int)order);

	_values[(int)order] = val;
}

bool QstField::hasValue(const BinaryValueOrder &order) const
{
	return ((int)order) < _values.size();
}

bool QstField::isBinaryFilter() const
{
	return	hasValue(OrderSecond);
}

bool QstField::isVisible() const
{
	return _visibility == FieldVisible;
}

bool QstField::isInvisible() const
{
	return _visibility == FieldInvisible;
}

bool QstField::isNoneVisibility() const
{
	return _visibility == VisibilityNone;
}

bool QstField::isService() const
{
	return (_role == RolePrimaryKey || _role == RoleParentKey);
}

bool QstField::isValuesValid() const
{
	if (_values.empty())
		return false;

	for (int i = 0; i < _values.size(); ++i)
		if (!_values[i].isValid())
			return false;
return true;
}

bool QstField::isValid() const
{
	if (_name.isEmpty() && !isValuesValid())
		return false;

	if ((_purposes & PurposeAllValued_Mask) && !isValuesValid())
		return false;

	if ((_purposes & PurposeHasFieldName_Mask) && _name.isEmpty())
		return false;

return true;
}

}
