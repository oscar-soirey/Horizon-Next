#pragma once

#include "../../src/core/actor.h"
#include "../../src/core/gameplay/input.h"
#include "../../src/core/log/log.h"

#include <components/static_mesh.h>

class Player : public hge::HGE_Actor {
public:
	int life=100;

	hge::InputAction jump_action = "jump";
	hge::InputAxis1D move_forward = "move_x";

	Player()
	{
		printf("hello player");
		HPROPERTY(life, hge::Exposed);
		hn::LogViewport("HELLO FROM PLAYER", 200);
		auto* mesh = HCOMPONENT("mesh", hn::HN_StaticMesh);
		mesh->mesh_path = "scifi_girl_v.01.mesh";
	}

	void Init() override
	{
		HGE_Actor::Init();
		hn::LogViewport("INIT FROM PLAYER", 200);
	}

	~Player() override
	{
		hn::LogViewport("DESTRUCTOR PLAYER", 200);
	}

private:
	void ProcessInput(double dt) override
	{
		if (jump_action.IsPressed())
		{
			printf("jump");
		}
		move_forward.GetValue();
		printf("process input");
	}
};