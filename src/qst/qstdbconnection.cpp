/****************************************************************************
** QST 0.4.2a release
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

#include "qstdbconnection.h"

#include <QStringList>

#include <QDebug>

namespace Qst
{

/*!
	\class QstDBConnection
	\brief
	С помощью QstDBConnection создаются и тестируются подключения к СУБД.
	Позволяет создавать несколько подключений с разными именами.
	Позволяет тестировать подключения.
	Класс является надстройкой над классом QSqlDatabase.

	\inmodule Qst
*/

/*! Конструктор по умолчанию. */
QstDBConnection::QstDBConnection(const QString &driverName)
	:
	_driverName(driverName)
{
}

/*! Устанавливает имя хоста. */
void QstDBConnection::setHostName(const QString& host,
								  const QString &connectionName)
{
	_connectionSettingsMap[connectionName].setHostName(host);
}

/*! Устанавливает порт. */
void QstDBConnection::setPort(const int &port,
							  const QString &connectionName)
{
	_connectionSettingsMap[connectionName].setPort(port);
}

/*! Устанавливает название базы данных или источника данных ODBC. */
void QstDBConnection::setDatabaseName(const QString &databaseName,
									  const QString &connectionName)
{
	_connectionSettingsMap[connectionName].setDatabaseName(databaseName);
}

/*! Устанавливает имя пользователя (login). */
void QstDBConnection::setUserName(const QString &userName,
								  const QString &connectionName)
{
	_connectionSettingsMap[connectionName].setUserName(userName);
}

/*! Устанавливает пароль. */
void QstDBConnection::setPassword(const QString &password,
								  const QString &connectionName)
{
	_connectionSettingsMap[connectionName].setPassword(password);
}

/*! Устанавливает драйвер подключения к СУБД. */
void QstDBConnection::setDriverName(const QString& driverName)
{
	_driverName = driverName;
}

/*! Проверяет подключение к базе данных. */
bool QstDBConnection::isOpen() const
{
	return _db.isOpen();
}

/*! Устанавливает имя пользователя и пароль, подключает к базе данных.

	Возвращает true, если подключение было успешным. */
bool QstDBConnection::connect(const QString &userName,
							  const QString &password,
							  const QString &connectionName)
{
	if (!test(_connectionSettingsMap[connectionName].hostName(),
			  _connectionSettingsMap[connectionName].databaseName(),
			  userName,
			  password)) return false;

	setUserName(userName, connectionName);
	setPassword(password, connectionName);

	return _open(connectionName);
}

/*! Устанавливает имя хоста, имя базы данных, имя пользователя и пароль. Подключает
  к базе данных.

  Возвращает true, если подключение было успешным. */
bool QstDBConnection::connect(const QString &hostName,
							  const QString &databaseName,
							  const QString &userName,
							  const QString &password,
							  const QString &connectionName)
{
	if (!test(hostName, databaseName, userName, password)) return false;

	setHostName(hostName, connectionName);
	setPort(-1, connectionName);
	setDatabaseName(databaseName, connectionName);
	setUserName(userName, connectionName);
	setPassword(password, connectionName);

	return _open(connectionName);
}

/*! Устанавливает имя хоста, порт, имя базы данных, имя пользователя и пароль.
	Подключает к базе данных.

  Возвращает true, если подключение было успешным. */
bool QstDBConnection::connect(const QString &hostName,
							  const int &port,
							  const QString &databaseName,
							  const QString &userName,
							  const QString &password,
							  const QString &connectionName)
{
	if (!test(hostName, databaseName, userName, password)) return false;

	setHostName(hostName, connectionName);
	setPort(port, connectionName);
	setDatabaseName(databaseName, connectionName);
	setUserName(userName, connectionName);
	setPassword(password, connectionName);

	return _open(connectionName);
}

bool QstDBConnection::connect(const QstConnectionSettings &connectionSettings,
							  const QString &connectionName)
{
	_connectionSettingsMap[connectionName] = connectionSettings;

	if (!test(connectionName)) return false;

	return _open(connectionName);
}

/*! Возвращает последнюю ошибку QSqlDatabase. */
QSqlError QstDBConnection::lastError() const
{
	return _db.lastError();
}

/*! Тестирует переданные параметры на подключение к базе данных.

	После тестирования подключение, в общем случае, удаляется.

	Возвращает true, если подключение было успешным.*/
bool QstDBConnection::test(const QString &hostName,
						   const int &port,
						   const QString &databaseName,
						   const QString &userName,
						   const QString &password)
{
	QSqlDatabase testDB;

#ifdef QT_DEBUG
	qDebug() << "Available drivers:";
	QStringList drivers = QSqlDatabase::drivers();
	foreach(QString driver, drivers)	qDebug() << "\t" << driver;
#endif

	testDB = QSqlDatabase::addDatabase(_driverName, "TestConnection");

#ifdef QT_DEBUG
	qDebug() << "driver valid?" << testDB.isValid();
#endif

	testDB.setHostName(hostName);
	testDB.setDatabaseName(databaseName);
	testDB.setUserName(userName);
	testDB.setPassword(password);

	if (port != -1)
		testDB.setPort(port);

	testDB.open();

	if (!testDB.isOpen())
	{
#ifdef QT_DEBUG
		qDebug() << "Test failed. Error message: " << testDB.lastError().text();
#endif
	return false;
	};

#ifdef QT_DEBUG
	qDebug() << "Test success.";
#endif

testDB.close();
QSqlDatabase::removeDatabase("TestConnection");
return true;
}

/*! Тестирует переданные параметры на подключение к базе данных.

	После тестирования подключение, в общем случае, удаляется.

	Порт не устанавливается.

	Возвращает true, если подключение было успешным.*/
bool QstDBConnection::test(const QString &hostName,
						   const QString &databaseName,
						   const QString &userName,
						   const QString &password)
{
	return test(hostName, -1, databaseName, userName, password);
}

bool QstDBConnection::test(const QstConnectionSettings &connectionSettings)
{
	return test(connectionSettings.hostName(),
				connectionSettings.port(),
				connectionSettings.databaseName(),
				connectionSettings.userName(),
				connectionSettings.password());
}

/*! Тестирует установленные ранее параметры на подключение к БД.

	После тестирования подключение, в общем случае, удаляется.

	Возвращает true, если подключение было успешным.*/
bool QstDBConnection::test(const QString &connectionName)
{
	return test(_connectionSettingsMap[connectionName].hostName(),
				_connectionSettingsMap[connectionName].port(),
				_connectionSettingsMap[connectionName].databaseName(),
				_connectionSettingsMap[connectionName].userName(),
				_connectionSettingsMap[connectionName].password());
}

/*! Открывает подключение к БД с ранее установленными параметрами. */
bool QstDBConnection::open(const QString &connectionName)
{
	return _open(connectionName);
}

/*! Закрывает подключение к БД. */
void QstDBConnection::close()
{
	if (_db.isOpen()) _db.close();
}

/*! Добавляет и открывает новое подключение к БД.

	Возвращает true, если если подключение было успешным. */
bool QstDBConnection::addConnection(const QString &connectionName,
									const QString &hostName,
									const int &port,
									const QString &databaseName,
									const QString &userName,
									const QString &password)
{
	setHostName(hostName, connectionName);
	setPort(port, connectionName);
	setDatabaseName(databaseName, connectionName);
	setUserName(userName, connectionName);
	setPassword(password, connectionName);

	return _open(connectionName);
}

bool QstDBConnection::addConnection(const QString &connectionName,
									const QstConnectionSettings &connectionSettings)
{
	return addConnection(connectionName,
						 connectionSettings.hostName(),
						 connectionSettings.port(),
						 connectionSettings.databaseName(),
						 connectionSettings.userName(),
						 connectionSettings.password());
}

/*! Добавляет и открывает новое подключение к БД.

	Возвращает true, если если подключение было успешным. */
bool QstDBConnection::addConnection(const QString &connectionName,
									const QString &hostName,
									const QString &databaseName,
									const QString &userName,
									const QString &password)
{
	setHostName(hostName, connectionName);
	setPort(-1, connectionName);
	setDatabaseName(databaseName, connectionName);
	setUserName(userName, connectionName);
	setPassword(password, connectionName);

	return _open(connectionName);
}

/*! Возвращает ссылку на объект QSqlDatabase. */
QSqlDatabase & QstDBConnection::rdb()
{
	return 	_db;
}

/*! Возвращает указатель на объект QSqlDatabase. */
QSqlDatabase * QstDBConnection::pdb()
{
	return &_db;
}

/*! Возвращает настройки подключения по имени подключения. */
QstConnectionSettings QstDBConnection::connectionSettings(const QString &connectionName) const
{
	if (_connectionSettingsMap.contains(connectionName))
		return _connectionSettingsMap[connectionName];
return QstConnectionSettings();
}


bool QstDBConnection::_open(const QString &connectionName)
{
#ifdef QT_DEBUG
	qDebug() << "Available drivers:";
	QStringList drivers = QSqlDatabase::drivers();
	foreach(QString driver, drivers)	qDebug() << "\t" << driver;
#endif

	if (connectionName.isEmpty())
			_db = QSqlDatabase::addDatabase(_driverName);
	else
			_db = QSqlDatabase::addDatabase(_driverName, connectionName);

#ifdef QT_DEBUG
	qDebug() << "driver valid? " << _db.isValid();
	qDebug() << "Driver has QuerySize? " << _db.driver()->hasFeature(QSqlDriver::QuerySize);
#endif

	_db.setHostName(_connectionSettingsMap[connectionName].hostName());
	_db.setDatabaseName(_connectionSettingsMap[connectionName].databaseName());
	_db.setUserName(_connectionSettingsMap[connectionName].userName());
	_db.setPassword(_connectionSettingsMap[connectionName].password());

	if (_connectionSettingsMap[connectionName].port() != -1)
		_db.setPort(_connectionSettingsMap[connectionName].port());

#ifdef QT_DEBUG
	qDebug() << "hostName:" << _connectionSettingsMap[connectionName].hostName();
	qDebug() << "port:" << _connectionSettingsMap[connectionName].port();
	qDebug() << "databaseName:" << _connectionSettingsMap[connectionName].databaseName();
	qDebug() << "userName:" << _connectionSettingsMap[connectionName].userName();
	qDebug() << "password:" << _connectionSettingsMap[connectionName].password();
#endif

	_db.open();

	if (!_db.isOpen())
	{
#ifdef QT_DEBUG
		qDebug() << "DB open failed. Error message: " << _db.lastError().text();
#endif
	return false;
	}
	else
	{
#ifdef QT_DEBUG
	qDebug() << "Success DB open.";
	qDebug() << "ConnectionName:" << connectionName;
#endif
	return true;
	}
}

} // End of namespace Qst
