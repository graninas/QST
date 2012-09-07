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

#include "sqlquerycomposer.h"

#include "sqlgenconstants.h"

namespace Sql
{

	/*!
		\class SqlQueryComposer
		\brief Класс отвечает за компоновку запросов конкретного диалекта SQL.

		\inmodule Sql

		Для компоновки запроса требуется передать в класс SqlQueryComposer
		поля SqlField, описывающие этот запрос. Поля хранятся в SqlBatch,
		а за генерацию отвечает SqlGen. Именно SqlGen передает поля
		из SqlBatch в SqlQueryComposer.

		Трактовка SqlField для разных секций SQL-запроса может быть различна. Так,
		если SqlValue, сохраненный в SqlField, невалиден, то он игнорируется для
		секции WHERE, но для секции параметров процедуры заменяется на NULL.

		Основные случаи генерации и компоновки запросов можно найти в модульном тесте ut_sqlgen.cpp .

		\attention В QST версии 0.4 планируется ввести заменяемость компоновщика
		для разных диалектов SQL.

		\attention В QST версии 0.4 класс переработан. Изменено его имя.

		\sa  SqlBatch, SqlField, SqlGen, SqlValue */

	/*! Конструктор по умолчанию. */
SqlQueryComposer::SqlQueryComposer()
:	_fieldsClause(""),
	_sourcesClause(""),
	_whereClause(""),
	_orderByClause(""),
	_groupByClause(""),
	_havingClause("")
{
}

/*! Удаляет все поля, сохраненные в классе. */
void SqlQueryComposer::clear()
{
	_fieldsClause = "";
	_sourcesClause = "";
	_whereClause = "";
	_orderByClause = "";
	_groupByClause = "";
	_havingClause = "";

	_conditions.clear();
	_fields.clear();
	_sources.clear();
	_orderByItems.clear();
	_groupByItems.clear();
	_havingItems.clear();

	_insertValues.clear();
	_updateValues.clear();
	_parameters.clear();
}

/*! Добавляет источник в запросе SQL. Им являются названия таблиц, процедур,
	представлений. */
void SqlQueryComposer::addSource(const QString &source)
{
	_sources.append(source);
}

/*! Добавляет поле, отвечающее за секцию SELECT. */
void SqlQueryComposer::addSelectClauseField(const SqlField &field)
{
	if (field.purposes() & fp_select)
	{
		_addField(field.name());
	}
}

/*! Добавляет поле, отвечающее за все секции, кроме SELECT. */
void SqlQueryComposer::addStuffField(const SqlField &field)
{
	if (field.purposes() & (fp_insert | fp_update))
	{
		if (field.purposes() & fp_insert)
			_addInsertValue(field.name(), field.value());

		if (field.purposes() & fp_update)
			_addUpdateValue(field.name(), field.value());
	}

	if (field.purposes() & fp_where)
	{
		if (field.isBinaryFilter())
			_addBetweenCondition(field.name(), field.value(bvo_first), field.value(bvo_second));
		else

			_addCondition(field.name(), field.value());
	}

	if (field.purposes() & fp_select)
	{
			_addField(field.name());
	}

	if (field.purposes() & fp_order_by)
	{
		_addOrderByField(field.name());
	}

	if (field.purposes() & fp_group_by)
	{
		_addGroupByField(field.name());
	}

	if (field.purposes() & fp_parameter)
	{
		_addParameter(field.value());
	}
}

/*! Компонует и возвращает строку SQL-запроса типа sqlType. */
QString SqlQueryComposer::query(const SqlQueryTypes &sqlType) const
{
	QString resQuery;
	switch (sqlType)
	{
	case sql_select:
		resQuery = _SELECT_SQL_QUERY();
		break;
	case sql_insert:
		resQuery = _INSERT_SQL_QUERY();
		break;
	case sql_update:
		resQuery = _UPDATE_SQL_QUERY();
		break;
	case sql_delete:
		resQuery = _DELETE_SQL_QUERY();
		break;
	case sql_execute:
		resQuery = _EXECUTE_SQL_QUERY();
		break;
	default:
		Q_ASSERT_X(false, "SqlQueryComposer::query", "Unknown SQL type.");
	};

return resQuery.simplified();
}

void SqlQueryComposer::_addVariantBetweenCondition(const QString &field,
								   const SqlValue &value1,
								   const SqlValue &value2)
{
	_StringTemplate functorTemplate = _functorToStringTemplate(fo_between);
	QString strVal1 = value1.toString();
	QString strVal2 = value2.toString();
	QString strCondition = functorTemplate.stringTemplate().arg(field, strVal1, strVal2);

	_conditions.push_back(strCondition);
}


void SqlQueryComposer::_addStringBetweenCondition(const QString &field,
								   const SqlValue &value1,
								   const SqlValue &value2)
{
	_StringTemplate val1Template = _bracesToStringTemplate(value1.braces());
	_StringTemplate val2Template = _bracesToStringTemplate(value2.braces());
	_StringTemplate functorTemplate = _functorToStringTemplate(fo_between);

	QString strVal1 = val1Template.stringTemplate().arg(value1.toString());
	QString strVal2 = val2Template.stringTemplate().arg(value2.toString());
	QString strCondition = functorTemplate.stringTemplate().arg(field, strVal1, strVal2);

	_conditions.push_back(strCondition);
}

void SqlQueryComposer::_addDateBetweenCondition(const QString &field,
												const SqlValue &value1,
												const SqlValue &value2)
{
	_StringTemplate functorTemplate = _functorToStringTemplate(fo_between);

	QString strDate1 = DATE_CONVERT.arg(value1.toString(vb_bordered, vfn_not, vub_not));
	QString strDate2 = DATE_CONVERT.arg(value2.toString(vb_bordered, vfn_not, vub_not));
	QString strCondition = functorTemplate.stringTemplate().arg(field, strDate1, strDate2);

	_conditions.push_back(strCondition);
}


void SqlQueryComposer::_addVariantCondition(const QString &field,
												 const SqlValue &value)
{
	_StringTemplate functorTemplate = _functorToStringTemplate(value.functor());

	QString strVal = value.toString();
	QString strCondition = functorTemplate.string(field, strVal);

	_conditions.push_back(strCondition);
}

void SqlQueryComposer::_addStringCondition(const QString &field,
										   const SqlValue &value)
{
	_StringTemplate val1Template = _bracesToStringTemplate(value.braces());
	_StringTemplate functorTemplate = _functorToStringTemplate(value.functor());

	QString strCondition = functorTemplate.string(field, val1Template.string(value.toString()));

	_conditions.push_back(strCondition);
}

void SqlQueryComposer::_addDateCondition(const QString &field,
											  const SqlValue &value)
{
	_StringTemplate functorTemplate =_functorToStringTemplate(value.functor());

	QDate dateVal = value.value().toDate();
	QString strDate = DATE_CONVERT.arg(dateVal.toString(DATE_FORMAT));
	QString strCondition = functorTemplate.string(field, strDate);

	_conditions.push_back(strCondition);
}

void SqlQueryComposer::_addBetweenCondition(const QString &field,
												 const SqlValue &value1,
												 const SqlValue &value2)
{
	const QVariant &rVarVal1 = value1.value();
	const QVariant &rVarVal2 = value2.value();

	if (rVarVal1.type() == QVariant::Date)
		_addDateBetweenCondition(field, rVarVal1, rVarVal2);
	else
		if (rVarVal1.type() == QVariant::ByteArray || rVarVal1.type() == QVariant::String)
			_addStringBetweenCondition(field, value1, value2);
		else
			_addVariantBetweenCondition(field, value1, value2);
}

void SqlQueryComposer::_addCondition(const QString &field,
										  const SqlValue &value)
{
	if (!value.isValid())
		return;

	const QVariant &rVarVal = value.value();

	if (rVarVal.type() == QVariant::Date)
		_addDateCondition(field, value);
	else
		if (rVarVal.type() == QVariant::ByteArray || rVarVal.type() == QVariant::String)
			_addStringCondition(field, value);
		else
			_addVariantCondition(field, value);
}

void SqlQueryComposer::_addField(const QString &field)
{
	Q_ASSERT(!field.isEmpty());
	_fields.append(field);
}

void SqlQueryComposer::_addInsertValue(const QString &field,
									   const SqlValue &value)
{
	QString strValue;

	if (value.value().type() == QVariant::Date)
	{
		strValue = DATE_CONVERT.arg(value.toString(vb_bordered, vfn_not, vub_not));
	}
	else
	if (value.value().type() == QVariant::String
		|| value.value().type() == QVariant::ByteArray
		|| value.value().type() == QVariant::DateTime)

		strValue = value.toString(vb_bordered, vfn_fill, vub_not);
	else
		strValue = value.toString(vb_none, vfn_fill, vub_not);

	_insertValues.insert(field, strValue);
}

void SqlQueryComposer::_addUpdateValue(const QString &field,
									   const SqlValue &value)
{
	if (value.isValid())
	{
		QString strValue;

		if (value.value().type() == QVariant::Date)
		{
			strValue = DATE_CONVERT.arg(value.toString(vb_bordered, vfn_not, vub_not));
		}
		else

		if (value.value().type() == QVariant::String
			|| value.value().type() == QVariant::ByteArray
			|| value.value().type() == QVariant::DateTime)

			strValue = value.toString(vb_bordered, vfn_fill, vub_not);
		else
			strValue = value.toString(vb_none, vfn_fill, vub_not);

		_updateValues.insert(field, strValue);
	}
}

void SqlQueryComposer::_addParameter(const SqlValue &value)
{
	if (!value.isValid() || value.isNull())
		_parameters.push_back(value.toString(vb_none, vfn_fill, vub_not));
	else
	{
		if (value.value().type() == QVariant::String
			|| value.value().type() == QVariant::ByteArray
			|| value.value().type() == QVariant::Date
			|| value.value().type() == QVariant::DateTime)

			_parameters.push_back(value.toString(vb_bordered, vfn_fill, vub_not));

		else

			_parameters.push_back(value.toString(vb_none, vfn_fill, vub_not));
	}
}

void SqlQueryComposer::_addOrderByField(const QString &field)
{
	Q_ASSERT(!field.isEmpty());
	_orderByItems.append(field);
}

void SqlQueryComposer::_addGroupByField(const QString &field)
{
	Q_ASSERT(!field.isEmpty());
	_groupByItems.append(field);
}

SqlQueryComposer::_StringTemplate SqlQueryComposer::_bracesToStringTemplate(const FuzzyBraces &braces) const
{
	switch(braces)
	{
		case fb_left:	return _StringTemplate(1, QString("'%%1'"));
		case fb_right:	return _StringTemplate(1, QString("'%1%'"));
		case fb_both:	return _StringTemplate(1, QString("'%%1%'"));
		case fb_none:	return _StringTemplate(1, QString("'%1'"));
	};
return _StringTemplate(1, QString("%1"));
};

SqlQueryComposer::_StringTemplate SqlQueryComposer::_functorToStringTemplate(const Functor &functor) const
{
//Здесь %1 - поле, %2 - значение1, %3 - значение2.

	switch(functor)
	{
			case fo_like:			return _StringTemplate(2, " %1 LIKE %2 ");
			case fo_equal:			return _StringTemplate(2, " %1 = %2 ");
			case fo_not_equal:		return _StringTemplate(2, " %1 <> %2 ");
			case fo_less:			return _StringTemplate(2, " %1 < %2 ");
			case fo_greater:		return _StringTemplate(2, " %1 > %2 ");
			case fo_less_equal:		return _StringTemplate(2, " %1 <= %2 ");
			case fo_greater_equal:	return _StringTemplate(2, " %1 >= %2 ");
			case fo_between:		return _StringTemplate(3, " %1 BETWEEN %2 AND %3 ");
			case fo_is_null:		return _StringTemplate(1, " %1 IS NULL ");
			case fo_is_not_null:	return _StringTemplate(1, " %1 IS NOT NULL ");
	 case fo_not_equal_or_null:		return _StringTemplate(2, " %1 <> %2 OR %1 IS NULL ");
	 case fo_equal_or_null:			return _StringTemplate(2, " %1 == %2 OR %1 IS NULL ");
	 case fo_less_or_null:			return _StringTemplate(2, " %1 < %2 OR %1 IS NULL ");
	 case fo_greater_or_null:		return _StringTemplate(2, " %1 > %2 OR %1 IS NULL ");
	 case fo_less_equal_or_null:	return _StringTemplate(2, " %1 <= %2 OR %1 IS NULL ");
	 case fo_greater_equal_or_null:	return _StringTemplate(2, " %1 >= %2 OR %1 IS NULL ");
	 case fo_like_or_null:			return _StringTemplate(2, " %1 LIKE %2 OR %1 IS NULL ");


	 case fo_none:	Q_ASSERT_X(false,
							   "SqlQueryComposer::_functorToStringTemplate",
							   "Empty functor - no template available.");

	};
return _StringTemplate();
}

QString  SqlQueryComposer::_SELECT_SQL_QUERY() const
{
	return
		_SELECT()	+
		_FROM()		+
		_WHERE()	+
		_GROUP_BY()+
		_HAVING()+
		_ORDER_BY();
}

QString  SqlQueryComposer::_INSERT_SQL_QUERY() const
{
	if (_insertValues.empty())
		return QString();

	return	_INSERT() +
			_VALUES();
}

QString  SqlQueryComposer::_UPDATE_SQL_QUERY() const
{
	if (_updateValues.empty())
		return QString();

	return _UPDATE()	+
			_SET()		+
			_WHERE();
}

QString  SqlQueryComposer::_DELETE_SQL_QUERY() const
{
	return _DELETE()	+
			_WHERE();
}

QString  SqlQueryComposer::_EXECUTE_SQL_QUERY() const
{
	return	_EXECUTE()	+
			_PARAMETERS() ;
}

QString SqlQueryComposer::_CLAUSE(const QString &stringClause,
									   const QStringList &items,
									   const QString &clauseWord,
									   const bool &clauseWordRequired,
									   const QString &itemsDelimiter,
									   const QString &openBrace,
									   const QString &closeBrace
									   ) const
{
int itemsCount = items.size();

	QString res;

	if (!clauseWordRequired && stringClause.isEmpty() && !itemsCount) return "";

	res = clauseWord + openBrace + stringClause;

	if (!stringClause.isEmpty() && itemsCount) res += itemsDelimiter;

	for (int i = 0; i < itemsCount - 1; ++i)
		res += items[i] + itemsDelimiter;

	if (itemsCount)
		res += items[itemsCount - 1];

	if (closeBrace.isEmpty())
		res += " ";

return res + closeBrace;
}


QString SqlQueryComposer::_SELECT() const
{
	return _CLAUSE(_fieldsClause, _fields, SQL_SELECT, true, ", ");
}

QString SqlQueryComposer::_INSERT() const
{
	return
		_CLAUSE(_sourcesClause, _sources, SQL_INSERT, true, ", ") +
		_CLAUSE("", _insertValues.uniqueKeys(), "", true, ", ", "(", ")");
}

QString SqlQueryComposer::_VALUES() const
{
	return _CLAUSE("", _insertValues.values(), SQL_VALUES, true, ", ", "(", ")");
}

QString SqlQueryComposer::_UPDATE() const
{
	return _CLAUSE(_sourcesClause, _sources, SQL_UPDATE, true, ", ");
}

QString SqlQueryComposer::_SET() const
{
	QString res = SQL_SET;

	QStringList fields = _updateValues.uniqueKeys();
	QStringList values = _updateValues.values();
	int fieldsCount = fields.size();

	for (int i = 0; i < fieldsCount - 1; ++i)
		res += fields[i] + " = " + values[i] + ", ";
	if (fieldsCount) res += fields[fieldsCount - 1] + " = " + values[fieldsCount - 1] + " ";


	return res;
}

QString SqlQueryComposer::_DELETE() const
{
	return _CLAUSE(_sourcesClause, _sources, SQL_DELETE, true, ", ");
}

QString SqlQueryComposer::_FROM() const
{
	return _CLAUSE(_sourcesClause, _sources, SQL_FROM, true, ", ");
}

QString SqlQueryComposer::_WHERE() const
{
	return _CLAUSE(_whereClause, _conditions, SQL_WHERE, false, " AND ");
}

QString SqlQueryComposer::_EXECUTE() const
{
	return _CLAUSE("", _sources, SQL_EXECUTE, true, ", ");
}

QString SqlQueryComposer::_PARAMETERS() const
{
	return _CLAUSE("", _parameters, "", false, ", ");
}

QString SqlQueryComposer::_ORDER_BY() const
{
	return _CLAUSE(_orderByClause, _orderByItems, SQL_ORDER_BY, false, ", ");
}

QString SqlQueryComposer::_GROUP_BY() const
{
	return _CLAUSE(_groupByClause, _groupByItems, SQL_GROUP_BY, false, ", ");
}

QString SqlQueryComposer::_HAVING() const
{
	return _CLAUSE(_havingClause, _havingItems, SQL_HAVING, false, " AND ");
}

}
