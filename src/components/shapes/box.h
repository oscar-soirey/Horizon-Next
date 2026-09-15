#ifndef HGE_SHAPE_BOX_H
#define HGE_SHAPE_BOX_H

#include "shape.h"

namespace hge
{
	class ENGINE_API HGE_ShapeBox : public HGE_Shape {
	public:
		HGE_Vec2 _physics_box_size{1.f};

		HGE_ShapeBox(HGE_Actor* parent);

	private:
		void PhysicsChanged() override;
	};
}

#endif