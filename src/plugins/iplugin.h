#pragma once

#include <unordered_map>
#include <string>

#include "../core/api/build_dll.h"

#define HORIZON_PLUGIN(plugin_) extern "C" { \
__declspec(dllexport) hn::IPlugin* LoadPlugin() { return new plugin_(); }}


namespace hn
{
	struct EngineInfos;
	class EditorWindow;

	class ENGINE_API IPlugin {
	public:
		IPlugin();
		virtual ~IPlugin();

		virtual void Init(EngineInfos* engine_infos);
		virtual void Tick(double dt);

		virtual void StartGame();
		virtual void GameTick(double dt);
		virtual void EndGame();

		//Create multiple windows
		virtual std::unordered_map<std::string, EditorWindow> HandleWindow();
		virtual void HandleTitleBar();
		virtual void HandleToolbar();
	};
}