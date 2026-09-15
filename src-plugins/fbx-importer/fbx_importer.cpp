#include "fbx_importer.h"

#include <iostream>

namespace hn::plugins
{
	FBXImporter::~FBXImporter()
	{
		std::cout << "Unregistered" << std::endl;
	}

	void FBXImporter::Init(EngineInfos *engine_infos)
	{
		std::cout << "fbx importer loaded" << std::endl;
	}

	void FBXImporter::Tick(double dt)
	{
	}
}
