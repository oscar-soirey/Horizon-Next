#include "SystemPlugin.h"

#include "../iplugin.h"

#include <filesystem>
#include <iostream>

namespace fs = std::filesystem;


std::string FindFirstDll(const fs::path& folder)
{
	for (const auto& entry : fs::directory_iterator(folder))
	{
		if (!entry.is_regular_file())
			continue;

		if (entry.path().extension() == ".dll")
			return entry.path().string();
	}

	//vide si rien trouvé
	return {};
}

namespace hn::Private
{
#ifdef _WIN32
	SysPlugin::SysPlugin(const char *shared_file_path)
	{
		//charger la dll
		std::filesystem::path fs_path = shared_file_path;
		std::string dll_path = fs_path.string();

		sysmodule_ = LoadLibraryA(dll_path.c_str());
		if (!sysmodule_)
		{
			std::cout << "DLL plugin loading error : unable to load dll: " << dll_path << std::endl;
			return;
		}

		auto load_plugin_fn = (LoadPluginFn)GetProcAddress(sysmodule_, "LoadPlugin");
		plugin_ = load_plugin_fn();

		std::cout << "Plugin registered with success" << std::endl;
	}

#elif defined (__linux__)
	SysPlugin::SysPlugin(const char *parent_path)=0;
#elif defined (__APPLE__)
	SysPlugin::SysPlugin(const char *parent_path)=0;
#endif

#ifdef _WIN32
	SysPlugin::~SysPlugin()
	{
		delete plugin_;
		std::cout << "Plugin destructor called" << std::endl;
		FreeLibrary(sysmodule_);
	}
#elif defined (__linux__)
	SysPlugin::~SysPlugin()=0;
#elif defined (__APPLE__)
	SysPlugin::~SysPlugin()=0;
#endif

	IPlugin *SysPlugin::GetPlugin()
	{
		return plugin_;
	}

}