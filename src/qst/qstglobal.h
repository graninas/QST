/****************************************************************************
** QST 0.4.2a beta
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

#ifndef QSTGLOBAL_H
#define QSTGLOBAL_H


#include <QString>
#include <QMap>

const QString QST_DEFAULT_DATABASE_DRIVER = "QODBC";

#define QST_VALUE_SET_DEFAULT_FUNCTORS
#undef  QST_EXTENDED_DEBUG_MESSAGES
#define QST_USE_BOTH_BRACES_WHEN_NONE_BRACES
#define	QST_ALWAYS_EXTRACT_ITEM_NAME
#undef  QST_VALUE_NULL_SUBSTITUTE_BY_DEFAULT


namespace Qst
{
	//! Функторы сравнения SQL.
	typedef enum {
				  FunctorNone		= 0x0,
				  FunctorNotEqual	= 0x1,
				  FunctorEqual		= 0x2,
				  FunctorLess		= 0x4,
				  FunctorGreater	= 0x8,
				  FunctorLessEqual	= 0x10,
				  FunctorGreaterEqual	= 0x20,
				  FunctorLike			= 0x40,
				  FunctorNotEqualOrNull = 0x80,
				  FunctorEqualOrNull	= 0x100,
				  FunctorLessOrNull		= 0x200,
				  FunctorGreaterOrNull	= 0x400,
				  FunctorLessEqualOrNull	= 0x800,
				  FunctorGreaterEqualOrNull = 0x1000,
				  FunctorLikeOrNull			= 0x2000,
				  FunctorBetween			= 0x4000,
				  FunctorIsNull				= 0x8000,
				  FunctorIsNotNull			= 0x10000,
				FunctorUnary_Mask  = FunctorIsNull
									| FunctorIsNotNull,
				FunctorBinary_Mask = FunctorNotEqual
									| FunctorEqual
									| FunctorLess
									| FunctorGreater
									| FunctorLessEqual
									| FunctorGreaterEqual
									| FunctorLike,
				FunctorTrinary_Mask = FunctorBetween,
				FunctorCombined_Mask = FunctorNotEqualOrNull
									   | FunctorEqualOrNull
									   | FunctorLessOrNull
									   | FunctorGreaterOrNull
									   | FunctorLikeOrNull
			  }
		CompareFunctor;



	//! "Нечеткие скобки"
	/*! Определяют, заключать ли строковую константу в %, н-р: '%лектриче%'. */
	typedef enum	{
						BracesNone = 0,
						BracesLeft,
						BracesRight,
						BracesBoth
					}
				FuzzyBraces;


	//! Политика сравнения строк в фильтре.
	/*! Используется в специальном конструкторе класса QstValue. */
	typedef enum	{
						MatchFlagDependent = 0,	/*!< Точность сравнения строк определяется флагом (fullStringMatch в конструкторе QstValue).*/
						MatchBracesDependent,	/*!< Точность сравнения строк определяется нечеткими скобками, сохраненными в QstValue. */
						MatchFuzzy,				/*!< Нечеткие скобки устанавливаются в BracesBoth, а функтор - в FunctorLike. */
						MatchFull				/*!< Нечеткие скобки игнорируются, а функтор становится FunctorEqual. */
					}
			MatchPolicy;



	//! Номер значения QstValue в QstField.
	/*! Используется для получения первого или второго значения
	из QstField, если QstField является бинарным фильтром BETWEEN
	(QstField::isBinaryFilter() == true).

	Пример.
\code
QstField qstField("order_date",
				  QstValue(QDate(2009,1,10)),
				  QstValue(QDate(2009,2,15))  );

QVariant res = qstField.value(OrderSecond).value(); //res == QDate(2009,2,15)
\endcode
	*/
	typedef enum	{
						OrderFirst = 0,
						OrderSecond = 1
					}
			BinaryValueOrder;


	//! Типы запросов SQL.
	typedef enum	{
						QuerySelect,
						QueryInsert,
						QueryUpdate,
						QueryDelete,
						QueryExecute
					}
			QueryType;


	//! Отображаемость поля.
	/*! Поля QstField, отмеченные как FieldVisible, будут отображаться
	во view и comboBox. */
	typedef enum	{
						VisibilityNone = 0,
						FieldVisible,
						FieldInvisible
					}
			FieldVisibility;


	//! Назначение поля.
	/*! Указывает, в какой секции SQL-запроса используется данное поле. */
	typedef enum {
				  PurposeNone		= 0x0,
				  PurposeSelect		= 0x1,
				  PurposeWhere		= 0x2,
				  PurposeInsert		= 0x4,
				  PurposeUpdate		= 0x8,
				  PurposeOrderBy	= 0x10,
				  PurposeGroupBy	= 0x20,
				  PurposeParameter	= 0x40,
				  PurposeAll_Mask	= PurposeSelect
									| PurposeWhere
									| PurposeInsert
									| PurposeUpdate
									| PurposeOrderBy
									| PurposeGroupBy
									| PurposeParameter,
				  PurposeValued_Mask =
									PurposeInsert
								  | PurposeUpdate
								  | PurposeParameter
								  | PurposeWhere,
				  PurposeModifying_Mask =
									PurposeInsert
								  | PurposeUpdate,
				  PurposeHasFieldName_Mask =
										PurposeSelect
									  | PurposeWhere
									  | PurposeInsert
									  | PurposeUpdate
									  | PurposeOrderBy
									  | PurposeGroupBy,
				  PurposeAllButSelect_Mask =
										PurposeAll_Mask ^ PurposeSelect
								  } FieldPurpose;

	Q_DECLARE_FLAGS(FieldPurposes, FieldPurpose)
	Q_DECLARE_OPERATORS_FOR_FLAGS(FieldPurposes)


	//! Секции SQL-запросов.
	typedef enum {
					ClauseNone		= 0x0,
					ClauseSelect	= 0x1,
					ClauseWhere		= 0x2,
					ClauseInsertInto = 0x4,
					ClauseUpdate	= 0x8,
					ClauseOrderBy	= 0x10,
					ClauseGroupBy	= 0x20,
					ClauseParameters = 0x40,
					ClauseHaving	= 0x100,
					ClauseFrom		= 0x200,
					ClauseValues	= 0x400,
					ClauseSet		= 0x800,
					ClauseDelete	= 0x1000,
					ClauseExecute	= 0x2000
				}
			QueryClause;

	Q_DECLARE_FLAGS(QueryClauses, QueryClause)
	Q_DECLARE_OPERATORS_FOR_FLAGS(QueryClauses)


	typedef QMap<QueryClause, QString> QueryClauseMap;


	//! Роль поля.
	typedef enum	{
						RoleNone = 0,			/*!< Роль отсутствует. */
						RolePrimaryKey	= 1,	/*!< Первичный ключ. */
						RoleParentKey = 2		/*!< Ключ на родительскую запись. */
					}
			FieldRole;


	//! Обрамление значения.
	/*! При генерации запросов апострофами обрамляются даты, строки. */
	typedef enum	{
						ValueNotBordered = 0,
						ValueBordered
					}
			ValueBordering;

	//! Подстановка слова "NULL" при генерации запросов.
	typedef enum	{
						NullNotSubstitute = 0,
						NullSubstitute
					}
			NullSubstitution;

	//! Использование нечетких скобок в строках, если они заданы параметром FuzzyBraces.
	typedef enum	{
						BracesNotUse,
						BracesUse
					}
			FuzzyBracesUsage;

	//! Тип модели - плоская или иерархическая (древовидная).
	typedef enum	{
						ModelPlain,
						ModelTree
					} ModelType;

	//! Специальный тип для создания нулевого QstValue.
	/*! Передается в конструктор QstValue, чтобы создать
	валидный, но нулевой QstValue.

	Пример.
\code
QstValue val1 = QstValue();
qDebug() << val1.isValid();		//false
qDebug() << val1.isNull();		//true

QstValue val2(nt_null);
qDebug() << val2.isValid();		//true
qDebug() << val2.isNull();		//true
\endcode
	*/
	typedef enum {Null = 0} NullType;


	// Константы, упрощающие жизнь.
	const QString ID_VALUE			= "ID";
	const QString PARENT_ID_VALUE	= "Parent_ID";
	const QString EXCL_ID			= "Excl_ID";
	const int	  LAST_ID			= 0xffff;
	const QString MAX_ID			= "max(ID)";
	const QString COUNT_ID			= "count(ID)";
}

#endif // QSTGLOBAL_H
