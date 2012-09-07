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

#ifndef SQLTREEMODEL_H
#define SQLTREEMODEL_H

#include <QAbstractItemModel>
#include <QSqlQuery>

#include "treeitem.h"

		class SqlTreeModel : public QAbstractItemModel
		{
			Q_OBJECT

  public:
			SqlTreeModel(QObject* parent = NULL);
			virtual ~SqlTreeModel();

			virtual void setKeyField(const int &keyField);
			virtual void setParentField(const int &parentField);

			virtual void setQuery(const QSqlQuery & query);
			virtual void setQuery(const QString & query, const QSqlDatabase &db = QSqlDatabase::database());

			virtual int rowCount(const QModelIndex& parent = QModelIndex()) const;
			virtual int columnCount(const QModelIndex& parent = QModelIndex()) const;

			virtual Qt::ItemFlags flags(const QModelIndex &index) const;
			virtual QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
			virtual QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
			virtual QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const;
			virtual QModelIndex parent(const QModelIndex& index = QModelIndex()) const;

			virtual bool setHeaderData(int section, Qt::Orientation orientation,
									   const QVariant & value, int role = Qt::EditRole);

			virtual void clear();


			virtual bool canFetchMore ( const QModelIndex & parent = QModelIndex() ) const;
			virtual void fetchMore ( const QModelIndex & parent = QModelIndex() );

  private:
			TreeItem *_root;
			QSqlQuery _query;

			int _keyField;
			int _parentField;

			void _createTree();
		};



#endif // SQLTREEMODEL_H
