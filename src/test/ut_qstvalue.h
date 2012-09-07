/****************************************************************************
** QST 0.4.1 pre-alpha
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

#ifndef UT_QSTVALUE_H
#define UT_QSTVALUE_H

#include <QObject>
#include <QTest>

#include "qst/qstvalue.h"

using namespace Qst;


class ut_QstValue : public QObject
{
	Q_OBJECT

public:
	ut_QstValue();

private slots:

	void isValid();
	void isValid_data();

	void isNull();
	void isNull_data();

	void value();
	void value_data();

	void functor();
	void functor_data();

	void braces();
	void braces_data();

	void toString();		// ValueUnbordered,		NullFilled, NotUseBraces
	void toString_data();

	void toString1();		// ValueBordered, NullFilled, NotUseBraces
	void toString1_data();

	void toString2();		// ValueUnbordered,		NotNullFilled, NotUseBraces
	void toString2_data();

	void toString3();		// ValueBordered, NotNullFilled, NotUseBraces
	void toString3_data();

// -------------------------------------------------------------------------- //

	void toString4();		// ValueUnbordered,		NullFilled, UseBraces
	void toString4_data();

	void toString5();		// ValueBordered, NullFilled, UseBraces
	void toString5_data();

	void toString6();		// ValueUnbordered,		NotNullFilled, UseBraces
	void toString6_data();

	void toString7();		// ValueBordered, NotNullFilled, UseBraces
	void toString7_data();

};

#endif // UT_QSTVALUE_H
