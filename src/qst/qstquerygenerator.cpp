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

#include "qstquerygenerator.h"
#include "qstquerycomposer.h"

namespace Qst
{


QstQueryGenerator::QstQueryGenerator()
	:
	_batch(QstBatch()),
	_queryType(QuerySelect)
{
}

QstQueryGenerator::QstQueryGenerator(const QstBatch &batch, const QueryType &queryType)
	:
	_batch(batch),
	_queryType(queryType)
{
}




QstBatch QstQueryGenerator::batch() const
{
	return _batch;
}

QueryType QstQueryGenerator::queryType() const
{
	return _queryType;
}

bool QstQueryGenerator::isValid() const
{
	return _batch.isValid();
}

QString QstQueryGenerator::query() const
{
	if (!isValid())
		return QString();

	QstQueryComposer composer;

	QStringList sources = _batch.sources();

	for (int i = 0; i < sources.size(); ++i)
		composer.addSource(sources[i]);

	QstFieldsVector selectClauseFields = _batch.fields(PurposeSelect);
	QstFieldsVector stuffFields = _batch.fields(PurposeAllButSelect_Mask);

	for (int i = 0; i < selectClauseFields.size(); ++i)
		composer.addSelectClauseField(selectClauseFields[i]);

	for (int i = 0; i < stuffFields.size(); ++i)
		composer.addStuffField(stuffFields[i]);

return composer.query(_queryType);
}


}
