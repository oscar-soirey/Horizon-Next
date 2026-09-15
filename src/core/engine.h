#pragma once

#include "data/ini_parser.h"
#include "api/build_dll.h"
#include "log/log.h"

#include <unordered_map>
#include <vector>

struct ImportedPlugin;
class HNFrameListener;
class EngineStreambuf;
class PrintfInterceptor;
class OgreLogRedirector;

struct rendering_interface;

namespace hge
{
	class HGE_Level;
}

namespace hn
{
	class IPlugin;
	class Engine;
	class LuaVM;

	namespace Private { class SysModule; }

	class ENGINE_API Logger {
	public:
		Logger()=default;
		virtual ~Logger()=default;
		void SetEnginePointer(Engine* engine);
		virtual void Log(const char* msg, ELog_type mode, bool print_to_viewport_, float viewport_time_);
		//Do not call cout of printf within this function!
		virtual void RedirectMessages(const std::string& buffer){}

	private:
		Engine* engine_=nullptr;
	};

	class ENGINE_API Engine {
		friend class ::HNFrameListener;
		friend class ::EngineStreambuf;
		friend class ::PrintfInterceptor;
		friend class ::OgreLogRedirector;

	public:
		Engine(const char* configuration_file, bool editor, Logger* logger=nullptr);
		~Engine();

		void StartMainLoop();

		void StartGame();
		void EndGame();

		double GetDeltaTime() const;

		void* GetRenderingBackend() const;

		/**
 * Assign a created level to the current level
 */
		void CreateLevel(const char* from_file="");

		/**
		 * Delete current level instance and assign current level to nullptr
		 */
		void DestroyCurrentLevel();

		/**
		 * @return Current level pointer
		 */
		hge::HGE_Level* GetCurrentLevel_();

		//Log
		void SetCustomLoggerSubsystem(Logger* logger);
		void Log(const char* msg, ELog_type type=ELog_type::MSG, bool print_to_viewport_=false, float viewport_time_=5.f);
		Logger* GetLogger() const;


		//Scripting
		LuaVM* GetLuaVM() const;


		//Gameplay
		float global_time_dilatation=1.f;


	private:
		bool using_editor_=false;
		bool game_tick_enabled_=false;

		double current_delta_time_=1.f;

		rendering_interface* rendering_backend_=nullptr;
		HN_Ini* config_=nullptr;


		//Level
		hge::HGE_Level* current_opened_level_=nullptr;

		//Log
		EngineStreambuf* streambuf_=nullptr;
		PrintfInterceptor* printfbuf=nullptr;
		std::streambuf* default_cout_buf_=nullptr;
		Logger* logger_=nullptr;

		//scripting
		LuaVM* lua_vm_=nullptr;

		//Plugins
		std::unordered_map<std::string, ImportedPlugin*> plugins_;
	};





	//Utility functions
	ENGINE_API std::string GetEngineVersion();
}