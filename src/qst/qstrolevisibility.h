/****************************************************************************
** QST 0.4.2a beta
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

#ifndef QSTROLEVISIBILITY_H
#define QSTROLEVISIBILITY_H

#include "qstglobal.h"

namespace Qst
{

namespace QstSpecial
{


class QstRoleVisibility
{
private:
	Qst::FieldRole			_role;
	Qst::FieldVisibility	_visibility;

public:
	QstRoleVisibility(const Qst::FieldRole &role = Qst::RoleNone,
					  const Qst::FieldVisibility &visibility = Qst::VisibilityNone);

	Qst::FieldRole			role() const;
	Qst::FieldVisibility	visibility() const;

	friend bool operator<(const QstRoleVisibility& val1,
						  const QstRoleVisibility& val2)
	{
		return	(val1._role < val2._role);
	}
};


}	// End of namespace QstSpecial

}	// End of namespace Qst

#endif // QSTROLEVISIBILITY_H
