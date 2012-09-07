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

#include "qsttreeitem.h"

namespace Qst
{

QstTreeItem::QstTreeItem()
:
_parent(NULL)
{
}

QstTreeItem::QstTreeItem(const QList<QVariant>& data, QstTreeItem* parent)
{
	_itemData = data;
	_parent = parent;
}

QstTreeItem::~QstTreeItem()
{
	clear();
}



void QstTreeItem::appendChild(QstTreeItem * child)
{
	Q_ASSERT(child != NULL);

	_childItems.append(child);
}

QstTreeItem* QstTreeItem::child(const int &index)
{
	return _childItems.value(index, NULL);
}

int QstTreeItem::childCount() const
{
	return _childItems.count();
}

int QstTreeItem::colsCount() const
{
	return _itemData.count();
}

QVariant QstTreeItem::data (const int &columnIndex) const
{
	// returns invalid QVariant if index out of bounds.
	return _itemData.value(columnIndex);
}

QstTreeItem* QstTreeItem::parent()
{
	return _parent;
}

int QstTreeItem::row() const
{
	if (_parent)
		return _parent->_childItems.indexOf(const_cast<QstTreeItem*>(this));

	return 0;
}

QstTreeItem* QstTreeItem::parentOf(const int &columnToCompare,
							 const QVariant &parentID)
{
	if (!parentID.isValid())
		return NULL;

	return _parentOf(this, columnToCompare, parentID);
}

QstTreeItem* QstTreeItem::_parentOf(QstTreeItem* node,
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
		QstTreeItem * item = _parentOf(node->_childItems[i], columnToCompare, parentID);
		if (item)		return item;
	}

return NULL;
}


void  QstTreeItem::clear()
{
	qDeleteAll(_childItems);
	_childItems.clear();
	_itemData.clear();
	_parent = NULL;
}


void QstTreeItem::setData(const int &columnIndex, const QVariant &data)
{
Q_ASSERT(columnIndex >=0 && columnIndex < _itemData.size());

_itemData[columnIndex] = data;
}

bool QstTreeItem::hasData(const int &childIndex, const int &columnIndex)
{
if (childIndex >= 0 && childIndex < _childItems.size())
	if (_childItems[childIndex]->data(columnIndex).isValid())
		return true;
return false;
}

}
