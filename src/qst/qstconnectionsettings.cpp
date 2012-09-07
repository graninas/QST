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

#include "qstconnectionsettings.h"

namespace Qst
{

/*!
	\class QstConnectionSettings
	\brief
	Класс хранит настройки подключения к БД: порт, хост, имя базы данных,
	имя пользователя и пароль.

	\inmodule Qst
*/

/*! Конструктор по умолчанию. */
QstConnectionSettings::QstConnectionSettings()
	:
	_hostName(""),
	_port(-1),
	_databaseName(""),
	_userName(""),
	_password("")
{
}

/*! Конструктор, принимающий параметры подключения, включая номер порта. */
QstConnectionSettings::QstConnectionSettings(const QString &hostName,
											 const int &port,
											 const QString &databaseName,
											 const QString &userName,
											 const QString &password)
	 :
	 _hostName(hostName),
	 _port(port),
	 _databaseName(databaseName),
	 _userName(userName),
	 _password(password)
{
}

/*! Конструктор, принимающий все параметры подключения кроме номера порта.
	Устанавливается порт -1. */
QstConnectionSettings::QstConnectionSettings(const QString &hostName,
											 const QString &databaseName,
											 const QString &userName,
											 const QString &password)
	:
	_hostName(hostName),
	_port(-1),
	_databaseName(databaseName),
	_userName(userName),
	_password(password)
{
}

/*! Возвращает имя хоста. */
QString QstConnectionSettings::hostName() const
{
	return _hostName;
}

/*! Возвращает номер порта. */
int	QstConnectionSettings::port() const
{
	return _port;
}

/*! Возвращает имя базы данных. */
QString QstConnectionSettings::databaseName() const
{
	return _databaseName;
}

/*! Возвращает имя пользователя. */
QString QstConnectionSettings::userName() const
{
	return _userName;
}

/*! Возвращает пароль. */
QString QstConnectionSettings::password() const
{
	return _password;
}

/*! Устанавливает имя хоста. */
void QstConnectionSettings::setHostName(const QString &hostName)
{
	_hostName = hostName;
}

/*! Устанавливает порт. */
void QstConnectionSettings::setPort(const int &port)
{
	_port = port;
}

/*! Устанавливает имя базы данных. */
void QstConnectionSettings::setDatabaseName(const QString &databaseName)
{
	_databaseName = databaseName;
}

/*! Устанавливает имя пользователя. */
void QstConnectionSettings::setUserName(const QString &userName)
{
	_userName = userName;
}

/*! Устанавливает пароль. */
void QstConnectionSettings::setPassword(const QString &password)
{
	_password = password;
}

/*! Возвращает true, что-то из перечисленного если не установлено:
	имя хоста, имя базы данных, имя пользователя или пароль. */
bool QstConnectionSettings::isNull() const
{
	return _hostName.isEmpty()
			|| _databaseName.isEmpty()
			|| _userName.isEmpty()
			|| _password.isEmpty();
}


}
