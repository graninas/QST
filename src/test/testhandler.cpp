#include "testhandler.h"

using namespace Qst;

namespace QstTest
{

	/*!
		\class TestHandler
		\brief Класс-хэндлер, использующийся в модульном тесте ut_QstAbstractModelHandler.
	*/

TestHandler::TestHandler()
{
}

QstBatch TestHandler::_selector(const int &queryNumber) const
{
	QstBatch batch;

	if (queryNumber == TEST1)
	{
		batch	<< "table1"
				<< QstField(RolePrimaryKey, "ID")
				<< QstField("Name", FieldVisible, "Наименование", 120)

				<< QstField("ID", value(ID_VALUE))
				<< QstField("OrderDate", value("DateBegin"), value("DateEnd"))
				;
	}
	else
	{
		Q_ASSERT(false);
	}

	return batch;
}


} // End of namespace QstTest
