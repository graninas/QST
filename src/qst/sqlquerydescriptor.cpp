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

#include "sqlquerydescriptor.h"


namespace Sql
{


/*!
	\class SqlQueryDescriptor
	\brief
	Хранит всю необходимую информацию для генерации конкретного SQL-запроса.

	\inmodule Sql

	Чтобы сгенерировать SQL-запрос в QST, нужны SqlBatch, заполненный
	полями SqlField, и вид SQL-запроса (SELECT, UPDATE, INSERT и т.д.). Кроме того,
	если в классе-хэндлере описаны несколько вариантов запроса, то они имеют свой
	номер - queryNumber, - он нужен функциям AbstractModelHandler, чтобы пересобирать
	нужный запрос с новыми параметрами в любое время. Вся эта информация
	(SqlBatch, SqlQueryTypes, queryNumber) хранится в SqlQueryDescriptor'е.

	Заполненный SqlQueryDescriptor возвращается DFD-генерирующими функциями
	_selector(), _inserter(), _updater(), _deleter() и _executor().

	\attention В QST версии 0.4 во многом переработана концепция DFD,
	в том числе, DFD-генерирующие функции возвращают только SqlBatch.
	Переименованы некоторые функции класса SqlQueryDescriptor, изменено его имя. */

	/*! Основной конструктор. */
	SqlQueryDescriptor::SqlQueryDescriptor(const SqlBatch &batch,
										   const SqlQueryTypes &type,
										   const int &queryNumber)
		:
		_batch(batch),
		_type(type),
		_queryNumber(queryNumber)
	{
	}

	/*! Конструктор по умолчанию. */
	SqlQueryDescriptor::SqlQueryDescriptor()
	 : _batch(SqlBatch()),
	 _type(sql_select),
	 _queryNumber(0)
	{
	}


	/*! Возвращает пакет SqlBatch. */
	SqlBatch SqlQueryDescriptor::batch() const
	{
		return _batch;
	}

	/*! Устанавливает пакет SqlBatch. */
	void SqlQueryDescriptor::setBatch(const SqlBatch &batch)
	{
		_batch = batch;
	}

	/*! Возвращает тип запроса. */
	SqlQueryTypes SqlQueryDescriptor::type() const
	{
		return _type;
	}

	/*! Устанавливает тип запроса. */
	void SqlQueryDescriptor::setType(const SqlQueryTypes &type)
	{
		_type = type;
	}

	/*! Возвращает номер запроса в DFD-генерирующей функции. */
	int SqlQueryDescriptor::queryNumber() const
	{
		return _queryNumber;
	}

	/*! Устанавливает номер запроса в DFD-генерирующей функции. */
	void SqlQueryDescriptor::setQueryNumber(const int &queryNumber)
	{
		_queryNumber = queryNumber;
	}
}
