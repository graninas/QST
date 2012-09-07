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

#include "qstabstractmodelhandler.h"

#include <QObject>
#include <QDebug>

namespace Qst
{

	QstAbstractModelHandler::QstAbstractModelHandler()
		:
		_rowCount(0)
	{
	}

	QstAbstractModelHandler::~QstAbstractModelHandler()
	{
	}

	/*!
		\class QstAbstractModelHandler
		\brief
		Класс используется для упрощения работы с моделями,
		запросами и представлениями.

		\inmodule Sql

		QstAbstractModelHandler - это абстрактный класс, в котором
		заключены функции по работе с источниками данных, функции по
		выбору определенного значения из модели данных, функции
		настройки модели и представления, функции, отвечающие за
		генерацию запросов SQL.

		Класс является абстрактным. Для работы с определенным объектом
		БД создается наследник, у которого переопределяются виртуальные
		функции _selector(), _inserter(), _deleter(), _updater()
		и _executor(), отвечающие за генерацию конкретного DFD-описателя.
		(DFD = Declarative Field Descriptor), который впоследствии
		служит для генерации SQL-запросов. Пример:

		\code
		class NomenclatureHandler : public QstAbstractModelHandler
		{
		public:
			NomenclatureHandler();

		private:

		  virtual QstQueryDescriptor _selector(
				const ModelTypes &modelType = mt_plain,
				const int &queryNumber = 0) const;
		  virtual QstQueryDescriptor _inserter(const int &queryNumber = 0) const;
		  virtual QstQueryDescriptor _updater(const int &queryNumber = 0) const;
		  virtual QstQueryDescriptor _deleter(const int &queryNumber = 0) const;
		  virtual QstQueryDescriptor _executor(const int &queryNumber = 0) const;
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
		QstBatch batch;

		batch.addSource("tPersons");

		batch	<< QstField("Name", fv_visible, fr_none, "Имя", 100)
				<< QstField("Age", fv_visible, fr_none, "Возраст", 80)
				<< QstField("Birthday", fv_visible, fr_none, "Дата рождения", 80)
				<< QstField("ID", fv_invisible, fr_id);

		QstQueryDescriptor descriptor(batch, sql_select, queryNumber);
		\endcode

		Конечная структура descriptor содержит всю необходимую
		информацию для создания SELECT-запроса. Более того, в ней
		так же указано, будет ли поле отображаться в представлении
		(значение fv_visible), какое будет название у колонки, а также
		какую выставлять ширину колонке (последние два значения).

		При добавлении источника данных QstAbstractModelHandler возьмет эту
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

		При создании источника QstAbstractModelHandler запрашивает виртуальную
		функцию _selector(), чтобы получить QstQueryDescriptor. Затем
		источник будет добавлен в список источников, генератор сгенерирует
		SQL-запрос SELECT-типа, и запрос будет загружен в модель, а та - активирована.

		Класс QstAbstractModelHandler позволяет загружать в разные источники
		данных разные SELECT-запросы. Делается это с помощью параметра
		queryNumber функции reloadSource(). В переопределенной
		функции наследника _selector() программист должен выдавать нужный
		описатель в зависимости от параметра queryNumber:

		\code
		// somewhere in nomenclaturehandler.cpp:
	QstQueryDescriptor NomenclatureHandler::_selector(
			const ModelTypes &modelType,
			const int &queryNumber) const
	{
		QstBatch batch;

		if (queryNumber == QUERY1)
		{
			batch.addSource("Nomenclature");

			batch	<< QstField("Name", fv_visible, fr_none, "Наименование", 70)
			  << QstField("ID", fv_invisible, fr_id);
		}
		else
		if (queryNumber == QUERY2)
		{
			batch	<< QstField("Name", fv_visible, fr_none, "Наименование", 70)
			  << QstField("ID", fv_invisible, fr_id)

			  << QstField("GroupAttr", QstValue(0), fp_where);
		}
		else
		{
			Q_ASSERT(false);
		}

	return QstQueryDescriptor(batch, sql_select);
	}
		\endcode

		? екомендуется использовать предопределенные константы для каждого
		запроса, а если пришло неверное значение queryNumber, - то
		выдавать сообщение об ошибке с помощью Q_ASSERT(false),
		как это сделано в примере.

		Класс использует две модели данных: QstPlainQueryModel, которая
		наследуется от QQstPlainQueryModel и практически ничем от родителя
		не отличается, и QstTreeQueryModel - read-only модель для отображения
		дерева. (? абота древовидной модели пока оставляет желать лучшего.)
		Дополнительно к queryNumber в функции _select() есть
		параметр modelType. Если используется QstPlainQueryModel, он равен
		mt_plain; в противном случае - mt_tree.

		Прикрепить представление к источнику данных можно функциями
		setTreeView(), setTableView() и setComboBox(). Функции возвращают
		индекс представления в списке для доступа к нему через другие
		функции класса.

		\attention TreeView и TableView добавляются в один список, а
		ComboBox - в другой. TreeView и TableView являются моделями
		QAbstractItemView, в то время как ComboBox не является, поэтому
		нельзя совместить их в одном списке. Во избежание ошибок функции
		по работе с этими представлениями разделены.

		Настоящая мощь класса проявляется при работе с данными. Следующий
		пример показывает, как можно получить значение конкретного поля
		из хэндлера.

		\code
		\\ somewhere in mainqindow.h:
		#include "phonecodeshandler.h"

		\\ ...

		QstPlainQueryModel	_queryModel;
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
		за удобство программист расплачивается скоростью работы программы,
		так как все значения хранятся в QMap, и доступ к ним осуществляется
		по имени.

		Чтобы получить ранее записанное значение, существует функция value().
		Ее хорошей особенностью является то, что в нее можно передать,
		что возвращать, когда значения с таким именем в списке не нашлось.
		По умолчанию вместо ненайденного значения возвращается QstValue().
		Это важно помнить при создании DFD-описателя.

		\sa QstField, QstValue, sqlglobal.h
		*/

	////////////////////////////////////////////////////////////////////////////////

	/*! Перезагружает источник данных для плоской модели.

	Если такового нет, создает новый.

	При вызове метода _selector() передает queryNumber == 0 и modelType == mt_plain. */
	void QstAbstractModelHandler::reload(QstPlainQueryModel * model,
										 const QSqlDatabase &db)
	{
		_reload(model, ModelPlain, 0, false, db);
	}

	/*! Перезагружает источник данных для иерархической модели.

	Если такового нет, создает новый.

	При вызове метода _selector() передает queryNumber == 0 и modelType == mt_tree. */
	void QstAbstractModelHandler::reload(QstTreeQueryModel * model,
										 const QSqlDatabase &db)
	{
		_reload(model, ModelTree, 0, false, db);
	}

	/*! Перезагружает источник данных для плоской модели. Сохраняет номер запроса.

	Если источника нет, создает новый.

	В метод _selector() передаются queryNumber и modelType == mt_plain. */
	void QstAbstractModelHandler::reload(const int &queryNumber,
										 QstPlainQueryModel * model,
										 const QSqlDatabase &db)
	{
		_reload(model, ModelPlain, queryNumber, true, db);
	}

	/*! Перезагружает источник данных для иерархической модели. Сохраняет номер запроса.

	Если источника нет, создает новый.

	В метод _selector() передаются queryNumber и modelType == mt_tree. */
	void QstAbstractModelHandler::reload(const int &queryNumber,
										 QstTreeQueryModel * model,
										 const QSqlDatabase &db)
	{
		_reload(model, ModelTree, queryNumber, true, db);
	}


	////////////////////////////////////////////////////////////////////////////////

	/*! Связывает модель, находящуюся в источнике данных с именем sourceName, и
	представление tableView.

	У одной модели может быть несколько представлений.

	Возвращается индекс, выданный этому tableView в списке представлений, связанных
	с моделью.

	\attention Представления tableView и treeView добавляются в один и тот же список,
	таким образом, их индексы зависимы. */
	int QstAbstractModelHandler::setTableView(QTableView * tableView)
	{
		Q_ASSERT(tableView != NULL);

		int index;
		index = _modelDescriptor.addView(tableView, true);

		_setViewSettings<QTableView>(tableView, _modelDescriptor.queryDescriptor());

		return index;
	}

	/*! Связывает модель, находящуюся в источнике данных с именем sourceName, и
	представление treeView.

	У одной модели может быть несколько представлений.

	Возвращается индекс, выданный этому treeView в списке представлений,
	связанных с моделью.

	\attention Представления tableView и treeView добавляются в один и тот же список,
	таким образом, их индексы зависимы. */
	int QstAbstractModelHandler::setTreeView(QTreeView * treeView)
	{
		Q_ASSERT(treeView != NULL);

		int index;
		index = _modelDescriptor.addView(treeView, true);

		_setViewSettings<QTreeView>(treeView, _modelDescriptor.queryDescriptor());

		return index;
	}

	int QstAbstractModelHandler::setListView(QListView * listView)
	{
		Q_ASSERT(listView != NULL);

		int index;

		index = _modelDescriptor.addView(listView, true);

		_setListViewSettings(listView, _modelDescriptor.queryDescriptor());

		return index;
	}

	/*! Связывает модель, находящуюся в источнике данных с именем sourceName, и
	выпадающий список comboBox.

	У одной модели может быть несколько выпадающих списков.

	Возвращается индекс, выданный этому comboBox.

	В качестве отображаемого поля выбирается первое fv_visible поле из дескриптора
	(описателя). */
	int QstAbstractModelHandler::setComboBox(QComboBox * comboBox)
	{
		Q_ASSERT(comboBox != NULL);

		int index;
		index = _modelDescriptor.addComboBox(comboBox, true);

		return index;
	}

	/*! Экспериментальная функция для получения количества строк.

	Наиболее актуальна для драйверов ODBC.

	? аботоспособность не подтверждена. */
	int QstAbstractModelHandler::rowCount()
	{
		return _modelDescriptor.pModel()->rowCount();
	}

	/*! Удаляет источник данных. Модель удаляется, если явно передать deleteModel == true.
	По умолчанию deleteModel == false. */
	void QstAbstractModelHandler::clear(const bool &deleteModel)
	{
		if (deleteModel)
		{
			Q_ASSERT(_modelDescriptor.pModel() != NULL);
			delete _modelDescriptor.pModel();
		}
		_modelDescriptor = QstModelDescriptor();
	}


	/*! Возвращает индекс сервисного поля.

		Данные берутся из модели, которая связана с источником данных sourceName.

		В случае ошибки возвращает -1. */
	int QstAbstractModelHandler::serviceFieldIndex(const FieldRole &role) const
	{
		Q_ASSERT(role != RoleNone);

		return _modelDescriptor.queryDescriptor().batch().columnIndex(role);
	}

	/*! Возвращает значение ключевого поля той строчки, на которую
	указывает index.

		Данные берутся из модели, которая связана с источником данных sourceName.

		Если ключевое поле не найдено, возвращает QVariant(). */
	QVariant  QstAbstractModelHandler::keyValueOfRow(const QModelIndex &index) const
	{
		if (!index.isValid())
			return QVariant();

		const QAbstractItemModel *model = _modelDescriptor.pModel();
		const int	keyFieldIndex       = serviceFieldIndex(RolePrimaryKey);

		QModelIndex resIndex = model->index(index.row(), keyFieldIndex, index.parent());

		if (!resIndex.isValid())
			return QVariant();

		return resIndex.data();
	}

	/*! Возвращает значение ключевого поля в строке row.

		Данные берутся из модели, которая связана с источником данных sourceName.

		Если ключевое поле не найдено, возвращает QVariant(). */
	QVariant QstAbstractModelHandler::keyValueOfRow(const int &row) const
	{
		const QAbstractItemModel *model = _modelDescriptor.pModel();
		const int keyFieldIndex         = serviceFieldIndex(RolePrimaryKey);

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
	QVariant QstAbstractModelHandler::keyValueOfCurrent(QAbstractItemView * view) const
	{
		Q_ASSERT(view != NULL);

		return keyValueOfRow(view->currentIndex());
	}

	/*! Возвращает значение ключевого поля той строчки, которая
	  является текущей в переданном comboBox.

	  Данные берутся из модели, которая связана с источником данных sourceName.

	  Если ключевое поле не найдено, или текущий comboBox.currentIndex() невалиден,
	  возвращает QVariant(). */
	QVariant QstAbstractModelHandler::keyValueOfCurrent(QComboBox * comboBox) const
	{
		Q_ASSERT(comboBox != NULL);

		return keyValueOfRow(comboBox->currentIndex());
	}

	/*! Возвращает значение ключевого поля той строчки, которая
	  является текущей во view.

	  View берется из источника данных sourceName. Если это единственный view,
	  то параметр viewIndex можно не указывать. Если же в источнике данных
	  находятся несколько view, то необходимо указать нужный индекс.

	  Данные берутся из модели, которая связана с источником данных sourceName.

	  Если ключевое поле не найдено, или текущий индекс во view невалиден,
	  возвращает QVariant(). */
	QVariant QstAbstractModelHandler::keyValueOfView(const int &viewIndex) const
	{
		QAbstractItemView * view;
		view = _modelDescriptor.view(viewIndex);

		Q_ASSERT(view != NULL);

		return keyValueOfCurrent(view);
	}

	/*! Возвращает значение ключевого поля той строчки, которая
	  является текущей в comboBox.

	  ComboBox берется из источника данных sourceName. Если это единственный comboBox,
	  то параметр comboBoxIndex можно не указывать. Если же в источнике данных
	  находятся несколько comboBox, то необходимо указать нужный индекс.

	  Данные берутся из модели, которая связана с источником данных sourceName.

	  Если ключевое поле не найдено, или текущий индекс в comboBox невалиден,
	  возвращает QVariant(). */
	QVariant QstAbstractModelHandler::keyValueOfComboBox(const int &comboBoxIndex) const
	{
		QComboBox * comboBox;
		comboBox = _modelDescriptor.comboBox(comboBoxIndex);

		Q_ASSERT(comboBox != NULL);

		return keyValueOfCurrent(comboBox);
	}

	QVariantList  QstAbstractModelHandler::selectedKeysOfView(const FieldRole &role,
															  const int &viewIndex) const
	{
		QAbstractItemView *		view  = _modelDescriptor.view(viewIndex);

		QModelIndexList indexesList = view->selectionModel()->selectedIndexes();

		int keysColumnIndex = serviceFieldIndex(role);

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
	void QstAbstractModelHandler::setValue(const QString &valueName,
										   const QstValue &value)
	{
		Q_ASSERT(!valueName.isEmpty());

		_values[valueName] = value;
	}

	/*! Возвращает значение по имени valueName.

		Если значение отсутствует, возвращает defaultValue.

		По умолчанию defaultValue == QstValue().

		При неопределенном INVALID_SQLVALUE_BY_DEFAULT, defaultValue меняется
		на QstValue(nt_null). */
	QstValue QstAbstractModelHandler::value(const QString &valueName,
											const QstValue &defaultValue) const
	{
		//	Прошлая конструкция:
		//	return _values.value(valueName, defaultValue);
		//	Недостаток: если значение есть, но инвалидное, а defaultValue - не
		//	инвалидное, то вернется инвалидное вместо defaultValue.

		QstValue val = _values.value(valueName, QstValue());
		if (!val.isValid())
			return defaultValue;
		return val;
	}

	/*! Возвращает значение по имени valueName.

		Если значение отсутствует, возвращает QstValue(nt_null).

		Требует вторым параметром nt_null, что позволяет записать
		[code]
		value("val", QstValue(nt_null))
		[/code]
		короче:
		[code]
		value("val", nt_null)
		[/code]
	*/
	QstValue QstAbstractModelHandler::value(const QString &valueName,
											const NullType &nullType) const
	{
		//	Прошлая конструкция:
		//	return _values.value(valueName, defaultValue);
		//	Недостаток: если значение есть, но инвалидное, а defaultValue - не
		//	инвалидное, то вернется инвалидное вместо defaultValue.

		QstValue val = _values.value(valueName, QstValue());
		if (!val.isValid())
			return QstValue(nullType);
		return val;
	}


	/*! Очищает список значений. */
	void QstAbstractModelHandler::clearValues()
	{
		_values.clear();
	}


	/*! Возвращает данные поля fieldName в строке row.

		Чтобы получить данные, один раз обращается к БД.
		Для этого создает DFD-описатель с помощью _selector(mt_plain, queryNumber),
		генерирует SELECT-запрос, выполняет его и создает временную QstPlainQueryModel.
		В модели и содержатся необходимые данные.

		Возвращает QVariant(), если ячейка не найдена, или данные невалидны. */
	QVariant QstAbstractModelHandler::SelectToValue(const QString fieldName,
													const int &queryNumber,
													const bool &extractItemName,
													const int &row,
													const QSqlDatabase &db)
	{
		QstBatch batch = this->_selector(ModelPlain, queryNumber);
		QstQueryDescriptor	descriptor(batch, QuerySelect, queryNumber);

		int column = descriptor.batch().columnIndex(fieldName, extractItemName);

		return _selectToValue(row, column, descriptor, db);
	}

	/*! Возвращает данные полей, чьи заголовки перечислены в списке fieldNamesList.

		Чтобы получить данные всех полей, один раз обращается к БД.

		Принцип тот же, что и у функции SelectToValue(QString, int, int, QSqlDatabase),
	только для множества полей. */
	QVariantMap QstAbstractModelHandler::SelectToMap(const QStringList &fieldNamesList,
													 const int &queryNumber,
													 const bool &extractItemName,
													 const int &row,
													 const QSqlDatabase &db)
	{
		QstBatch batch = this->_selector(ModelPlain, queryNumber);
		QstQueryDescriptor	descriptor(batch, QuerySelect, queryNumber);

		return _selectToMap(fieldNamesList, descriptor, extractItemName, row, db);
	}

	/*! Генерирует INSERT-запрос, описанный в _inserter(), и выполняет его. */
	bool QstAbstractModelHandler::Insert(const int &queryNumber,
										 const QSqlDatabase &db)
	{
		Q_ASSERT(db.isOpen());

		QstBatch batch = this->_inserter(queryNumber);
		QstQueryDescriptor	descriptor(batch, QueryInsert, queryNumber);

		QSqlQuery			query(db);

		QstQueryGenerator gen(descriptor.batch(), descriptor.queryType());
#ifdef QT_DEBUG
		qDebug() << "For the Insert function,";
		qDebug() << "query of type" << descriptor.queryType() << "and number" << queryNumber << "will be:";
		qDebug() << gen.query();
#endif

		if (!query.prepare(gen.query()))
			return false;

		return query.exec();
	}

	/*! Генерирует UPDATE-запрос, описанный в _updater(), и выполняет его. */
	bool QstAbstractModelHandler::Update(const int &queryNumber,
										 const QSqlDatabase &db)
	{
		Q_ASSERT(db.isOpen());

		QstBatch batch = this->_updater(queryNumber);
		QstQueryDescriptor	descriptor(batch, QueryUpdate, queryNumber);

		Q_ASSERT(descriptor.queryType() == QueryUpdate);

		QSqlQuery			query(db);
		QstQueryGenerator gen(descriptor.batch(), descriptor.queryType());

#ifdef QT_DEBUG
		qDebug() << "For the Update function,";
		qDebug() << "query of type" << descriptor.queryType() << "and number" << queryNumber << "will be:";
		qDebug() << gen.query();
#endif

		if (!query.prepare(gen.query()))
			return false;

		return query.exec();
	}

	/*! Генерирует DELETE-запрос, описанный в _deleter(), и выполняет его. */
	bool QstAbstractModelHandler::Delete(const int &queryNumber,
										 const QSqlDatabase &db)
	{
		Q_ASSERT(db.isOpen());

		QstBatch batch = this->_deleter(queryNumber);
		QstQueryDescriptor	descriptor(batch, QueryDelete, queryNumber);

		Q_ASSERT(descriptor.queryType() == QueryDelete);

		QSqlQuery			query(db);

		QstQueryGenerator gen(descriptor.batch(), descriptor.queryType());
#ifdef QT_DEBUG
		qDebug() << "For the Delete function,";
		qDebug() << "query of type" << descriptor.queryType() << "and number" << queryNumber << "will be:";
		qDebug() << gen.query();
#endif

		if (!query.prepare(gen.query()))
			return false;

		return query.exec();
	}

	/*! Выполняет хранимую процедуру, описанную функцией _executor(). */
	bool QstAbstractModelHandler::Exec(const int &queryNumber,
									   const QSqlDatabase &db)
	{
		Q_ASSERT(db.isOpen());

		QstBatch batch = this->_executor(queryNumber);
		QstQueryDescriptor	descriptor(batch, QueryExecute, queryNumber);

		Q_ASSERT(descriptor.queryType() == QueryExecute);

		QSqlQuery	query(db);

		QstQueryGenerator gen(descriptor.batch(), descriptor.queryType());
#ifdef QT_DEBUG
		qDebug() << "For the Exec function,";
		qDebug() << "query of type" << descriptor.queryType() << "and number" << queryNumber << "will be:";
		qDebug() << gen.query();
#endif

		if (!query.prepare(gen.query()))
			return false;

		return query.exec();
	}

	/*! Удаляет текущую во view строку из БД, обновляет содержимое модели.

	  Функция предполагает, что _deleter() содержит следующий фильтр:
	  \code
	  batch << QstField(<Ключевое_поле_в_БД>, value(ID_VALUE, QstValue(nt_null)), fp_where);
	  \endcode
	*/
	bool QstAbstractModelHandler::DeleteCurrent(QAbstractItemView * view,
												const QSqlDatabase &db)
	{
		QVariant id = keyValueOfCurrent(view);
		bool res = false;

		if (id.isValid())
		{
			QstValue oldId = value(ID_VALUE);
			setValue(ID_VALUE, id);
			res = Delete();
			setValue(ID_VALUE, oldId);
		}
		return res;
	}

	/*! Удаляет текущую в comboBox строку из БД, обновляет содержимое модели.

	  Функция предполагает, что _deleter() содержит следующий фильтр:
	  \code
	  batch << QstField(<Ключевое_поле_в_БД>, value(ID_VALUE, QstValue(nt_null)), fp_where);
	  \endcode
	*/
	bool QstAbstractModelHandler::DeleteCurrent(QComboBox * comboBox,
												const QSqlDatabase &db)
	{
		QVariant id = keyValueOfCurrent(comboBox);
		bool res = false;

		if (id.isValid())
		{
			QstValue oldId = value(ID_VALUE);
			setValue(ID_VALUE, id);
			res = Delete();
			setValue(ID_VALUE, oldId);
		}
		return res;
	}

	/*! Генерирует и возвращает SELECT-запрос. */
	QString QstAbstractModelHandler::SelectQuery(const int &queryNumber,
												 const ModelType &type) const
	{
		QstBatch batch = this->_selector(type, queryNumber);
		QstQueryDescriptor	descriptor(batch, QuerySelect, queryNumber);
		QstQueryGenerator gen(descriptor.batch(), descriptor.queryType());
		return gen.query();
	}

	/*! Генерирует и возвращает INSERT-запрос. */
	QString QstAbstractModelHandler::InsertQuery(const int &queryNumber) const
	{
		QstBatch batch = this->_inserter(queryNumber);
		QstQueryDescriptor	descriptor(batch, QueryInsert, queryNumber);
		QstQueryGenerator gen(descriptor.batch(), descriptor.queryType());
		return gen.query();
	}

	/*! Генерирует и возвращает UPDATE-запрос. */
	QString QstAbstractModelHandler::UpdateQuery(const int &queryNumber) const
	{
		QstBatch batch = this->_updater(queryNumber);
		QstQueryDescriptor	descriptor(batch, QueryUpdate, queryNumber);
		QstQueryGenerator gen(descriptor.batch(), descriptor.queryType());
		return gen.query();
	}

	/*! Генерирует и возвращает DELETE-запрос. */
	QString QstAbstractModelHandler::DeleteQuery(const int &queryNumber) const
	{
		QstBatch batch = this->_deleter(queryNumber);
		QstQueryDescriptor	descriptor(batch, QueryDelete, queryNumber);
		QstQueryGenerator gen(descriptor.batch(), descriptor.queryType());
		return gen.query();
	}

	/*! Генерирует и возвращает EXEC-запрос. */
	QString QstAbstractModelHandler::ExecQuery(const int &queryNumber) const
	{
		QstBatch batch = this->_executor(queryNumber);
		QstQueryDescriptor	descriptor(batch, QueryExecute, queryNumber);
		QstQueryGenerator gen(descriptor.batch(), descriptor.queryType());
		return gen.query();
	}

	/*! Возвращает указатель на сохраненную модель.

		Если источника sorceName не найдено, или указатель на модель равен NULL,
		выпадает в Q_ASSERT(). */
	QAbstractItemModel *QstAbstractModelHandler::pModel()
	{
		return _modelDescriptor.pModel();
	}

	/*! Возвращает константный указатель на сохраненную модель.

		Если источника sorceName не найдено, или указатель на модель равен NULL,
		выпадает в Q_ASSERT(). */
	QAbstractItemModel *QstAbstractModelHandler::pModel() const
	{
		return _modelDescriptor.pModel();
	}

	/*! Возвращает дескриптор модели, содержащейся в источнике данных sourceName.

		Если источника sorceName не найдено, выпадает в Q_ASSERT().

	\sa QstModelDescriptor */
	QstModelDescriptor QstAbstractModelHandler::modelDescriptor() const
	{
		return _modelDescriptor;
	}

	// ------------------------------------------------------------------------- //

	/*! Возвращает описатель для запроса SELECT-типа по виду модели и номеру запроса.

		Функция переопределяется в наследующем классе с нужным функционалом.

		Неявно вызывается при создании и перезагрузке источника данных (модель, view/comboBox),
		а так же из функций SelectToValue(), SelectToMap(), keyValueOfRow(),
		keyValueOfCurrent(), keyValueOfView(), keyValueOfComboBox(), setValueFromView(),
		setValueFromComboBox().

	\sa QstQueryDescriptor, QstField, QstValue, DFDL */
	QstBatch QstAbstractModelHandler::_selector(const ModelType &modelType,
												const int &queryNumber) const
	{
		Q_ASSERT(false);
		return QstBatch();
	}

	/*! Возвращает описатель для запроса INSERT-типа по номеру запроса.

		Функция переопределяется в наследующем классе с нужным функционалом.

		Неявно вызывается из функции Insert().

	\sa QstQueryDescriptor, QstField, QstValue, DFDL */
	QstBatch  QstAbstractModelHandler::_inserter(const int &queryNumber) const
	{
		Q_ASSERT(false);
		return QstBatch();
	}

	/*! Возвращает описатель для запроса UPDATE-типа по номеру запроса.

		Функция переопределяется в наследующем классе с нужным функционалом.

		Неявно вызывается из функции Update().

	\sa QstQueryDescriptor, QstField, QstValue, DFDL */
	QstBatch QstAbstractModelHandler::_updater(const int &queryNumber) const
	{
		Q_ASSERT(false);
		return QstBatch();
	}

	/*! Возвращает описатель для запроса DELETE-типа по номеру запроса.

		Функция переопределяется в наследующем классе с нужным функционалом.

		Неявно вызывается из функций Delete(), DeleteCurrent().

	\sa QstQueryDescriptor, QstField, QstValue, DFDL */
	QstBatch QstAbstractModelHandler::_deleter(const int &queryNumber) const
	{
		Q_ASSERT(false);
		return QstBatch();
	}

	/*! Возвращает описатель для запроса EXEC-типа по номеру запроса.

		Функция переопределяется в наследующем классе с нужным функционалом.

		Неявно вызывается из функции Exec().

	\sa QstQueryDescriptor, QstField, QstValue, DFDL */
	QstBatch QstAbstractModelHandler::_executor(const int &queryNumber) const
	{
		Q_ASSERT(false);
		return QstBatch();
	}


	void QstAbstractModelHandler::_loadModelDescriptor(QAbstractItemModel * model,
													   const ModelType &type,
													   const int &queryNumber,
													   const bool &useQueryNumber,
													   const QSqlDatabase &db)
	{
		Q_ASSERT(model != NULL);
		Q_ASSERT(db.isOpen());

		QstBatch batch;
		if (useQueryNumber)
			batch = this->_selector(type, queryNumber);
		else
			batch = this->_selector(type);

		QstQueryDescriptor queryDescriptor(batch, QuerySelect, queryNumber);

		_modelDescriptor = QstModelDescriptor(model, type, queryDescriptor);

		if (type == ModelPlain)
			_loadModel(queryDescriptor, (QstPlainQueryModel*)model, db);
		else
			_loadModel(queryDescriptor, (QstTreeQueryModel*)model, db);
	}


	void QstAbstractModelHandler::_reload(QAbstractItemModel * model,
										  const ModelType &type,
										  const int &queryNumber,
										  const bool &reloadQueryNumber,
										  const QSqlDatabase &db)
	{
		Q_ASSERT(db.isOpen());

		if (!_modelDescriptor.isValid())
		{
			_loadModelDescriptor(model, type, queryNumber, reloadQueryNumber, db);
		}
		else
		{
			_modelDescriptor.demodelizeViews();

			int newQueryNumber;

			if (reloadQueryNumber)	newQueryNumber = queryNumber;
			else					newQueryNumber = _modelDescriptor.queryDescriptor().queryNumber();

			QstBatch batch = this->_selector(type, newQueryNumber);
			QstQueryDescriptor newQueryDescriptor(batch, QuerySelect, newQueryNumber);

			_modelDescriptor.setQueryDescriptor(newQueryDescriptor);

			if (!model)
			{
				if (type == ModelPlain)
					_loadModel(newQueryDescriptor, (QstPlainQueryModel*)pModel(), db);
				else
					_loadModel(newQueryDescriptor, (QstTreeQueryModel*)pModel(), db);
			}
			else
			{
				if (type == ModelPlain)
					_loadModel(newQueryDescriptor, (QstPlainQueryModel*)model, db);
				else
					_loadModel(newQueryDescriptor, (QstTreeQueryModel*)model, db);

				_modelDescriptor.setModel(model);
			}

			_modelDescriptor.setModelType(type);
			_modelDescriptor.modelizeViews();
		}
	}



	/*! Генерирует запрос, передает его в плоскую модель. */
	void  QstAbstractModelHandler::_loadModel(const QstQueryDescriptor &descriptor,
											  QstPlainQueryModel * model,
											  const QSqlDatabase &db)
	{
		QstQueryGenerator gen(descriptor.batch(), descriptor.queryType());
#ifdef QT_DEBUG
		qDebug() << "For the loadModel function,";
		qDebug() << "query of type" << descriptor.queryType() << "and number" << descriptor.queryNumber() << "will be:";
		qDebug() << gen.query();
#endif

		model->setQuery(gen.query(), db);
	}


	/*! Генерирует запрос, устанавливает индексы ключевых полей в иерархической
	модели и передает в нее запрос. */
	void  QstAbstractModelHandler::_loadModel(const QstQueryDescriptor &descriptor,
											  QstTreeQueryModel * model,
											  const QSqlDatabase &db)
	{
		QstBatch batch = descriptor.batch();

		model->setKeyField(batch.columnIndex(RolePrimaryKey));

		int parentFieldIndex = batch.columnIndex(RoleParentKey);
		if (parentFieldIndex != -1)
			model->setParentField(parentFieldIndex);

		QstQueryGenerator gen(descriptor.batch(), descriptor.queryType());
#ifdef QT_DEBUG
		qDebug() << "For the loadModel function,";
		qDebug() << "query of type" << descriptor.queryType() << "and number" << descriptor.queryNumber() << "will be:";
		qDebug() << gen.query();
#endif

		model->setQuery(gen.query(), db);
	}


	template<class T> void QstAbstractModelHandler::_setViewSettings(T * view, const QstQueryDescriptor &descriptor)
	{
		QstBatch batch = descriptor.batch();
		QstFieldsVector fields = batch.fields();

		int columnIndex;

		for (int i = 0; i < fields.size(); ++i)
		{
			const QstField &rField = fields[i];

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

	void QstAbstractModelHandler::_setListViewSettings(QListView * view, const QstQueryDescriptor &descriptor)
	{
		QstBatch batch = descriptor.batch();
		QstFieldsVector fields = batch.fields();

		int columnIndex;

		for (int i = 0; i < fields.size(); ++i)
		{
			const QstField &rField = fields[i];

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



	QVariant QstAbstractModelHandler::_selectToValue(const int &row,
													 const int &column,
													 const QstQueryDescriptor &descriptor,
													 const QSqlDatabase &db)
	{
		Q_ASSERT(db.isOpen());
		Q_ASSERT(descriptor.queryType() == QuerySelect);

		QstPlainQueryModel queryModel;

		QstQueryGenerator gen(descriptor.batch(), descriptor.queryType());
#ifdef QT_DEBUG
		qDebug() << "For the SelectToValue function,";
		qDebug() << "query of type" << descriptor.queryType() << "and number" << descriptor.queryNumber() << "will be:";
		qDebug() << gen.query();
#endif

		queryModel.setQuery(gen.query(), db);

		QModelIndex index = queryModel.index(row, column, QModelIndex());

		if (index.isValid())
			return index.data(Qt::DisplayRole);

		return QVariant();
	}

	QVariantMap QstAbstractModelHandler::_selectToMap(const QStringList &fieldNamesList,
													  const QstQueryDescriptor &descriptor,
													  const bool &extractItemName,
													  const int &row,
													  const QSqlDatabase &db)
	{
		Q_ASSERT(db.isOpen());
		Q_ASSERT(descriptor.queryType() == QuerySelect);

		QstQueryGenerator gen(descriptor.batch(), descriptor.queryType());
#ifdef QT_DEBUG
		qDebug() << "For the SelectToMap function,";
		qDebug() << "query of type" << descriptor.queryType() << "and number" << descriptor.queryNumber() << "will be:";
		qDebug() << gen.query();
#endif

		QstPlainQueryModel	queryModel;
		queryModel.setQuery(gen.query(), db);

		QstBatch batch = descriptor.batch();

		QVariantMap		resMap;
		int				column;
		QModelIndex		index;

		for (int i = 0; i < fieldNamesList.size(); ++i)
		{
			column	= batch.columnIndex(fieldNamesList[i], extractItemName);
			index	= queryModel.index(row, column, QModelIndex());

			if (index.isValid())
				resMap[fieldNamesList[i]] = index.data(Qt::DisplayRole);
		}

		return resMap;
	}
}	// End of namespace Qst
