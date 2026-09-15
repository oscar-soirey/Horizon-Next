#ifndef HGE_BASE_TCOMPONENT_H
#define HGE_BASE_TCOMPONENT_H

#include "base_component.h"

namespace hge
{
	class ENGINE_API HGE_SceneComponent : public HGE_Component {
		friend class HGE_Actor;

	public:
		HGE_Transform relative_transform{};

		explicit HGE_SceneComponent(HGE_Actor* _parent);

		HGE_Transform GetAbsoluteTransform();
		HGE_Vec3 GetAbsoluteLocation();
		HGE_Vec3 GetAbsoluteRotation();
		HGE_Vec3 GetAbsoluteScale();

	protected:
		virtual void TransformModified(){}
	};
}

#endif