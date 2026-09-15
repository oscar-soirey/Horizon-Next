#ifndef HGE_CHARACTER_H
#define HGE_CHARACTER_H

#include "actor.h"
#include "../components/shapes/capsule.h"

namespace hge
{
	class ENGINE_API HGE_Character : public HGE_Actor {
	public:
		bool _movement_apply_gravity=true;
		float _movement_max_fall_vel=1000.f;
		float _movement_jump_force=10.f;
		float _movement_max_x_vel=5.f;

		HGE_Character();

		void Init() override;
		void Tick(double _dt) override;

		/**
		 * Not available if gravity is disabled
		 */
		void Jump();
		void MoveX(float x);

	private:
		HGE_ShapeCapsule* capsule_component_=nullptr;

		//physics internal
		bool grounded_=false;
		bool can_move_x_=true;
		bool on_ceiling_=false;
		bool requested_jump_=false;
		float requested_x_movement_vel_=0.f;
	};
}

#endif