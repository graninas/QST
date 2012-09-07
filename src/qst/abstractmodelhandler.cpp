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

#include "abstractmodelhandler.h"

#include <QObject>
#include <QDebug>

namespace Sql
{

//! Конструктор по умолчанию.
AbstractModelHandler::AbstractModelHandler()
:
	_rowCount(0)
{
}

//! Чисто виртуальный деструктор.
AbstractModelHandler::~AbstractModelHandler()
{
}

/*!
	\class AbstractModelHandler
	\brief
	Класс используется для упрощения работы с моделями,
	запросами и представлениями.

	\inmodule Sql

	AbstractModelHandler - это абстрактный класс, в котором
	заключены функции по работе с источниками данных, функции по
	выбору определенного значения из модели данных, функции
	настройки модели и представления, функции, отвечающие за
	генерацию запросов SQL.

	Класс является абстрактным. Для работы с определенным объектом
	БД создается наследник, у которого переопределяются виртуальные
	DFD-генерирующие функции _selector(), _inserter(), _deleter(), _updater()
	и _executor(), отвечающие за генерацию конкретного DFD-описателя.
	(DFD = Declarative Field Descriptor), который впоследствии
	служит для генерации SQL-запросов. Пример:

	\code
	class NomenclatureHandler : public AbstractModelHandler
	{
	public:
		NomenclatureHandler();

	private:

	  virtual SqlQueryDescriptor _selector(
			const SqlQueryModelTypes &modelType = mt_plain,
			const int &queryNumber = 0) const;
	  virtual SqlQueryDescriptor _inserter(const int &queryNumber = 0) const;
	  virtual SqlQueryDescriptor _updater(const int &queryNumber = 0) const;
	  virtual SqlQueryDescriptor _deleter(const int &queryNumber = 0) const;
	  virtual SqlQueryDescriptor _executor(const int &queryNumber = 0) const;
	};
	\endcode

	Эти функции неявно вызываются всякий раз, когда необходимо
	создать запрос для модели, вставить данные в БД или выбрать
	значение определенного поля. Например, функция Delete()
	базового класса вызывает функцию _deleter() дочернего класса,
	которая возвращает DFD-описатель для запроса DELETE. Затем
	описатель передается генератору, и базовый класс получает
	запрос на языке SQL, который затем выполняет.

	Группа этих функций отвечает за создание описателей. Всего
	в классе три группы функций:
	- создание и удаление источников данных;
	- извлечение значений полей из источников данных;
	- создание описателей.

	Центральным является понятие об источнике данных. Источник
	данных - это связка "DFD-описатель - SQL-запрос - Модель -
	Представление". Представления в связке может не быть; тогда
	некоторые функции класса будут возвращать невалидные данные.

	Допустим, нам нужно создать TableView, в котором бы отображались
	данные запроса "SELECT Name, Age, Birthday FROM tPersons". На
	языке DFDL этот запрос выглядит следующим образом:

	\code
	SqlBatch batch;

	batch.addSource("tPersons");

	batch	<< SqlField("Name", fv_visible, fr_none, "Имя", 100)
			<< SqlField("Age", fv_visible, fr_none, "Возраст", 80)
			<< SqlField("Birthday", fv_visible, fr_none, "Дата рождения", 80)
			<< SqlField("ID", fv_invisible, fr_id);

	SqlQueryDescriptor descriptor(batch, sql_select, queryNumber);
	\endcode

	Конечная структура descriptor содержит всю необходимую
	информацию для создания SELECT-запроса. Более того, в ней
	так же указано, будет ли поле отображаться в представлении
	(значение fv_visible), какое будет название у колонки, а также
	какую выставлять ширину колонке (последние два значения).

	При добавлении источника данных AbstractModelHandler возьмет эту
	информацию и настроит модель/представление соответствующим
	образом. Создать источник данных можно функцией reloadSource().
	если его нет, и перезагрузить, если он есть.

	Источники данных различаются по именам. Класс поддерживает схему,
	когда одному источнику данных может
	быть сопоставлено несколько представлений, но всегда каждый
	источник данных связан только с определенной моделью с помощью имени.
	Не запрещается передавать разным источникам одни и те же
	модели, но в этом случае не гарантируется, что представления
	будут отображать верные данные.

	При создании источника AbstractModelHandler запрашивает виртуальную
	функцию _selector(), чтобы получить SqlQueryDescriptor. Затем
	источник будет добавлен в список источников, генератор сгенерирует
	SQL-запрос SELECT-типа, и запрос будет загружен в модель, а та - активирована.

	Класс AbstractModelHandler позволяет загружать в разные источники
	данных разные SELECT-запросы. Делается это с помощью параметра
	queryNumber функции reloadSource(). В переопределенной
	функции наследника _selector() программист должен выдавать нужный
	описатель в зависимости от параметра queryNumber:

	\code
	// somewhere in nomenclaturehandler.cpp:
SqlQueryDescriptor NomenclatureHandler::_selector(
		const SqlQueryModelTypes &modelType,
		const int &queryNumber) const
{
	SqlBatch batch;

	if (queryNumber == QUERY1)
	{
		batch.addSource("Nomenclature");

		batch	<< SqlField("Name", fv_visible, fr_none, "Наименование", 70)
		  << SqlField("ID", fv_invisible, fr_id);
	}
	else
	if (queryNumber == QUERY2)
	{
		batch	<< SqlField("Name", fv_visible, fr_none, "Наименование", 70)
		  << SqlField("ID", fv_invisible, fr_id)

		  << SqlField("GroupAttr", SqlValue(0), fp_where);
	}
	else
	{
		Q_ASSERT(false);
	}

return SqlQueryDescriptor(batch, sql_select, queryNumber);
}
	\endcode

	Рекомендуется использовать предопределенные константы для каждого
	запроса, а если пришло неверное значение queryNumber, - то
	выдавать сообщение об ошибке с помощью Q_ASSERT(false),
	как это сделано в примере.

	Класс использует две модели данных: SqlQueryModel (переопределенный
	класс QSqlQueryModel) и SqlTreeModel - read-only модель для отображения
	дерева. (Работа древовидной модели пока оставляет желать лучшего.)
	Дополнительно к queryNumber в функции _select() есть
	параметр modelType. Если используется SqlQueryModel, он равен
	mt_plain; в противном случае - mt_tree.

	Прикрепить представление к источнику данных можно функциями
	setTreeView(), setTableView() и setComboBox(). Функции возвращают
	индекс представления в списке для доступа к нему через другие
	функции класса.

	\attention TableView, TreeView и ListView добавляются в один список, а
	ComboBox - в другой. TableView, TreeView и ListView являются моделями
	QAbstractItemView, в то время как ComboBox не является, поэтому
	нельзя совместить их в одном списке. Во избежание ошибок функции
	по работе с этими представлениями разделены.

	Настоящая полезность класса проявляется при работе с данными. Следующий
	пример показывает, как можно получить значение конкретного поля
	из хэндлера.

	\code
	\\ somewhere in mainqindow.h:
	#include "phonecodeshandler.h"

	\\ ...

	SqlQueryModel	_queryModel;
	PhoneCodesHandler   _phonesHandler;

	\\ somewhere in mainqindow.cpp:

	_phonesHandler.reloadSource(PHONE_CODES_SOURCE, PHONE_CODES, &_queryModel);

	_phonesHandler.setTreeView(PHONE_CODES_SOURCE, ui->treeView);
	_phonesHandler.setComboBox(PHONE_CODES_SOURCE, ui->comboBox);

	QVariant id = _phonesHandler.keyValueOfComboBox(PHONE_CODES_SOURCE);
	_phonesHandler.setValue("ID", id);	// Same for "setValue(ID_VALUE, id)"
	QVariant code = _phonesHandler.SelectToValue("Code", PHONE_CODES);
	\endcode

	При каждом вызове SelectToValue() генерируется и
	исполняется SQL-запрос SELECT-типа. Извлекаемым значением может быть
	поле как с флагом fv_visible, так и с флагом fv_invisible. Функция
	setValue() позволяет загружать в класс любые данные. Конечно,
	за удобство программист расплачивается скоростью работы кода.

	Чтобы получить ранее записанное значение, существует функция value().
	Ее хорошей особенностью является то, что в нее можно передать,
	что возвращать, когда значения с таким именем в списке не нашлось.
	По умолчанию вместо ненайденного значения возвращается SqlValue().
	Это важно помнить при создании DFD-описателя.


	\attention В QST версии 0.4 существенно переработан класс AbstractModelHandler.
	Изменено его имя. Устранены источники данных, т.к. были неудобны
	и практически не использовались. Переименованы некоторые функции.
	Улучшено быстродействие. Пересмотрены DFD-генерирующие функции
	в сторону упрощения.

	\sa SqlField, SqlValue, sqlglobal.h
	*/

////////////////////////////////////////////////////////////////////////////////

/*! Перезагружает источник данных для плоской модели.

Если такового нет, создает новый.

При вызове метода _selector() передает queryNumber == 0 и modelType == mt_plain. */
void AbstractModelHandler::reloadSource(const QString &sourceName,
										SqlQueryModel * model,
										const QSqlDatabase &db)
{
	_reloadSource(sourceName, model, mt_plain, 0, false, db);
}

/*! Перезагружает источник данных для иерархической модели.

Если такового нет, создает новый.

При вызове метода _selector() передает queryNumber == 0 и modelType == mt_tree. */
void AbstractModelHandler::reloadSource(const QString &sourceName,
										SqlTreeModel * model,
										const QSqlDatabase &db)
{
	_reloadSource(sourceName, model, mt_tree, 0, false, db);
}

/*! Перезагружает источник данных для плоской модели. Сохраняет номер запроса.

Если источника нет, создает новый.

В метод _selector() передаются queryNumber и modelType == mt_plain. */
void AbstractModelHandler::reloadSource(const QString &sourceName,
										const int &queryNumber,
										SqlQueryModel * model,
										const QSqlDatabase &db)
{
	_reloadSource(sourceName, model, mt_plain, queryNumber, true, db);
}

/*! Перезагружает источник данных для иерархической модели. Сохраняет номер запроса.

Если источника нет, создает новый.

В метод _selector() передаются queryNumber и modelType == mt_tree. */
void AbstractModelHandler::reloadSource(const QString &sourceName,
										const int &queryNumber,
										SqlTreeModel * model,
										const QSqlDatabase &db)
{
	_reloadSource(sourceName, model, mt_tree, queryNumber, true, db);
}


////////////////////////////////////////////////////////////////////////////////

/*! Связывает модель, находящуюся в источнике данных с именем sourceName, и
представление TableView.

У одной модели может быть несколько представлений.

Возвращается индекс, выданный этому TableView в списке представлений, связанных
с моделью.

\attention Представления tableView, TreeView и ListView добавляются в один и тот же список,
таким образом, их индексы зависимы. */
int AbstractModelHandler::setTableView(const QString &sourceName, QTableView * tableView)
{
Q_ASSERT(tableView != NULL);

	int index;
	SqlModelDescriptor &modelDescr = rModelDescriptor(sourceName);
	index = modelDescr.addView(tableView, true);

	_setViewSettings<QTableView>(tableView, modelDescr.queryDescriptor());

return index;
}

/*! Связывает модель, находящуюся в источнике данных с именем sourceName, и
представление TreeView.

У одной модели может быть несколько представлений.

Возвращается индекс, выданный этому TreeView в списке представлений,
связанных с моделью.

\attention Представления TableView, TreeView и ListView добавляются в один и тот же список,
таким образом, их индексы зависимы. */
int AbstractModelHandler::setTreeView(const QString &sourceName, QTreeView * treeView)
{
	Q_ASSERT(treeView != NULL);

	int index;
	SqlModelDescriptor &modelDescr = rModelDescriptor(sourceName);
	index = modelDescr.addView(treeView, true);

	_setViewSettings<QTreeView>(treeView, queryDescriptor(sourceName));

return index;
}

/*! Связывает модель, находящуюся в источнике данных с именем sourceName, и
представление ListView.

У одной модели может быть несколько представлений.

Возвращается индекс, выданный этому ListView в списке представлений,
связанных с моделью.

\attention Представления TableView, TreeView и ListView добавляются в один и тот же список,
таким образом, их индексы зависимы. */
int AbstractModelHandler::setListView(const QString &sourceName, QListView * listView)
{
	Q_ASSERT(listView != NULL);

	int index;
	SqlModelDescriptor &modelDescr = rModelDescriptor(sourceName);
	index = modelDescr.addView(listView, true);

	_setListViewSettings(listView, queryDescriptor(sourceName));

return index;
}

/*! Связывает модель, находящуюся в источнике данных с именем sourceName, и
выпадающий список comboBox.

У одной модели может быть несколько выпадающих списков.

Возвращается индекс, выданный этому comboBox.

В качестве отображаемого поля выбирается первое fv_visible поле из дескриптора
(описателя). */
int AbstractModelHandler::setComboBox(const QString &sourceName,
									   QComboBox * comboBox)
{
	Q_ASSERT(comboBox != NULL);

	int index;
	SqlModelDescriptor &modelDescr = rModelDescriptor(sourceName);
	index = modelDescr.addComboBox(comboBox, true);

return index;
}

/*! Удаляет источник данных. Модель удаляется, если явно передать deleteModel == true.
По умолчанию deleteModel == false. */
void AbstractModelHandler::deleteSource(const QString &sourceName, const bool &deleteModel)
{
	if (_sources.contains(sourceName))
	{
		if (deleteModel)
			delete pModel(sourceName);

	_sources.remove(sourceName);
	}
}


/*! Возвращает индекс сервисного поля.

	Данные берутся из модели, которая связана с источником данных sourceName.

	В случае ошибки возвращает -1. */
int AbstractModelHandler::serviceFieldIndex(const QString &sourceName,
											const SqlFieldRoles &role) const
{
	Q_ASSERT(role != fr_none);

	return queryDescriptor(sourceName).batch().columnIndex(role);
}

/*! Возвращает значение ключевого поля той строчки, на которую
указывает index.

	Данные берутся из модели, которая связана с источником данных sourceName.

	Если ключевое поле не найдено, возвращает QVariant(). */
QVariant  AbstractModelHandler::keyValueOfRow(const QString &sourceName,
											  const QModelIndex &index) const
{
	if (!index.isValid())
		return QVariant();

	const QAbstractItemModel *model = cpModel(sourceName);
	const int	keyFieldIndex       = serviceFieldIndex(sourceName, fr_id);

	QModelIndex resIndex = model->index(index.row(), keyFieldIndex, index.parent());

	if (!resIndex.isValid())
		return QVariant();

return resIndex.data();
}

/*! Возвращает значение ключевого поля в строке row.

	Данные берутся из модели, которая связана с источником данных sourceName.

	Если ключевое поле не найдено, возвращает QVariant(). */
QVariant AbstractModelHandler::keyValueOfRow(const QString &sourceName,
											 const int &row) const
{
	const QAbstractItemModel *model = cpModel(sourceName);
	const int keyFieldIndex         = serviceFieldIndex(sourceName, fr_id);

	QModelIndex resIndex = model->index(row, keyFieldIndex, QModelIndex());

	if (!resIndex.isValid())
		return QVariant();

return resIndex.data();
}


/*! Возвращает значение ключевого поля той строчки, которая
  является текущей в переданном view.

  Данные берутся из модели, которая связана с источником данных sourceName.

  Если ключевое поле не найдено, или текущий view.currentIndex() невалиден,
  возвращает QVariant(). */
QVariant AbstractModelHandler::keyValueOfCurrent(const QString &sourceName,
												 QAbstractItemView * view) const
{
	Q_ASSERT(view != NULL);

	return keyValueOfRow(sourceName, view->currentIndex());
}

/*! Возвращает значение ключевого поля той строчки, которая
  является текущей в переданном comboBox.

  Данные берутся из модели, которая связана с источником данных sourceName.

  Если ключевое поле не найдено, или текущий comboBox.currentIndex() невалиден,
  возвращает QVariant(). */
QVariant AbstractModelHandler::keyValueOfCurrent(const QString &sourceName,
												 QComboBox * comboBox) const
{
	Q_ASSERT(comboBox != NULL);

	return keyValueOfRow(sourceName, comboBox->currentIndex());
}

/*! Возвращает значение ключевого поля той строчки, которая
  является текущей во view.

  View берется из источника данных sourceName. Если это единственный view,
  то параметр viewIndex можно не указывать. Если же в источнике данных
  находятся несколько view, то необходимо указать нужный индекс.

  Данные берутся из модели, которая связана с источником данных sourceName.

  Если ключевое поле не найдено, или текущий индекс во view невалиден,
  возвращает QVariant(). */
QVariant AbstractModelHandler::keyValueOfView(const QString &sourceName,
											  const int &viewIndex) const
{
	SqlModelDescriptor descr = modelDescriptor(sourceName);
	QAbstractItemView * view;
	view = descr.view(viewIndex);

	Q_ASSERT(view != NULL);

	return keyValueOfCurrent(sourceName, view);
}

/*! Возвращает значение ключевого поля той строчки, которая
  является текущей в comboBox.

  ComboBox берется из источника данных sourceName. Если это единственный comboBox,
  то параметр comboBoxIndex можно не указывать. Если же в источнике данных
  находятся несколько comboBox, то необходимо указать нужный индекс.

  Данные берутся из модели, которая связана с источником данных sourceName.

  Если ключевое поле не найдено, или текущий индекс в comboBox невалиден,
  возвращает QVariant(). */
QVariant AbstractModelHandler::keyValueOfComboBox(const QString &sourceName,
												  const int &comboBoxIndex) const
{
	SqlModelDescriptor descr = modelDescriptor(sourceName);
	QComboBox * comboBox;
	comboBox = descr.comboBox(comboBoxIndex);

	Q_ASSERT(comboBox != NULL);

	return keyValueOfCurrent(sourceName, comboBox);
}

/*! Возвращает список ключей выделенных во view записей.

	Если выделенных записей нет, возвращает пустой список. */
QVariantList  AbstractModelHandler::selectedKeysOfView(const QString &sourceName,
													   const int &viewIndex) const
{
	SqlModelDescriptor		descr = modelDescriptor(sourceName);
	QAbstractItemView *		view  = descr.view(viewIndex);

	QModelIndexList indexesList = view->selectionModel()->selectedIndexes();

	int keysColumnIndex = serviceFieldIndex(sourceName, fr_id);

	QVariantList keysList;

	for (int i = 0; i < indexesList.size(); ++i)
	{
		if (indexesList[i].column() == keysColumnIndex)
		{
#ifdef QT_DEBUG
			qDebug() << "key = " << i << " value = " << indexesList[i].data();
#endif
			keysList.append(indexesList[i].data());
		}
	}

	return keysList;
}

/*! Сохраняет value под именем valueName. */
void AbstractModelHandler::setValue(const QString &valueName,
								   const SqlValue &value)
{
	Q_ASSERT(!valueName.isEmpty());

	_values[valueName] = value;
}

/*! Сохраняет keyValueOfView(sourceName, viewIndex) под именем valueName. */
void AbstractModelHandler::setValueFromView(const QString &sourceName,
								  const QString &valueName,
								  const int &viewIndex)
{
	setValue(valueName, keyValueOfView(sourceName, viewIndex));
}

/*! Сохраняет keyValueOfComboBox(sourceName, comboBoxIndex) под именем valueName. */
void AbstractModelHandler::setValueFromComboBox(const QString &sourceName,
									  const QString &valueName,
									  const int &comboBoxIndex)
{
	setValue(valueName, keyValueOfComboBox(sourceName, comboBoxIndex));
}

/*! Возвращает значение по имени valueName.

	Если значение отсутствует, возвращает defaultValue.

	По умолчанию defaultValue == SqlValue().

	При неопределенном INVALID_SQLVALUE_BY_DEFAULT, defaultValue меняется
	на SqlValue(nt_null). */
SqlValue AbstractModelHandler::value(const QString &valueName,
									 const SqlValue &defaultValue) const
{
	SqlValue val = _values.value(valueName, SqlValue());
	if (!val.isValid())
		return defaultValue;
return val;
}

/*! Возвращает значение по имени valueName.

	Если значение отсутствует, возвращает SqlValue(nt_null).

	Требует вторым параметром nt_null, что позволяет записать
	[code]
	value("val", SqlValue(nt_null))
	[/code]
	короче:
	[code]
	value("val", nt_null)
	[/code]
*/
SqlValue AbstractModelHandler::value(const QString &valueName,
									 const NullType &nullType) const
{
	SqlValue val = _values.value(valueName, SqlValue());
	if (!val.isValid())
		return SqlValue(nullType);
return val;
}

/*! Сохраняет список значений. */
void AbstractModelHandler::setValuesMap(const SqlValuesMap &valuesMap)
{
	_valuesMap = valuesMap;
}

/*! Возвращает список сохраненных ранее значений. */
SqlValuesMap AbstractModelHandler::valuesMap() const
{
	return _valuesMap;
}


/*! Очищает список значений. */
void AbstractModelHandler::clearValues()
{
	_values.clear();
}

/*! Возвращает данные поля fieldName в строке row.

	Чтобы получить данные, один раз обращается к БД.
	Для этого создает DFD-описатель с помощью _selector(mt_plain, queryNumber),
	генерирует SELECT-запрос, выполняет его и создает временную SqlQueryModel.
	В модели и содержатся необходимые данные.

	Возвращает QVariant(), если ячейка не найдена, или данные невалидны. */
QVariant AbstractModelHandler::SelectToValue(const QString fieldName,
											 const int &queryNumber,
											 const int &row,
											 const QSqlDatabase &db)
{
	SqlQueryDescriptor	descriptor = this->_selector(mt_plain, queryNumber);

	int column = descriptor.batch().columnIndex(fieldName);

	return _selectToValue(row, column, descriptor, db);
}

/*! Возвращает данные поля fieldName в строке row.

	Чтобы получить данные, один раз обращается к БД.
	Для этого получает DFD-описатель из источника данных с именем sourceName,
	генерирует SELECT-запрос, выполняет его и создает временную SqlQueryModel.
	В модели и содержатся необходимые данные.

	Возвращает QVariant(), если ячейка не найдена, или данные невалидны. */
QVariant AbstractModelHandler::SelectToValue(const QString sourceName,
											 const QString fieldName,
											 const int &row,
											 const QSqlDatabase &db)
{
	SqlQueryDescriptor	descriptor = queryDescriptor(sourceName);

	int column = descriptor.batch().columnIndex(fieldName);

	return _selectToValue(row, column, descriptor, db);
}

/*! Возвращает данные полей, чьи заголовки перечислены в списке fieldNamesList.

	Чтобы получить данные всех полей, один раз обращается к БД.

	Принцип тот же, что и у функции SelectToValue(QString, int, int, QSqlDatabase),
только для множества полей. */
QVariantMap AbstractModelHandler::SelectToMap(const QStringList &fieldNamesList,
											  const int &queryNumber,
											  const int &row,
											  const QSqlDatabase &db)
{
	SqlQueryDescriptor	descriptor = this->_selector(mt_plain, queryNumber);

	return _selectToMap(fieldNamesList, descriptor, row, db);
}

/*! Возвращает данные полей, чьи заголовки перечислены в списке fieldNamesList.

	Чтобы получить данные всех полей, один раз обращается к БД.

	Принцип тот же, что и у функции SelectToValue(QString, QString, int, QSqlDatabase),
только для множества полей. */
QVariantMap AbstractModelHandler::SelectToMap(const QString sourceName,
											  const QStringList &fieldNamesList,
											  const int &row,
											  const QSqlDatabase &db)
{
	SqlQueryDescriptor	descriptor = queryDescriptor(sourceName);

	return _selectToMap(fieldNamesList, descriptor, row, db);
}

/*! Генерирует INSERT-запрос, описанный в _inserter(), и выполняет его. */
bool AbstractModelHandler::Insert(const int &queryNumber,
								  const QSqlDatabase &db)
{
	Q_ASSERT(db.isOpen());

	SqlQueryDescriptor	descriptor = this->_inserter(queryNumber);

	Q_ASSERT(descriptor.type() == sql_insert);

	QSqlQuery	query(db);
	SqlGen		sqlGen(descriptor.batch(), descriptor.type());

#ifdef QT_DEBUG
	qDebug() << "For the Insert function,";
	qDebug() << "query of type" << descriptor.type() << "and number" << queryNumber << "will be:";
	qDebug() << sqlGen.query();
#endif

	if (!query.prepare(sqlGen.query()))
		return false;

return query.exec();
}

/*! Генерирует UPDATE-запрос, описанный в _updater(), и выполняет его. */
bool AbstractModelHandler::Update(const int &queryNumber,
								  const QSqlDatabase &db)
{
	Q_ASSERT(db.isOpen());

	SqlQueryDescriptor	descriptor = this->_updater(queryNumber);

	Q_ASSERT(descriptor.type() == sql_update);

	QSqlQuery	query(db);
	SqlGen		sqlGen(descriptor.batch(), descriptor.type());

#ifdef QT_DEBUG
	qDebug() << "For the Update function,";
	qDebug() << "query of type" << descriptor.type() << "and number" << queryNumber << "will be:";
	qDebug() << sqlGen.query();
#endif

	if (!query.prepare(sqlGen.query()))
		return false;

return query.exec();
}

/*! Генерирует DELETE-запрос, описанный в _deleter(), и выполняет его. */
bool AbstractModelHandler::Delete(const int &queryNumber,
								  const QSqlDatabase &db)
{
	Q_ASSERT(db.isOpen());

	SqlQueryDescriptor	descriptor = this->_deleter(queryNumber);

	Q_ASSERT(descriptor.type() == sql_delete);

	QSqlQuery	query(db);
	SqlGen		sqlGen(descriptor.batch(), descriptor.type());

#ifdef QT_DEBUG
	qDebug() << "For the Delete function,";
	qDebug() << "query of type" << descriptor.type() << "and number" << queryNumber << "will be:";
	qDebug() << sqlGen.query();
#endif

	if (!query.prepare(sqlGen.query()))
		return false;

return query.exec();
}

/*! Выполняет хранимую процедуру, описанную функцией _executor(). */
bool AbstractModelHandler::Exec(const int &queryNumber,
										 const QSqlDatabase &db)
{
	Q_ASSERT(db.isOpen());

	SqlQueryDescriptor	descriptor = this->_executor(queryNumber);

	Q_ASSERT(descriptor.type() == sql_execute);

	QSqlQuery	query(db);
	SqlGen		sqlGen(descriptor.batch(), descriptor.type());

#ifdef QT_DEBUG
	qDebug() << "For the Exec function,";
	qDebug() << "query of type" << descriptor.type() << "and number" << queryNumber << "will be:";
	qDebug() << sqlGen.query();
#endif

	if (!query.prepare(sqlGen.query()))
		return false;

return query.exec();
}

/*! Удаляет текущую во view строку из БД, обновляет содержимое модели.

  Функция предполагает, что _deleter() содержит следующий фильтр:
  \code
  batch << SqlField(<Ключевое_поле_в_БД>, value(ID_VALUE, SqlValue(nt_null)), fp_where);
  \endcode
*/
bool AbstractModelHandler::DeleteCurrent(const QString &sourceName,
										 QAbstractItemView * view,
										 const QSqlDatabase &db)
{
	QVariant id = keyValueOfCurrent(sourceName, view);
	bool res = false;

	if (id.isValid())
	{
		SqlValue oldId = value(ID_VALUE);
		setValue(ID_VALUE, id);
			res = Delete();
		setValue(ID_VALUE, oldId);
	}
return res;
}

/*! Генерирует и возвращает SELECT-запрос. */
QString AbstractModelHandler::SelectQuery(const int &queryNumber,
										  const SqlQueryModelTypes &type) const
{
	SqlQueryDescriptor	descriptor = this->_selector(type, queryNumber);
	SqlGen sqlGen(descriptor.batch(), descriptor.type());
	return sqlGen.query();
}

/*! Генерирует и возвращает INSERT-запрос. */
QString AbstractModelHandler::InsertQuery(const int &queryNumber) const
{
	SqlQueryDescriptor	descriptor = this->_inserter(queryNumber);
	SqlGen sqlGen(descriptor.batch(), descriptor.type());
	return sqlGen.query();
}

/*! Генерирует и возвращает UPDATE-запрос. */
QString AbstractModelHandler::UpdateQuery(const int &queryNumber) const
{
	SqlQueryDescriptor	descriptor = this->_updater(queryNumber);
	SqlGen sqlGen(descriptor.batch(), descriptor.type());
	return sqlGen.query();
}

/*! Генерирует и возвращает DELETE-запрос. */
QString AbstractModelHandler::DeleteQuery(const int &queryNumber) const
{
	SqlQueryDescriptor	descriptor = this->_deleter(queryNumber);
	SqlGen sqlGen(descriptor.batch(), descriptor.type());
	return sqlGen.query();
}

/*! Генерирует и возвращает EXEC-запрос. */
QString AbstractModelHandler::ExecQuery(const int &queryNumber) const
{
	SqlQueryDescriptor	descriptor = this->_executor(queryNumber);
	SqlGen sqlGen(descriptor.batch(), descriptor.type());
	return sqlGen.query();
}

/*! Удаляет текущую в comboBox строку из БД, обновляет содержимое модели.

  Функция предполагает, что _deleter() содержит следующий фильтр:
  \code
  batch << SqlField(<Ключевое_поле_в_БД>, value(ID_VALUE, SqlValue(nt_null)), fp_where);
  \endcode
*/
bool AbstractModelHandler::DeleteCurrent(const QString &sourceName,
										 QComboBox * comboBox,
										 const QSqlDatabase &db)
{
	QVariant id = keyValueOfCurrent(sourceName, comboBox);
	bool res = false;

	if (id.isValid())
	{
		SqlValue oldId = value(ID_VALUE);
		setValue(ID_VALUE, id);
			res = Delete();
		setValue(ID_VALUE, oldId);
	}
return res;
}

/*! Возвращает указатель на сохраненную модель.

	Если источника sorceName не найдено, или указатель на модель равен NULL,
	выпадает в Q_ASSERT(). */
QAbstractItemModel *AbstractModelHandler::pModel(const QString &sourceName)
{
	Q_ASSERT(!sourceName.isEmpty());
	Q_ASSERT(_sources.contains(sourceName));
	Q_ASSERT(_sources[sourceName].pModel());

	return _sources[sourceName].pModel();
}

/*! Возвращает константный указатель на сохраненную модель.

	Если источника sorceName не найдено, или указатель на модель равен NULL,
	выпадает в Q_ASSERT(). */
QAbstractItemModel *AbstractModelHandler::cpModel(const QString &sourceName) const
{
	Q_ASSERT(!sourceName.isEmpty());
	Q_ASSERT(_sources.contains(sourceName));
	Q_ASSERT(_sources[sourceName].cpModel());

	return _sources[sourceName].cpModel();
}

/*! Возвращает дескриптор модели, содержащейся в источнике данных sourceName.

	Если источника sorceName не найдено, выпадает в Q_ASSERT().

\sa SqlModelDescriptor */
SqlModelDescriptor AbstractModelHandler::modelDescriptor(const QString &sourceName) const
{
#ifdef QT_DEBUG
	if (!_sources.contains(sourceName))
		qDebug() << "SourceName:" << sourceName;
#endif

	Q_ASSERT(!sourceName.isEmpty());
	Q_ASSERT(_sources.contains(sourceName));

	return _sources[sourceName];
}

/*! Возвращает дескриптор (описатель) запроса, содержащейся
в источнике данных sourceName.

	Если источника sorceName не найдено, выпадает в Q_ASSERT().

\sa SqlQueryDescriptor, DFDL */
SqlQueryDescriptor AbstractModelHandler::queryDescriptor(const QString &sourceName) const
{
	return modelDescriptor(sourceName).queryDescriptor();
}

/*! Возвращает ссылку на дескриптор (описатель) запроса, содержащейся
в источнике данных sourceName.

	Если источника sorceName не найдено, выпадает в Q_ASSERT().

\sa SqlQueryDescriptor, DFDL */
SqlModelDescriptor & AbstractModelHandler::rModelDescriptor(const QString &sourceName)
{
	Q_ASSERT(!sourceName.isEmpty());
	Q_ASSERT(_sources.contains(sourceName));

	return _sources[sourceName];
}

// ------------------------------------------------------------------------- //

/*! Возвращает описатель для запроса SELECT-типа по виду модели и номеру запроса.

	Функция переопределяется в наследующем классе с нужным функционалом.

	Неявно вызывается при создании и перезагрузке источника данных (модель, view/comboBox),
	а так же из функций SelectToValue(), SelectToMap(), keyValueOfRow(),
	keyValueOfCurrent(), keyValueOfView(), keyValueOfComboBox(), setValueFromView(),
	setValueFromComboBox().

\sa SqlQueryDescriptor, SqlField, SqlValue, DFDL */
SqlQueryDescriptor AbstractModelHandler::_selector(const SqlQueryModelTypes &modelType,
												   const int &queryNumber) const
{
	Q_ASSERT(false);
	return SqlQueryDescriptor(SqlBatch(), sql_select, queryNumber);
}

/*! Возвращает описатель для запроса INSERT-типа по номеру запроса.

	Функция переопределяется в наследующем классе с нужным функционалом.

	Неявно вызывается из функции Insert().

\sa SqlQueryDescriptor, SqlField, SqlValue, DFDL */
SqlQueryDescriptor  AbstractModelHandler::_inserter(const int &queryNumber) const
{
	Q_ASSERT(false);
	return SqlQueryDescriptor(SqlBatch(), sql_insert, queryNumber);
}

/*! Возвращает описатель для запроса UPDATE-типа по номеру запроса.

	Функция переопределяется в наследующем классе с нужным функционалом.

	Неявно вызывается из функции Update().

\sa SqlQueryDescriptor, SqlField, SqlValue, DFDL */
SqlQueryDescriptor AbstractModelHandler::_updater(const int &queryNumber) const
{
	Q_ASSERT(false);
	return SqlQueryDescriptor(SqlBatch(), sql_update, queryNumber);
}

/*! Возвращает описатель для запроса DELETE-типа по номеру запроса.

	Функция переопределяется в наследующем классе с нужным функционалом.

	Неявно вызывается из функций Delete(), DeleteCurrent().

\sa SqlQueryDescriptor, SqlField, SqlValue, DFDL */
SqlQueryDescriptor AbstractModelHandler::_deleter(const int &queryNumber) const
{
	Q_ASSERT(false);
	return SqlQueryDescriptor(SqlBatch(), sql_delete, queryNumber);
}

/*! Возвращает описатель для запроса EXEC-типа по номеру запроса.

	Функция переопределяется в наследующем классе с нужным функционалом.

	Неявно вызывается из функции Exec().

\sa SqlQueryDescriptor, SqlField, SqlValue, DFDL */
SqlQueryDescriptor AbstractModelHandler::_executor(const int &queryNumber) const
{
	Q_ASSERT(false);
	return SqlQueryDescriptor(SqlBatch(), sql_execute, queryNumber);
}


void AbstractModelHandler::_addSource(const QString &sourceName,
									  QAbstractItemModel * model,
									  const SqlQueryModelTypes &type,
									  const int &queryNumber,
									  const bool &useQueryNumber,
									  const QSqlDatabase &db)
{
	Q_ASSERT(!sourceName.isEmpty());
	Q_ASSERT(!_sources.contains(sourceName));
	Q_ASSERT(model != NULL);
	Q_ASSERT(db.isOpen());

	SqlQueryDescriptor queryDescriptor;
	if (useQueryNumber)
		queryDescriptor = this->_selector(type, queryNumber);
	else
		queryDescriptor = this->_selector(type);

	Q_ASSERT(queryDescriptor.type() == sql_select);

	SqlModelDescriptor modelDescriptor =
			SqlModelDescriptor(model, type, queryDescriptor);

	_sources.insert(sourceName, modelDescriptor);

	if (type == mt_plain)
		_loadModel(queryDescriptor, (SqlQueryModel*)model, db);
	else
		_loadModel(queryDescriptor, (SqlTreeModel*)model, db);
}


void AbstractModelHandler::_reloadSource(const QString &sourceName,
										 QAbstractItemModel * model,
										 const SqlQueryModelTypes &type,
										 const int &queryNumber,
										 const bool &reloadQueryNumber,
										 const QSqlDatabase &db)
{
	Q_ASSERT(!sourceName.isEmpty());
	Q_ASSERT(db.isOpen());

	if (!_sources.contains(sourceName))
	{
		_addSource(sourceName, model, type, queryNumber, reloadQueryNumber, db);
	}
	else
	{
		SqlModelDescriptor & modelDescr = rModelDescriptor(sourceName);

		modelDescr.demodelizeViews();

		int newQueryNumber;

			SqlQueryDescriptor oldDescriptor = modelDescr.queryDescriptor();

				if (reloadQueryNumber)	newQueryNumber = queryNumber;
				else					newQueryNumber = oldDescriptor.queryNumber();

			SqlQueryDescriptor newDescriptor = this->_selector(type, newQueryNumber);

			Q_ASSERT(newDescriptor.type() == sql_select);

			if (!model)
			{
				if (type == mt_plain)
					_loadModel(newDescriptor, (SqlQueryModel*)pModel(sourceName), db);
				else
					_loadModel(newDescriptor, (SqlTreeModel*)pModel(sourceName), db);
			}
			else
			{
				if (type == mt_plain)
					_loadModel(newDescriptor, (SqlQueryModel*)model, db);
				else
					_loadModel(newDescriptor, (SqlTreeModel*)model, db);

				_sources[sourceName].setModel(model);
			}

			_sources[sourceName].setQueryDescriptor(newDescriptor);

		modelDescr.setModelType(type);
		modelDescr.modelizeViews();
	}
}



/*! Генерирует запрос, передает его в плоскую модель. */
void  AbstractModelHandler::_loadModel(const SqlQueryDescriptor &descriptor,
									   SqlQueryModel * model,
									   const QSqlDatabase &db)
{
	SqlGen sqlGen(descriptor.batch(), descriptor.type());
#ifdef QT_DEBUG
	qDebug() << "For the loadModel function,";
	qDebug() << "query of type" << descriptor.type() << "and number" << descriptor.queryNumber() << "will be:";
	qDebug() << sqlGen.query();
#endif

	model->setQuery(sqlGen.query(), db);
}


/*! Генерирует запрос, устанавливает индексы ключевых полей в иерархической
модели и передает в нее запрос. */
void  AbstractModelHandler::_loadModel(const SqlQueryDescriptor &descriptor,
									   SqlTreeModel * model,
									   const QSqlDatabase &db)
{
	SqlBatch batch = descriptor.batch();

	model->setKeyField(batch.columnIndex(fr_id));

	int parentFieldIndex = batch.columnIndex(fr_parent);
	if (parentFieldIndex != -1)
		model->setParentField(parentFieldIndex);

	SqlGen sqlGen(batch, descriptor.type());

#ifdef QT_DEBUG
	qDebug() << "For the loadModel function,";
	qDebug() << "query of type" << descriptor.type() << "and number" << descriptor.queryNumber() << "will be:";
	qDebug() << sqlGen.query();
#endif

	model->setQuery(sqlGen.query(), db);
}


template<class T> void AbstractModelHandler::_setViewSettings(T * view, const SqlQueryDescriptor &descriptor)
{
	SqlBatch batch = descriptor.batch();
	SqlFieldsVector fields = batch.fields();

	int columnIndex;

	for (int i = 0; i < fields.size(); ++i)
	{
		const SqlField &rField = fields[i];

		if (rField.isVisible())
		{
			columnIndex = batch.columnIndex(rField.name());
			Q_ASSERT(columnIndex != -1);

			view->setColumnWidth(columnIndex, rField.columnWidth());
			view->model()->setHeaderData(columnIndex,
										 rField.orientation(),
										 rField.columnTitle(),
										 Qt::DisplayRole);
		}
		else
			if (rField.isInvisible())
			{
				columnIndex = batch.columnIndex(rField.name());
				Q_ASSERT(columnIndex != -1);

				view->setColumnHidden(columnIndex, true);
			}
	};
}

void AbstractModelHandler::_setListViewSettings(QListView * view, const SqlQueryDescriptor &descriptor)
{
	SqlBatch batch = descriptor.batch();
	SqlFieldsVector fields = batch.fields();

	int columnIndex;

	for (int i = 0; i < fields.size(); ++i)
	{
		const SqlField &rField = fields[i];

		if (rField.isVisible())
		{
			columnIndex = batch.columnIndex(rField.name());
			Q_ASSERT(columnIndex != -1);

			view->model()->setHeaderData(columnIndex,
										 rField.orientation(),
										 rField.columnTitle(),
										 Qt::DisplayRole);
		}
		else
			if (rField.isInvisible())
			{
				columnIndex = batch.columnIndex(rField.name());
				Q_ASSERT(columnIndex != -1);
			}
	};
}



QVariant AbstractModelHandler::_selectToValue(const int &row,
											  const int &column,
											  const SqlQueryDescriptor &descriptor,
											  const QSqlDatabase &db)
{
	Q_ASSERT(db.isOpen());
	Q_ASSERT(descriptor.type() == sql_select);

	SqlQueryModel queryModel;
	SqlGen sqlGen(descriptor.batch(), descriptor.type());

#ifdef QT_DEBUG
	qDebug() << "For the SelectToValue function,";
	qDebug() << "query of type" << descriptor.type() << "and number" << descriptor.queryNumber() << "will be:";
	qDebug() << sqlGen.query();
#endif

	queryModel.setQuery(sqlGen.query(), db);

	QModelIndex index = queryModel.index(row, column, QModelIndex());

	if (index.isValid())
		return index.data(Qt::DisplayRole);

return QVariant();
}

QVariantMap AbstractModelHandler::_selectToMap(const QStringList &fieldNamesList,
											   const SqlQueryDescriptor &descriptor,
											   const int &row,
											   const QSqlDatabase &db)
{
	Q_ASSERT(db.isOpen());
	Q_ASSERT(descriptor.type() == sql_select);
	SqlGen sqlGen(descriptor.batch(), descriptor.type());

	#ifdef QT_DEBUG
	qDebug() << "For the SelectToMap function,";
	qDebug() << "query of type" << descriptor.type() << "and number" << descriptor.queryNumber() << "will be:";
		qDebug() << sqlGen.query();
	#endif

	SqlQueryModel	queryModel;
					queryModel.setQuery(sqlGen.query(), db);

	SqlBatch batch = descriptor.batch();

	QVariantMap		resMap;
	int				column;
	QModelIndex		index;

	for (int i = 0; i < fieldNamesList.size(); ++i)
	{
		column	= batch.columnIndex(fieldNamesList[i]);
		index	= queryModel.index(row, column, QModelIndex());

		if (index.isValid())
			resMap[fieldNamesList[i]] = index.data(Qt::DisplayRole);
	}	

	return resMap;
}


}	// End of namespace Sql
