#pragma once

#include "../../core/api/build_dll.h"

namespace hn::tools
{
	ENGINE_API int MakeArchive(const char* assets_path);
	ENGINE_API int CompileLuaFiles(const char* assets_path);
	ENGINE_API int GenerateProjectFiles(const char* project_main_path, const char* generator="Visual Studio 17 2022");
	ENGINE_API int CompileGame(const char* project_main_path, int max_workers);
}