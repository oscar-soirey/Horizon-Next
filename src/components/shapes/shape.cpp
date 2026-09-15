#include "shape.h"
#include "../../core/actor.h"

#include "private/shape_internal.h"

namespace hge
{
	HGE_Shape::HGE_Shape(HGE_Actor *parent) :
		HGE_Component(parent), internal_(new priv::ShapeInternal())
	{
		HPROPERTY(_physics_density, Exposed, PhysicsChanged());
		HPROPERTY(_physics_friction, Exposed, PhysicsChanged());
		HPROPERTY(_physics_bounciness, Exposed, PhysicsChanged());

		HPROPERTY(_physics_enable_events, Exposed, PhysicsChanged());

		parent->ED_physics_mode_changed.Subscribe([this]{ PhysicsChanged(); });
	}

	HGE_Shape::~HGE_Shape()
	{
		//b2DestroyShape(internal_->shape_id, true);
		delete internal_;
	}

	b2ShapeId *HGE_Shape::GetInternalShapeID() const
	{
		//return &internal_->shape_id;
		return nullptr;
	}

}