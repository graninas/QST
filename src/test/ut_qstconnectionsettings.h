/****************************************************************************
** QST 0.4.2a release
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

#ifndef UT_QSTCONNECTIONSETTINGS_H
#define UT_QSTCONNECTIONSETTINGS_H

#include <QObject>
#include <QTest>

#include "qst/qstconnectionsettings.h"

namespace QstTest
{

class ut_QstConnectionSettings : public QObject
{
	Q_OBJECT

public:
    ut_QstConnectionSettings();

private slots:

	void initTestCase();

	void hostName();
	void hostName_data();

	void port();
	void port_data();

	void databaseName();
	void databaseName_data();

	void userName();
	void userName_data();

	void password();
	void password_data();

	void isNull();
	void isNull_data();

private:

	Qst::QstConnectionSettings m_cs1, m_cs2, m_cs3;
};

} // End of namespace QstTest

#endif // UT_QSTCONNECTIONSETTINGS_H
