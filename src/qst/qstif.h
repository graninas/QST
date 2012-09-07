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

#ifndef QSTIF_H
#define QSTIF_H

#include <QtGlobal>

namespace Qst
{

	/*!
		\class QstIf
		\brief
		Шаблонный класс. Предназначен для выбора объектов по условию.
		В принимающем классе определяется, что делать с объектом, возвращаемым
		по условиям true или false, а так же что делать, если не установлены
		истинное или ложное значение возвращаемого объекта. Наиболее близкая
		аналогия - оператор ?: .

		\inmodule Qst

		Класс позволяет уместить многострочный код в одну строку.
\code
// Было:
class T;

	if (condition)
		receiver.addClass(T(OneCase))
	else
		receiver.addClass(T(OtherCase))
\endcode
\code
// Стало:
class T;
QstIf<T> QstIf_T;

	receiver.addClass(QstIf_T(condition, T(OneCase), T(OtherCase)))
\endcode

		В приемнике должна быть описана реакция на переданный объект QstIf_T.
	*/

template <class Type> class QstIf
{
public:

	typedef Type T;

private:

	bool	_hasTrueValue;
	bool	_hasFalseValue;

	bool	_condition;
	Type	_trueValue;
	Type	_falseValue;

public:
	/*! Конструктор по умолчанию. */
	QstIf()
		:
		_hasTrueValue(false),
		_hasFalseValue(false),
		_condition(false)
	{
	}

	/*! Конструктор, принимающий только истинное значение объекта Type. */
	QstIf(const bool &cond,
		  const Type &trueValue)
			  :
			  _hasTrueValue(true),
			  _hasFalseValue(false),
			  _condition(cond),
			  _trueValue(trueValue)
	{
	}

	/*! Конструктор, принимающий как истинное, так и ложное значение объекта Type. */
	QstIf(const bool &cond,
		  const Type &trueValue,
		  const Type &falseValue)
			  :
			  _hasTrueValue(true),
			  _hasFalseValue(true),
			  _condition(cond),
			  _trueValue(trueValue),
			  _falseValue(falseValue)
	{
	}

	/*! Возвраащет истинное значение, если cond == true и ложное, если cond == false.

		Если значение не установлено, возвращает значение по умолчанию. */
	Type value() const
	{
		if (_condition && _hasTrueValue)
			return _trueValue;

		if (!_condition && _hasFalseValue)
			return _falseValue;

	return Type();
	}

	/*! Возвращает истинное значение вне зависимости от состояния cond.

	  \attention Значение должно быть установлено.*/
	Type trueValue() const
	{
		Q_ASSERT(_hasTrueValue);
		return _trueValue;
	}

	/*! Возвращает ложное значение вне зависимости от состояния cond.

	  \attention Значение должно быть установлено.*/
	Type falseValue() const
	{
		Q_ASSERT(_hasFalseValue);
		return _falseValue;
	}

	/*! Возвращает true, если истинное значение установлено. */
	bool hasTrueValue() const
	{
		return _hasTrueValue;
	}

	/*! Возвращает true, если ложное значение установлено. */
	bool hasFalseValue() const
	{
		return _hasFalseValue;
	}

	/*! Возвращает условие. */
	bool condition() const
	{
		return _condition;
	}
};


}

#endif // QSTIF_H
