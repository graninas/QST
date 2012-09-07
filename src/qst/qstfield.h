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

#ifndef QSTFIELD_H
#define QSTFIELD_H

#include "qstvalue.h"
#include <QVector>
#include <QMap>
#include <QString>

namespace Qst
{

class QstField
{
public:
	QstField();

	// Конструктор для служебных ключевых полей в секции SELECT.
	// const * char версия для параметра columnTitle. Переводится
	// функцией tr.
	QstField(const FieldRole &role,
			 const QString &name,
			 const FieldVisibility &visibility = FieldInvisible,
			 const char *columnTitle = "",
			 const int &columnWidth = 150,
			 const Qt::Orientation &titleOrientation = Qt::Horizontal);

	// Конструктор для поля в секции SELECT
	// const * char версия для параметра columnTitle. Переводится
	// функцией tr.
	QstField(const QString &name,
			 const FieldVisibility &visibility = FieldVisible,
			 const char *columnTitle = "",
			 const int &columnWidth = 150,
			 const Qt::Orientation &titleOrientation = Qt::Horizontal);

	// Конструктор для одинарного фильтра;
	// для поля в секциях INSERT/UPDATE;
	// для параметров процедуры.
	QstField(const QString &name,
			 const QstValue &value,
			 const FieldPurposes purposes = PurposeValued_Mask
			 );

	// Конструктор для бинарного фильтра ("BETWEEN").
	QstField(const QString &name,
			 const QstValue &value1,
			 const QstValue &value2,
			 const FieldPurposes purposes = PurposeWhere);// для удобства. Игнорируется.


	// Конструктор для параметра процедуры.
	QstField(const QstValue &value,
			 const FieldPurposes purposes = PurposeParameter);// для удобства. Игнорируется.

	// Конструктор для ORDER BY и GROUP BY.
	QstField(const QString &name,
			 const FieldPurposes purposes);

	QString name() const;
	void setName(const QString &name);

	void setVisible(const bool &visible);
	FieldVisibility visibility() const;
	void setVisibility(const FieldVisibility &visibility);

	QString columnTitle() const;
	void setColumnTitle(const QString &title);

	int columnWidth() const;
	void setColumnWidth(const int &width);

	Qt::Orientation orientation() const;
	void setOrientation(const Qt::Orientation &orientation);

	FieldPurposes	purposes() const;
	FieldRole		role() const;

	QstValue value(const BinaryValueOrder &order = OrderFirst) const;
	void setValue(const QstValue & val, const BinaryValueOrder &order = OrderFirst);

	bool hasValue(const BinaryValueOrder &order = OrderFirst) const;
	bool isBinaryFilter() const;

	bool isVisible() const;
	bool isInvisible() const;
	bool isNoneVisibility() const;

	bool isService() const;

	bool isValuesValid() const;
	bool isValid() const;

private:

	QString				_name;
	FieldVisibility		_visibility;
	QString				_columnTitle;
	int					_columnWidth;
	Qt::Orientation		_orientation;

	FieldRole		_role;
	FieldPurposes	_purposes;

	QstValueVector		_values;

public:

	friend bool operator == (const QstField &val1, const QstField &val2)
	{
		return	(val1.name() == val2.name())
				&&
				(val1.value(OrderFirst) == val2.value(OrderFirst))
				&&
				(val1.value(OrderSecond) == val2.value(OrderSecond))
				&&
				(val1.visibility() == val2.visibility())
				&&
				(val1.columnTitle() == val2.columnTitle())
				&&
				(val1.columnWidth() == val2.columnWidth())
				&&
				(val1.orientation() == val2.orientation())
				&&
				(val1.purposes() == val2.purposes())
				&&
				(val1.role() == val2.role());
	}
};


////////////////////////////////////////////////////////////////////////////////

typedef QVector<QstField>	QstFieldVector;
typedef QStringList			QstFieldNameList;

////////////////////////////////////////////////////////////////////////////////

}
#endif // QSTFIELD_H
