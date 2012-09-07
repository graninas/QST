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

#ifndef QSTBATCH_H
#define QSTBATCH_H

#include "qstfield.h"

#include <QStringList>
#include <QList>
#include <QMap>

#include "qstdefaultitemnameextractor.h"
#include "qstrolevisibility.h"
#include "qstif.h"

namespace Qst
{


class QstBatch;
typedef QMap<QString, QstBatch> QstBatchMap;

typedef QstIf<QstField> QstFieldIf;
typedef QstIf<QString>	QstSourceIf;

class QstBatch
{
public:

	typedef enum {SourcesNameList, SourcesBatch} SourcesType;

private:

	QStringList			_sources;
	QString				_name;
	QstFieldVector		_fields;

	QstAbstractItemNameExtractor *	_itemExtractor;
	QstDefaultItemNameExtractor		_defaultItemExtractor;

public:
	QstBatch(const QString name = QString());
	QstBatch(QstAbstractItemNameExtractor * itemExtractor);

	void addSource(const QString &source);
	void addSource(const QstSourceIf &sourceIf);

#ifdef QST_ALWAYS_EXTRACT_ITEM_NAME
	void addField(const QstField &field, const bool &extractItemName = true);
	void addField(const QstFieldIf &fieldIf, const bool &extractItemName = true);
#else
	void addField(const QstField &field, const bool &extractItemName = false);
	void addField(const QstFieldIf &fieldIf, const bool &extractItemName = false);
#endif

	QStringList sources() const;

	QstFieldVector fields(const FieldPurposes &purposes = PurposeAll_Mask) const;

	void clear();
	void clearSources();

	bool isValid() const;

#ifdef QST_ALWAYS_EXTRACT_ITEM_NAME
	int columnIndex(const QString &fieldName, const bool &extractItemName = true) const;
#else
	int columnIndex(const QString &fieldName, const bool &extractItemName = false) const;
#endif
	int columnIndex(const FieldRole &role) const;

	QstBatch & operator<<(const QString &source);
	QstBatch & operator<<(const QstSourceIf &sourceIf);

	QstBatch & operator<<(const QstField &field);
	QstBatch & operator<<(const QstFieldIf &fieldIf);


	friend bool operator == (const QstBatch &val1, const QstBatch &val2)
	{
		return val1.sources() == val2.sources()
				&& val1.fields() == val2.fields();

	}

private:

	// Данная система будет пересмотрена.
	QMap<FieldVisibility, QStringList>			_fieldNamesLists;
	QMap<QstSpecial::QstRoleVisibility, int>	_serviceFieldsIndexes;
	QMap<FieldRole, FieldVisibility>			_rolesVisibility;

	QString _extractItemName(const QString &fullFieldName) const;

	void _fillRoleVisMap(const QstField &field, const FieldVisibility &visibility);
};



}

#endif // QSTBATCH_H


