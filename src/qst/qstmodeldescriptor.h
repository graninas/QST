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

#ifndef QSTMODELDESCRIPTOR_H
#define QSTMODELDESCRIPTOR_H

#include "qstquerydescriptor.h"
#include <QAbstractItemModel>
#include <QAbstractItemView>
#include <QComboBox>
#include <QListView>
#include <QList>
#include <QVector>

#include "qstglobal.h"

namespace Qst
{

namespace QstSpecial
{

class QstModelDescriptor
{
public:

	typedef QList<QAbstractItemView *>	ItemViewListType;
	typedef QList<QComboBox *>			ComboBoxListType;
	typedef QList<QListView *>			ListViewListType;

	typedef QVector<int>				CurrentIndexVectorType;

private:

	QAbstractItemModel*	_model;
	ModelType  _modelType;
	QstQueryDescriptor	_queryDescriptor;
	ItemViewListType	_views;
	ComboBoxListType	_comboBoxes;
	ListViewListType	_listViews;

	CurrentIndexVectorType	_viewsCurrentIndexes;
	CurrentIndexVectorType	_comboBoxesCurrentIndexes;

public:
	QstModelDescriptor();

	QstModelDescriptor(QAbstractItemModel* model,
					   const ModelType & modelType,
					   const QstQueryDescriptor & descriptor);

	QAbstractItemModel *pModel();
	QAbstractItemModel *pModel() const;

	void setModel(QAbstractItemModel* model);
	int addView(QAbstractItemView *view, const bool &modelize = false);
	int addComboBox(QComboBox *comboBox, const bool &modelize = false);

	QAbstractItemView * view(const int &index = 0);
	QAbstractItemView * view(const int &index = 0) const;
	QComboBox		  * comboBox(const int &index = 0);
	QComboBox		  * comboBox(const int &index = 0) const;

	ModelType modelType() const;
	void setModelType(const ModelType & modelType);

	void clearViews();
	void modelizeViews();
	void demodelizeViews();

	QstQueryDescriptor	queryDescriptor() const;
	void				setQueryDescriptor(const QstQueryDescriptor &descriptor);

	bool isValid() const;
};



}	// End of namespace QstSpecial

}	// End of namespace Qst

#endif // QSTMODELDESCRIPTOR_H
