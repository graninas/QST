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

#include "qsttreequerymodel.h"

#include <QSqlRecord>

namespace Qst
{

	/*!
		\class QstTreeQueryModel
		\brief
		Древовидная модель данных. Использует первичный ключ и ключ
		на родительскую запись для создания древовидной структуры.

		\inmodule Qst
	*/

/*! Основной конструктор и конструктор по умолчанию. */
QstTreeQueryModel::QstTreeQueryModel(QObject* parent)
	:
	QAbstractItemModel(parent),
	_root(NULL),
	_keyField(-1),
	_parentField(-1)
{
}

/*! Деструктор. Рекурсивно удаляет элементы древовидной структуты. */
QstTreeQueryModel::~QstTreeQueryModel()
{
	if (_root)
		delete _root;
	_root = NULL;
}

/*! Устанавливает индекс ключевого поля.

	Индекс используется в классах QstBatch и QstAbstractModelHandler. */
void QstTreeQueryModel::setKeyField(const int &keyField)
{
	_keyField = keyField;
}

/*! Устанавливает индекс ключа на родительскую запись.

	Индекс используется в классах QstBatch и QstAbstractModelHandler. */
void QstTreeQueryModel::setParentField(const int &parentField)
{
	_parentField = parentField;
}

/*! Ограничивает использование QSqlQuery, поскольку это может привести
	к неопределенному поведению. */
void QstTreeQueryModel::setQuery(const QSqlQuery & query)
{
	Q_ASSERT_X(false, "setQuery", "Forbidden function.");
}

/*! Устанавливает запрос, подключается к базе данных, получает данные
	и создает древовидную структуру. */
void QstTreeQueryModel::setQuery(const QString& query, const QSqlDatabase &db)
{
	_query = QSqlQuery(query, db);

	_lastError = _query.lastError();

	Q_ASSERT(_query.isActive());

	QSqlRecord record = _query.record();
	QList<QVariant> rootHeader;
	int fieldsCount = record.count();


	// Вписываем в шапку все поля.
	for (int i = 0; i < fieldsCount; ++i)
		rootHeader << record.fieldName(i);

	// Если было дерево, удаляем. Создаем новое дерево.
	if (_root) delete _root;
		_root = new QstTreeItem(rootHeader);
	Q_CHECK_PTR(_root);

	_createTree();
}


/*! Возвращает количество дочерних элементов у parent.

	Если parent инвалидный, возвращает количество дочерних элементов
	для коренного элемента.*/
int QstTreeQueryModel::rowCount(const QModelIndex& parent) const
{
	Q_ASSERT(_root != NULL);

	QstTreeItem *parentItem;
	if (parent.column() > 0)
		return 0;

	if (!parent.isValid())
		parentItem = _root;
	else
		parentItem = static_cast<QstTreeItem*>(parent.internalPointer());

	return parentItem->childCount();
}

/*! Возвращает количество колонок у parent.

	Если parent инвалидный, возвращает количество колонок
	для коренного элемента*/
int QstTreeQueryModel::columnCount(const QModelIndex& parent) const
{
	Q_ASSERT(_root != NULL);

	QstTreeItem *item;

	if (parent.isValid())
		item = static_cast<QstTreeItem*>(parent.internalPointer());
	else
		item = _root;

	Q_ASSERT(item != NULL);

	return item->colsCount();
}

/*! Возвращает флаги элемента. */
Qt::ItemFlags QstTreeQueryModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return 0;

	return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

/*! Возвращает данные элемента. */
QVariant QstTreeQueryModel::data(const QModelIndex& index, int role) const
{
	Q_ASSERT(_root != NULL);

	if (!index.isValid())
		return QVariant();

	if (role != Qt::DisplayRole)
		return QVariant();

	QstTreeItem *item = static_cast<QstTreeItem*>(index.internalPointer());

	return item->data(index.column());
}

/*! Возвращает заголовок элемента. */
QVariant QstTreeQueryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (!_root) return QVariant();

	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return _root->data(section);

	return QVariant();
}

/*! Возвращает модельный индекс по заданным значениям row, column и parent. */
QModelIndex QstTreeQueryModel::index(int row, int column, const QModelIndex& parent) const
{
		Q_ASSERT(_root != NULL);

	if (!hasIndex(row, column, parent))
		return QModelIndex();

	QstTreeItem *parentItem;

	if (!parent.isValid())
		parentItem = _root;
	else
		parentItem = static_cast<QstTreeItem*>(parent.internalPointer());

	QstTreeItem *childItem = parentItem->child(row);
	if (childItem)
		return createIndex(row, column, childItem);
	else
		return QModelIndex();
}

/*! Возвращает родительский элемент. */
QModelIndex QstTreeQueryModel::parent(const QModelIndex& index) const
{
	Q_ASSERT(_root != NULL);

	if (!index.isValid())
		return QModelIndex();

	QstTreeItem *childItem = static_cast<QstTreeItem*>(index.internalPointer());
	QstTreeItem *parentItem = childItem->parent();

	if (parentItem == _root)
		return QModelIndex();

	return createIndex(parentItem->row(), 0, parentItem);
}

/*! Устанавливает заголовок для секции. */
bool QstTreeQueryModel::setHeaderData(int section, Qt::Orientation orientation,
								 const QVariant & value, int role)
{
	if ((section < 0)
		|| ((orientation == Qt::Horizontal) && (section >= columnCount()))
		|| ((orientation == Qt::Vertical) && (section >= rowCount())))
		{
		return false;
	}

	Q_ASSERT(_root != NULL);
	_root->setData(section, value);

	return true;
}

/*! Удалет дерево данных. */
void QstTreeQueryModel::clear()
{
	delete _root;
	_root = NULL;
}

/*! Всегда возвращает false, поскольку дерево строится сразу по всем данным.

	Функция добавлена для совместимости. */
bool QstTreeQueryModel::canFetchMore ( const QModelIndex & parent ) const
{
	return false;
}

/*! Ничего не делает.

	Функция добавлена для совместимости. */
void QstTreeQueryModel::fetchMore ( const QModelIndex & parent)
{
	// Do nothing
	return;
}

/*! Возвращает последнюю ошибку, возникшую при выполнении запроса. */
QSqlError QstTreeQueryModel::lastError() const
{
	return _lastError;
}

void QstTreeQueryModel::_createTree()
{
	Q_ASSERT(_root != NULL);

	while (_query.next())
	{
		QSqlRecord record = _query.record();
		int fieldsCount = record.count();

		QList<QVariant> newRow;

		for (int i = 0; i < fieldsCount; ++i)
			newRow << record.value(i);

		QstTreeItem * item = _root->parentOf(_keyField, record.value(_parentField));

		if (!item)
		{
			QstTreeItem* newQstTreeItem = new QstTreeItem(newRow, _root);
			_root->appendChild(newQstTreeItem);
		}
		else
		{
			QstTreeItem* newQstTreeItem = new QstTreeItem(newRow, item);
			item->appendChild(newQstTreeItem);
		}
	}
}

}
