#pragma once

#include "../api/build_dll.h"

namespace hn
{
	class Engine;

	enum class ELog_type {
		MSG, WARNING, ERR
	};
	using enum ELog_type;

	void InitLog(Engine* engine);
	ENGINE_API void Log(const char* msg, ELog_type mode=MSG, bool print_to_viewport_=false, float viewport_time_=5.f);
	ENGINE_API void LogViewport(const char* msg, float time=5.f);
}