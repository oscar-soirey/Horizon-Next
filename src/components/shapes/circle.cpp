#include "circle.h"

#include "../../core/actor.h"
#include "private/shape_internal.h"

namespace hge
{
	HGE_ShapeCircle::HGE_ShapeCircle(HGE_Actor *parent) : HGE_Shape(parent)
	{
		HPROPERTY(_physics_circle_radius, Exposed, PhysicsChanged());
	}

	void HGE_ShapeCircle::PhysicsChanged()
	{/*
		if (b2Shape_IsValid(internal_->shape_id))
			b2DestroyShape(internal_->shape_id, true);

		b2Circle circle{{0.f,0.f}, _physics_circle_radius};

		b2ShapeDef shape_def = b2DefaultShapeDef();

		shape_def.density = _physics_density;

		//events
		shape_def.enableContactEvents = _physics_enable_events;
		shape_def.enableHitEvents = _physics_enable_events;

		internal_->shape_id = b2CreateCircleShape(*parent_->GetRigidBody(), &shape_def, &circle);

		b2Shape_SetFriction(internal_->shape_id, _physics_friction);
		b2Shape_SetRestitution(internal_->shape_id, _physics_bounciness);*/
	}
}
