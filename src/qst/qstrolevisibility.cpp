#include "qstrolevisibility.h"

namespace QstSpecial
{

QstRoleVisibility::QstRoleVisibility(const Qst::FieldRole &role,
									 const Qst::FieldVisibility &visibility)
	:
	_role(role),
	_visibility(visibility)
{
}

	Qst::FieldRole			QstRoleVisibility::role() const
	{
		return _role;
	}

	Qst::FieldVisibility	QstRoleVisibility::visibility() const
	{
		return _visibility;
	}


}
