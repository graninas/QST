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

#ifndef SQLFIELD_H
#define SQLFIELD_H

#include "sqlvalue.h"
#include <QVector>
#include <QMap>
#include <QString>

namespace Sql
{


class SqlField
{
public:
	// Конструктор по умолчанию.
	SqlField();

	// Конструктор для поля в секции SELECT
	// const * char версия для параметра columnTitle. Переводится
	// функцией tr.
	SqlField(const QString &name,
			 const SqlFieldVisibility &visibility = fv_visible,
			 const SqlFieldRoles &role = fr_none,
			 const char *columnTitle = "",
			 const int &columnWidth = 0,
			 const Qt::Orientation &titleOrientation = Qt::Horizontal);

	// Конструктор для одинарного фильтра;
	// для поля в секциях INSERT/UPDATE;
	// для параметров процедуры.
	SqlField(const QString &name,
			 const SqlValue &value,
			 const SqlFieldPurposes purposes = fp_all_valued
			 );

	// Конструктор для бинарного фильтра ("BETWEEN").
	SqlField(const QString &name,
				const SqlValue &value1,
				const SqlValue &value2,
				const SqlFieldPurposes purposes = fp_where);// для удобства. Игнорируется.


	// Конструктор для параметра процедуры.
	SqlField(const SqlValue &value,
			 const SqlFieldPurposes purposes = fp_parameter);// для удобства. Игнорируется.

	// Конструктор для ORDER BY и GROUP BY.
	SqlField(const QString &name,
			 const SqlFieldPurposes purposes);

	QString name() const;
	void setName(const QString &name);

	void setVisible(const bool &visible);
	SqlFieldVisibility visibility() const;
	void setVisibility(const SqlFieldVisibility &visibility);

	QString columnTitle() const;
	void setColumnTitle(const QString &title);

	int columnWidth() const;
	void setColumnWidth(const int &width);

	Qt::Orientation orientation() const;
	void setOrientation(const Qt::Orientation &orientation);

	SqlFieldPurposes	purposes() const;
	SqlFieldRoles		role() const;

	SqlValue value(const BinaryValueOrder &order = bvo_first) const;
	void setValue(const SqlValue & val, const BinaryValueOrder &order = bvo_first);

	bool hasValue(const BinaryValueOrder &order = bvo_first) const;
	bool isBinaryFilter() const;

	bool isVisible() const;
	bool isInvisible() const;
	bool isNoneVisibility() const;

	bool isService() const;

	bool isSelectClauseField() const;
	bool isStuffField() const;

	bool isValuesValid() const;
	bool isValid() const;

private:

	QString				_name;
	SqlFieldVisibility	_visibility;
	QString				_columnTitle;
	int					_columnWidth;
	Qt::Orientation		_orientation;

	SqlFieldRoles		_role;
	SqlFieldPurposes	_purposes;

	SqlValuesVector		_values;

public:

	friend bool operator == (const SqlField &val1, const SqlField &val2)
	{
		return	(val1.name() == val2.name())
				&&
				(val1.value(bvo_first) == val2.value(bvo_first))
				&&
				(val1.value(bvo_second) == val2.value(bvo_second))
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

typedef QVector<SqlField> SqlFieldsVector;

////////////////////////////////////////////////////////////////////////////////

}
#endif // SQLFIELD_H
