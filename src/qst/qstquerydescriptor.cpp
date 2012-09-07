/****************************************************************************
** QST 0.4.2a rc
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

#include "qstquerydescriptor.h"

namespace Qst
{

namespace QstSpecial
{

	/*!
	\class QstQueryDescriptor
	\brief
		Служебный класс, используемый в QstAbstractModelHandler для хранения
		описателей. Представляет собой полный описатель SQL-запроса
		(пакет QstBatch + номер пакета в хэндлере + вид запроса QueryType).

	\inmodule Qst
	\inmodule QstSpecial
	*/

	/*! Конструктор по умолчанию. */
	QstQueryDescriptor::QstQueryDescriptor()
	 : _batch(QstBatch()),
	 _type(QuerySelect),
	 _queryNumber(0)
	{
	}

	/*! Основной конструктор. */
	QstQueryDescriptor::QstQueryDescriptor(const QstBatch &batch,
										   const QueryType &type,
										   const int &queryNumber)
		:
		_batch(batch),
		_type(type),
		_queryNumber(queryNumber)
	{
	}

	/*! Возвращает пакет QstBatch. */
	QstBatch QstQueryDescriptor::batch() const
	{
		return _batch;
	}

	/*! Устанавливает пакет QstBatch. */
	void QstQueryDescriptor::setBatch(const QstBatch &batch)
	{
		_batch = batch;
	}

	/*! Возвращает тип запроса. */
	QueryType QstQueryDescriptor::queryType() const
	{
		return _type;
	}

	/*! Устанавливает тип запроса. */
	void QstQueryDescriptor::setQueryType(const QueryType &type)
	{
		_type = type;
	}

	/*! Возвращает номер пакета в классе-хэндлере. */
	int QstQueryDescriptor::queryNumber() const
	{
		return _queryNumber;
	}

	/*! Устанавливает номер пакета в классе-хэндлере. */
	void QstQueryDescriptor::setQueryNumber(const int &queryNumber)
	{
		_queryNumber = queryNumber;
	}

}	// End of namespace QstSpecial

}	// End of namespace Qst
