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

#ifndef QSTTREEITEM_H
#define QSTTREEITEM_H

#include <QList>
#include <QVariant>

namespace Qst
{

class QstTreeItem
{
public:
	QstTreeItem();
	QstTreeItem(const QList<QVariant>& data, QstTreeItem* parent = 0);
	~QstTreeItem();


	void appendChild(QstTreeItem * child);
	QstTreeItem* child(const int &index);
	int childCount() const;
	int colsCount() const;
	QVariant data (const int &columnIndex) const;
	QstTreeItem* parent();
	int row() const;

	QstTreeItem* parentOf(const int &columnToCompare, const QVariant &parentID);

	void clear();

	void setData(const int &columnIndex, const QVariant &data);

	bool hasData(const int &childIndex, const int &columnIndex);


private:
	QList<QstTreeItem*> _childItems;
	QList<QVariant> _itemData;
	QstTreeItem* _parent;


	QstTreeItem* _parentOf(QstTreeItem* node, const int &columnIndex, const QVariant &parentID);
};

}

#endif // QSTTREEITEM_H
