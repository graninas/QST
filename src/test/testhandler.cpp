/****************************************************************************
** QST 0.4.2a rc
** Copyright (C) 2010 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Test module of the QsT SQL Tools.
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

#include "testhandler.h"

using namespace Qst;

namespace QstTest
{

	/*!
		\class TestHandler
		\brief Класс-хэндлер, использующийся в модульном тесте ut_QstAbstractModelHandler.
	*/

TestHandler::TestHandler()
{
}

QstBatch TestHandler::_selector(const int &queryNumber) const
{
	QstBatch batch;

	if (queryNumber == TEST1)
	{
		batch	<< "table1"
				<< QstField(RolePrimaryKey, "ID")
				<< QstField("Name", FieldVisible, "Наименование", 120)

				<< QstField("ID", value(ID_VALUE))
				<< QstField("OrderDate", value("DateBegin"), value("DateEnd"))
				;
	}
	else
	{
		Q_ASSERT(false);
	}

	return batch;
}


} // End of namespace QstTest
