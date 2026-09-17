#include "fbx_importer.h"

#include <iostream>

#include <QApplication>

#include "editor/common.h"
#include "../../src/editor/window/main_window.h"
#include "editor/window/editor_settings.h"
#include "plugins/EngineInfos.h"

namespace hn::plugins
{
	FBXImporter::~FBXImporter()
	{
		std::cout << "Unregistered" << std::endl;
	}

	void FBXImporter::Init(EngineInfos *engine_infos)
	{
		auto* mainWindow = static_cast<editor::EditorMain*>(engine_infos->editor_main_window);
		hn::editor::AddSettingSection("FBX Importer", new editor::SettingSection("FBX Importer"));
	}

	void FBXImporter::Tick(double dt)
	{
	}
}
