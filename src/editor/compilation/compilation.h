#pragma once

namespace hn::editor
{
	int MakeArchive(const char* assets_path);
	int CompileLuaFiles(const char* assets_path);
	int GenerateProjectFiles(const char* project_main_path, const char* generator="Visual Studio 17 2022");
	int CompileGame(const char* project_main_path, int max_workers);
}