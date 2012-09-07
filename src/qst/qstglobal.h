/****************************************************************************
** QST 0.4.1 pre-alpha
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
#include <QDebug>


#define QST_VALUE_SET_DEFAULT_FUNCTORS
#undef  QST_EXTENDED_DEBUG_MESSAGES
#define QST_USE_BOTH_BRACES_WHEN_NONE_BRACES
#define	QST_ALWAYS_EXTRACT_ITEM_NAME


namespace Qst
{
	//! Функторы сравнения SQL.
	/*! Особую роль играет функтор fo_between. Для него требуется 2 значения,
		например: BETWEEN dateBegin AND dateEnd.

		Функторы, оканчивающиеся на _null, хорошо применять тогда, когда
		аргумент может быть задан, а может быть и NULL.
		Такое бывает, если у записи в БД есть период ее актуальности
		(dateBegin, dateEnd). */
	typedef enum {FunctorNone = 0,
				  FunctorNotEqual = 1,
				  FunctorEqual = 2,
				  FunctorLess,
				  FunctorGreater,
				  FunctorLessEqual,
				  FunctorGreaterEqual,
				  FunctorLike,
				  FunctorNotEqualOrNull,
				  FunctorEqualOrNull,
				  FunctorLessOrNull,
				  FunctorGreaterOrNull,
				  FunctorLessEqualOrNull,
				  FunctorGreaterEqualOrNull,
				  FunctorLikeOrNull,
				  FunctorBetween,
				  FunctorIsNull,
				  FunctorIsNotNull}
		CompareFunctor;


	//! "Нечеткие скобки" :)
	/*! Определяют, заключать ли строковую константу в %, н-р: '%лектриче%'. */
	typedef enum {BracesNone = 0,
				  BracesLeft,
				  BracesRight,
				  BracesBoth}	FuzzyBraces;


	//! Порядок значения SqlValue в SqlField.
	/*! Используется для получения первого или второго значения
	из SqlField, если \n SqlField.isBinaryFilter() == true. Пример:
	\code
	SqlField sqlField("order_date",
					  SqlValue(QDate(2009,1,10)),
					  SqlValue(QDate(2009,2,15))  );
	bool isBinaryFilter = sqlField.isBinaryFilter();	//true
	QVariant res = sqlField.value(bvo_second).value(); //res == QDate(2009,2,15)
	\endcode */
	typedef enum {OrderFirst = 0,
				  OrderSecond = 1} BinaryValueOrder;

	//! Типы запросов SQL.
	/*! Используются при создании описателя (SqlQueryDescriptor).*/
	typedef enum {QuerySelect,
				  QueryInsert,
				  QueryUpdate,
				  QueryDelete,
				  QueryExecute} QueryType;

	//! Отображаемость поля.
	/*! Поля SqlField, отмеченные как fv_visible, будут отображаться
	во view и comboBox, а так же по этим полям можно будет запрашивать
	информацию с помощью функций AbstractModelHandler::SelectToValue().*/
	typedef enum {VisibilityNone = 0,
				  FieldVisible,
				  FieldInvisible} FieldVisibility;


	//! Назначение поля.
	/*! Указывает, в какой секции SQL-запроса используется данное поле. */

	typedef enum {PurposeNone = 0x0,
				  PurposeSelect = 0x1,
				  PurposeWhere = 0x2,
				  PurposeInsert = 0x4,
				  PurposeUpdate = 0x8,
				  PurposeOrderBy = 0x10,
				  PurposeGroupBy = 0x20,
				  PurposeParameter = 0x40,
				  PurposeAll_Mask = PurposeSelect
									| PurposeWhere
									| PurposeInsert
									| PurposeUpdate
									| PurposeOrderBy
									| PurposeGroupBy
									| PurposeParameter,
				  PurposeAllValued_Mask =
									PurposeInsert
								  | PurposeUpdate
								  | PurposeWhere
								  | PurposeParameter,
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


	//! Роль поля.
	/*! Для полей, не являющихся ключевыми или ссылками на родительскую
	запись, при создании SqlField следует указывать (там, где необходимо) fr_none.*/
	typedef enum {
				RoleNone = 0,	/*!< Роль отсутствует. */
				RolePrimaryKey	= 1,	/*!< Идентификатор (первичный ключ). */
				RoleParentKey = 2	/*!< идентификатор родительской записи. */
				} FieldRole;


	//! Обрамление значения.
	/*! Апострофами обрамляются даты, строки. */
	typedef enum {ValueNotBordered = 0,
				  ValueBordered} ValueBordering;

	//! Подстановка NULL.
	/*!  */
	typedef enum {NullNotSubstitute = 0,
				  NullSubstitute} NullSubstitution;

	//! Использование "нечетких скобок" в строках, если они заданы параметром FuzzyBraces.
	typedef enum {BracesNotUse,
				  BracesUse} FuzzyBracesUsage;

	//! Тип модели - плоская или иерархическая (древовидная).
	typedef enum {ModelPlain,
				  ModelTree} ModelType;

	//! Специальный тип для создания нулевого SqlValue.
	/*! Передается в конструктор SqlValue, чтобы создать
	валидный, но нулевой SqlValue.

	\code
	SqlValue val1 = SqlValue();
	qDebug() << val1.isValid();		//false
	qDebug() << val1.isNull();		//true

	SqlValue val2(nt_null);
	qDebug() << val2.isValid();		//true
	qDebug() << val2.isNull();		//true
	\endcode
	*/
	typedef enum {Null = 0} NullType;

	//! Константы, упрощающие жизнь.
	const QString ID_VALUE			= "ID";
	const QString PARENT_ID_VALUE	= "Parent_ID";
	const QString EXCL_ID			= "Excl_ID";
	const int	  LAST_ID			= 0xffff;
	const QString MAX_ID			= "max(ID)";
	const QString COUNT_ID			= "count(ID)";
}

#endif // QSTGLOBAL_H
