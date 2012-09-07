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

#include "sqlbatch.h"

namespace Sql
{

/*!
	\class SqlBatch
	\brief Класс хранит в себе описатели полей SqlField и источники для SQL-запросов
	(имя таблицы, имя процедуры и т.п.).

	\inmodule Sql

	Пакеты SqlBatch составляются в DFD-генерирующих функциях _selector(),
	_inserter(), _updater(), _deleter() и _executor() класса AbstractModelHandler
	и всех его наследников.

	При генерации запросов AbstractModelHandler вызывает одну из вышеперечисленных
	функций, получает из нее SqlQueryDescriptor, который, в свою очередь, содержит
	SqlBatch. Далее этот пакет вместе с типом SQL-запроса передается в генератор
	запросов SqlGen. Теперь с помощью SqlGen можно сгенерировать и получить SQL-запрос.

	Класс отвечает так же за то, в какой последовательности будут расставлены
	поля в секции SELECT, и какой им будет присвоен индекс. Индекс поля важен
	для AbstractModelHandler'а. Чтобы обратиться к полю по имени, необходимо знать
	его индекс в последовательности полей; а так как последовательность зависит
	от алгоритма, заложенного в SqlBatch, то именно SqlBatch должен отвечать
	за сопоставление индекса и имени поля.

	Благодаря этой абстракции, стало возможным получить индекс поля и использовать
	его как column для моделей данных в системе Interview.

	Типичный SqlBatch выглядит примерно так.
	\code
	SqlBatch batch;

	batch.addSource("vBaseUnits");

	batch	<< SqlField("ID", fv_invisible, fr_id)
	<< SqlField("Name", fv_visible, fr_none, "Наименование", 100);
	\endcode

	За дополнительными примерами генерации запросов обращаться в модульный тест ut_sqlgen.cpp .

	\sa SqlField, AbstractModelHandler, SqlValue, SqlGen
	*/

/*! Конструктор по умолчанию. */
SqlBatch::SqlBatch()
{
}

/*! Добавляет источник для SQL-запроса. Им является название процедуры, имя таблицы,
	имя представления. Если запрос SELECT-типа, то источников может быть несколько. */
void SqlBatch::addSource(const QString &source)
{
	_sources.append(source);
}

/*! Добавляет в пакет поле SqlField. */
void SqlBatch::addField(const SqlField &field)
{
	SqlField resField = field;

	if (!resField.isValid() && (resField.purposes() & fp_parameter))
	{
		resField.setValue(SqlValue(nt_null));
	}

	_fields.append(resField);

	if (resField.isVisible())
	{
		_fieldNamesLists[fv_visible].append(resField.name());

		if (resField.isService())
		{
			_serviceFieldsIndexes[_RoleVisType(resField.role(), fv_visible)] =
				_fieldNamesLists[fv_visible].size() - 1;

			Q_ASSERT(!_rolesVisibility.contains(resField.role()));
			_rolesVisibility[resField.role()] = fv_visible;
		}

	}
	else
		if (resField.isInvisible())
		{
			_fieldNamesLists[fv_invisible].append(resField.name());

			if (resField.isService())
			{
				_serviceFieldsIndexes[_RoleVisType(resField.role(), fv_invisible)] =
					_fieldNamesLists[fv_invisible].size() - 1;

				Q_ASSERT(!_rolesVisibility.contains(resField.role()));
				_rolesVisibility[resField.role()] = fv_invisible;
			}
		}
}

/*! Возвращает источники для запросов SQL.

	\sa SqlBatch::addSource()*/
QStringList SqlBatch::sources() const
{
	return _sources;
}

/*! Возвращает поля, назначение (SqlFieldPurpose) которых - fp_select. */
SqlFieldsVector SqlBatch::selectClauseFields() const
{
	SqlFieldsVector visFields;
	SqlFieldsVector invisFields;

	for (int i = 0; i < _fields.size(); ++i)
	{
		if (_fields[i].isVisible())
			visFields.append(_fields[i]);
		else
			if (_fields[i].isInvisible())
				invisFields.append(_fields[i]);
	};

	return visFields + invisFields;
}

/*! Возвращает поля, назначение (SqlFieldPurpose) которых - любые, кроме fp_select. */
SqlFieldsVector SqlBatch::stuffFields() const
{
	SqlFieldsVector resFields;

	for (int i = 0; i < _fields.size(); ++i)
	{
		if (_fields[i].isStuffField())
		{
			resFields.append(_fields[i]);
		}
	};

	return resFields;
}

/*! Возвращает все поля. */
SqlFieldsVector SqlBatch::fields() const
{
	return _fields;
}

/*! Очищает список полей и источников. */
void SqlBatch::clear()
{
	_sources.clear();
	_fields.clear();

	_fieldNamesLists.clear();
	_serviceFieldsIndexes.clear();
	_rolesVisibility.clear();
}

/*! Очищает список источников. */
void SqlBatch::clearSources()
{
	_sources.clear();
}

/*! Возвращает ложь в следующих случаях:
	- если не добавлено ни одного источника;
	- если нет ни одного валидного поля. */
bool SqlBatch::isValid() const
{
	if (_sources.isEmpty())
		return false;

	bool fieldsValid = false;
	for (int i = 0; i < _fields.size(); ++i)
	{
		if (_fields[i].isValid())
			fieldsValid = true;
	}

	return fieldsValid;
}

/*! Возвращает индекс поля по его имени.

	Индекс может быть использован в модели системы Interview в качестве параметра column. */
int SqlBatch::columnIndex(const QString &fieldName) const
{
	QStringList visFields = _fieldNamesLists[fv_visible];
	QStringList invisFields = _fieldNamesLists[fv_invisible];

	if (visFields.contains(fieldName))
		return visFields.indexOf(fieldName);

	if(invisFields.contains(fieldName))
	{
		return visFields.size() + invisFields.indexOf(fieldName);
	}
return -1;
}

/*! Возвращает индекс поля по его роли.

	Индекс может быть использован в  модели системы Interview в качестве параметра column. */
int SqlBatch::columnIndex(const SqlFieldRoles &role) const
{
	if (_rolesVisibility.contains(role))
	{
		SqlFieldVisibility vis = _rolesVisibility[role];

		if (vis == fv_visible)
		{
			Q_ASSERT(_serviceFieldsIndexes.contains(_RoleVisType(role, fv_visible)));
			return _serviceFieldsIndexes[_RoleVisType(role, fv_visible)];
		}
		else
		{
			 return _fieldNamesLists[fv_visible].size() +
					_serviceFieldsIndexes[_RoleVisType(role, fv_invisible)];
		}
	}
return -1;
}

/*! Оператор, позволяющий добавлять источники удобнее, чем функцией addSource(). */
SqlBatch & SqlBatch::operator<<(const QString &source)
{
	addSource(source);
return *this;
}

/*! Оператор, позволяющий добавлять поля удобнее, чем функцией addField(). */
SqlBatch & SqlBatch::operator<<(const SqlField &field)
{
	addField(field);

return *this;
}

}

