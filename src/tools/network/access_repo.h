#pragma once

#include <string>
#include <vector>

#include "../../core/api/build_dll.h"

namespace hn::tools
{
	struct ENGINE_API PluginEntry {
		std::string name, category, description, downloadUrl, version, sha256;
	};

	ENGINE_API std::vector<PluginEntry> FetchPluginManifest(const std::string& manifestUrl);
	ENGINE_API int DownloadPlugin(const char* downloadUrl, const char* destPath);
}