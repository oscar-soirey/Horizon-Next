#include "engine.h"

#include "../plugins/iplugin.h"
#include "../plugins/Private/SystemPlugin.h"
#include "actor.h"
#include "Private/input_manager.h"
#include "level.h"
#include "log/redirect.h"
#include "log/log.h"
#include "../scripting/lua_vm.h"
#include "filesystem.h"
#include "../modules/Private/SystemModule.h"
#include "Private/EngineRenderingBackend.h"

#include <string>
#include <iostream>
#include <unordered_map>
#include <filesystem>

#include <Ogre.h>
#include <Bites/OgreApplicationContext.h>
#include <Bites/OgreTrays.h>
#include <OgreWindowEventUtilities.h>



#define DEBUG printf("%d\n", __LINE__)



[[maybe_unused]] static const std::unordered_map<std::string, std::string> api_friendly_names = {
	{"Direct3D11", "Direct3D11 Rendering Subsystem"},
	{"OpenGL", "OpenGL 3+ Rendering Subsystem"},
	{"Vulkan", "Vulkan Rendering Subsystem"}
};






namespace hn
{
	void Logger::SetEnginePointer(Engine *engine)
	{
		engine_ = engine;
	}

	void Logger::Log(const char *msg, ELog_type mode, bool print_to_viewport_, float viewport_time_)
	{
		std::string prefix;
		if (mode == hn::WARNING)
		{
			prefix = "W: ";
		}
		else if (mode == hn::ERR)
		{
			prefix = "ERR: ";
		}
		std::string full_message = prefix + msg;
		if (print_to_viewport_)
		{
			static_cast<rendering_interface*>(engine_->GetRenderingBackend())->log_to_viewport(full_message.c_str(), viewport_time_);
		}
	}


	Engine::Engine(const char *configuration_file, bool editor, bool create_window, Logger* logger, LoopListener* loop_listener): using_editor_(editor), has_window_(create_window)
	{
		//Init log.h
		InitLog(this);

		//Creer le logger avant tout
		if (logger)
		{
			logger_ = logger;
		}
		else
		{
			logger_ = new Logger();
		}
		logger_->SetEnginePointer(this);


		//Creer le loop listener
		if (loop_listener)
		{
			loop_listener_ = loop_listener;
		}
		else
		{
			loop_listener_ = new LoopListener();
		}
		loop_listener_->SetEnginePointer(this);



		//Intercept messages
		//cout
		streambuf_ = new EngineStreambuf(this);
		//default_cout_buf_ = std::cout.rdbuf(streambuf_);
		//printf
		//printfbuf = new PrintfInterceptor(this);


		//Init filesystem
		InitFilesystem(using_editor_);


		config_ = new HN_Ini(configuration_file);
		rendering_backend_ = new rendering_interface(this, !using_editor_);


		//Register lua functions
		lua_vm_ = new LuaVM(this);
		lua_vm_->RegisterHNStdFunctions();



		//creer les plugins
		//lister tous les sous dossiers de "Plugins/"
		printf("Registring plugins from ./Plugins/\n");
		namespace fs = std::filesystem;
		const std::filesystem::path plugin_path = "Plugins";
		for (const auto& category : fs::directory_iterator(plugin_path))
		{
			printf("Searching from category: %ls\n", category.path().c_str());

			for (const auto& plugin_name : fs::directory_iterator(category))
			{
				printf("Registering plugin: %ls\n", plugin_name.path().c_str());

				fs::path plugin_shared_file_path;
#ifdef _WIN32
				for (const auto& entry : fs::directory_iterator(plugin_name))
				{ if (entry.is_regular_file() && entry.path().extension() == ".dll")
					{
						plugin_shared_file_path = entry.path();
						break;
				} }
#elif defined(__APPLE__)

#elif defined(__linux__)

#endif

				if (plugin_shared_file_path.empty())
				{
					printf("Plugin DLL was not found, skipping\n");
					continue;
				}
				printf("Plugin DLL was found, loading plugin...\n");

				auto* imp_p = new ImportedPlugin();

				std::string plugin_dll_path_string = plugin_shared_file_path.string();
				imp_p->sys_plugin = new Private::SysPlugin(plugin_dll_path_string.c_str());

				if (!imp_p->sys_plugin->GetPlugin())
				{
					printf("Plugin DLL was found but not valid. Hint: The plugin should be written in the save version as the current engine, wich is :%s\n", ENGINE_VERSION_STR);
					continue;
				}

				plugins_.emplace(plugin_dll_path_string, imp_p);


				printf("%ls, successfully registered\n", plugin_name.path().c_str());
			}
		}

		printf("Calling plugin functions...\n");


		for (const auto& [n, p]: plugins_)
		{
			p->sys_plugin->GetPlugin()->Init(nullptr);
		}

		printf("Plugins OK. Engine init OK\n");
	}

	Engine::~Engine()
	{
		//Delete tous les plugins (a revoir avec les dependances propres)
		for (const auto& [n, p] : plugins_)
		{
			delete p->sys_plugin;
			delete p;
		}

		delete rendering_backend_;
		delete config_;

		//Console Interception
		delete printfbuf;
		//restore
		std::cout.rdbuf(default_cout_buf_);
		delete streambuf_;

		delete logger_;
	}


	void Engine::StartMainLoop()
	{
		//handles while loop
		rendering_backend_->root->startRendering();

		//Close game
		rendering_backend_->ctx.closeApp();
	}

	void Engine::ProgressOneFrame(double dt)
	{
		current_delta_time_ = dt;


		if (loop_listener_)
		{
			loop_listener_->ExecuteLoop(dt);
		}


		//refresh les debug messages sur le viewport
		for (auto it = rendering_backend_->viewport_labels.begin(); it != rendering_backend_->viewport_labels.end(); )
		{
			it->time_remaning -= dt;
			if (it->time_remaning <= 0.0)
			{
				rendering_backend_->tray_mgr->destroyWidget(it->label);
				it = rendering_backend_->viewport_labels.erase(it);
			}
			else
			{
				++it;
			}
		}
		rendering_backend_->tray_mgr->frameRendered({1.f, Ogre::Real(dt)});


		//Call lua Update global function
		GetLuaVM()->Update((float)dt);



		//update physics (before update actors)
		if (game_tick_enabled_)
		{
			//call player controller tick (internal hidden function)
			//calls process input actor method (called before actor and physics update)
			PlayerControllersTick(dt);

			//update physics, before actor ticks and update
			//physics::UpdateWorld(_deltatime);
		}

		if (current_opened_level_)
		{
			for (const auto& a : current_opened_level_->GetActors())
			{
				a->Update(dt);
			}
		}

		if (game_tick_enabled_)
		{
			//update actors
			if (current_opened_level_)
			{
				for (const auto& a : current_opened_level_->GetActors())
				{
					a->Tick(dt);
				}
			}

			//reset just pressed keys (called after actors update)
			hge::priv::input::Tick();
		}


		for (const auto& [n, p]: plugins_)
		{
			p->sys_plugin->GetPlugin()->Tick(dt);
			if (game_tick_enabled_)
			{
				p->sys_plugin->GetPlugin()->GameTick(dt);
			}
		}

		if (has_window_)
		{
			Ogre::WindowEventUtilities::messagePump();
		}
		else
		{
			rendering_backend_->root->renderOneFrame();
		}
	}


	void *Engine::GetRenderingBackend() const
	{
		return rendering_backend_;
	}


	//Game Control
	void Engine::StartGame()
	{
		if (current_opened_level_)
		{
			for (const auto& a: current_opened_level_->GetActors())
			{
				if (a)
				{
					a->StartGame();
				}
				else printf("start game error, actor is not valid");
			}
		}
		game_tick_enabled_=true;
	}

	void Engine::EndGame()
	{
		if (current_opened_level_)
		{
			for (const auto& a: current_opened_level_->GetActors())
			{
				if (a)
				{
					a->EndGame();
				}
				else printf("end game error, actor is not valid");
			}
		}
		game_tick_enabled_=false;
	}


	//Levels
	void Engine::CreateLevel(const char* from_file)
	{
		//delete current level before
		delete current_opened_level_;

		current_opened_level_ = new hge::HGE_Level();

		if (!std::string(from_file).empty())
		{
			current_opened_level_->LoadFromFile(from_file, this);
		}
	}

	void Engine::DestroyCurrentLevel()
	{
		delete current_opened_level_;
		current_opened_level_ = nullptr;
	}

	hge::HGE_Level* Engine::GetCurrentLevel_()
	{
		return current_opened_level_;
	}



	void Engine::SetCustomLoggerSubsystem(Logger *logger)
	{
		delete logger_;
		logger_ = logger;
	}

	void Engine::Log(const char *msg, ELog_type type, bool viewport, float viewport_time_)
	{
		if (!logger_)
		{
			printf("Engine log error: tying to log but no logger class is assinged\n");
			return;
		}
		logger_->Log(msg, type, viewport, viewport_time_);
	}

	Logger *Engine::GetLogger() const
	{
		return logger_;
	}


	LuaVM *Engine::GetLuaVM() const
	{
		return lua_vm_;
	}


	double Engine::GetDeltaTime() const
	{
		return current_delta_time_;
	}



	//Utility functions
	std::string GetEngineVersion()
	{
		return ENGINE_VERSION_STR;
	}
}
