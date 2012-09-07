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

	typedef QMap<QString, QstValue>			  _QstValuesMapType;

	QstModelDescriptor	_modelDescriptor;
	_QstValuesMapType	_values;

	int		_rowCount;

public:

	QstAbstractModelHandler();
	virtual ~QstAbstractModelHandler() = 0;

// Для загрузки и обновления моделей данных лучше использовать эти функции:

// Перезагружает источник данных для плоской модели.
// Если такового нет, создает новый.
	virtual void reload(QstPlainQueryModel * model = NULL,
							  const QSqlDatabase &db = QSqlDatabase::database());

// Перезагружает источник данных для иерархической модели.
// Если такового нет, создает новый.
	virtual void reload(QstTreeQueryModel * model = NULL,
							  const QSqlDatabase &db = QSqlDatabase::database());

// Перезагружает источник данных для плоской модели. Сохраняет номер запроса.
// Если источника нет, создает новый.
	virtual void reload(const int &queryNumber,
							  QstPlainQueryModel * model = NULL,
							  const QSqlDatabase &db = QSqlDatabase::database());

// Перезагружает источник данных для иерархической модели. Сохраняет номер запроса.
// Если источника нет, создает новый.
	virtual void reload(const int &queryNumber,
							  QstTreeQueryModel * model = NULL,
							  const QSqlDatabase &db = QSqlDatabase::database());


// Установить для модели представление (view, comboBox).
// У одной модели может быть несколько представлений.
// Возвращается индекс, выданный этому view / comboBox.
	virtual int setTableView(QTableView * tableView);
	virtual int setTreeView(QTreeView * treeView);
	virtual int setListView(QListView * listView);

	virtual int setComboBox(QComboBox * comboBox);

// Специальная функция для получения количества строк.
// Наиболее актуальна для драйверов ODBC.

	virtual int rowCount();

// Удаление источника. Можно сразу уничтожить модель.
	virtual void clear(const bool &deleteModel = false);

// ------------------------------------------------------------------------ //

// Возвращают индекс колонки для служебных полей.
// -1, если поле не найдено.
	virtual int serviceFieldIndex(const FieldRole &role) const;

// Возвращают значение ключевого поля для строки, на которую указывает index.
	virtual QVariant keyValueOfRow(const QModelIndex &index) const;
	virtual QVariant keyValueOfRow(const int &row) const;

// Возвращают значение ключевого поля для текущей строки во view.
	virtual QVariant keyValueOfCurrent(QAbstractItemView * view) const;
	virtual QVariant keyValueOfCurrent(QComboBox * comboBox) const;

// Возвращает значение ключевого поля для текущей строки в сохраненном view / combobox.
	virtual QVariant keyValueOfView(const int &viewIndex = 0) const;
	virtual QVariant keyValueOfComboBox(const int &comboBoxIndex = 0) const;

	virtual QVariantList selectedKeysOfView(const FieldRole &role = RolePrimaryKey,
											const int &viewIndex = 0) const;


// Сохраняет значение с заданным именем в список значений.
// Используется для передачи в класс-хэндлер данных.
	virtual void	 setValue(const QString &valueName,
								 const QstValue &value = QstValue());

// Возвращает значение из списка значений по имени.
// Если константа INVALID_SQLVALUE_BY_DEFAULT определена, то при
// отсутствии значения возвращает инвалидный QstValue().
// Если константа не определена, возвращает нулевой QstValue()
	virtual QstValue value(const QString &valueName,
						   const QstValue &defaultValue = QstValue()) const;

// Та же самая функция, но позволяет заменить длинное
// value("val", QstValue(nt_null))
// на
// value("val", nt_null).
	virtual QstValue value(const QString &valueName,
						   const NullType &nullType) const;

// Очищает список значений.
	virtual void clearValues();

// ------------------------------------------------------------------------ //

	// Особая функция. Генерирует SELECT-запрос, выполняет его и возвращает
	// значение, найденное в ячейке (row, columnOf(fieldName)).
	// Если такового нет, возвращает invalid QVariant.
	virtual QVariant SelectToValue(const QString fieldName,
								   const int &queryNumber = 0,
								   const bool &extractItemName = true,
								   const int &row = 0,
								   const QSqlDatabase &db = QSqlDatabase::database());

	// Выбирает множество данных в QVariantMap, производя всего один запрос к БД.
	virtual QVariantMap SelectToMap(const QStringList &fieldNamesList,
									const int &queryNumber = 0,
									const bool &extractItemName = true,
									const int &row = 0,
									const QSqlDatabase &db = QSqlDatabase::database());

	// ------------------------------------------------------------------------ //

	// Функции, при вызове которых генерируются и выполняются соответствующие запросы.
		virtual bool Insert(const int &queryNumber = 0, const QSqlDatabase &db = QSqlDatabase::database());
		virtual bool Update(const int &queryNumber = 0, const QSqlDatabase &db = QSqlDatabase::database());
		virtual bool Delete(const int &queryNumber = 0, const QSqlDatabase &db = QSqlDatabase::database());

	// Генерируется и выполняется процедура.
		virtual bool Exec(const int &queryNumber = 0, const QSqlDatabase &db = QSqlDatabase::database());

	// Удаляется текущий элемент из view или ComboBox (если есть).
		virtual bool DeleteCurrent(QAbstractItemView * view,
								   const QSqlDatabase &db = QSqlDatabase::database());

		virtual bool DeleteCurrent(QComboBox * comboBox,
								   const QSqlDatabase &db = QSqlDatabase::database());


// -------------------------------------------------------------------------- //
		// Функции возвращают запрашиваемый SQL-запрос.

		QString SelectQuery(const int &queryNumber = 0,
							const ModelType &type = ModelPlain) const;
		QString InsertQuery(const int &queryNumber = 0) const;
		QString UpdateQuery(const int &queryNumber = 0) const;
		QString DeleteQuery(const int &queryNumber = 0) const;
		QString ExecQuery  (const int &queryNumber = 0) const;

// -------------------------------------------------------------------------- //

		virtual QAbstractItemModel *pModel();
		virtual QAbstractItemModel *pModel() const;
		virtual QstModelDescriptor modelDescriptor() const;

// Функции, которые необходимо переопределить в наследуемых классах
// для получения нужного поведения.

	virtual QstBatch _selector(const ModelType &modelType = ModelPlain,
										 const int &queryNumber = 0) const;
	virtual QstBatch _inserter(const int &queryNumber = 0) const;
	virtual QstBatch _updater(const int &queryNumber = 0) const;
	virtual QstBatch _deleter(const int &queryNumber = 0) const;

	virtual QstBatch _executor(const int &queryNumber = 0) const;

// -------------------------------------------------------------------------- //

	virtual void _loadModelDescriptor(QAbstractItemModel * model,
						   const ModelType &type,
						   const int &queryNumber,
						   const bool &useQueryNumber,
						   const QSqlDatabase &db);

	virtual void _reload(QAbstractItemModel * model,
							   const ModelType &type,
							   const int &queryNumber,
							   const bool &reloadQueryNumber,
							   const QSqlDatabase &db);

	virtual void _loadModel(const QstQueryDescriptor &descriptor,
							QstPlainQueryModel * model,
							const QSqlDatabase &db);

	virtual void _loadModel(const QstQueryDescriptor &descriptor,
							QstTreeQueryModel * model,
							const QSqlDatabase &db);

	template<class T> void _setViewSettings(T * view, const QstQueryDescriptor &descriptor);
	void				   _setListViewSettings(QListView * view, const QstQueryDescriptor &descriptor);

	QVariant _selectToValue(const int &row,
							const int &column,
							const QstQueryDescriptor &descriptor,
							const QSqlDatabase &db);

	QVariantMap _selectToMap(const QStringList &fieldNamesList,
							 const QstQueryDescriptor &descriptor,
							 const bool &extractItemName,
							 const int &row,
							 const QSqlDatabase &db);
};

}

#endif // QSTABSTRACTMODELHANDLER_H
