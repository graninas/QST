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

#include "dbconnection.h"

#include <QStringList>

/*!
	\class DBConnection
	\brief Позволяет создать подключение к СУБД.
*/

bool DBConnection::_open(const QString &connectionName)
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

	_db.setHostName(_hostName);
	_db.setDatabaseName(_databaseName);
	_db.setUserName(_userName);
	_db.setPassword(_password);

#ifdef QT_DEBUG
	qDebug() << "hostName:" << _hostName;
	qDebug() << "databaseName:" << _databaseName;
	qDebug() << "userName:" << _userName;
	qDebug() << "password:" << _password;
#endif

	_db.open();
//	_lastError = _db.lastError();

	if (!_db.isOpen())
	{
#ifdef QT_DEBUG
		qDebug() << "DB open failed. Error message: " << _lastError.text();
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

/*! Устанавливает название хоста. */
void DBConnection::setHostName(const QString& hostName)
{
	_hostName = hostName;
}

/*! Устанавливает имя пользователя. */
void DBConnection::setUserName(const QString& userName)
{
	_userName = userName;
}

/*! Устанавливает имя базы данных. */
void DBConnection::setDatabaseName(const QString& databaseName)
{
	_databaseName = databaseName;
}

/*! Устанавливает пароль пользователя. */
void DBConnection::setPassword(const QString& password)
{
	_password = password;
}

/*! Устанавливает имя драйвера подключения к БД. */
void DBConnection::setDriverName(const QString& driverName)
{
	_driverName = driverName;
}

/*! Возвращает истину, если текущее подключение к БД открыто. */
bool DBConnection::isOpen() const
{
	return _db.isOpen();
}

/*! Подключается к БД с заданными именем пользователя и паролем. Предполагается,
	что имена БД и хоста уже указаны функциями setDatabaseName и setHostName().

	При необходимости можно указать название подключения. */
bool DBConnection::connect(const QString& userName, const QString& password,
						   const QString &connectionName)
{
	if (!test(_hostName, _databaseName, userName, password)) return false;

	_userName = userName;
	_password = password;

	return _open(connectionName);
}

/*! Подключается к БД с заданными параметрами.

	При необходимости можно указать название подключения. */
bool DBConnection::connect(const QString& hostName, const QString& databaseName,
		 const QString& userName, const QString& password,
		 const QString &connectionName)
{
	if (!test(hostName, databaseName, userName, password)) return false;

	_hostName = hostName;
	_databaseName   = databaseName;
	_userName = userName;
	_password = password;

	return _open(connectionName);
}

/*! Тестирует подключение к БД с помощью заданных параметров.

	Если подключение успешное, удаляет его и возвращает истину. */
bool DBConnection::test(const QString& hostName, const QString& databaseName,
		 const QString& userName, const QString& password)
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

	testDB.open();
//	_lastError = testDB.lastError();

	if (!testDB.isOpen())
	{
#ifdef QT_DEBUG
		qDebug() << "Test failed. Error message: " << _lastError.text();
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

/*! Тестирует подключение к БД с помощью параметров, указанных ранее.

	Если подключение успешное, удаляет его и возвращает истину. */
bool DBConnection::test()
{
	return test(_hostName, _databaseName, _userName, _password);
}

/*! Возвращает последнюю сохраненную в классе ошибку подключения.

	\attention Работоспособность данной функции не подтверждена. */
QSqlError DBConnection::lastError() const
{
	return _lastError;
}

/*! Открывает подключение к БД.

	При необходимости можно указать название подключения. */
bool DBConnection::open(const QString &connectionName)
{
	return _open(connectionName);
}

/*! Открывает текущее подключение к БД. */
void DBConnection::close()
{
	if (_db.isOpen()) _db.close();
}

/*! Создает и открывает новое подключение к БД.

	Требуется указать имя подключения. */
bool DBConnection::addConnection(const QString &connectionName,
								 const QString& hostName,
								 const QString& databaseName,
								 const QString& userName,
								 const QString& password)
{
	_hostName = hostName;
	_databaseName   = databaseName;
	_userName = userName;
	_password = password;

	return _open(connectionName);
}

/*! Неконстантная ссылка на объект QSqlDatabase. */
QSqlDatabase & DBConnection::rdb()
{
	return 	_db;
}

/*! Неконстантный указатель на объект QSqlDatabase. */
QSqlDatabase * DBConnection::pdb()
{
	return &_db;
}

/*! Конструктор по умолчанию. */
DBConnection::DBConnection():
	_hostName(""),
	_databaseName(""),
	_userName(""),
	_password(""),
	_driverName("QODBC")
{
}
