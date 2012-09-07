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

#ifndef SQLMODELDESCRIPTOR_H
#define SQLMODELDESCRIPTOR_H

#include "sqlquerydescriptor.h"
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QComboBox>
#include <QListView>
#include <QList>
#include <QVector>

#include "sqlglobal.h"

namespace Sql
{

class SqlModelDescriptor
{
public:

	typedef QList<QAbstractItemView *>	ItemViewListType;
	typedef QList<QComboBox *>			ComboBoxListType;
	typedef QList<QListView *>			ListViewListType;

	typedef QVector<int>				CurrentIndexVectorType;

private:

	QAbstractItemModel*	_model;
	SqlQueryModelTypes  _modelType;
	SqlQueryDescriptor	_queryDescriptor;
	ItemViewListType	_views;
	ComboBoxListType	_comboBoxes;
	ListViewListType	_listViews;

	CurrentIndexVectorType	_viewsCurrentIndexes;
	CurrentIndexVectorType	_comboBoxesCurrentIndexes;

public:
	SqlModelDescriptor(QAbstractItemModel* model,
					   const SqlQueryModelTypes & modelType,
					   const SqlQueryDescriptor & descriptor);
	SqlModelDescriptor();

	QAbstractItemModel *pModel();
	QAbstractItemModel *cpModel() const;

	void setModel(QAbstractItemModel* model);
	int addView(QAbstractItemView *view, const bool &modelize = false);
	int addComboBox(QComboBox *comboBox, const bool &modelize = false);

	QAbstractItemView * view(const int &index = 0);
	QAbstractItemView * view(const int &index = 0) const;
	QComboBox		  * comboBox(const int &index = 0);
	QComboBox		  * comboBox(const int &index = 0) const;
	QListView		  * listView(const int &index = 0);
	QListView		  * listView(const int &index = 0) const;

	SqlQueryModelTypes modelType() const;
	void setModelType(const SqlQueryModelTypes & modelType);

	void clearViews();
	void modelizeViews();
	void demodelizeViews();

	SqlQueryDescriptor	queryDescriptor() const;
	void				setQueryDescriptor(const SqlQueryDescriptor &descriptor);
};



};
#endif // SQLMODELDESCRIPTOR_H
