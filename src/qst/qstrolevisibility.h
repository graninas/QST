#ifndef QSTROLEVISIBILITY_H
#define QSTROLEVISIBILITY_H

#include "qst/qstglobal.h"

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

}

#endif // QSTROLEVISIBILITY_H
