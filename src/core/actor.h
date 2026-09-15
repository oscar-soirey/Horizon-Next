/**
 *
 */

#ifndef HGE_ACTOR_H
#define HGE_ACTOR_H

#include "object.h"
#include "common.h"
#include "data/event_dispatcher.h"
#include "../components/base_component.h"

#include <functional>
#include <memory>

class b2BodyId;

typedef enum {
	NONE,
	STATIC,
	DYNAMIC,
	KINEMATIC
}HGE_PhysicsMode;

//not callable beacause the function doesn't have ENGINE_API specified
extern void PlayerControllersTick(double dt);

namespace hn { class Engine; }

namespace hge
{
	class ENGINE_API HGE_Actor : public HGE_Object {
		friend class HGE_Level;
		friend void ::PlayerControllersTick(double dt);
	public:
		HGE_Transform transform{};

		int _physics_mode = STATIC;
		bool _physics_auto_sleep = true;
		float _physics_gravity_scale = 1.f;


		//actor class is not copiable
		HGE_Actor(const HGE_Actor&) = delete;
		HGE_Actor& operator=(const HGE_Actor&) = delete;
		HGE_Actor(HGE_Actor&&) = default;
		HGE_Actor& operator=(HGE_Actor&&) = default;


		HGE_Actor();
		//Game logic
		void Tick(double _dt) override;
		//Actor logic (eg. Rendering)
		void Update(double _dt) override;

		HGE_Component* AddComponent(const char* _name, const std::function<std::unique_ptr<HGE_Component>(HGE_Actor*)>& _constructor);
		const std::unordered_map<std::string, std::unique_ptr<HGE_Component>>& GetComponents() const;
		HGE_Component* GetComponent(const char* _name);

		HGE_Vec3 GetAcceleration() const;

		//physics
		b2BodyId* GetRigidBody();

		//usefull for notify components
		HEventDispatcher<> ED_transform_modified;
		HEventDispatcher<> ED_physics_mode_changed;

		/**
		 * @param pc Player controller id
		 */
		virtual void OnPossessed(int pc);
		virtual void OnUnpossessed(int pc);
		//usefull for components, true is for possessed, false is for unpossessed
		//int is the player controller id
		HEventDispatcher<bool, int> ED_possess_state_changed;

		hn::Engine* GetEngine() const;

	private:
		std::unordered_map<std::string, std::unique_ptr<HGE_Component>> components_;

		virtual void ProcessInput(double dt){}

		void OnTransformChanged();
		void OnPhysicsModeChanged();
		void OnAutoSleepChanged();
		void OnAutoGravityScaleChanged();

		//backend information, do not modify
		hn::Engine* engine_internal_=nullptr;

		//physics handle ptr
		void* phys_hnd_=nullptr;

		HGE_Transform transform_last_frame_{};
	};
}

/**
 * display name,
 * class typename,
 * instructions before call init
 */
#define HCOMPONENT(__name__, __class__) \
	dynamic_cast<__class__*>(AddComponent(__name__, [ this ]( HGE_Actor* parent ){ return std::make_unique<__class__>(parent); }))


#endif