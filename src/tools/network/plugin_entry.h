#pragma once

#include "../../core/api/build_dll.h"
#include <string>

namespace hn::tools
{
	struct ENGINE_API PluginEntry {
		std::string name, category, description, downloadUrl, version, sha256;
	};
}