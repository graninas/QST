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

#ifndef SQLQUERYDESCRIPTOR_H
#define SQLQUERYDESCRIPTOR_H

#include "sqlglobal.h"
#include "sqlbatch.h"


namespace Sql
{

class SqlQueryDescriptor
{
private:

	SqlBatch		_batch;
	SqlQueryTypes	_type;
	int				_queryNumber;


public:
	SqlQueryDescriptor(const SqlBatch &batch,
					   const SqlQueryTypes &type,
					   const int &queryNumber);

	SqlQueryDescriptor();

	SqlBatch		batch() const;
	void			setBatch(const SqlBatch &batch);

	SqlQueryTypes	type() const;
	void			setType(const SqlQueryTypes &type);

	int				queryNumber() const;
	void			setQueryNumber(const int &queryNumber);

};


};

#endif // SQLQUERYDESCRIPTOR_H
