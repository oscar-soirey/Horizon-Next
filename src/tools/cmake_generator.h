#pragma once

#include <string>
#include <vector>

#include "../core/api/build_dll.h"

namespace hn::tools
{
	struct ENGINE_API GameCMakeConfig
	{
		std::string game_name;

		std::vector<std::string> source_files;

		std::string editor_dir;

		std::string engine_target = "horizon-next";

		std::string ogre_sdk_path =
				"S:/Programmation/Libraries/OgreSDK";

		std::string binary_directory =
				"${CMAKE_BINARY_DIR}";

		bool copy_ogre_dlls = true;
		bool copy_ogre_media = true;
		bool copy_engine_dll = true;
	};


	ENGINE_API std::string GenerateGameCMake(
			const GameCMakeConfig& config
	);
}