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

#ifndef TREEITEM_H
#define TREEITEM_H


#include <QList>
#include <QVariant>


class TreeItem
{
public:
	TreeItem();
	TreeItem(const QList<QVariant>& data, TreeItem* parent = 0);
	~TreeItem();


	void appendChild(TreeItem * child);
	TreeItem* child(const int &index);
	int childCount() const;
	int colsCount() const;
	QVariant data (const int &columnIndex) const;
	TreeItem* parent();
	int row() const;

	TreeItem* parentOf(const int &columnToCompare, const QVariant &parentID);

	void clear();

	void setData(const int &columnIndex, const QVariant &data);

	bool hasData(const int &childIndex, const int &columnIndex);


private:
	QList<TreeItem*> _childItems;
	QList<QVariant> _itemData;
	TreeItem* _parent;


	TreeItem* _parentOf(TreeItem* node, const int &columnIndex, const QVariant &parentID);
};

#endif // TREEITEM_H
