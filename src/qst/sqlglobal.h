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

#ifndef SQLGLOBAL_H
#define SQLGLOBAL_H


#include <QString>
#include <QDebug>
#include <QFlags>


#define SQL_VALUE_SET_DEFAULT_FUNCTORS
#undef  SQL_EXTENDED_DEBUG_MESSAGES
#define SQL_USE_BRACES_PRIORITY_GREATER_THEN_NONE_BRACES


namespace Sql
{
	//! Функторы сравнения SQL.
	/*! Особую роль играет функтор fo_between. Для него требуется 2 значения,
		например: BETWEEN dateBegin AND dateEnd.

		Функторы, оканчивающиеся на _null, хорошо применять тогда, когда
		аргумент может быть задан, а может быть и NULL.
		Такое бывает, если у записи в БД есть период ее актуальности
		(dateBegin, dateEnd).

	\attention В QST версии 0.4 перечисление и его элементы переименованы в
	соответствии со стилем Qt 4.x. */
	typedef enum {fo_none = 0, fo_not_equal = 1, fo_equal = 2, fo_less,
				  fo_greater, fo_less_equal,
				  fo_greater_equal, fo_like,
				  fo_not_equal_or_null, fo_equal_or_null, fo_less_or_null,
				  fo_greater_or_null, fo_less_equal_or_null,
				  fo_greater_equal_or_null, fo_like_or_null,
				  fo_between, fo_is_null, fo_is_not_null}
		Functor;


	//! "Нечеткие скобки"
	/*! Определяют, заключать ли строковую константу в %, н-р: '%лектриче%'.

	\attention В QST версии 0.4 перечисление и его элементы переименованы в
	соответствии со стилем Qt 4.x. */
	typedef enum {fb_none = 0, fb_left, fb_right, fb_both}	FuzzyBraces;


	//! Порядок значения SqlValue в SqlField.
	/*! Используется для получения первого или второго значения
	из SqlField, если \n SqlField.isBinaryFilter() == true. Пример:
	\code
	SqlField sqlField("order_date",
					  SqlValue(QDate(2009,1,10)),
					  SqlValue(QDate(2009,2,15))  );
	bool isBinaryFilter = sqlField.isBinaryFilter();	//true
	QVariant res = sqlField.value(bvo_second).value(); //res == QDate(2009,2,15)
	\endcode

	\attention В QST версии 0.4 перечисление и его элементы переименованы в
	соответствии со стилем Qt 4.x. */
	typedef enum {bvo_first=0, bvo_second=1} BinaryValueOrder;

	//! Типы запросов SQL.
	/*! Используются при создании описателя (SqlQueryDescriptor).

	\attention В QST версии 0.4 перечисление и его элементы переименованы в
	соответствии со стилем Qt 4.x. */
	typedef enum {sql_select, sql_insert, sql_update, sql_delete,
					sql_execute} SqlQueryTypes;

	//! Отображаемость поля.
	/*! Поля SqlField, отмеченные как fv_visible, будут отображаться
	во view и comboBox, а так же по этим полям можно будет запрашивать
	информацию с помощью функций AbstractModelHandler::SelectToValue().

	\attention В QST версии 0.4 перечисление и его элементы переименованы в
	соответствии со стилем Qt 4.x. */
	typedef enum {fv_none = 0, fv_visible, fv_invisible} SqlFieldVisibility;


	//! Назначение поля.
	/*! Указывает, в какой секции SQL-запроса используется данное поле.

	\attention В QST версии 0.4 перечисление и его элементы переименованы в
	соответствии со стилем Qt 4.x. */
	typedef enum {fp_none = 0x0,
				  fp_select = 0x1,
				  fp_where = 0x2,
				  fp_insert = 0x4,
				  fp_update = 0x8,
				  fp_order_by = 0x10,
				  fp_group_by = 0x20,
				  fp_parameter = 0x40,

				  fp_all_valued = fp_insert
								  | fp_update
								  | fp_where
								  | fp_parameter,
				  fp_modifying = fp_insert
								 | fp_update,
				  fp_has_field_name = fp_select
									  | fp_where
									  | fp_insert
									  | fp_update
									  | fp_order_by
									  | fp_group_by
								  } SqlFieldPurpose;

	Q_DECLARE_FLAGS(SqlFieldPurposes, SqlFieldPurpose)
	Q_DECLARE_OPERATORS_FOR_FLAGS(SqlFieldPurposes)

	//! Роль поля.
	/*! Для полей, не являющихся ключевыми или ссылками на родительскую
	запись, при создании SqlField следует указывать (там, где необходимо) fr_none.

	\attention В QST версии 0.4 перечисление и его элементы переименованы в
	соответствии со стилем Qt 4.x. */
	typedef enum {
				fr_none = 0,	/*!< Роль отсутствует. */
				fr_id	= 1,	/*!< Идентификатор (первичный ключ). */
				fr_parent = 2	/*!< идентификатор родительской записи. */
				} SqlFieldRoles;


	//! Обрамление значения.
	/*! Апострофами обрамляются даты, строки.

	\attention В QST версии 0.4 перечисление и его элементы переименованы в
	соответствии со стилем Qt 4.x. */
	typedef enum {vb_none = 0, vb_bordered} SqlValueBordering;

	//! Игнорирование обрамления.
	/*! Может быть выставлено, чтобы игнорировать / не игнорировать
	параметр SqlValueBordering. Это бывает нужно при создании специфического
	описателя.

	\attention В QST версии 0.4 перечисление и его элементы переименованы в
	соответствии со стилем Qt 4.x. */
	typedef enum {vbi_not_ignore = 0, vbi_ignore} SqlValueBorderIgnoring;

	//! Заполнение NULL-значением генерируемой строки в методе SqlValue::toString().
	/*! Возвращается QString() при vfn_not и SQL_NULL_VALUE при vfn_fill.

	\attention В QST версии 0.4 перечисление и его элементы переименованы в
	соответствии со стилем Qt 4.x. */
	typedef enum {vfn_not = 0, vfn_fill} SqlValueFillNull;

	//! Использование "нечетких скобок" в строках, если они заданы параметром FuzzyBraces.
	/*! \attention В QST версии 0.4 перечисление и его элементы переименованы в
	соответствии со стилем Qt 4.x.  */
	typedef enum {vub_not, vub_use} SqlValueUseBraces;

	//! Тип модели - плоская или иерархическая (древовидная).
	/*! \attention В QST версии 0.4 перечисление и его элементы переименованы в
	соответствии со стилем Qt 4.x.  */
	typedef enum {mt_plain, mt_tree} SqlQueryModelTypes;

	typedef enum {so_asc, so_desc} SqlSortOrder;

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

	\attention В QST версии 0.4 перечисление и его элементы переименованы в
	соответствии со стилем Qt 4.x.
	*/
	typedef enum {nt_null = 0} NullType;

	//! Константы, упрощающие жизнь.
	const QString ID_VALUE			= "ID";
	const QString PARENT_ID_VALUE	= "Parent_ID";
	const QString EXCL_ID			= "Excl_ID";
	const int	  LAST_ID			= 0xffff;
	const QString MAX_ID			= "max(ID)";
	const QString COUNT_ID			= "count(ID)";
}

#endif // SQLGLOBAL_H
