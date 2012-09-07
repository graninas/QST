#ifndef TESTHANDLER_H
#define TESTHANDLER_H

#include "qst/qstabstractmodelhandler.h"

namespace QstTest
{

const int TEST1 = 1;

class TestHandler : public Qst::QstAbstractModelHandler
{
public:
    TestHandler();

private:

	Qst::QstBatch	_selector(const int &queryNumber) const;
};

} // End of namespace QstTest

#endif // TESTHANDLER_H
