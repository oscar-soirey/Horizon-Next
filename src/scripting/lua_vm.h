#pragma once

#include "../core/api/build_dll.h"

#include <vector>
#include <filesystem>

extern "C"
{
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

namespace hn
{
	class Engine;

	class ENGINE_API LuaVM {
	public:
		using LuaFunction = int (*)(lua_State*);

		LuaVM(Engine* engine);
		~LuaVM();

		void Update(float dt);

		void RegisterHNStdFunctions();
		void RegisterFunction(const char* name, LuaFunction function);

		bool DoFromFile(const char* path);

		const std::vector<std::filesystem::path>& GetScriptStack() const;

	private:
		lua_State* L=nullptr;
		hn::Engine* engine_=nullptr;

		std::vector<std::filesystem::path> script_stack_;
	};
}