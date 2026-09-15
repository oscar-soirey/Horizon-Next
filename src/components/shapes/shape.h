//parent de tous les shapes pour centraliser les propriétés communes

#ifndef HGE_SHAPE_COMP_H
#define HGE_SHAPE_COMP_H

#include "../base_component.h"

class b2ShapeId;

namespace hge
{
	namespace priv { struct ShapeInternal; }

	class ENGINE_API HGE_Shape : public HGE_Component {
	public:
		float _physics_density=1.f;
		float _physics_friction=1.f;
		float _physics_bounciness=0.f;

		//allows to get contact and hit events
		bool _physics_enable_events=true;

		explicit HGE_Shape(HGE_Actor* parent);
		~HGE_Shape() override;

		b2ShapeId* GetInternalShapeID() const;

	protected:
		priv::ShapeInternal* internal_;

		virtual void PhysicsChanged()=0;
	};
}

#endif
