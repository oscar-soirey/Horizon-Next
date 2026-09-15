#include "character.h"

#include "engine.h"

namespace hge
{
	HGE_Character::HGE_Character()
	{
		HPROPERTY(_movement_apply_gravity, Exposed);
		HPROPERTY(_movement_max_fall_vel, Exposed);
		HPROPERTY(_movement_jump_force, Exposed);
		HPROPERTY(_movement_max_x_vel, Exposed);
	}

	void HGE_Character::Init()
	{
		HGE_Actor::Init();

		_physics_mode = KINEMATIC;
		capsule_component_ = HCOMPONENT("capsule_comp", HGE_ShapeCapsule);
		capsule_component_->_physics_enable_events=true;
	}

void HGE_Character::Tick(double _dt)
	{/*
		b2Vec2 vel = b2Body_GetLinearVelocity(*GetRigidBody());

		//vélocité réelle avant toute modification, utile pour la detection de collision plus tard
		float raw_vel_x = vel.x;

		//apply x
		vel.x = can_move_x_ ? requested_x_movement_vel_ : 0.f;

		//apply y (gravity or jump)
		if (_movement_apply_gravity)
		{
			if (requested_jump_)
			{
				// if(can jump)
				vel.y = _movement_jump_force;
				requested_jump_ = false;
			}
			//not requested jump -> apply gravity
			else
			{
				float world_gravity = b2World_GetGravity(physics::GetWorld()).y;
				if (!grounded_)
				{
					vel.y += world_gravity * (float)_dt * _physics_gravity_scale;
					vel.y = std::max(vel.y, -_movement_max_fall_vel);
				}
				else
				{
					vel.y = 0;
				}
			}
		}

		b2Body_SetLinearVelocity(*GetRigidBody(), vel);

		//reset x movement request
		requested_x_movement_vel_=0.f;


		//collision detection
		grounded_=false;
		can_move_x_=true;


		struct CastContext {
			b2CastOutput floor_output = {};
			b2CastOutput wall_output = {};
			b2ShapeId selfId;
		};

		//collider callback
		auto castCallback = [](b2ShapeId shapeId, b2Vec2 point, b2Vec2 normal, float fraction, void* ctx) -> float
		{
			auto* c = static_cast<CastContext*>(ctx);

			//ignore the player self shape
			if (B2_ID_EQUALS(shapeId, c->selfId))
			{
				//skip this one
				return -1.f;
			}

			if (normal.y >= 0.7f)
			{
				c->floor_output.hit = true;
				c->floor_output.fraction = fraction;
				c->floor_output.point = point;
				c->floor_output.normal = normal;
			}
			else
			{
				c->wall_output.hit = true;
				c->wall_output.fraction = fraction;
				c->wall_output.point = point;
				c->wall_output.normal = normal;
			}

			return fraction;
		};

		b2Vec2 origin = {transform.location_.x, transform.location_.y};

		b2Capsule capsule;
		capsule.center1 = {origin.x, origin.y - capsule_component_->_physics_capsule_height/2};
		capsule.center2 = {origin.x, origin.y + capsule_component_->_physics_capsule_height/2};
		capsule.radius = capsule_component_->_physics_capsule_radius;
		b2ShapeProxy proxy = b2MakeProxy((b2Vec2*)&capsule, 2, capsule_component_->_physics_capsule_radius);

		//cast floor
		b2Vec2 translation_floor = {0.f, -0.1f};
		CastContext ctx_floor;
		ctx_floor.selfId = *capsule_component_->GetInternalShapeID();
		b2World_CastShape(physics::GetWorld(), &proxy, translation_floor, b2DefaultQueryFilter(), castCallback, &ctx_floor);

		//floor hit
		if (vel.y <= 0)
		{
			grounded_ = ctx_floor.floor_output.hit;
		}
		if (grounded_)
		{
			transform.location_.y = ctx_floor.floor_output.point.y + capsule_component_->_physics_capsule_height/2 + capsule_component_->_physics_capsule_radius;
		}

		//wall hit
		CastContext ctx_wall;
		ctx_wall.selfId = *capsule_component_->GetInternalShapeID();

		printf("vel x : %f\n", vel.x);

		//mouvement vers la droite
		if (raw_vel_x > 0)
		{
			//wall cast
			float translation_x = 0.1f;
			b2Vec2 translation_wall = {translation_x, 0.f};
			b2World_CastShape(physics::GetWorld(), &proxy, translation_wall, b2DefaultQueryFilter(), castCallback, &ctx_wall);

			can_move_x_ = !ctx_wall.wall_output.hit;
		}
		//vers la gauche
		else if (raw_vel_x < 0)
		{
			//wall cast
			float translation_x = -0.1f;
			b2Vec2 translation_wall = {translation_x, 0.f};
			b2World_CastShape(physics::GetWorld(), &proxy, translation_wall, b2DefaultQueryFilter(), castCallback, &ctx_wall);

			can_move_x_ = !ctx_wall.wall_output.hit;
		}
		//replacer le personnage en cas de collision latérale
		if (!can_move_x_)
		{
			if (raw_vel_x > 0)
			{
				transform.location_.x = ctx_wall.wall_output.point.x - capsule_component_->_physics_capsule_radius;
			}
			else if (raw_vel_x < 0)
			{
				transform.location_.x = ctx_wall.wall_output.point.x + capsule_component_->_physics_capsule_radius;
			}
		}*/


		//**update box2d**//
		HGE_Actor::Tick(_dt);
	}

	void HGE_Character::Jump()
	{
		requested_jump_=true;
	}

	void HGE_Character::MoveX(float x)
	{
		requested_x_movement_vel_ = x*_movement_max_x_vel;
	}
}
