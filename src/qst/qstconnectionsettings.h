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

#ifndef QSTCONNECTIONSETTINGS_H
#define QSTCONNECTIONSETTINGS_H

#include <QString>

namespace Qst
{

class QstConnectionSettings
{
private:

	QString _hostName;
	int		_port;
	QString _databaseName;
	QString _userName;
	QString _password;

public:
	QstConnectionSettings();
	QstConnectionSettings(const QString &hostName,
						  const int &port,
						  const QString &databaseName,
						  const QString &userName,
						  const QString &password);

	QstConnectionSettings(const QString &hostName,
						  const QString &databaseName,
						  const QString &userName,
						  const QString &password);

	QString hostName() const;
	int		port() const;
	QString databaseName() const;
	QString userName() const;
	QString password() const;

	void setHostName(const QString &hostName);
	void setPort(const int &port);
	void setDatabaseName(const QString &databaseName);
	void setUserName(const QString &userName);
	void setPassword(const QString &password);

	bool isNull() const;
};


}

#endif // QSTCONNECTIONSETTINGS_H

