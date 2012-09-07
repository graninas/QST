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

#include "qstbatch.h"

namespace Qst
{

QstBatch::QstBatch(const QString name)
	:
	_name(name),
	_itemExtractor(NULL)
{
}

QstBatch::QstBatch(QstAbstractItemNameExtractor * itemExtractor)
	:	_itemExtractor(itemExtractor)
{
	Q_CHECK_PTR(_itemExtractor);
}

void QstBatch::addSource(const QString &source)
{
	_sources.append(source);
}

void QstBatch::addField(const QstField &field, const bool &extractItemName)
{
	QstField	resField = field;
	QString		resFieldName = resField.name();

	if (!resField.isValid() && (resField.purposes() & PurposeParameter))
	{
		resField.setValue(QstValue(Null));
	}

	_fields.append(resField);

if (extractItemName)
	resFieldName = _extractItemName(resField.name());

	if (resField.isVisible())
	{
		_fieldNamesLists[FieldVisible].append(resFieldName);

		_fillRoleVisMap(resField, FieldVisible);
	}
	else
		if (resField.isInvisible())
		{
			_fieldNamesLists[FieldInvisible].append(resFieldName);

			_fillRoleVisMap(resField, FieldInvisible);
		}
}

QStringList QstBatch::sources() const
{
	return _sources;
}

QstFieldsVector QstBatch::fields(const FieldPurposes &purposes) const
{
	if (purposes == PurposeAll_Mask)
	return _fields;

QstFieldsVector resFields;

	if (purposes & PurposeSelect)
	{
		QstFieldsVector visFields;
		QstFieldsVector invisFields;

		for (int i = 0; i < _fields.size(); ++i)
		{
			if (_fields[i].isVisible())
				visFields.append(_fields[i]);
			else
				if (_fields[i].isInvisible())
					invisFields.append(_fields[i]);
		};

		resFields = visFields + invisFields;
	}
	else
	{
		for (int i = 0; i < _fields.size(); ++i)
		{
			if (_fields[i].purposes() & purposes)
			{
				resFields.append(_fields[i]);
			}
		};
	}

return resFields;
}

void QstBatch::clear()
{
	_sources.clear();
	_fields.clear();

	_fieldNamesLists.clear();
	_serviceFieldsIndexes.clear();
	_rolesVisibility.clear();
}

void QstBatch::clearSources()
{
	_sources.clear();
}

bool QstBatch::isValid() const
{
	if (_sources.isEmpty())
		return false;

	bool fieldsValid = false;
	for (int i = 0; i < _fields.size(); ++i)
	{
		if (_fields[i].isValid())
			fieldsValid = true;
	}

	return fieldsValid;
}

int QstBatch::columnIndex(const QString &fieldName, const bool &extractItemName) const
{
	QString resFieldName = fieldName;
	if (extractItemName)
		resFieldName = _extractItemName(fieldName);

	QStringList visFields = _fieldNamesLists[FieldVisible];
	QStringList invisFields = _fieldNamesLists[FieldInvisible];

	if (visFields.contains(resFieldName))
		return visFields.indexOf(resFieldName);

	if(invisFields.contains(resFieldName))
	{
		return visFields.size() + invisFields.indexOf(resFieldName);
	}
return -1;
}

int QstBatch::columnIndex(const FieldRole &role) const
{
	if (_rolesVisibility.contains(role))
	{
		FieldVisibility vis = _rolesVisibility[role];

		if (vis == FieldVisible)
		{
			Q_ASSERT(_serviceFieldsIndexes.contains(QstSpecial::QstRoleVisibility(role, FieldVisible)));
			return _serviceFieldsIndexes[QstSpecial::QstRoleVisibility(role, FieldVisible)];
		}
		else
		{
			 return _fieldNamesLists[FieldVisible].size() +
					_serviceFieldsIndexes[QstSpecial::QstRoleVisibility(role, FieldInvisible)];
		}
	}
return -1;
}

QstBatch & QstBatch::operator<<(const QString &source)
{
	addSource(source);
return *this;
}

QstBatch & QstBatch::operator<<(const QstField &field)
{
	addField(field);

return *this;
}

QString QstBatch::_extractItemName(const QString &fullFieldName) const
{
	if (_itemExtractor == NULL)
		return _defaultItemExtractor.extractItemName(fullFieldName);
	else
	{
		return _itemExtractor->extractItemName(fullFieldName);
	}
}

void QstBatch::_fillRoleVisMap(const QstField &field, const FieldVisibility &visibility)
{
	if (field.isService())
	{
		_serviceFieldsIndexes[QstSpecial::QstRoleVisibility(field.role(), visibility)] =
			_fieldNamesLists[visibility].size() - 1;

		Q_ASSERT(!_rolesVisibility.contains(field.role()));
		_rolesVisibility[field.role()] = visibility;
	}
}

}

