#ifndef HGE_SHAPE_CIRCLE_H
#define HGE_SHAPE_CIRCLE_H

#include "shape.h"

class b2ShapeId;

namespace hge
{
	class ENGINE_API HGE_ShapeCircle : public HGE_Shape {
	public:
		float _physics_circle_radius=0.4f;

		explicit HGE_ShapeCircle(HGE_Actor* parent);

	private:
		void PhysicsChanged() override;
	};
}

#endif