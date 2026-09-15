#include "log.h"

#include "../engine.h"

static hn::Engine* engine_=nullptr;

namespace hn
{
	void InitLog(Engine* engine)
	{
		engine_ = engine;
	}

	void Log(const char* msg, ELog_type mode, bool print_to_viewport_, float viewport_time_)
	{
		if (!engine_)
		{
			return;
		}
		engine_->Log(msg, mode, print_to_viewport_, viewport_time_);
	}

	void LogViewport(const char* msg, float time)
	{
		Log(msg, MSG, true, time);
	}
}