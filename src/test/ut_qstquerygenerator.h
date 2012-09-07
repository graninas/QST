/****************************************************************************
** QST 0.4.2a release
** Copyright (C) 2010 Granin A.S.
** Contact: Granin A.S. (graninas@gmail.com)
**
** This file is part of the Test module of the QsT SQL Tools.
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

#ifndef UT_QSTQUERYGENERATOR_H
#define UT_QSTQUERYGENERATOR_H

#include <QObject>
#include <QTest>

#include "qst/qstquerygenerator.h"

namespace QstTest
{

class ut_QstQueryGenerator : public QObject
{
	Q_OBJECT
public:
	ut_QstQueryGenerator();

private slots:

	void initTestCase();

	void query();
	void query_data();

	void queryPartsClauseSelect();
	void queryPartsClauseSelect_data();

	void queryPartsClauseFrom();
	void queryPartsClauseFrom_data();

	void queryPartsClauseWhere();
	void queryPartsClauseWhere_data();

	void queryPartsClauseOrderBy();
	void queryPartsClauseOrderBy_data();

	void batch();
	void batch_data();

	void queryType();
	void queryType_data();

	void isValid();
	void isValid_data();

private:

	Qst::QstBatch m_b1, m_b2, m_b3, m_b4, m_b5, m_b6;

	Qst::QstBatch m_b7, m_b8, m_b9;
};

} // End of namespace QstTest


#endif // UT_QSTQUERYGENERATOR_H
