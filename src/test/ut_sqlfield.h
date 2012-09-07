/****************************************************************************
** QST 0.3c beta
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

#ifndef UT_SQLFIELD_H
#define UT_SQLFIELD_H

#include <QObject>
#include <QTest>

#include "qst/sqlfield.h"
using namespace Sql;

class ut_SqlField : public QObject
{
	Q_OBJECT

public:
    ut_SqlField();

private slots:

	void name();
	void name_data();

	void visibility();
	void visibility_data();

	void columnTitle();
	void columnTitle_data();

	void orientation();
	void orientation_data();

	void purposes();
	void purposes_data();

	void role();
	void role_data();

	void value();
	void value_data();

	void value1();
	void value1_data();

	void value2();
	void value2_data();

	void hasValue();
	void hasValue_data();

	void hasValue1();
	void hasValue1_data();

	void hasValue2();
	void hasValue2_data();

	void isBinaryFilter();
	void isBinaryFilter_data();

	void isVisible();
	void isVisible_data();

	void isInvisible();
	void isInvisible_data();

	void isNoneVisibility();
	void isNoneVisibility_data();

	void isService();
	void isService_data();

	void isSelectClauseField();
	void isSelectClauseField_data();

	void isStuffField();
	void isStuffField_data();

	void isValuesValid();
	void isValuesValid_data();

	void isValid();
	void isValid_data();
};

#endif // UT_SQLFIELD_H
