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

#include "qstabstractmodelhandler.h"

#include <QObject>
#include <QDebug>

namespace Qst
{

using namespace QstSpecial;

	/*!
		\class QstAbstractModelHandler
		\brief
		Класс является основным инструментом для генерации запросов, работы
		с моделями, представлениями и ключевыми полями.

		\inmodule Qst

		QstAbstractModelHandler - это абстрактный класс, в котором
		заключены функции по выбору определенного значения
		из представления / модели данных, функции настройки модели и представления,
		функции, отвечающие за генерацию и вызов SQL-запросов.

		Класс является абстрактным. Для работы с определенным объектом
		БД создается наследник, у которого переопределяются виртуальные
		функции _selector(), _inserter(), _deleter(), _updater()
		и _executor(), отвечающие за генерацию конкретного DFD-описателя.
		(DFD = Declarative Field Descriptor). Фактически, класс-наследник
		задает интерфейс для работы с конкретным объектом БД:
		с таблицей, представлением, хранимой процедурой или пользовательской функцией.

		Пример.
		Класса-хэндлер для работы с номенклатурой. Переопределены функции _selector()
		(для генерирования SELECT-запросов) и _executor() (для вызова хранимой процедуры).
		Так же создана функция createNomenclature(), при вызове которой происходит
		(каким-либо образом) создание в базе данных одной номенклатурной записи.

\code
const int NOMENCLATURE = 1;
const int NOMENCLATURE_COUNT = 2;

class NomenclatureHandler : public QstAbstractModelHandler
{
public:
	NomenclatureHandler();

	void createNomenclature(const QVariant &nomenclatureName, const QVariant &nomenclatureGroupID);

private:

	Qst::QstBatch _selector(const int &queryNumber) const;
	Qst::QstBatch _executor(const int &queryNumber) const;
};
\endcode

		Функции _selector(), _inserter(), _deleter(), _updater()
		и _executor() вызываются всякий раз, когда необходимо
		выполнить запрос, обновить модель данных, выбрать
		значение определенного поля и т.п. Например, при вызове функции Delete(),
		описанной в QstAbstractModelHandler, с помощью функции _deleter()
		создается DFD-описатель, затем по описателю компонуется SQL-запрос
		типа DELETE и выполняется. Разумеется, функция _deleter() должна быть
		переопределена в дочернем классе, иначе возникнет ошибка.

		В переопределенных генерирующих функциях заполняется структура QstBatch,
		представляющая собой контейнер DFD-описателей. Можно заполнять QstBatch
		по-разному, создавая таким образом разные SQL-запросы к одному и тому же
		объекту БД.

		Пример. С помощью нижеописанного _selector() можно создать два запроса
		(запросы в после генерации могут иметь несколько иной вид в зависимости
		от разных факторов):

		I.
\code
SELECT ID, Name, NomenclatureGroupName, NomenclatureGroupID
FROM vNomenclature
WHERE Name LIKE '%чай%'
\endcode

		II.
\code
SELECT count(ID)
FROM vNomenclature
\endcode

\code
QstBatch NomenclatureHandler::_selector(const int &queryNumber) const
{
	QstBatch batch;

	if (queryNumber == NOMENCLATURE)	// Запрос I.
	{
		batch	<< "vNomenclature"

				// Поля для секции SELECT.
				<< QstField(RolePrimaryKey, "ID", FieldInvisible)	// Ключевое поле.
				<< QstField("Name", FieldVisible, "Наименование", 150)
				<< QstField("NomenclatureGroupName", FieldVisible, "Группа", 100)
				<< QstField("NomenclatureGroupID", FieldInvisible)

				// Фильтры для секции WHERE. Могут отсутствовать, если
				// value() возвратит инвалидный QstValue().
				<< QstField("Name", value("Name"))
				<< QstField("NomenclatureGroupID", value("NomenclatureGroupID"))
				;
	}
	else
		if (queryNumber == NOMENCLATURE_COUNT) // Запрос II.
		{
			batch	<< "vNomenclature"
					<< QstField(RolePrimaryKey, "count(ID)", FieldInvisible)
					;
		}
	else
	{
		Q_ASSERT(false);	// Ошибка, если передан неверный queryNumber.
	}

	return batch;
}
\endcode

		После того, как класс-хэндлер заполнен, его можно использовать.
		Пусть требуется выбрать данные в таблицу QTableView с помощью этого хэндлера.
		Создаются объект класса-хэндлера и модель данных:
\code
NomenclatureHandler			nomenclatureHandler;
Qst::QstPlainQueryModel		nomenclatureModel;
\endcode

		Далее в классе-хэндлере задается нужный описатель, привязываются модель
		и объект QTableView:
\code
nomenclatureHandler.reload(NOMENCLATURE,
						   &nomenclatureModel);
nomenclatureHandler.setTableView(ui->NomenclatureTable);
\endcode

		Естественно, данные будут отображаться только в течение жизни объектов
		nomenclatureHandler и nomenclatureModel.

		\sa reload()

		С помощью настроенного класса-хэндлера можно выбрать ключевое значение
		строки, которая выделена в представлении:
\code
QVariant selectedNomenclatureRowID = nomenclatureHandler.keyValueOfView();
\endcode

		Либо выбрать ключевые поля для нескольких выделенных строк:
\code
QVariantList selectedNomenclatureRowIDs = nomenclatureHandler.selectedKeysOfView();
\endcode

		\sa keyValueOfView(), selectedKeysOfView()

		Точно так же можно привязать к модели данных другие представления, например,
		выпадающий список:
\code
nomenclatureHandler.reload(NOMENCLATURE,
						   &nomenclatureModel);
nomenclatureHandler.setTableView(ui->NomenclatureTable);

nomenclatureHandler.setTableView(ui->NomenclatureTable2);
nomenclatureHandler.setComboBox(ui->NomenclatureComboBox);
\endcode

		Если установлено несколько представлений, то нужно указывать, из какого
		представления выбирать ключевые поля. Для этого в функциях keyValueOfView(),
		selectedKeysOfView(), keyValueOfComboBox() предусмотрен параметр-индекс
		нужного представления (возвращается функциями setTableView(), setComboBox() и др.)

		\attention
		Объекты классов QTreeView, QListView и QTableView добавляются
		в один список, а объекты класса QComboBox - в другой, поэтому индексы
		для первых трех классов идут вместе, а индексы для класса
		QComboBox считаются отдельно.

		\sa SelectToValue(), SelectToMap(), value(), setValue(), QstField, QstValue
		*/

	/*! Конструктор по умолчанию. */
	QstAbstractModelHandler::QstAbstractModelHandler()
		:
		_loaded(false)
	{
	}

	/*! Чисто виртуальный деструктор. */
	QstAbstractModelHandler::~QstAbstractModelHandler()
	{
	}

	////////////////////////////////////////////////////////////////////////////////

	/*! Перезагружает плоскую модель данных. */
	void QstAbstractModelHandler::reload(const int &queryNumber,
										 QstPlainQueryModel * model,
										 const QSqlDatabase &db)
	{
		_reload(model, ModelPlain, queryNumber, true, db);
	}

	/*! Перезагружает древовидную модель данных.*/
	void QstAbstractModelHandler::reload(const int &queryNumber,
										 QstTreeQueryModel * model,
										 const QSqlDatabase &db)
	{
		_reload(model, ModelTree, queryNumber, true, db);
	}


	/*! Перезагружает загруженную ранее модель данных.

		Если модель не была загружена ранее, выдает ошибку. */
void QstAbstractModelHandler::reload(const QSqlDatabase &db)
{
	Q_ASSERT(_loaded);

	_reload(pModel(),
			_modelDescriptor.modelType(),
			_modelDescriptor.queryDescriptor().queryNumber(),
			false,
			db);
}

/*! Возвращает true, если модель данных была загружена ранее, и false в противном случае. */
bool QstAbstractModelHandler::isLoaded() const
{
	return _loaded;
}

	////////////////////////////////////////////////////////////////////////////////

	/*! Связывает модель данных и объект класса QTableView (представление).
	К одной и той же модели можно привязать несколько представлений.

	Возвращает индекс представления в списке представлений. Так же в этот
	список попадают классы QTreeView и QListView. */
	int QstAbstractModelHandler::setTableView(QTableView * tableView)
	{
		Q_ASSERT(tableView != NULL);

		int index = _modelDescriptor.addView(tableView, true);

		_setViewSettings<QTableView>(tableView, _modelDescriptor.queryDescriptor());

		return index;
	}

	/*! Связывает модель данных и объект класса QTreeView (представление).
	К одной и той же модели можно привязать несколько представлений.

	Возвращает индекс представления в списке представлений. Так же в этот
	список попадают классы QTableView и QListView. */
	int QstAbstractModelHandler::setTreeView(QTreeView * treeView)
	{
		Q_ASSERT(treeView != NULL);

		int index = _modelDescriptor.addView(treeView, true);

		_setViewSettings<QTreeView>(treeView, _modelDescriptor.queryDescriptor());

		return index;
	}

	/*! Связывает модель данных и объект класса QListView (представление).
	К одной и той же модели можно привязать несколько представлений.

	Возвращает индекс представления в списке представлений. Так же в этот
	список попадают классы QTableView и QTreeView. */
	int QstAbstractModelHandler::setListView(QListView * listView)
	{
		Q_ASSERT(listView != NULL);

		int index = _modelDescriptor.addView(listView, true);

		_setListViewSettings(listView, _modelDescriptor.queryDescriptor());

		return index;
	}

	/*! Связывает модель данных и объект класса QComboBox (выпадающий список).
	К одной и той же модели можно привязать несколько выпадающих списков.

	Возвращает индекс привязанного QComboBox.*/
	int QstAbstractModelHandler::setComboBox(QComboBox * comboBox)
	{
		Q_ASSERT(comboBox != NULL);

		int index;
		index = _modelDescriptor.addComboBox(comboBox, true);

		return index;
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
		_loaded = false;
	}


	/*! Возвращает индекс сервисного поля.

		В случае ошибки возвращает -1. */
	int QstAbstractModelHandler::serviceFieldIndex(const FieldRole &role) const
	{
		Q_ASSERT(role != RoleNone);

		return _modelDescriptor.queryDescriptor().batch().columnIndex(role);
	}

	/*! Возвращает значение ключевого поля той строчки, на которую
	указывает index.

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

	/*! Возвращает значение ключевого поля строки row.

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


	/*! Возвращает значение ключевого выделенной строки во view.

	  Если ключевое поле не найдено, или текущий view.currentIndex() невалиден,
	  возвращает QVariant(). */
	QVariant QstAbstractModelHandler::keyValueOfCurrent(QAbstractItemView * view) const
	{
		Q_ASSERT(view != NULL);

		return keyValueOfRow(view->currentIndex());
	}

	/*! Возвращает значение ключевого поля выделенной строки в comboBox.

	  Если ключевое поле не найдено, или comboBox.currentIndex() невалиден,
	  возвращает QVariant(). */
	QVariant QstAbstractModelHandler::keyValueOfCurrent(QComboBox * comboBox) const
	{
		Q_ASSERT(comboBox != NULL);

		return keyValueOfRow(comboBox->currentIndex());
	}

	/*! Возвращает значение ключевого поля строки, выделенной во view.

	Если это единственный view, привязанный к модели данных, то параметр viewIndex
	можно не указывать. Если же в источнике данных находятся несколько view,
	то необходимо указать нужный индекс.

	Если ключевое поле не найдено, или текущий индекс во view невалиден,
	возвращает QVariant(). */
	QVariant QstAbstractModelHandler::keyValueOfView(const int &viewIndex) const
	{
		QAbstractItemView * view;
		view = _modelDescriptor.view(viewIndex);

		Q_ASSERT(view != NULL);

		return keyValueOfCurrent(view);
	}

	/*! Возвращает значение ключевого поля строки, выделенной в comboBox.

	Если это единственный comboBox, привязанный к модели данных, параметр
	comboBoxIndex можно не указывать. Если же в источнике данных находятся
	несколько comboBox, необходимо указать нужный индекс.

	Если ключевое поле не найдено, или текущий индекс в comboBox невалиден,
	возвращает QVariant(). */
	QVariant QstAbstractModelHandler::keyValueOfComboBox(const int &comboBoxIndex) const
	{
		QComboBox * comboBox;
		comboBox = _modelDescriptor.comboBox(comboBoxIndex);

		Q_ASSERT(comboBox != NULL);

		return keyValueOfCurrent(comboBox);
	}

	/*! Возвращает список ключевых значений для строк, выделенных во view.

	Может вернуть значения полей с FieldRole == RolePrimaryKey
	или с FieldRole == RoleParentKey. */
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

	/*! Сохраняет value под именем valueName в массиве значений.

	\sa value(), valuesMap(), setValuesMap()*/
	void QstAbstractModelHandler::setValue(const QString &valueName,
										   const QstValue &value)
	{
		Q_ASSERT(!valueName.isEmpty());

		_valuesMap[valueName] = value;
	}

	/*! Возвращает значение по имени valueName из массива значений.

		Если значение отсутствует, возвращает defaultValue.
	\sa setValue(), valuesMap(), setValuesMap()*/
	QstValue QstAbstractModelHandler::value(const QString &valueName,
											const QstValue &defaultValue) const
	{
		QstValue val = _valuesMap.value(valueName, QstValue());
		if (!val.isValid())
			return defaultValue;
		return val;
	}

	/*! Возвращает значение по имени valueName из массива значений.

		Если значение отсутствует, возвращает QstValue(nt_null).

		Функция позволяет сократить запись:
		\code
		value("val", nt_null)
		\endcode
		против
		\code
		value("val", QstValue(nt_null))
		\endcode

		\sa setValue(), valuesMap(), setValuesMap()
	*/
	QstValue QstAbstractModelHandler::value(const QString &valueName,
											const NullType &nullType) const
	{
		QstValue val = _valuesMap.value(valueName, QstValue());
		if (!val.isValid())
			return QstValue(nullType);
		return val;
	}

	/*! Устанавливает массив значений. Прошлые значения затираются.
	  \sa valuesMap(), value(), setValue()
	  */
	void QstAbstractModelHandler::setValuesMap(const QstValueMap &valuesMap)
	{
		_valuesMap = valuesMap;
	}

	/*! Возвращает массив значений. */
	QstValueMap QstAbstractModelHandler::valuesMap() const
	{
		return _valuesMap;
	}

	/*! Очищает массив значений. */
	void QstAbstractModelHandler::clearValues()
	{
		_valuesMap.clear();
	}

	/*! Возвращает тип текущей загруженной модели. */
	ModelType QstAbstractModelHandler::modelType() const
	{
		return _modelType;
	}

	/*! Возвращает данные поля fieldName в строке row.

	Чтобы получить данные, генерирует запрос с помощью _selector(),
	выполняет его.

	Возвращает QVariant(), если поле с именем fieldName не найдено.

	\sa SelectToMap()*/
	QVariant QstAbstractModelHandler::SelectToValue(const QString fieldName,
													const int &queryNumber,
													const int &row,
													const bool &extractItemName,
													const QSqlDatabase &db)
	{
		QstBatch btch = this->_selector(queryNumber);
		QstQueryDescriptor	descriptor(btch, QuerySelect, queryNumber);

		int column = btch.columnIndex(fieldName, extractItemName);

		return _selectToValue(row, column, descriptor, db);
	}

	/*! Возвращает данные полей, переданных в fieldNamesList.

	Чтобы получить данные, генерирует запрос с помощью _selector(),
	выполняет его.

	\sa SelectToValue()*/
	QVariantMap QstAbstractModelHandler::SelectToMap(const int &queryNumber,
													 const QStringList &fieldNamesList,
													 const int &row,
													 const bool &extractItemName,
													 const QSqlDatabase &db)
	{
		QstBatch btch = this->_selector(queryNumber);
		QstQueryDescriptor	descriptor(btch, QuerySelect, queryNumber);

		return _selectToMap(fieldNamesList, descriptor, extractItemName, row, db);
	}

	/*! Генерирует INSERT-запрос, описанный в _inserter(), выполняет его.

	\sa SelectToValue(), SelectToMap(), Update(), Delete(), Exec()*/
	bool QstAbstractModelHandler::Insert(const int &queryNumber,
										 const QSqlDatabase &db)
	{
		Q_ASSERT(db.isOpen());

		QstBatch btch = this->_inserter(queryNumber);

		QSqlQuery			query(db);

		QstQueryGenerator gen(btch, QueryInsert);
#ifdef QT_DEBUG
		qDebug() << "For the Insert function, queryType == QueryInsert ";
		qDebug() << "and queryNumber == " << queryNumber << " query will be:";
		qDebug() << gen.query();
#endif

		if (!query.prepare(gen.query()))
			return false;

		return query.exec();
	}

	/*! Генерирует UPDATE-запрос, описанный в _updater(), выполняет его.

	\sa SelectToValue(), SelectToMap(), Insert(), Delete(), Exec()*/
	bool QstAbstractModelHandler::Update(const int &queryNumber,
										 const QSqlDatabase &db)
	{
		Q_ASSERT(db.isOpen());

		QstBatch btch = this->_updater(queryNumber);

		QSqlQuery			query(db);
		QstQueryGenerator gen(btch, QueryUpdate);

#ifdef QT_DEBUG
		qDebug() << "For the Update function, queryType == QueryUpdate ";
		qDebug() << "and queryNumber == " << queryNumber << " query will be:";
		qDebug() << gen.query();
#endif

		if (!query.prepare(gen.query()))
			return false;

		return query.exec();
	}

	/*! Генерирует DELETE-запрос, описанный в _deleter(), выполняет его.

	\sa SelectToValue(), SelectToMap(), Update(), Insert(), Exec()*/
	bool QstAbstractModelHandler::Delete(const int &queryNumber,
										 const QSqlDatabase &db)
	{
		Q_ASSERT(db.isOpen());

		QstBatch btch = this->_deleter(queryNumber);

		QSqlQuery			query(db);
		QstQueryGenerator gen(btch, QueryDelete);

#ifdef QT_DEBUG
		qDebug() << "For the Delete function, queryType == QueryDelete ";
		qDebug() << "and queryNumber == " << queryNumber << " query will be:";
		qDebug() << gen.query();
#endif

		if (!query.prepare(gen.query()))
			return false;

		return query.exec();
	}

	/*! Генерирует EXEC-запрос, описанный в _executor(), выполняет его.

	\sa SelectToValue(), SelectToMap(), Update(), Insert(), Delete() */
	bool QstAbstractModelHandler::Exec(const int &queryNumber,
									   const QSqlDatabase &db)
	{
		Q_ASSERT(db.isOpen());

		QstBatch btch = this->_executor(queryNumber);

		QSqlQuery	query(db);
		QstQueryGenerator gen(btch, QueryExecute);

#ifdef QT_DEBUG
		qDebug() << "For the Exec function, queryType == QueryExecute ";
		qDebug() << "and queryNumber == " << queryNumber << " query will be:";
		qDebug() << gen.query();
#endif

		if (!query.prepare(gen.query()))
			return false;

		return query.exec();
	}

	/*! Геренирует и возвращает запрос соответствующего типа с номером queryType. */
	QString QstAbstractModelHandler::generateQuery(const QueryType &queryType,
												   const int &queryNumber)
	{
		QstBatch btch = batch(queryType, queryNumber);
		QstQueryGenerator	gen(btch, queryType);
		return gen.query();
	}

	/*! Возвращает указатель на модель данных. */
	QAbstractItemModel *QstAbstractModelHandler::pModel()
	{
		return _modelDescriptor.pModel();
	}

	/*! Возвращает константный указатель модель данных. */
	QAbstractItemModel *QstAbstractModelHandler::pModel() const
	{
		return _modelDescriptor.pModel();
	}

	/*! Возвращает дескриптор модели.

	\sa QstSpecial::QstModelDescriptor */
	QstModelDescriptor QstAbstractModelHandler::modelDescriptor() const
	{
		return _modelDescriptor;
	}

	/*! Возвращает QstBatch для запроса типа queryType с номером queryNumber. */
	QstBatch QstAbstractModelHandler::batch(const QueryType &queryType,
											const int &queryNumber)
	{
		switch (queryType)
		{
		case QuerySelect:  return this->_selector(queryNumber);
		case QueryInsert:  return this->_inserter(queryNumber);
		case QueryUpdate:  return this->_updater(queryNumber);
		case QueryDelete:  return this->_deleter(queryNumber);
		case QueryExecute: return this->_executor(queryNumber);

		default: Q_ASSERT_X(false, "batch", "Unknown QueryType.");
		};

		return QstBatch();
	}

	// ------------------------------------------------------------------------- //

	/*! Возвращает QstBatch (DFD-описатель) для запроса SELECT-типа.

		Функция, при необходимости, должна быть переопределена в классе-наследнике.

		Вызывается функцией reload(), функциями SelectToValue(), SelectToMap(),
		keyValueOfRow(), keyValueOfCurrent(), keyValueOfView(), keyValueOfComboBox(),
		selectedKeysOfView(), функциями batch(), generateQuery(). */
	QstBatch QstAbstractModelHandler::_selector(const int &queryNumber) const
	{
		Q_ASSERT(false);
		return QstBatch();
	}

	/*! Возвращает QstBatch (DFD-описатель) для запроса INSERT-типа.

		Функция, при необходимости, должна быть переопределена в классе-наследнике.

		Вызывается из функции Insert(), функциями batch(), generateQuery(). */
	QstBatch  QstAbstractModelHandler::_inserter(const int &queryNumber) const
	{
		Q_ASSERT(false);
		return QstBatch();
	}

	/*! Возвращает QstBatch (DFD-описатель) для запроса UPDATE-типа.

		Функция, при необходимости, должна быть переопределена в классе-наследнике.

		Вызывается из функции Update(), функциями batch(), generateQuery().*/
	QstBatch QstAbstractModelHandler::_updater(const int &queryNumber) const
	{
		Q_ASSERT(false);
		return QstBatch();
	}

	/*! Возвращает QstBatch (DFD-описатель) для запроса DELETE-типа.

		Функция, при необходимости, должна быть переопределена в классе-наследнике.

		Вызывается из функции Delete(), функциями batch(), generateQuery().*/
	QstBatch QstAbstractModelHandler::_deleter(const int &queryNumber) const
	{
		Q_ASSERT(false);
		return QstBatch();
	}

	/*! Возвращает QstBatch (DFD-описатель) для запроса EXEC-типа.

		Функция, при необходимости, должна быть переопределена в классе-наследнике.

		Вызывается из функции Exec(), функциями batch(), generateQuery().*/
	QstBatch QstAbstractModelHandler::_executor(const int &queryNumber) const
	{
		Q_ASSERT(false);
		return QstBatch();
	}


	void QstAbstractModelHandler::_loadModelDescriptor(QAbstractItemModel * model,
													   const ModelType &modelType,
													   const int &queryNumber,
													   const QSqlDatabase &db)
	{
		Q_ASSERT(model != NULL);
		Q_ASSERT(db.isOpen());

		_modelType = modelType;

		QstBatch btch = this->_selector(queryNumber);

		QstQueryDescriptor queryDescriptor(btch, QuerySelect, queryNumber);

		_modelDescriptor = QstModelDescriptor(model, modelType, queryDescriptor);

		if (modelType == ModelPlain)
			_loadModel(queryDescriptor, (QstPlainQueryModel*)model, db);
		else
			_loadModel(queryDescriptor, (QstTreeQueryModel*)model, db);
	}


	void QstAbstractModelHandler::_reload(QAbstractItemModel * model,
										  const ModelType &modelType,
										  const int &queryNumber,
										  const bool &reloadQueryNumber,
										  const QSqlDatabase &db)
	{
		Q_ASSERT(db.isOpen());

		if (!_modelDescriptor.isValid())
		{
			_loadModelDescriptor(model, modelType, queryNumber, db);
		}
		else
		{
			_modelDescriptor.demodelizeViews();

			int newQueryNumber;

			if (reloadQueryNumber)	newQueryNumber = queryNumber;
			else					newQueryNumber = _modelDescriptor.queryDescriptor().queryNumber();

			QstBatch btch = this->_selector(newQueryNumber);
			QstQueryDescriptor newQueryDescriptor(btch, QuerySelect, newQueryNumber);

			_modelDescriptor.setQueryDescriptor(newQueryDescriptor);

			if (!model)
			{
				if (modelType == ModelPlain)
					_loadModel(newQueryDescriptor, (QstPlainQueryModel*)pModel(), db);
				else
					_loadModel(newQueryDescriptor, (QstTreeQueryModel*)pModel(), db);
			}
			else
			{
				if (modelType == ModelPlain)
					_loadModel(newQueryDescriptor, (QstPlainQueryModel*)model, db);
				else
					_loadModel(newQueryDescriptor, (QstTreeQueryModel*)model, db);

				_modelDescriptor.setModel(model);
			}

			_modelDescriptor.setModelType(modelType);
			_modelDescriptor.modelizeViews();
		}

	_loaded = true;
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
		QstBatch btch = descriptor.batch();

		model->setKeyField(btch.columnIndex(RolePrimaryKey));

		int parentFieldIndex = btch.columnIndex(RoleParentKey);
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
		QstBatch btch = descriptor.batch();
		QstFieldVector fields = btch.fields();

		int columnIndex;

		for (int i = 0; i < fields.size(); ++i)
		{
			const QstField &rField = fields[i];

			if (rField.isVisible())
			{
				columnIndex = btch.columnIndex(rField.name());
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
				columnIndex = btch.columnIndex(rField.name());
				Q_ASSERT(columnIndex != -1);

				view->setColumnHidden(columnIndex, true);
			}
		};
	}

	void QstAbstractModelHandler::_setListViewSettings(QListView * view, const QstQueryDescriptor &descriptor)
	{
		QstBatch btch = descriptor.batch();
		QstFieldVector fields = btch.fields();

		int columnIndex;

		for (int i = 0; i < fields.size(); ++i)
		{
			const QstField &rField = fields[i];

			if (rField.isVisible())
			{
				columnIndex = btch.columnIndex(rField.name());
				Q_ASSERT(columnIndex != -1);

				view->model()->setHeaderData(columnIndex,
											 rField.orientation(),
											 rField.columnTitle(),
											 Qt::DisplayRole);
			}
			else
				if (rField.isInvisible())
				{
				columnIndex = btch.columnIndex(rField.name());
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

		QSqlError err = queryModel.lastError();
		if (err.isValid())
		{
			qDebug() << err.text();
			return QVariant();
		}
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

		QSqlError err = queryModel.lastError();
		if (err.isValid())
		{
			qDebug() << err.text();
			return QVariantMap();
		}
		QstBatch btch = descriptor.batch();

		QVariantMap		resMap;
		int				column;
		QModelIndex		index;

		for (int i = 0; i < fieldNamesList.size(); ++i)
		{
			column	= btch.columnIndex(fieldNamesList[i], extractItemName);
			index	= queryModel.index(row, column, QModelIndex());

			if (index.isValid())
				resMap[fieldNamesList[i]] = index.data(Qt::DisplayRole);
		}

		return resMap;
	}

}	// End of namespace Qst
