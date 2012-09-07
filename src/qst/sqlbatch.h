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

#ifndef SQLBATCH_H
#define SQLBATCH_H

#include "sqlfield.h"

#include <QStringList>
#include <QList>
#include <QMap>

namespace Sql
{

class SqlBatch
{
private:

	class _RoleVisType
	{
	public:
		_RoleVisType(const SqlFieldRoles role,
					 SqlFieldVisibility visibility)
		   :_role(role), _visibility(visibility)
		{
		}

		SqlFieldRoles		_role;
		SqlFieldVisibility	_visibility;
		friend bool operator<(const _RoleVisType& val1,
							  const _RoleVisType& val2)
		{
			return	(val1._role < val2._role);
		}
	};

	QMap<SqlFieldVisibility, QStringList>	_fieldNamesLists;
	QMap<_RoleVisType, int>					_serviceFieldsIndexes;
	QMap<SqlFieldRoles, SqlFieldVisibility> _rolesVisibility;

private:

	QStringList			_sources;
	SqlFieldsVector		_fields;

public:
    SqlBatch();

	void addSource(const QString &source);
	void addField(const SqlField &field);

	QStringList sources() const;


	SqlFieldsVector selectClauseFields() const;
	SqlFieldsVector stuffFields() const;

	SqlFieldsVector fields() const;

	void clear();
	void clearSources();

	bool isValid() const;

// Returns -1 if no item matched
	int columnIndex(const QString &fieldName) const;
	int columnIndex(const SqlFieldRoles &role) const;

	SqlBatch & operator<<(const QString &source);
	SqlBatch & operator<<(const SqlField &field);


	friend bool operator == (const SqlBatch &val1, const SqlBatch &val2)
	{
		return val1.sources() == val2.sources()
				&& val1.fields() == val2.fields();

	}
};



}
#endif // SQLBATCH_H
