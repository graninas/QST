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

#ifndef QSTQUERYDESCRIPTOR_H
#define QSTQUERYDESCRIPTOR_H

#include "qstglobal.h"
#include "qstbatch.h"

namespace Qst
{

namespace QstSpecial
{

class QstQueryDescriptor
{
private:

	QstBatch	_batch;
	QueryType	_type;
	int			_queryNumber;


public:
	QstQueryDescriptor();

	QstQueryDescriptor(const QstBatch &batch,
					   const QueryType &type,
					   const int &queryNumber);

	QstBatch		batch() const;
	void			setBatch(const QstBatch &batch);

	QueryType	queryType() const;
	void			setQueryType(const QueryType &type);

	int				queryNumber() const;
	void			setQueryNumber(const int &queryNumber);

};


}	// End of namespace QstSpecial

}	// End of namespace Qst

#endif // QSTQUERYDESCRIPTOR_H
