/**
 * This class is used by the engine at the start to load plugin .dll or .so
 */

#ifndef HGE_SYS_PLUGIN
#define HGE_SYS_PLUGIN

#include "../../core/api/build_dll.h"

#ifdef _WIN32
#include <Windows.h>
#define SYSTEM_MODULE HMODULE
#elif defined(__linux__)
#define SYSTEM_MODULE
#elif defined(__APPLE__)
#define SYSTEM_MODULE
#endif

namespace hn
{
	class IPlugin;
}

namespace hn::Private
{
	class ENGINE_API SysPlugin {
	public:
		explicit SysPlugin(const char* shared_file_path);
		~SysPlugin();

		IPlugin* GetPlugin();

	private:
		SYSTEM_MODULE sysmodule_;
		using LoadPluginFn = IPlugin* (*)();
		IPlugin* plugin_ = nullptr;
	};
}

#endif