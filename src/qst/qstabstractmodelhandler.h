/****************************************************************************
** QST 0.4.2a beta
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

#ifndef QSTABSTRACTMODELHANDLER_H
#define QSTABSTRACTMODELHANDLER_H

#include <QString>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlTableModel>
#include <QTableView>
#include <QTreeView>
#include <QListView>
#include <QComboBox>
#include <QLabel>
#include <QAbstractItemView>
#include <QMessageBox>

#include <QMap>
#include <QVariantMap>

#include "qstglobal.h"
#include "qstquerygenerator.h"
#include "qstmodeldescriptor.h"

#include "qstplainquerymodel.h"
#include "qsttreequerymodel.h"

namespace Qst
{

class QstAbstractModelHandler
{
protected:

	QstSpecial::QstModelDescriptor	_modelDescriptor;

	QstValueMap			_valuesMap;

	bool				_loaded;

public:

	QstAbstractModelHandler();
	virtual ~QstAbstractModelHandler() = 0;

	virtual void reload(const int &queryNumber,
							  QstPlainQueryModel * model = NULL,
							  const QSqlDatabase &db = QSqlDatabase::database());

	virtual void reload(const int &queryNumber,
							  QstTreeQueryModel * model = NULL,
							  const QSqlDatabase &db = QSqlDatabase::database());

	virtual void reload(const QSqlDatabase &db = QSqlDatabase::database());

	virtual bool isLoaded() const;

	virtual int setTableView(QTableView * tableView);
	virtual int setTreeView(QTreeView * treeView);
	virtual int setListView(QListView * listView);

	virtual int setComboBox(QComboBox * comboBox);

	virtual void clear(const bool &deleteModel = false);

// ------------------------------------------------------------------------ //

	virtual int serviceFieldIndex(const FieldRole &role) const;

	virtual QVariant keyValueOfRow(const QModelIndex &index) const;
	virtual QVariant keyValueOfRow(const int &row) const;

	virtual QVariant keyValueOfCurrent(QAbstractItemView * view) const;
	virtual QVariant keyValueOfCurrent(QComboBox * comboBox) const;

	virtual QVariant keyValueOfView(const int &viewIndex = 0) const;
	virtual QVariant keyValueOfComboBox(const int &comboBoxIndex = 0) const;

	virtual QVariantList selectedKeysOfView(const FieldRole &role = RolePrimaryKey,
											const int &viewIndex = 0) const;

	virtual void	 setValue(const QString &valueName,
								 const QstValue &value = QstValue());

	virtual QstValue value(const QString &valueName,
						   const QstValue &defaultValue = QstValue()) const;

	virtual QstValue value(const QString &valueName,
						   const NullType &nullType) const;

	void			setValuesMap(const QstValueMap &valuesMap);
	QstValueMap	valuesMap() const;

	virtual void clearValues();

// ------------------------------------------------------------------------ //

	virtual QVariant SelectToValue(const QString fieldName,
								   const int &queryNumber,
								   const int &row = 0,
								   const bool &extractItemName = true,
								   const QSqlDatabase &db = QSqlDatabase::database());

	virtual QVariantMap SelectToMap(const int &queryNumber,
									const QStringList &fieldNamesList,
									const int &row = 0,
									const bool &extractItemName = true,
									const QSqlDatabase &db = QSqlDatabase::database());

	// ------------------------------------------------------------------------ //

		virtual bool Insert(const int &queryNumber, const QSqlDatabase &db = QSqlDatabase::database());
		virtual bool Update(const int &queryNumber, const QSqlDatabase &db = QSqlDatabase::database());
		virtual bool Delete(const int &queryNumber, const QSqlDatabase &db = QSqlDatabase::database());
		virtual bool Exec(const int &queryNumber, const QSqlDatabase &db = QSqlDatabase::database());

		QString generateQuery(const QueryType &queryType,
							  const int &queryNumber);

// -------------------------------------------------------------------------- //

		virtual QAbstractItemModel *pModel();
		virtual QAbstractItemModel *pModel() const;
		virtual QstSpecial::QstModelDescriptor	modelDescriptor() const;
		virtual QstBatch			batch(const QueryType &queryType,
										  const int &queryNumber);
// -------------------------------------------------------------------------- //

	virtual QstBatch _selector(const int &queryNumber) const;
	virtual QstBatch _inserter(const int &queryNumber) const;
	virtual QstBatch _updater(const int &queryNumber) const;
	virtual QstBatch _deleter(const int &queryNumber) const;
	virtual QstBatch _executor(const int &queryNumber) const;

// -------------------------------------------------------------------------- //

	virtual void _loadModelDescriptor(QAbstractItemModel * model,
						   const ModelType &modelType,
						   const int &queryNumber,
						   const QSqlDatabase &db);

	virtual void _reload(QAbstractItemModel * model,
							   const ModelType &modelType,
							   const int &queryNumber,
							   const bool &reloadQueryNumber,
							   const QSqlDatabase &db);

	virtual void _loadModel(const QstSpecial::QstQueryDescriptor &descriptor,
							QstPlainQueryModel * model,
							const QSqlDatabase &db);

	virtual void _loadModel(const QstSpecial::QstQueryDescriptor &descriptor,
							QstTreeQueryModel * model,
							const QSqlDatabase &db);

	template<class T> void _setViewSettings(T * view, const QstSpecial::QstQueryDescriptor &descriptor);
	void				   _setListViewSettings(QListView * view, const QstSpecial::QstQueryDescriptor &descriptor);

	QVariant _selectToValue(const int &row,
							const int &column,
							const QstSpecial::QstQueryDescriptor &descriptor,
							const QSqlDatabase &db);

	QVariantMap _selectToMap(const QStringList &fieldNamesList,
							 const QstSpecial::QstQueryDescriptor &descriptor,
							 const bool &extractItemName,
							 const int &row,
							 const QSqlDatabase &db);
};

}

#endif // QSTABSTRACTMODELHANDLER_H
