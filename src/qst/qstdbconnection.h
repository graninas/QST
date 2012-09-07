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

#ifndef QSTDBCONNECTION_H
#define QSTDBCONNECTION_H

#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QSqlDriver>

#include <QString>

#include <QDebug>

class QstDBConnection
{
private:

	QSqlDatabase _db;
	QString _hostName;
	QString _databaseName;
	QString	_userName;
	QString _password;

	QSqlError _lastError;

	QString _driverName;
	bool _open(const QString &connectionName = QString());

public:

	void setHostName(const QString& hostName);
	void setUserName(const QString& userName);
	void setDatabaseName(const QString& databaseName);
	void setPassword(const QString& password);

	void setDriverName(const QString& driverName);

	bool isOpen() const;

	bool connect(const QString& userName, const QString& password, const QString &connectionName = QString());

	bool connect(const QString& hostName, const QString& databaseName,
		 const QString& userName, const QString& password, const QString &connectionName = QString());

	bool test(const QString& hostName, const QString& databaseName,
		 const QString& userName, const QString& password);

	bool test();
	QSqlError lastError() const;

	bool open(const QString &connectionName = QString());
	void close();

	bool addConnection(const QString &connectionName,
					   const QString& hostName,
					   const QString& databaseName,
					   const QString& userName,
					   const QString& password);

	QSqlDatabase &rdb();
	QSqlDatabase *pdb();

	QstDBConnection();
};

#endif // QSTDBCONNECTION_H
