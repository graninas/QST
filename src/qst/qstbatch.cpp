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

#include "qstbatch.h"

namespace Qst
{
	/*!
		\class QstBatch
		\brief
		Контейнер для DFD-описателей. В QstBatch передаются объекты QstField,
		которые затем используются для генерации разных секций SQL-запроса.

		\inmodule Qst

		Примеры генерируемых запросов по описателям.

		SQL-запрос SELECT-типа:
\code
SELECT Name, ID FROM vBaseUnits WHERE ID < 20
\endcode
\code
	QstBatch batch;
			// Источник данных запроса.
	batch	<<"vBaseUnits"

			// Ключевое поле.
			<< QstField(RolePrimaryKey, "ID")

			// Поле для секции SELECT, которое будет отображаться
			// QTableView, QTreeView, QComboBox в колонке "Наименование" с шириной 100.
			<< QstField("Name", FieldVisible,  "Наименование", 100)

			// Фильтр - поле для секции WHERE.
			<< QstField("ID", value(ID_VALUE)) // value(ID_VALUE) == QstValue(20, FunctorLess)
			;
\endcode

		SQL-запрос SELECT-типа:
\code
SELECT	ProductName, GroupName, [Count], BaseUnitName, Article,
		Price, DiscountsSeted, ID, WarehouseName, Warehouse_ID, Nomenclature_ID
FROM	vProducts
WHERE
		Warehouse_ID = 5
		AND ProductName LIKE '%конфет%'
\endcode
\code
QstBatch batch;
batch	<< "vProducts"

		<< QstField(RolePrimaryKey, "ID")

		<< QstField("ProductName", FieldVisible,  "Наименование", 140)
		<< QstField("GroupName", FieldVisible,  "Группа", 120)
		<< QstField("[Count]", FieldVisible,  "Количество", 100)
		<< QstField("BaseUnitName", FieldVisible,  "Ед.\nизм.", 60)
		<< QstField("Article", FieldVisible,  "Артикул", 100)
		<< QstField("Price", FieldVisible,  "Закупочная\nцена", 110)
		<< QstField("DiscountsSeted", FieldVisible,  "Скидки", 60)

		<< QstField("WarehouseName", FieldInvisible,  "Склад", 150)

		<< QstField("Warehouse_ID")
		<< QstField("Nomenclature_ID")

		// Фильтры.
		<< QstField("ID", value(ID_VALUE))                  // value(ID_VALUE) == QstValue()
		<< QstField("Warehouse_ID", value("Warehouse_ID"))  // value("Warehouse_ID") == QstValue(5)
		<< QstField("ProductName", value("ProductName"))    // value("ProductName") == QstValue("конфет")
		;
\endcode

		SQL-запрос EXEC-типа:
\code
EXEC pInsertNomenclature 10, 'Новая номенклатура', 'Арт4949', 4, 3200.0
\endcode
\code
QstBatch batch;
batch	<< "pInsertNomenclature"

		<< QstField(value("Group_ID"))    // value("Group_ID") == QstValue(10)
		<< QstField(value("Name"))        // value("Name") == QstValue("Новая номенклатура")
		<< QstField(value("Article"))     // value("Article") == QstValue("Арт4949")
		<< QstField(value("BaseUnit_ID")) // value("BaseUnit_ID") == QstValue(4)
		<< QstField(value("BasePrice"))   // value("BasePrice") == QstValue(3200.0f)
		;
\endcode

	*/

	/*! Конструктор по умолчанию. */
QstBatch::QstBatch(const QString name)
	:
	_name(name),
	_itemExtractor(NULL)
{
}

/*! Конструктор, принимающий объект класса QstAbstractItemNameExtractor. */
QstBatch::QstBatch(QstAbstractItemNameExtractor * itemExtractor)
	:	_itemExtractor(itemExtractor)
{
	Q_CHECK_PTR(_itemExtractor);
}

/*! Добавляет источник данных SQL-запроса. Им может быть таблица в секции FROM,
	название хранимой процедуры, название представления и т.п. */
void QstBatch::addSource(const QString &source)
{
	_sources.append(source);
}

/*! Добавляет источник данных SQL-запроса по условию.

	\sa QstIf
*/
void QstBatch::addSource(const QstSourceIf &sourceIf)
{
	if (sourceIf.condition() && sourceIf.hasTrueValue())
		addSource(sourceIf.trueValue());
	else
		if (!sourceIf.condition() && sourceIf.hasFalseValue())
			addSource(sourceIf.falseValue());
}

/*! Добавляет поле в список полей.

	Второй параметр указывает, извлекать ли фактическое имя поля из field.name().
*/
void QstBatch::addField(const QstField &field, const bool &extractItemName)
{
		QString		resFieldName = field.name();

	_fields.append(field);

if (extractItemName)
	resFieldName = _extractItemName(field.name());

	if (field.isVisible())
	{
		_fieldNamesLists[FieldVisible].append(resFieldName);

		_fillRoleVisMap(field, FieldVisible);
	}
	else
		if (field.isInvisible())
		{
			_fieldNamesLists[FieldInvisible].append(resFieldName);

			_fillRoleVisMap(field, FieldInvisible);
		}
}

/*! Добавляет поле по условию.

	Второй параметр указывает, извлекать ли фактическое имя поля из field.name().
*/
void QstBatch::addField(const QstFieldIf &fieldIf, const bool &extractItemName)
{
	if (fieldIf.condition() && fieldIf.hasTrueValue())
		addField(fieldIf.trueValue(), extractItemName);
	else
		if (!fieldIf.condition() && fieldIf.hasFalseValue())
			addField(fieldIf.falseValue(), extractItemName);
}

/*! Возвращает список источников данных SQL-запроса. */
QStringList QstBatch::sources() const
{
	return _sources;
}

/*! Возвращает массив полей, у которых в purposes() есть флаги purposes. */
QstFieldVector QstBatch::fields(const FieldPurposes &purposes) const
{
	if (purposes == PurposeAll_Mask)
	return _fields;

QstFieldVector resFields;

	if (purposes & PurposeSelect)
	{
		QstFieldVector visFields;
		QstFieldVector invisFields;

		for (int i = 0; i < _fields.size(); ++i)
		{
			if (_fields[i].isVisible())
				visFields.append(_fields[i]);
			else
				if (_fields[i].isInvisible())
					invisFields.append(_fields[i]);
		};

		resFields = visFields + invisFields;
	}
	else
	{
		for (int i = 0; i < _fields.size(); ++i)
		{
			if (_fields[i].purposes() & purposes)
			{
				resFields.append(_fields[i]);
			}
		};
	}

return resFields;
}

/*! Очищает пакет QstBatch. */
void QstBatch::clear()
{
	_sources.clear();
	_fields.clear();

	_fieldNamesLists.clear();
	_serviceFieldsIndexes.clear();
	_rolesVisibility.clear();
}

/*! Удаляет источники SQL-запросов. */
void QstBatch::clearSources()
{
	_sources.clear();
}

/*! Возвращает true, если QstBatch содержит валидные данные.

	Инвалидными данные считаются в любом из следующих случаев:
	- источник данных пуст;
	- все QstField инвалидны.
*/
bool QstBatch::isValid() const
{
	if (_sources.isEmpty())
		return false;

	for (int i = 0; i < _fields.size(); ++i)
	{
		if (_fields[i].isValid())
			return true;
	}

return false;
}

/*! Возвращает индекс поля с именем fieldName в секции SELECT.

	В каком бы порядке ни были добавлены поля в QstBatch,
	сначала будут идти все видимые поля (QstField::isVisible() == true),
	затем - все невидимые ключевые поля (QstField::isInvisible() == true
	с ролями RolePrimaryKey и RoleParentKey), и затем - все остальные невидимые поля.

	Возвращает -1, если поле с именем fieldName не найдено.
*/
int QstBatch::columnIndex(const QString &fieldName, const bool &extractItemName) const
{
	QString resFieldName = fieldName;
	if (extractItemName)
		resFieldName = _extractItemName(fieldName);

	QStringList visFields = _fieldNamesLists[FieldVisible];
	QStringList invisFields = _fieldNamesLists[FieldInvisible];

	if (visFields.contains(resFieldName))
		return visFields.indexOf(resFieldName);

	if(invisFields.contains(resFieldName))
	{
		return visFields.size() + invisFields.indexOf(resFieldName);
	}
return -1;
}

/*! Возвращает индекс поля с ролью role.

  Возвращает -1, если поле с ролью role не найдено.
  */
int QstBatch::columnIndex(const FieldRole &role) const
{
	if (_rolesVisibility.contains(role))
	{
		FieldVisibility vis = _rolesVisibility[role];

		if (vis == FieldVisible)
		{
			Q_ASSERT(_serviceFieldsIndexes.contains(QstSpecial::QstRoleVisibility(role, FieldVisible)));
			return _serviceFieldsIndexes[QstSpecial::QstRoleVisibility(role, FieldVisible)];
		}
		else
		{
			 return _fieldNamesLists[FieldVisible].size() +
					_serviceFieldsIndexes[QstSpecial::QstRoleVisibility(role, FieldInvisible)];
		}
	}
return -1;
}

/*! Добавляет источник данных. */
QstBatch & QstBatch::operator<<(const QString &source)
{
	addSource(source);
return *this;
}

/*! Добавляет источник данных по условию . */
QstBatch & QstBatch::operator<<(const QstSourceIf &sourceIf)
{
	addSource(sourceIf);
return *this;
}

/*! Добавляет поле QstField. */
QstBatch & QstBatch::operator<<(const QstField &field)
{
	addField(field);

return *this;
}

/*! Добавляет поле QstField по условию. */
QstBatch & QstBatch::operator<<(const QstFieldIf &fieldif)
{
	addField(fieldif);

return *this;
}

QString QstBatch::_extractItemName(const QString &fullFieldName) const
{
	if (_itemExtractor == NULL)
		return _defaultItemExtractor.extractItemName(fullFieldName);
	else
	{
		return _itemExtractor->extractItemName(fullFieldName);
	}
}

void QstBatch::_fillRoleVisMap(const QstField &field, const FieldVisibility &visibility)
{
	if (field.isService())
	{
		_serviceFieldsIndexes[QstSpecial::QstRoleVisibility(field.role(), visibility)] =
			_fieldNamesLists[visibility].size() - 1;

		Q_ASSERT(!_rolesVisibility.contains(field.role()));
		_rolesVisibility[field.role()] = visibility;
	}
}

}

