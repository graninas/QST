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

#ifndef SQLGEN_H
#define SQLGEN_H

#include "sqlbatch.h"
#include "sqlglobal.h"

namespace Sql
{

class SqlGen
{
public:
	SqlGen();
	SqlGen(const SqlBatch &batch, const SqlQueryTypes &queryType = sql_select);

	QString query() const;

	SqlBatch batch() const;
	SqlQueryTypes queryType() const;

	bool isValid() const;

private:

	SqlBatch		_batch;
	SqlQueryTypes	_queryType;
};

};
#endif // SQLGEN_H
