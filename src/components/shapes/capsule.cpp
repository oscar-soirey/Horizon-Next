#include "capsule.h"

#include "../../core/actor.h"
#include "private/shape_internal.h"

namespace hge
{
	HGE_ShapeCapsule::HGE_ShapeCapsule(HGE_Actor *parent) : HGE_Shape(parent)
	{
		HPROPERTY(_physics_capsule_height, Exposed, PhysicsChanged());
		HPROPERTY(_physics_capsule_radius, Exposed, PhysicsChanged());
	}

	void HGE_ShapeCapsule::PhysicsChanged()
	{/*
		if (b2Shape_IsValid(internal_->shape_id))
			b2DestroyShape(internal_->shape_id, true);

		b2Capsule capsule{
									{0.f, std::max(0.01f, _physics_capsule_height/2) },
									{ 0.f, -std::max(0.01f, _physics_capsule_height/2) },
									_physics_capsule_radius
								};

		b2ShapeDef shape_def = b2DefaultShapeDef();

		shape_def.density = _physics_density;

		//events
		shape_def.enableContactEvents = _physics_enable_events;
		shape_def.enableHitEvents = _physics_enable_events;

		internal_->shape_id = b2CreateCapsuleShape(*parent_->GetRigidBody(), &shape_def, &capsule);

		b2Shape_SetFriction(internal_->shape_id, _physics_friction);
		b2Shape_SetRestitution(internal_->shape_id, _physics_bounciness);*/
	}
}
