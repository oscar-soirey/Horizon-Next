#ifndef HGE_INPUT_H
#define HGE_INPUT_H

#include <vector>
#include <string>

#include "../api/build_dll.h"

namespace hge
{
	class ENGINE_API InputAction {
	public:
		InputAction(const char* name);
		~InputAction()=default;

		bool IsPressed();
		bool IsHeld();
		bool IsReleased();

	private:
		std::vector<int> keys_;
	};

	typedef struct {
		int key;
		float value;
	}action_t;

	class ENGINE_API InputAxis1D {
	public:
		InputAxis1D(const char* name);
		~InputAxis1D()=default;

		float GetValue();

	private:
		std::vector<action_t> actions_;
	};
}

#endif