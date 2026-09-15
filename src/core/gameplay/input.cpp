#include "input.h"

#include "../private/input_manager.h"

#include <vector>
#include <algorithm>

namespace hge
{
	InputAction::InputAction(const char *name):
		keys_(priv::input::GetActionKeys(name))
	{ }

	bool InputAction::IsPressed()
	{
		for (int k : keys_)
		{
			const auto& just = priv::input::GetJustPressedKeys();
			if (std::find(just.begin(), just.end(), k) != just.end())
				return true;
		}
		return false;
	}

	bool InputAction::IsHeld()
	{
		for (int k : keys_)
		{
			const auto& just = priv::input::GetPressedKeys();

			if (std::find(just.begin(), just.end(), k) != just.end())
				return true;
		}
		return false;
	}

	bool InputAction::IsReleased()
	{
		for (int k : keys_)
		{
			const auto& just = priv::input::GetJustReleasedKeys();
			if (std::find(just.begin(), just.end(), k) != just.end())
				return true;
		}
		return false;
	}

	InputAxis1D::InputAxis1D(const char *name):
		actions_(priv::input::GetAxisActions(name))
	{ }

	float InputAxis1D::GetValue()
	{
		float val = 0.f;
		const auto& pressed = priv::input::GetPressedKeys();

		for (const auto& a : actions_)
		{
			if (std::find(pressed.begin(), pressed.end(), a.key) != pressed.end())
				val += a.value;
		}
		return std::clamp(val, -1.f, 1.f);
	}

}