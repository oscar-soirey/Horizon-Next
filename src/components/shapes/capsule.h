#ifndef HGE_SHAPE_CAPSULE_H
#define HGE_SHAPE_CAPSULE_H

#include "shape.h"

class b2ShapeId;

namespace hge
{
	class ENGINE_API HGE_ShapeCapsule : public HGE_Shape {
	public:
		float _physics_capsule_height=1.5f;
		float _physics_capsule_radius=0.4f;

		explicit HGE_ShapeCapsule(HGE_Actor* parent);

	private:
		void PhysicsChanged() override;
	};
}

#endif