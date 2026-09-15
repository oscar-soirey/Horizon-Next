#pragma once

#include <vector>
#include "../gameplay/input.h"

namespace hge::priv::input
{
	//dll export
	void ENGINE_API LoadConfigFile(const char* path);
	void ENGINE_API InjectKeyDown(int key);
	void ENGINE_API InjectKeyUp(int key);

	//Internal functions//

	//to manage the just pressed and just released events
	void Tick();

	const std::vector<int>& GetPressedKeys();
	const std::vector<int>& GetJustPressedKeys();
	const std::vector<int>& GetJustReleasedKeys();

	const std::vector<int>& GetActionKeys(const char* name);
	const std::vector<action_t>& GetAxisActions(const char* name);
}