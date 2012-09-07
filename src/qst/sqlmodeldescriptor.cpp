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

#include "sqlmodeldescriptor.h"

namespace Sql
{
	/*!
		\class SqlModelDescriptor
		\brief
		Класс хранит связки "Модель - Представление" конкретного источника данных.
		Используется для внутренних нужд AbstractModelHandler.

		\inmodule Sql */


	SqlModelDescriptor::SqlModelDescriptor(QAbstractItemModel* model,
										   const SqlQueryModelTypes & modelType,
										   const SqlQueryDescriptor & descriptor)
				 :
				 _model(model),
				 _modelType(modelType),
				 _queryDescriptor(descriptor)
	{
		Q_ASSERT(model != NULL);
	}

	SqlModelDescriptor::SqlModelDescriptor()
	 :
	 _model(NULL),
	 _modelType(mt_plain),
	 _queryDescriptor(SqlQueryDescriptor())
	{
	}

	QAbstractItemModel *SqlModelDescriptor::pModel()
	{
		return _model;
	}

	QAbstractItemModel *SqlModelDescriptor::cpModel() const
	{
		return _model;
	}

	void SqlModelDescriptor::setModel(QAbstractItemModel* model)
	{
		Q_ASSERT(model != NULL);
		_model = model;
	}

	int SqlModelDescriptor::addView(QAbstractItemView *view, const bool &modelize)
	{
		if (!_views.contains(view))
			_views.append(view);

		if (modelize)
		{
			Q_ASSERT(_model != NULL);
			view->setModel(_model);
		}
		return _views.indexOf(view);
	}

	int SqlModelDescriptor::addComboBox(QComboBox *comboBox, const bool &modelize)
	{
		if (!_comboBoxes.contains(comboBox))
			_comboBoxes.append(comboBox);

		if (modelize)
		{
			Q_ASSERT(_model != NULL);
			comboBox->setModel(_model);
		}
		return _comboBoxes.indexOf(comboBox);
	}

	QAbstractItemView * SqlModelDescriptor::view(const int &index)
	{
		return _views.value(index, NULL);
	}

	QAbstractItemView * SqlModelDescriptor::view(const int &index) const
	{
		return _views.value(index, NULL);
	}

	QComboBox * SqlModelDescriptor::comboBox(const int &index)
	{
		return _comboBoxes.value(index, NULL);
	}

	QComboBox * SqlModelDescriptor::comboBox(const int &index) const
	{
		return _comboBoxes.value(index, NULL);
	}

	QListView  * SqlModelDescriptor::listView(const int &index)
	{
		return _listViews.value(index, NULL);
	}

	QListView  * SqlModelDescriptor::listView(const int &index) const
	{
		return _listViews.value(index, NULL);
	}

	SqlQueryModelTypes SqlModelDescriptor::modelType() const
	{
		return _modelType;
	}

	void SqlModelDescriptor::setModelType(const SqlQueryModelTypes & modelType)
	{
		_modelType = modelType;
	}

	void SqlModelDescriptor::clearViews()
	{
		demodelizeViews();
		_views.clear();
		_comboBoxes.clear();
		_listViews.clear();

		_viewsCurrentIndexes.clear();
		_comboBoxesCurrentIndexes.clear();
	}

	void SqlModelDescriptor::modelizeViews()
	{
		Q_ASSERT(_model != NULL);
		Q_ASSERT(_viewsCurrentIndexes.size() == _views.size());
		Q_ASSERT(_comboBoxesCurrentIndexes.size() == _comboBoxes.size());

		for (int i = 0; i < _views.size(); ++i)
		{
			_views[i]->setModel(_model);
			_views[i]->setCurrentIndex(_model->index(_viewsCurrentIndexes[i], 0, QModelIndex()));
		}

		for (int i = 0; i < _comboBoxes.size(); ++i)
		{
			_comboBoxes[i]->setModel(_model);
			_comboBoxes[i]->setCurrentIndex(_comboBoxesCurrentIndexes[i]);
		}

		for (int i = 0; i < _listViews.size(); ++i)
		{
			_listViews[i]->setModel(_model);
		}
	}

	void SqlModelDescriptor::demodelizeViews()
	{
		_viewsCurrentIndexes.resize(_views.size());
		for (int i = 0; i < _views.size(); ++i)
		{
			_viewsCurrentIndexes[i] = _views[i]->currentIndex().row();
			_views[i]->setModel(NULL);
		}

		_comboBoxesCurrentIndexes.resize(_comboBoxes.size());
		for (int i = 0; i < _comboBoxes.size(); ++i)
		{
			_comboBoxesCurrentIndexes[i] = _comboBoxes[i]->currentIndex();
			_comboBoxes[i]->setModel(_model);
		}

		for (int i = 0; i < _listViews.size(); ++i)
		{
			_listViews[i]->setModel(_model);
		}
	}

	SqlQueryDescriptor SqlModelDescriptor::queryDescriptor() const
	{
		return _queryDescriptor;
	}

	void SqlModelDescriptor::setQueryDescriptor(const SqlQueryDescriptor &descriptor)
	{
		_queryDescriptor = descriptor;
	}



}
