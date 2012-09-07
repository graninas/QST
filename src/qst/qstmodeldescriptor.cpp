/****************************************************************************
** QST 0.4.1 pre-alpha
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

#include "qstmodeldescriptor.h"

namespace Qst
{
	QstModelDescriptor::QstModelDescriptor()
	 :
	 _model(NULL),
	 _modelType(ModelPlain),
	 _queryDescriptor(QstQueryDescriptor())
	{
	}

	QstModelDescriptor::QstModelDescriptor(QAbstractItemModel* model,
										   const ModelType & modelType,
										   const QstQueryDescriptor & descriptor)
				 :
				 _model(model),
				 _modelType(modelType),
				 _queryDescriptor(descriptor)
	{
		Q_ASSERT(model != NULL);
	}

	QAbstractItemModel *QstModelDescriptor::pModel()
	{
		return _model;
	}

	QAbstractItemModel *QstModelDescriptor::pModel() const
	{
		return _model;
	}

	void QstModelDescriptor::setModel(QAbstractItemModel* model)
	{
		Q_ASSERT(model != NULL);
		_model = model;
	}

	int QstModelDescriptor::addView(QAbstractItemView *view, const bool &modelize)
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

	int QstModelDescriptor::addComboBox(QComboBox *comboBox, const bool &modelize)
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

	QAbstractItemView * QstModelDescriptor::view(const int &index)
	{
		return _views.value(index, NULL);
	}

	QAbstractItemView * QstModelDescriptor::view(const int &index) const
	{
		return _views.value(index, NULL);
	}

	QComboBox * QstModelDescriptor::comboBox(const int &index)
	{
		return _comboBoxes.value(index, NULL);
	}

	QComboBox * QstModelDescriptor::comboBox(const int &index) const
	{
		return _comboBoxes.value(index, NULL);
	}

	QListView  * QstModelDescriptor::listView(const int &index)
	{
		return _listViews.value(index, NULL);
	}

	QListView  * QstModelDescriptor::listView(const int &index) const
	{
		return _listViews.value(index, NULL);
	}

	ModelType QstModelDescriptor::modelType() const
	{
		return _modelType;
	}

	void QstModelDescriptor::setModelType(const ModelType & modelType)
	{
		_modelType = modelType;
	}

	void QstModelDescriptor::clearViews()
	{
		demodelizeViews();
		_views.clear();
		_comboBoxes.clear();
		_listViews.clear();

		_viewsCurrentIndexes.clear();
		_comboBoxesCurrentIndexes.clear();
	}

	void QstModelDescriptor::modelizeViews()
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

	void QstModelDescriptor::demodelizeViews()
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

	QstQueryDescriptor QstModelDescriptor::queryDescriptor() const
	{
		return _queryDescriptor;
	}

	void QstModelDescriptor::setQueryDescriptor(const QstQueryDescriptor &descriptor)
	{
		_queryDescriptor = descriptor;
	}


	bool QstModelDescriptor::isValid() const
	{
		return _model != NULL;
	}

}
