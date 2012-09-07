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

#ifndef QSTQUERYCOMPOSER_H
#define QSTQUERYCOMPOSER_H

#include "qstglobal.h"
#include "qstfield.h"

#include <QString>
#include <QStringList>
#include <QDate>
#include <QVariant>
#include <QMap>

namespace Qst
{

class QstQueryComposer
{
public:

	typedef QMap<QString, QString>	ValuesMapType;

public:
	QstQueryComposer();

	void clear();

	void addSource(const QString &source);

	void addSelectClauseField(const QstField &field);
	void addStuffField(const QstField &field);

	QString query(const QueryType &queryType = QuerySelect) const;

	QueryClauseMap queryParts(const QueryClauses &clauses) const;

private:

	QString _fieldsClause;
	QString _sourcesClause;
	QString _whereClause;
	QString _orderByClause;
	QString _groupByClause;
	QString _havingClause;

	QStringList			_fields;
	QStringList			_conditions;
	QStringList			_sources;
	QStringList			_parameters;
	ValuesMapType	_insertValues;
	ValuesMapType	_updateValues;


	QStringList _orderByItems;
	QStringList _groupByItems;
	QStringList _havingItems;

	class _StringTemplate
	{
	public:
		_StringTemplate(const int & argsCount, const QString stringTemplate)
			:
			_argsCount(argsCount),
			_stringTemplate(stringTemplate)
		{
		}

		_StringTemplate()
			:
			_argsCount(0),
			_stringTemplate(QString())
		{
		}

		int argsCount() const
		{
			return _argsCount;
		}

		QString stringTemplate() const
		{
			return _stringTemplate;
		}

		QString string(const QString & arg1) const
		{
			if (_argsCount == 3)
				return _stringTemplate.arg(arg1, "", "");
			else
				if (_argsCount == 2)
					return _stringTemplate.arg(arg1, "");

			return _stringTemplate.arg(arg1);
		}

		QString string(const QString & arg1, const QString & arg2) const
		{
			if (_argsCount == 3)
				return _stringTemplate.arg(arg1, arg2, "");
			else
				if (_argsCount == 2)
					return _stringTemplate.arg(arg1, arg2);

			return _stringTemplate.arg(arg1);
		}

		QString string(const QString & arg1, const QString & arg2, const QString & arg3) const
		{
			if (_argsCount == 3)
				return _stringTemplate.arg(arg1, arg2, arg3);
			else
				if (_argsCount == 2)
					return _stringTemplate.arg(arg1, arg2);

			return _stringTemplate.arg(arg1);
		}

	private:
		int			_argsCount;
		QString		_stringTemplate;
	};


	void	_addVariantBetweenCondition(const QString &field,
										const QstValue &value1,
										const QstValue &value2);

	void	_addStringBetweenCondition(const QString &field,
									   const QstValue &value1,
									   const QstValue &value2);

	void	_addDateBetweenCondition(const QString &field,
									 const QstValue &value1,
									 const QstValue &value2);


	void	_addVariantCondition(const QString &field,
								 const QstValue &value);

	void	_addStringCondition(const QString &field,
								const QstValue &value);

	void	_addDateCondition(const QString &field,
							  const QstValue &value);



	void	_addBetweenCondition(const QString &field,
								 const QstValue &value1,
								 const QstValue &value2);

	void	_addCondition(const QString &field, const QstValue &value);

	void	_addField(const QString &field);

	void	_addInsertValue(const QString &field, const QstValue &value);
	void	_addUpdateValue(const QString &field, const QstValue &value);
	void	_addParameter(const QstValue &value);
	void	_addOrderByField(const QString &field);
	void	_addGroupByField(const QString &field);

	_StringTemplate _bracesToStringTemplate(const FuzzyBraces &braces) const;
	_StringTemplate _functorToStringTemplate(const CompareFunctor &functor) const;


	QString _SELECT_SQL_QUERY() const;
	QString _INSERT_SQL_QUERY() const;
	QString _UPDATE_SQL_QUERY() const;
	QString _DELETE_SQL_QUERY() const;
	QString _EXECUTE_SQL_QUERY() const;


	QString _CLAUSE(const QString &stringClause,
				   const QStringList &items,
				   const QString &clauseWord,
				   const bool &clauseWordRequired,
				   const QString &itemsDelimiter,
				   const QString &openBrace = "",
				   const QString &closeBrace = "") const;

	QString _SELECT() const;

	QString _INSERT() const;
	QString _VALUES() const;

	QString _UPDATE() const;
	QString _SET() const;

	QString _DELETE() const;

	QString _FROM() const;
	QString _WHERE() const;

	QString _EXECUTE() const;
	QString _PARAMETERS() const;

	QString _ORDER_BY() const;
	QString _GROUP_BY() const;
	QString _HAVING() const;
};



}

#endif // QSTQUERYCOMPOSER_H
