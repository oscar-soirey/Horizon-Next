#include "lua_vm.h"

#include "../core/engine.h"
#include "../core/filesystem.h"

#include <iostream>

namespace
{
	using namespace hn;

	LuaVM* vm=nullptr;

	void Lua_ELog_type(lua_State* L)
	{
		lua_newtable(L);

		lua_pushinteger(L, static_cast<int>(ELog_type::MSG));
		lua_setfield(L, -2, "MSG");

		lua_pushinteger(L, static_cast<int>(ELog_type::WARNING));
		lua_setfield(L, -2, "WARNING");

		lua_pushinteger(L, static_cast<int>(ELog_type::ERR));
		lua_setfield(L, -2, "ERR");

		lua_setglobal(L, "ELog_type");
	}


	void Lua_Engine(lua_State* L, Engine* engine)
	{
		auto** userdata = static_cast<Engine**>(lua_newuserdata(L, sizeof(Engine*)));

		*userdata = engine;

		luaL_getmetatable(L, "Engine");
		lua_setmetatable(L, -2);

		lua_setglobal(L, "engine");
	}
	int Lua_Engine_GetDeltaTime(lua_State* L)
	{
		auto* engine = *static_cast<Engine**>(luaL_checkudata(L, 1, "Engine"));
		lua_pushnumber(L, engine->GetDeltaTime());
		return 1;
	}
	void Lua_RegisterEngine(lua_State* L)
	{
		luaL_newmetatable(L, "Engine");

		lua_pushcfunction(L, Lua_Engine_GetDeltaTime);
		lua_setfield(L, -2, "GetDeltaTime");

		lua_pushvalue(L, -1);
		lua_setfield(L, -2, "__index");

		lua_pop(L, 1);
	}


	int Lua_Log(lua_State* L)
	{
		std::string msg = luaL_checkstring(L, 1);
		auto mode = (hn::ELog_type)luaL_checkinteger(L, 2);
		bool viewport = lua_toboolean(L, 3);
		auto time = static_cast<float>(luaL_checknumber(L, 4));

		hn::Log(msg.c_str(), mode, viewport, time);
		return 0;
	}

	int Lua_dofile(lua_State* L)
	{
		const char* file = luaL_checkstring(L, 1);

		std::filesystem::path parent =
				vm->GetScriptStack().back().parent_path();

		std::filesystem::path full_path =
				parent / file;

		std::string full_path_string = full_path.string();
		if (!vm->DoFromFile(full_path_string.c_str()))
		{
			return luaL_error(
					L,
					"cannot open %s",
					full_path.string().c_str()
			);
		}

		return 0;
	}
}

namespace hn
{
	LuaVM::LuaVM(Engine *engine): engine_(engine)
	{
		L = luaL_newstate();
		luaL_openlibs(L);
	}

	LuaVM::~LuaVM()
	{
		lua_close(L);
	}

	void LuaVM::Update(float dt)
	{
		lua_getglobal(L, "Update");

		if (!lua_isfunction(L, -1))
		{
			lua_pop(L, 1);
			return;
		}

		lua_pushnumber(L, dt);

		if (lua_pcall(L, 1, 0, 0) != LUA_OK)
		{
			std::cerr << "Lua Update error: "
								<< lua_tostring(L, -1)
								<< '\n';

			lua_pop(L, 1);
		}
	}

	void LuaVM::RegisterHNStdFunctions()
	{
		vm=this;

		//Register enums
		Lua_ELog_type(L);

		//Register global objects
		Lua_RegisterEngine(L);
		Lua_Engine(L, engine_);

		//Overload some lua functions
		lua_pushcfunction(L, Lua_dofile);
		lua_setglobal(L, "dofile");

		//Register functions
		RegisterFunction("Log", Lua_Log);
	}

	void LuaVM::RegisterFunction(const char* name, LuaFunction function)
	{
		lua_register(L, name, function);
	}

	bool LuaVM::DoFromFile(const char *path)
	{
		std::string code = GetFileContent(path, nullptr);

		if (code.empty())
			return false;

		script_stack_.push_back(GetParentPath(path));

		bool result = luaL_dostring(L, code.c_str());

		script_stack_.pop_back();

		return result;
	}

	const std::vector<std::filesystem::path>& LuaVM::GetScriptStack() const
	{
		return script_stack_;
	}
}
