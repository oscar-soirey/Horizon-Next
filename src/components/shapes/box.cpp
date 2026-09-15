#include "box.h"

#include "../../core/actor.h"
#include "private/shape_internal.h"

namespace hge
{
	HGE_ShapeBox::HGE_ShapeBox(HGE_Actor *parent) : HGE_Shape(parent)
	{
		HPROPERTY(_physics_box_size, Access::Exposed, PhysicsChanged());
	}

	void HGE_ShapeBox::PhysicsChanged()
	{/*
		if (b2Shape_IsValid(internal_->shape_id))
			b2DestroyShape(internal_->shape_id, true);

		b2Polygon shape_box = b2MakeBox(std::max(0.01f, _physics_box_size.x), std::max(0.01f, _physics_box_size.y));

		b2ShapeDef shape_def = b2DefaultShapeDef();

		shape_def.density = _physics_density;

		//events
		shape_def.enableContactEvents = _physics_enable_events;
		shape_def.enableHitEvents = _physics_enable_events;

		internal_->shape_id = b2CreatePolygonShape(*parent_->GetRigidBody(), &shape_def, &shape_box);

		b2Shape_SetFriction(internal_->shape_id, _physics_friction);
		b2Shape_SetRestitution(internal_->shape_id, _physics_bounciness);*/
	}
}
