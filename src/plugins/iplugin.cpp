#include "iplugin.h"

#include "EngineInfos.h"

namespace hn
{
	class EditorWindow{};

	IPlugin::IPlugin()
	= default;

	IPlugin::~IPlugin()
	= default;

	void IPlugin::Init(EngineInfos *engine_infos)
	{

	}

	void IPlugin::Tick(double dt)
	{

	}

	void IPlugin::StartGame()
	{

	}

	void IPlugin::EndGame()
	{

	}

	void IPlugin::GameTick(double dt)
	{

	}

	std::unordered_map<std::string, EditorWindow> IPlugin::HandleWindow()
	{
		return {};
	}

	void IPlugin::HandleTitleBar()
	{

	}

	void IPlugin::HandleToolbar()
	{

	}
}
