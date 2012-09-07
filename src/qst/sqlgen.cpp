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

#include "sqlgen.h"

#include "sqlquerycomposer.h"

namespace Sql
{

/*!
	\class SqlGen
	\brief
	Класс отвечает за генерацию запросов на основе пакета SqlBatch и
	типа запроса.

	\inmodule Sql

	За примерами генерации запросов обращаться в модульный тест ut_sqlgen.cpp .

	\attention В QST версии 0.4 класс переработан. Изменено его имя.

	\sa SqlQueryComposer, SqlBatch, SqlField, SqlValue */

/*! Конструктор по умолчанию. */
SqlGen::SqlGen()
	:
	_batch(SqlBatch()),
	_queryType(sql_select)
{
}

/*! Основной конструктор. */
SqlGen::SqlGen(const SqlBatch &batch, const SqlQueryTypes &queryType)
	:
	_batch(batch),
	_queryType(queryType)
{
}

/*! Генерирует и возвращает SQL-запрос.

	Передает все необходимые данные компоновщику SqlQueryComposer,
	инициирует компоновку и получает от компоновщика готовый запрос.

	\sa SqlQueryComposer*/
QString SqlGen::query() const
{
	if (!isValid())
		return QString();

	SqlQueryComposer composer;

	QStringList sources = _batch.sources();

	for (int i = 0; i < sources.size(); ++i)
		composer.addSource(sources[i]);

	SqlFieldsVector selectClauseFields = _batch.selectClauseFields();
	SqlFieldsVector stufFields = _batch.stuffFields();

	for (int i = 0; i < selectClauseFields.size(); ++i)
		composer.addSelectClauseField(selectClauseFields[i]);

	for (int i = 0; i < stufFields.size(); ++i)
		composer.addStuffField(stufFields[i]);

return composer.query(_queryType);
}

/*! Возвращает сохраненный в классе пакет SqlBatch. */
SqlBatch SqlGen::batch() const
{
	return _batch;
}

/*! Возвращает сохраненный в классе тип SQL-запроса. */
SqlQueryTypes SqlGen::queryType() const
{
	return _queryType;
}

/*! Возвращает истину, если пакет SqlBatch валиден.

	\sa SqlBatsh::isValid() */
bool SqlGen::isValid() const
{
	return _batch.isValid();
}

}
