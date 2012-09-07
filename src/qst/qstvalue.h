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

#ifndef QSTVALUE_H
#define QSTVALUE_H

#include "qstglobal.h"
#include <QVariant>
#include <QVector>
#include <QMap>
#include <QDate>

namespace Qst
{

class QstValue
{
public:

	// Основной конструктор.
	// Создает invalid QstValue, если не указать параметров.
	QstValue(const QVariant &value = QVariant(),
			 const CompareFunctor &functor = FunctorNone,
			 const FuzzyBraces &braces = BracesNone);

	// Конструктор с расширенной функциональностью для создания фильтров
	// на формах, где требуется точное и неточное совпадение строк.
	// В отличие от предыдущего конструктора создает QstValue только тогда, когда
	// enable = true. В качестве enabled может быть передан параметр isChecked
	// от QComboBox.
	QstValue(const bool &enabled,
			 const QString &valueString,
			 const bool &fullStringMatch,
			 const CompareFunctor &functor = FunctorLike,
			 const FuzzyBraces &braces = BracesBoth,
			 const MatchPolicy &matchPolicy = MatchFlagDependent);

	// Создает QstValue на основе другого, при этом изменяет functor и braces.
	QstValue(const QstValue &other,
			 const CompareFunctor &functor,
			 const FuzzyBraces &braces = BracesNone);

	// Конструктор копирования.
	QstValue(const QstValue &other);

	// QVariant-like конструктор для конструирования NULL-значений.
	QstValue(const NullType &null);

	QVariant	value() const;
	CompareFunctor		functor() const;
	FuzzyBraces	braces() const;

	QString		toString(const ValueBordering &bordering = ValueNotBordered,
						 const FuzzyBracesUsage &useBraces = BracesNotUse,
#ifdef QST_VALUE_NULL_SUBSTITUTE_BY_DEFAULT
						 const NullSubstitution  &nullSubstitute = NullSubstitute
#else
						 const NullSubstitution  &nullSubstitute = NullNotSubstitute
#endif
						 ) const;

	void setValue(const QVariant &value);
	void setFunctor(const CompareFunctor &functor);
	void setBraces(const FuzzyBraces &braces);

	bool isNull() const;
	bool isValid() const;

private:

	QVariant		_value;

	CompareFunctor	_functor;
	FuzzyBraces		_braces;

	QString _toString(const QString &strVal,
					  const ValueBordering &bordering,
					  const FuzzyBracesUsage &useBraces) const;

	QString _toDateString(const QDate &date,
						  const ValueBordering &bordering,
						  const FuzzyBracesUsage &useBraces) const;

	QString _toEmptyString(const ValueBordering &bordering,
						   const NullSubstitution &fillSqlNull,
						   const FuzzyBracesUsage &useBraces) const;

	QString _toInvalidString(const ValueBordering &bordering,
						   const NullSubstitution &fillSqlNull,
						   const FuzzyBracesUsage &useBraces) const;

	QString _bracesToStringTemplate(const FuzzyBracesUsage &useBraces) const;

	QString _borderedEmptyString(const NullSubstitution &fillSqlNull,
								 const FuzzyBracesUsage &useBraces) const;

	QString _notBorderedEmptyString(const NullSubstitution &fillSqlNull,
									const FuzzyBracesUsage &useBraces) const;

	QString _borderedNullFilledEmptyString(const FuzzyBracesUsage &useBraces) const;
	QString _borderedNotNullFilledEmptyString(const FuzzyBracesUsage &useBraces) const;

	QString _notBorderedNullFilledEmptyString(const FuzzyBracesUsage &useBraces) const;
	QString _notBorderedNotNullFilledEmptyString(const FuzzyBracesUsage &useBraces) const;

public:
	friend bool operator < (const QstValue &val1, const QstValue &val2)
	{
		return	(val1.value().toString() < val2.value().toString())
				&&
				(val1.functor() < val2.functor())
				&&
				(val1.braces() < val2.braces());
	}

	friend bool operator == (const QstValue &val1, const QstValue &val2)
	{
		return	(val1.value() == val2.value())
				&&
				(val1.functor() == val2.functor())
				&&
				(val1.braces() == val2.braces());
	}
};

////////////////////////////////////////////////////////////////////////////////

typedef QVector<QstValue>			QstValueVector;
typedef QMap<QString, QstValue>		QstValueMap;

////////////////////////////////////////////////////////////////////////////////


}


#endif // QSTVALUE_H
