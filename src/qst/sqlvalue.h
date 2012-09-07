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

#ifndef SQLVALUE_H
#define SQLVALUE_H

#include "sqlglobal.h"
#include <QVariant>
#include <QVector>
#include <QDate>

namespace Sql
{

class SqlValue
{
public:

	// Основной конструктор.
	// Создает invalid SqlValue, если не указать параметров.
	SqlValue(const QVariant &value = QVariant(),
			 const Functor &functor = fo_none,
			 const FuzzyBraces &braces = fb_none);

	// Создает SqlValue на основе другого, при этом изменяет functor и braces.
	SqlValue(const SqlValue &other,
			 const Functor &functor,
			 const FuzzyBraces &braces = fb_none);

	// Конструктор копирования.
	SqlValue(const SqlValue &other);

	// QVariant-like конструктор для конструирования NULL-значений.
	SqlValue(const NullType &null);

	// Конструктор, позволяющий обойти обрамление значения
	// при компоновке UPDATE/SET в SqlQueryComposer.
	SqlValue(const SqlValue &other,
			 const SqlValueBorderIgnoring &ignore);
	SqlValue(const QVariant &value,
			 const SqlValueBorderIgnoring &ignore,
			 const Functor &functor = fo_none,
			 const FuzzyBraces &braces = fb_none);


	QVariant	value() const;
	Functor		functor() const;
	FuzzyBraces	braces() const;

	SqlValueBorderIgnoring borderIgnoring() const;

	QString		toString(const SqlValueBordering &bordering = vb_none,
						 const SqlValueFillNull  &fillSqlNull = vfn_fill,
						 const SqlValueUseBraces &useBraces = vub_not) const;

	QVariant	&rValue();

	void setValue(const QVariant &value);
	void setFunctor(const Functor &functor);
	void setBraces(const FuzzyBraces &braces);

	bool isNull() const;
	bool isValid() const;

	bool isIgnoreBordering() const;

private:

	QVariant _value;

	Functor		_functor;
	FuzzyBraces	_braces;

	SqlValueBorderIgnoring _ignore;

	QString _toString(const QString &strVal,
					  const SqlValueBordering &bordering,
					  const SqlValueUseBraces &useBraces) const;

	QString _toDateString(const QDate &date,
						  const SqlValueBordering &bordering,
						  const SqlValueUseBraces &useBraces) const;

	QString _toEmptyString(const SqlValueBordering &bordering,
						   const SqlValueFillNull &fillSqlNull,
						   const SqlValueUseBraces &useBraces) const;

	QString _toInvalidString(const SqlValueBordering &bordering,
						   const SqlValueFillNull &fillSqlNull,
						   const SqlValueUseBraces &useBraces) const;

	QString _bracesToStringTemplate(const SqlValueUseBraces &useBraces) const;

	QString _borderedEmptyString(const SqlValueFillNull &fillSqlNull,
								 const SqlValueUseBraces &useBraces) const;

	QString _notBorderedEmptyString(const SqlValueFillNull &fillSqlNull,
									const SqlValueUseBraces &useBraces) const;

	QString _borderedNullFilledEmptyString(const SqlValueUseBraces &useBraces) const;
	QString _borderedNotNullFilledEmptyString(const SqlValueUseBraces &useBraces) const;

	QString _notBorderedNullFilledEmptyString(const SqlValueUseBraces &useBraces) const;
	QString _notBorderedNotNullFilledEmptyString(const SqlValueUseBraces &useBraces) const;

public:
	friend bool operator < (const SqlValue &val1, const SqlValue &val2)
	{
		return	(val1.value().toString() < val2.value().toString())
				&&
				(val1.functor() < val2.functor())
				&&
				(val1.braces() < val2.braces())
				&&
				(val1.borderIgnoring() < val2.borderIgnoring());
	}

	friend bool operator == (const SqlValue &val1, const SqlValue &val2)
	{
		return	(val1.value() == val2.value())
				&&
				(val1.functor() == val2.functor())
				&&
				(val1.braces() == val2.braces())
				&&
				(val1.borderIgnoring() == val2.borderIgnoring());
	}
};

////////////////////////////////////////////////////////////////////////////////

typedef QVector<SqlValue>			SqlValuesVector;
typedef QMap<QString, SqlValue>		SqlValuesMap;

////////////////////////////////////////////////////////////////////////////////


}


#endif // SQLVALUE_H
