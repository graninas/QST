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

#include "qstquerycomposer.h"

#include "qstgenconstants.h"

namespace Qst
{

	/*!
	\class QstQueryComposer
	\brief
		Класс отвечает за компоновку конкретных секций SQL-запроса по переданным
		в него полям QstField.

	\inmodule Qst
	*/

/*! Конструктор по умолчанию. */
QstQueryComposer::QstQueryComposer()
:	_fieldsClause(""),
	_sourcesClause(""),
	_whereClause(""),
	_orderByClause(""),
	_groupByClause(""),
	_havingClause("")
{
}

/*! Очищает все поля. */
void QstQueryComposer::clear()
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

/*! Добавляет источник данных SQL-запроса. */
void QstQueryComposer::addSource(const QString &source)
{
	_sources.append(source);
}

/*! Добавляет поле для секции SELECT. */
void QstQueryComposer::addSelectClauseField(const QstField &field)
{
	if (field.purposes() & PurposeSelect)
	{
		_addField(field.name());
	}
}

/*! Добавляет поля других секций, а так же параметры процедуры и значения для UPDATE, INSERT. */
void QstQueryComposer::addStuffField(const QstField &field)
{
	if (field.purposes() & (PurposeInsert | PurposeUpdate))
	{
		if (field.purposes() & PurposeInsert)
			_addInsertValue(field.name(), field.value());

		if (field.purposes() & PurposeUpdate)
			_addUpdateValue(field.name(), field.value());
	}

		if (field.purposes() & PurposeWhere)
		{
			if (field.isBinaryFilter())
			{
				if (field.value(OrderFirst).isNull()
					|| field.value(OrderSecond).isNull())
				return;

				_addBetweenCondition(field.name(), field.value(OrderFirst), field.value(OrderSecond));
			}
			else
			{
				if (!field.value().isValid())
					return;

				_addCondition(field.name(), field.value());
			}
		}

	if (field.purposes() & PurposeSelect)
	{
			_addField(field.name());
	}

	if (field.purposes() & PurposeOrderBy)
	{
		_addOrderByField(field.name());
	}

	if (field.purposes() & PurposeGroupBy)
	{
		_addGroupByField(field.name());
	}

	if (field.purposes() & PurposeParameter)
	{
		QstValue resValue = field.value();
		if (!resValue.isValid())
			resValue = QstValue(Null);

		_addParameter(resValue);
	}
}

/*! Компонует запрос типа queryType. */
QString QstQueryComposer::query(const QueryType &queryType) const
{
	QString resQuery;
	switch (queryType)
	{
	case QuerySelect:
		resQuery = _SELECT_SQL_QUERY();
		break;
	case QueryInsert:
		resQuery = _INSERT_SQL_QUERY();
		break;
	case QueryUpdate:
		resQuery = _UPDATE_SQL_QUERY();
		break;
	case QueryDelete:
		resQuery = _DELETE_SQL_QUERY();
		break;
	case QueryExecute:
		resQuery = _EXECUTE_SQL_QUERY();
		break;
	default:
		Q_ASSERT_X(false, "QstQueryComposer::query", "Unknown SQL type.");
	};

return resQuery.simplified();
}

/*! Компонует секции, переданные в clauses и возвращает массив результатов. */
QueryClauseMap QstQueryComposer::queryParts(const QueryClauses &clauses) const
{
	QueryClauseMap map;

	if (clauses.testFlag(ClauseSelect))
		map[ClauseSelect] = _SELECT().simplified();

	if (clauses.testFlag(ClauseFrom))
		map[ClauseFrom] = _FROM().simplified();

	if (clauses.testFlag(ClauseWhere))
		map[ClauseWhere] = _WHERE().simplified();

	if (clauses.testFlag(ClauseInsertInto))
		map[ClauseInsertInto] = _INSERT().simplified();

	if (clauses.testFlag(ClauseUpdate))
		map[ClauseUpdate] = _UPDATE().simplified();

	if (clauses.testFlag(ClauseOrderBy))
		map[ClauseOrderBy] = _ORDER_BY().simplified();

	if (clauses.testFlag(ClauseGroupBy))
		map[ClauseGroupBy] = _GROUP_BY().simplified();

	if (clauses.testFlag(ClauseParameters))
		map[ClauseParameters] = _PARAMETERS().simplified();

	if (clauses.testFlag(ClauseHaving))
		map[ClauseHaving] = _HAVING().simplified();

	if (clauses.testFlag(ClauseValues))
		map[ClauseValues] = _VALUES().simplified();

	if (clauses.testFlag(ClauseSet))
		map[ClauseSet] = _SET().simplified();

	if (clauses.testFlag(ClauseDelete))
		map[ClauseDelete] = _DELETE().simplified();

	if (clauses.testFlag(ClauseExecute))
		map[ClauseExecute] = _EXECUTE().simplified();

	return map;
}

void QstQueryComposer::_addVariantBetweenCondition(const QString &field,
								   const QstValue &value1,
								   const QstValue &value2)
{
	_StringTemplate functorTemplate = _functorToStringTemplate(FunctorBetween);
	QString strVal1 = value1.toString();
	QString strVal2 = value2.toString();
	QString strCondition = functorTemplate.stringTemplate().arg(field, strVal1, strVal2);

	_conditions.push_back(strCondition);
}


void QstQueryComposer::_addStringBetweenCondition(const QString &field,
								   const QstValue &value1,
								   const QstValue &value2)
{
	_StringTemplate val1Template = _bracesToStringTemplate(value1.braces());
	_StringTemplate val2Template = _bracesToStringTemplate(value2.braces());
	_StringTemplate functorTemplate = _functorToStringTemplate(FunctorBetween);

	QString strVal1 = val1Template.stringTemplate().arg(value1.toString());
	QString strVal2 = val2Template.stringTemplate().arg(value2.toString());
	QString strCondition = functorTemplate.stringTemplate().arg(field, strVal1, strVal2);

	_conditions.push_back(strCondition);
}

void QstQueryComposer::_addDateBetweenCondition(const QString &field,
												const QstValue &value1,
												const QstValue &value2)
{
	_StringTemplate functorTemplate = _functorToStringTemplate(FunctorBetween);

	QString strDate1 = DATE_CONVERT.arg(value1.toString(ValueBordered));
	QString strDate2 = DATE_CONVERT.arg(value2.toString(ValueBordered));
	QString strCondition = functorTemplate.stringTemplate().arg(field, strDate1, strDate2);

	_conditions.push_back(strCondition);
}


void QstQueryComposer::_addVariantCondition(const QString &field,
												 const QstValue &value)
{
	_StringTemplate functorTemplate = _functorToStringTemplate(value.functor());

	QString strVal = value.toString();
	QString strCondition = functorTemplate.string(field, strVal);

	_conditions.push_back(strCondition);
}

void QstQueryComposer::_addStringCondition(const QString &field,
												const QstValue &value)
{
	_StringTemplate val1Template = _bracesToStringTemplate(value.braces());
	_StringTemplate functorTemplate = _functorToStringTemplate(value.functor());

	QString strCondition = functorTemplate.string(field, val1Template.string(value.toString()));

	_conditions.push_back(strCondition);
}

void QstQueryComposer::_addDateCondition(const QString &field,
											  const QstValue &value)
{
	_StringTemplate functorTemplate =_functorToStringTemplate(value.functor());

	QDate dateVal = value.value().toDate();
	QString strDate = DATE_CONVERT.arg(dateVal.toString(DATE_FORMAT));
	QString strCondition = functorTemplate.string(field, strDate);

	_conditions.push_back(strCondition);
}


void QstQueryComposer::_addBetweenCondition(const QString &field,
												 const QstValue &value1,
												 const QstValue &value2)
{
	const QVariant &rVarVal1 = value1.value();
	const QVariant &rVarVal2 = value2.value();

	if (rVarVal1.type() == QVariant::Date
		|| rVarVal1.type() == QVariant::DateTime)
		_addDateBetweenCondition(field, rVarVal1, rVarVal2);
	else
		if (rVarVal1.type() == QVariant::ByteArray || rVarVal1.type() == QVariant::String)
			_addStringBetweenCondition(field, value1, value2);
		else
			_addVariantBetweenCondition(field, value1, value2);
}



void QstQueryComposer::_addCondition(const QString &field,
										  const QstValue &value)
{
	const QVariant &rVarVal = value.value();

	if (rVarVal.type() == QVariant::Date)
		_addDateCondition(field, value);
	else
		if (rVarVal.type() == QVariant::ByteArray || rVarVal.type() == QVariant::String)
			_addStringCondition(field, value);
		else
			_addVariantCondition(field, value);
}

void QstQueryComposer::_addField(const QString &field)
{
	Q_ASSERT(!field.isEmpty());
	_fields.append(field);
}

void QstQueryComposer::_addInsertValue(const QString &field,
									   const QstValue &value)
{
	QString strValue;

	if (value.value().type() == QVariant::Date)
	{
		strValue = DATE_CONVERT.arg(value.toString(ValueBordered));
	}
	else
	if (value.value().type() == QVariant::String
		|| value.value().type() == QVariant::ByteArray
		|| value.value().type() == QVariant::DateTime)

		strValue = value.toString(ValueBordered, BracesNotUse, NullSubstitute);
	else
		strValue = value.toString(ValueNotBordered, BracesNotUse, NullSubstitute);

	_insertValues.insert(field, strValue);
}

void QstQueryComposer::_addUpdateValue(const QString &field,
									   const QstValue &value)
{
	if (value.isValid())
	{
		QString strValue;

		if (value.value().type() == QVariant::Date)
		{
			strValue = DATE_CONVERT.arg(value.toString(ValueBordered));
		}
		else

		if (value.value().type() == QVariant::String
			|| value.value().type() == QVariant::ByteArray
			|| value.value().type() == QVariant::DateTime)

			strValue = value.toString(ValueBordered, BracesNotUse, NullSubstitute);
		else
			strValue = value.toString(ValueNotBordered, BracesNotUse, NullSubstitute);

		_updateValues.insert(field, strValue);
	}
}

void QstQueryComposer::_addParameter(const QstValue &value)
{
	if (!value.isValid() || value.isNull())
		_parameters.push_back(value.toString(ValueNotBordered, BracesNotUse, NullSubstitute));
	else
	{
		if (value.value().type() == QVariant::String
			|| value.value().type() == QVariant::ByteArray
			|| value.value().type() == QVariant::Date
			|| value.value().type() == QVariant::DateTime)

			_parameters.push_back(value.toString(ValueBordered, BracesNotUse, NullSubstitute));

		else

			_parameters.push_back(value.toString(ValueNotBordered, BracesNotUse, NullSubstitute));
	}
}

void QstQueryComposer::_addOrderByField(const QString &field)
{
	Q_ASSERT(!field.isEmpty());
	_orderByItems.append(field);
}

void QstQueryComposer::_addGroupByField(const QString &field)
{
	Q_ASSERT(!field.isEmpty());
	_groupByItems.append(field);
}

QstQueryComposer::_StringTemplate QstQueryComposer::_bracesToStringTemplate(const FuzzyBraces &braces) const
{
	switch(braces)
	{
		case BracesLeft:	return _StringTemplate(1, QString("'%%1'"));
		case BracesRight:	return _StringTemplate(1, QString("'%1%'"));
		case BracesBoth:	return _StringTemplate(1, QString("'%%1%'"));
		case BracesNone:	return _StringTemplate(1, QString("'%1'"));
	};
return _StringTemplate(1, QString("%1"));
};

QstQueryComposer::_StringTemplate QstQueryComposer::_functorToStringTemplate(const CompareFunctor &functor) const
{
//Здесь %1 - поле, %2 - значение1, %3 - значение2.

	switch(functor)
	{
	case FunctorLike:			return _StringTemplate(2, " %1 LIKE %2 ");
	case FunctorEqual:			return _StringTemplate(2, " %1 = %2 ");
	case FunctorNotEqual:		return _StringTemplate(2, " %1 <> %2 ");
	case FunctorLess:			return _StringTemplate(2, " %1 < %2 ");
	case FunctorGreater:		return _StringTemplate(2, " %1 > %2 ");
	case FunctorLessEqual:		return _StringTemplate(2, " %1 <= %2 ");
	case FunctorGreaterEqual:	return _StringTemplate(2, " %1 >= %2 ");
	case FunctorBetween:		return _StringTemplate(3, " %1 BETWEEN %2 AND %3 ");
	case FunctorIsNull:			return _StringTemplate(1, " %1 IS NULL ");
	case FunctorIsNotNull:		return _StringTemplate(1, " %1 IS NOT NULL ");
	case FunctorNotEqualOrNull:		return _StringTemplate(2, " %1 <> %2 OR %1 IS NULL ");
	case FunctorEqualOrNull:		return _StringTemplate(2, " %1 == %2 OR %1 IS NULL ");
	case FunctorLessOrNull:			return _StringTemplate(2, " %1 < %2 OR %1 IS NULL ");
	case FunctorGreaterOrNull:		return _StringTemplate(2, " %1 > %2 OR %1 IS NULL ");
	case FunctorLessEqualOrNull:	return _StringTemplate(2, " %1 <= %2 OR %1 IS NULL ");
	case FunctorGreaterEqualOrNull:	return _StringTemplate(2, " %1 >= %2 OR %1 IS NULL ");
	case FunctorLikeOrNull:			return _StringTemplate(2, " %1 LIKE %2 OR %1 IS NULL ");


	 case FunctorNone:	Q_ASSERT_X(false,
							   "QstQueryComposer::_functorToStringTemplate",
							   "Empty functor - no template available.");

	case FunctorUnary_Mask:
	case FunctorBinary_Mask:
//	case FunctorTrinary_Mask:	// Закомментировано, т.к. содержит 1 функтор.
	case FunctorCombined_Mask:
		 Q_ASSERT_X(false,
					"QstQueryComposer::_functorToStringTemplate",
					"Too many Functors or unknown functor.");

	};
return _StringTemplate();
}

QString  QstQueryComposer::_SELECT_SQL_QUERY() const
{
	return
		_SELECT()	+
		_FROM()		+
		_WHERE()	+
		_GROUP_BY()+
		_HAVING()+
		_ORDER_BY();
}

QString  QstQueryComposer::_INSERT_SQL_QUERY() const
{
	if (_insertValues.empty())
		return QString();

	return	_INSERT() +
			_VALUES();
}

QString  QstQueryComposer::_UPDATE_SQL_QUERY() const
{
	if (_updateValues.empty())
		return QString();

	return _UPDATE()	+
			_SET()		+
			_WHERE();
}

QString  QstQueryComposer::_DELETE_SQL_QUERY() const
{
	return _DELETE()	+
			_WHERE();
}

QString  QstQueryComposer::_EXECUTE_SQL_QUERY() const
{
	return	_EXECUTE()	+
			_PARAMETERS() ;
}

QString QstQueryComposer::_CLAUSE(const QString &stringClause,
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


QString QstQueryComposer::_SELECT() const
{
	return _CLAUSE(_fieldsClause, _fields, SQL_SELECT, true, ", ");
}

QString QstQueryComposer::_INSERT() const
{
	return
		_CLAUSE(_sourcesClause, _sources, SQL_INSERT, true, ", ") +
		_CLAUSE("", _insertValues.uniqueKeys(), "", true, ", ", "(", ")");
}

QString QstQueryComposer::_VALUES() const
{
	return _CLAUSE("", _insertValues.values(), SQL_VALUES, true, ", ", "(", ")");
}

QString QstQueryComposer::_UPDATE() const
{
	return _CLAUSE(_sourcesClause, _sources, SQL_UPDATE, true, ", ");
}

QString QstQueryComposer::_SET() const
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

QString QstQueryComposer::_DELETE() const
{
	return _CLAUSE(_sourcesClause, _sources, SQL_DELETE, true, ", ");
}

QString QstQueryComposer::_FROM() const
{
	return _CLAUSE(_sourcesClause, _sources, SQL_FROM, true, ", ");
}

QString QstQueryComposer::_WHERE() const
{
	return _CLAUSE(_whereClause, _conditions, SQL_WHERE, false, " AND ");
}

QString QstQueryComposer::_EXECUTE() const
{
	return _CLAUSE("", _sources, SQL_EXECUTE, true, ", ");
}

QString QstQueryComposer::_PARAMETERS() const
{
	return _CLAUSE("", _parameters, "", false, ", ");
}

QString QstQueryComposer::_ORDER_BY() const
{
	return _CLAUSE(_orderByClause, _orderByItems, SQL_ORDER_BY, false, ", ");
}

QString QstQueryComposer::_GROUP_BY() const
{
	return _CLAUSE(_groupByClause, _groupByItems, SQL_GROUP_BY, false, ", ");
}

QString QstQueryComposer::_HAVING() const
{
	return _CLAUSE(_havingClause, _havingItems, SQL_HAVING, false, " AND ");
}

}
