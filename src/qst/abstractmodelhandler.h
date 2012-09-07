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

#ifndef ABSTRACTMODELHANDLER_H
#define ABSTRACTMODELHANDLER_H

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

#include "sqlglobal.h"
#include "sqlgen.h"
#include "sqlmodeldescriptor.h"

#include "sqltreemodel.h"
#include "sqlquerymodel.h"

#define INVALID_SQLVALUE_BY_DEFAULT

namespace Sql
{

class AbstractModelHandler
{
protected:

	typedef QMap<QString, SqlModelDescriptor> _SourcesMapType;
	typedef QMap<QString, SqlValue>			  _SqlValuesMapType;
	SqlValuesMap		_valuesMap;

	_SourcesMapType		_sources;
	_SqlValuesMapType	_values;

	int		_rowCount;

public:

	AbstractModelHandler();
	virtual ~AbstractModelHandler() = 0;

// Перезагружает источник данных для плоской модели.
// Если такового нет, создает новый.
	virtual void reloadSource(const QString &sourceName,
							  SqlQueryModel * model = NULL,
							  const QSqlDatabase &db = QSqlDatabase::database());

// Перезагружает источник данных для иерархической модели.
// Если такового нет, создает новый.
	virtual void reloadSource(const QString &sourceName,
							  SqlTreeModel * model = NULL,
							  const QSqlDatabase &db = QSqlDatabase::database());

// Перезагружает источник данных для плоской модели. Сохраняет номер запроса.
// Если источника нет, создает новый.
	virtual void reloadSource(const QString &sourceName,
							  const int &queryNumber,
							  SqlQueryModel * model = NULL,
							  const QSqlDatabase &db = QSqlDatabase::database());

// Перезагружает источник данных для иерархической модели. Сохраняет номер запроса.
// Если источника нет, создает новый.
	virtual void reloadSource(const QString &sourceName,
							  const int &queryNumber,
							  SqlTreeModel * model = NULL,
							  const QSqlDatabase &db = QSqlDatabase::database());


// Установить для модели представление (view, comboBox).
// У одной модели может быть несколько представлений.
// Возвращается индекс, выданный этому view / comboBox.
	virtual int setTableView(const QString &sourceName, QTableView * tableView);
	virtual int setTreeView(const QString &sourceName, QTreeView * treeView);
	virtual int setListView(const QString &sourceName, QListView * listView);

	virtual int setComboBox(const QString &sourceName, QComboBox * comboBox);

// Удаление источника. Можно сразу уничтожить модель.
	virtual void deleteSource(const QString &sourceName,
							  const bool &deleteModel = false);

// ------------------------------------------------------------------------ //

// Возвращают индекс колонки для служебных полей.
// -1, если поле не найдено.
	virtual int serviceFieldIndex(const QString &sourceName,
								  const SqlFieldRoles &role) const;

// Возвращают значение ключевого поля для строки, на которую указывает index.
	virtual QVariant keyValueOfRow(const QString &sourceName,
								   const QModelIndex &index) const;
	virtual QVariant keyValueOfRow(const QString &sourceName,
								   const int &row) const;

// Возвращают значение ключевого поля для текущей строки во view.
	virtual QVariant keyValueOfCurrent(const QString &sourceName,
									   QAbstractItemView * view) const;
	virtual QVariant keyValueOfCurrent(const QString &sourceName,
									   QComboBox * comboBox) const;

// Возвращает значение ключевого поля для текущей строки в сохраненном view / combobox.
	virtual QVariant keyValueOfView(const QString &sourceName,
									const int &viewIndex = 0) const;
	virtual QVariant keyValueOfComboBox(const QString &sourceName,
										const int &comboBoxIndex = 0) const;

// Возвращает список ключей выделенных во view записей.
	virtual QVariantList selectedKeysOfView(const QString &sourceName,
											const int &viewIndex = 0) const;


// Сохраняет значение с заданным именем в список значений.
// Используется для передачи в класс-хэндлер данных.
	virtual void	 setValue(const QString &valueName,
								 const SqlValue &value = SqlValue());

// Делает то же самое, но берет текущее значение из view или ComboBox.
	virtual void	 setValueFromView(const QString &sourceName,
									  const QString &valueName,
									  const int &viewIndex = 0);
	virtual void	 setValueFromComboBox(const QString &sourceName,
										  const QString &valueName,
										  const int &comboBoxIndex = 0);

// Возвращает значение из списка значений по имени.
// Если константа INVALID_SQLVALUE_BY_DEFAULT определена, то при
// отсутствии значения возвращает инвалидный SqlValue().
// Если константа не определена, возвращает нулевой SqlValue()
#ifdef INVALID_SQLVALUE_BY_DEFAULT
	virtual SqlValue value(const QString &valueName,
						   const SqlValue &defaultValue = SqlValue()) const;
#else
	virtual SqlValue value(const QString &valueName,
						   const SqlValue &defaultValue = SqlValue(nt_null)) const;
#endif

// Та же самая функция, но позволяет заменить длинное
// value("val", SqlValue(nt_null))
// на
// value("val", nt_null).
	virtual SqlValue value(const QString &valueName,
						   const NullType &nullType) const;

	virtual void setValuesMap(const SqlValuesMap &valMap);

	virtual SqlValuesMap valuesMap() const;

// Очищает список значений.
	virtual void clearValues();

// ------------------------------------------------------------------------ //

	// Особая функция. Генерирует SELECT-запрос, выполняет его и возвращает
	// значение, найденное в ячейке (row, columnOf(fieldName)).
	// Если такового нет, возвращает invalid QVariant.
	virtual QVariant SelectToValue(const QString fieldName,
								   const int &queryNumber = 0,
								   const int &row = 0,
								   const QSqlDatabase &db = QSqlDatabase::database());

	// Делает то же самое, по имени поля, и данные об SqlField берет из источника.
	virtual QVariant SelectToValue(const QString sourceName,
								   const QString fieldName,
								   const int &row = 0,
								   const QSqlDatabase &db = QSqlDatabase::database());

	// Выбирает множество данных в QVariantMap, производя всего один запрос к БД.
	virtual QVariantMap SelectToMap(const QStringList &fieldNamesList,
									const int &queryNumber = 0,
									const int &row = 0,
									const QSqlDatabase &db = QSqlDatabase::database());

	// Выбирает множество данных в QVariantMap, производя всего один запрос к БД.
	virtual QVariantMap SelectToMap(const QString sourceName,
									const QStringList &fieldNamesList,
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
		virtual bool DeleteCurrent(const QString &sourceName,
								   QAbstractItemView * view,
								   const QSqlDatabase &db = QSqlDatabase::database());

		virtual bool DeleteCurrent(const QString &sourceName,
								   QComboBox * comboBox,
								   const QSqlDatabase &db = QSqlDatabase::database());


// -------------------------------------------------------------------------- //
		// Функции возвращают SQL-запрос.

		QString SelectQuery(const int &queryNumber = 0,
							const SqlQueryModelTypes &type = mt_plain) const;
		QString InsertQuery(const int &queryNumber = 0) const;
		QString UpdateQuery(const int &queryNumber = 0) const;
		QString DeleteQuery(const int &queryNumber = 0) const;
		QString ExecQuery  (const int &queryNumber = 0) const;

// -------------------------------------------------------------------------- //

		virtual QAbstractItemModel *pModel(const QString &sourceName);
		virtual QAbstractItemModel *cpModel(const QString &sourceName) const;
		virtual SqlModelDescriptor modelDescriptor(const QString &sourceName) const;
		virtual SqlQueryDescriptor queryDescriptor(const QString &sourceName) const;
		virtual SqlModelDescriptor &rModelDescriptor(const QString &sourceName);

// Функции, которые необходимо переопределить в наследуемых классах
// для получения нужного поведения.

	virtual SqlQueryDescriptor _selector(const SqlQueryModelTypes &modelType = mt_plain,
										 const int &queryNumber = 0) const;
	virtual SqlQueryDescriptor _inserter(const int &queryNumber = 0) const;
	virtual SqlQueryDescriptor _updater(const int &queryNumber = 0) const;
	virtual SqlQueryDescriptor _deleter(const int &queryNumber = 0) const;

	virtual SqlQueryDescriptor _executor(const int &queryNumber = 0) const;

// -------------------------------------------------------------------------- //

	virtual void _addSource(const QString &sourceName,
						   QAbstractItemModel * model,
						   const SqlQueryModelTypes &type,
						   const int &queryNumber,
						   const bool &useQueryNumber,
						   const QSqlDatabase &db);

	virtual void _reloadSource(const QString &sourceName,
							   QAbstractItemModel * model,
							   const SqlQueryModelTypes &type,
							   const int &queryNumber,
							   const bool &reloadQueryNumber,
							   const QSqlDatabase &db);

	virtual void _loadModel(const SqlQueryDescriptor &descriptor,
							SqlQueryModel * model,
							const QSqlDatabase &db);

	virtual void _loadModel(const SqlQueryDescriptor &descriptor,
							SqlTreeModel * model,
							const QSqlDatabase &db);

	template<class T> void _setViewSettings(T * view, const SqlQueryDescriptor &descriptor);
	void				   _setListViewSettings(QListView * view, const SqlQueryDescriptor &descriptor);

	QVariant _selectToValue(const int &row,
							const int &column,
							const SqlQueryDescriptor &descriptor,
							const QSqlDatabase &db);

	QVariantMap _selectToMap(const QStringList &fieldNamesList,
							 const SqlQueryDescriptor &descriptor,
							 const int &row,
							 const QSqlDatabase &db);
};

}

#endif // ABSTRACTMODELHANDLER_H
