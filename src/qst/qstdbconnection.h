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

#ifndef QSTDBCONNECTION_H
#define QSTDBCONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlDriver>

#include <QString>
#include <QMap>

#include "qstglobal.h"
#include "qstconnectionsettings.h"

namespace Qst
{

class QstDBConnection
{
private:

	typedef QMap<QString, QstConnectionSettings> _ConnectionSettingsMap;

	QSqlDatabase _db;

	_ConnectionSettingsMap _connectionSettingsMap;

	QString _driverName;

public:

	QstDBConnection(const QString &driverName = QST_DEFAULT_DATABASE_DRIVER);

	void setHostName(const QString &host,
					 const QString &connectionName = QString());

	void setPort(const int& port,
				 const QString &connectionName = QString());

	void setDatabaseName(const QString &databaseName,
						 const QString &connectionName = QString());

	void setUserName(const QString &userName,
					 const QString &connectionName = QString());

	void setPassword(const QString &password,
					 const QString &connectionName = QString());

	void setDriverName(const QString& driverName);

	bool isOpen() const;

	bool connect(const QString &userName,
				 const QString &password,
				 const QString &connectionName = QString());

	bool connect(const QString &hostName,
				 const QString &databaseName,
				 const QString &userName,
				 const QString &password,
				 const QString &connectionName = QString());

	bool connect(const QString &hostName,
				 const int &port,
				 const QString &databaseName,
				 const QString &userName,
				 const QString &password,
				 const QString &connectionName = QString());

	bool connect(const QstConnectionSettings &connectionSettings,
				 const QString &connectionName = QString());

	bool test(const QString &hostName,
			  const int &port,
			  const QString &databaseName,
			  const QString &userName,
			  const QString &password);

	bool test(const QString &hostName,
			  const QString &databaseName,
			  const QString &userName,
			  const QString &password);
	bool test(const QstConnectionSettings &connectionSettings);

	bool test(const QString &connectionName = QString());	

	QSqlError lastError() const;

	bool open(const QString &connectionName = QString());
	void close();

	bool addConnection(const QString &connectionName,
					   const QString &hostName,
					   const int &port,
					   const QString &databaseName,
					   const QString &userName,
					   const QString &password);

	bool addConnection(const QString &connectionName,
					   const QString &hostName,
					   const QString &databaseName,
					   const QString &userName,
					   const QString &password);

	bool addConnection(const QString &connectionName,
					   const QstConnectionSettings &connectionSettings);

	QSqlDatabase &rdb();
	QSqlDatabase *pdb();

	QstConnectionSettings connectionSettings(const QString &connectionName) const;

private:

	bool _open(const QString &connectionName = QString());
};

} // End of namespace Qst

#endif // QSTDBCONNECTION_H
