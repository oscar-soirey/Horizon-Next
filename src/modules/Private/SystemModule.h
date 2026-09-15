#pragma once

#ifndef HGE_SYS_MODULE
#define HGE_SYS_MODULE

#include "../../core/api/build_dll.h"

#ifdef _WIN32
#include <Windows.h>
#define SYSTEM_MODULE HMODULE
#elif defined(__linux__)
#define SYSTEM_MODULE
#elif defined(__APPLE__)
#define SYSTEM_MODULE
#endif


namespace hn::Private
{
	class ENGINE_API SysModule {
	public:
		explicit SysModule(const char* shared_file_path);
		~SysModule();

		void RegisterFactory();
		void UnregisterFactory();

	private:
		SYSTEM_MODULE sysmodule_;

		using RegisterFactoryFn = void* (*)();
		RegisterFactoryFn register_classes_function;

		using UnregisterFactoryFn = void* (*)();
		UnregisterFactoryFn unregister_classes_function;
	};
}

#endif