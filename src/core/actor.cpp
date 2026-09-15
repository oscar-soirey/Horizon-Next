#include "actor.h"

#include "components/scene_component.h"

#define PHYS_HND ((ActorPhysicsHandle*)phys_hnd_)

struct ActorPhysicsHandle {
	~ActorPhysicsHandle()
	{
		//b2DestroyBody(body);
	}

	//b2BodyId body;
};

namespace hge
{
	void HGE_Actor::OnTransformChanged()
	{
		ED_transform_modified.Call();
		if (phys_hnd_)
		{
			//b2Body_SetAwake(PHYS_HND->body, true);
		}
	}

	void HGE_Actor::OnPhysicsModeChanged()
	{
		//don't create physics body if physics mode is none
		if (_physics_mode != NONE)
		{
			//opaque physics handle
			delete PHYS_HND;
			phys_hnd_ = new ActorPhysicsHandle();

			//body def
			//b2BodyDef body_def = b2DefaultBodyDef();
			/*
						switch (_physics_mode)
						{
							case STATIC:
							{
								body_def.type = b2_staticBody;
								break;
							}
							case DYNAMIC:
							{
								body_def.type = b2_dynamicBody;
								break;
							}
							case KINEMATIC:
							{
								body_def.type = b2_kinematicBody;
								break;
							}
							default: { break; }
						}

						body_def.position = (b2Vec2){transform.location_.x, transform.location_.y};

						//create body
						PHYS_HND->body = b2CreateBody(hge::physics::GetWorld(), &body_def);
					}
					else  //physics mode is NONE
					{
						delete PHYS_HND;
						phys_hnd_ = nullptr;
					}*/

			//notify
			ED_physics_mode_changed.Call();
		}
	}

	void HGE_Actor::OnAutoSleepChanged()
	{
		if (phys_hnd_)
		{
			//b2Body_EnableSleep(PHYS_HND->body, _physics_auto_sleep);
		}
	}

	void HGE_Actor::OnAutoGravityScaleChanged()
	{
		//if (phys_hnd_) b2Body_SetGravityScale(PHYS_HND->body, _physics_gravity_scale);
	}



	HGE_Actor::HGE_Actor()
	{
		HPROPERTY(transform, Exposed, OnTransformChanged());
		HPROPERTY(_physics_mode, Exposed, OnPhysicsModeChanged());
		HPROPERTY(_physics_auto_sleep, Exposed, OnAutoSleepChanged());
		HPROPERTY(_physics_gravity_scale, Exposed, OnAutoGravityScaleChanged());
	}


	void HGE_Actor::Tick(double _dt)
	{
		HGE_Object::Tick(_dt);

		//tick tous les components
		for (const auto& [key, comp] : components_)
		{
			comp->Tick(_dt);
		}
	}

	void HGE_Actor::Update(double _dt)
	{
		HGE_Object::Update(_dt);
/*
		//update physics only if physics mode is not NONE
		if (_physics_mode != NONE)
		{
			//modified explicitly by the user or the player
			if (transform_last_frame_ != transform)
			{
				b2Rot rot = b2MakeRot(transform.rotation_.z);
				b2Body_SetTransform(PHYS_HND->body, {transform.location_.x, transform.location_.y}, rot);
			}
			//not modified, read box2D transform
			else
			{
				if (b2Body_IsValid(PHYS_HND->body))
				{
					//get position and angle from box2D
					b2Vec2 pos = b2Body_GetPosition(PHYS_HND->body);
					b2Rot rot = b2Body_GetRotation(PHYS_HND->body);

					float angle = b2Rot_GetAngle(rot);

					transform.location_ = HGE_Vec3(pos.x, pos.y, transform.location_.z);
					transform.rotation_.z = angle;
				}
			}

			transform_last_frame_=transform;
		}
		*/

		//update tous les components
		for (const auto& [key, comp] : components_)
		{
			comp->Update(_dt);
		}
	}


	HGE_Vec3 HGE_Actor::GetAcceleration() const
	{
		return {};
	}

	HGE_Component* HGE_Actor::AddComponent(const char *_name, const std::function<std::unique_ptr<HGE_Component>(HGE_Actor*)>& _constructor)
	{
		std::unique_ptr<HGE_Component> uniqueComp = _constructor(this);
		HGE_Component* ptrComp = uniqueComp.get();
		//on initialise le parent du comp avec this
		ptrComp->parent_ = this;
		components_.emplace(_name, std::move(uniqueComp));
		return ptrComp;
	}
	const std::unordered_map<std::string, std::unique_ptr<HGE_Component>>& HGE_Actor::GetComponents() const
	{
		return components_;
	}
	HGE_Component* HGE_Actor::GetComponent(const char *_name)
	{
		auto it = components_.find(_name);
		if (it == components_.end())
		{
			return nullptr;
		}
		return it->second.get();
	}


	hn::Engine* HGE_Actor::GetEngine() const
	{
		return engine_internal_;
	}

	b2BodyId *HGE_Actor::GetRigidBody()
	{
		if (!phys_hnd_) return nullptr;
		//return &PHYS_HND->body;
	}

	void HGE_Actor::OnPossessed(int pc)
	{
		ED_possess_state_changed.Call(true, pc);
	}
	void HGE_Actor::OnUnpossessed(int pc)
	{
		ED_possess_state_changed.Call(false, pc);
	}


}
