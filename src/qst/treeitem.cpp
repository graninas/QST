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

#include "treeitem.h"

#include <QDebug>


TreeItem::TreeItem()
	:
	_parent(NULL)
{
}

TreeItem::TreeItem(const QList<QVariant>& data, TreeItem* parent)
{
	_itemData = data;
	_parent = parent;
}

TreeItem::~TreeItem()
{
	clear();
}



void TreeItem::appendChild(TreeItem * child)
{
	Q_ASSERT(child != NULL);

	_childItems.append(child);
}

TreeItem* TreeItem::child(const int &index)
{
	return _childItems.value(index, NULL);
}

int TreeItem::childCount() const
{
	return _childItems.count();
}

int TreeItem::colsCount() const
{
	return _itemData.count();
}

QVariant TreeItem::data (const int &columnIndex) const
{
	// returns invalid QVariant if index out of bounds.
	return _itemData.value(columnIndex);
}

TreeItem* TreeItem::parent()
{
	return _parent;
}

int TreeItem::row() const
{
	if (_parent)
		return _parent->_childItems.indexOf(const_cast<TreeItem*>(this));

	return 0;
}

TreeItem* TreeItem::parentOf(const int &columnToCompare,
							 const QVariant &parentID)
{
	if (!parentID.isValid())
		return NULL;

	return _parentOf(this, columnToCompare, parentID);
}

TreeItem* TreeItem::_parentOf(TreeItem* node,
							  const int &columnToCompare,
							  const QVariant &parentID)
{
	Q_ASSERT(parentID.isValid());

	if (!node)	return NULL;

	if (columnToCompare >= 0 && columnToCompare < node->_itemData.size())
	{
		if (node->_itemData[columnToCompare].value<QString>() == parentID.value<QString>())
			return node;
	}

	for (int i=0; i < node->_childItems.size(); ++i)
	{
		TreeItem * item = _parentOf(node->_childItems[i], columnToCompare, parentID);
		if (item)		return item;
	}

	return NULL;
}


void  TreeItem::clear()
{
	qDeleteAll(_childItems);
	_childItems.clear();
	_itemData.clear();
	_parent = NULL;
}


void TreeItem::setData(const int &columnIndex, const QVariant &data)
{
	Q_ASSERT(columnIndex >=0 && columnIndex < _itemData.size());

	_itemData[columnIndex] = data;
}

bool TreeItem::hasData(const int &childIndex, const int &columnIndex)
{
	if (childIndex >= 0 && childIndex < _childItems.size())
		if (_childItems[childIndex]->data(columnIndex).isValid())
			return true;
	return false;
}
