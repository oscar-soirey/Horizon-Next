#pragma once

#include <string>
#include <vector>

#include "plugin_entry.h"

namespace hn::tools
{

	ENGINE_API std::vector<PluginEntry> FetchPluginManifest(const std::string& manifestUrl);
	ENGINE_API std::string ExtractFileNameFromUrl(const std::string& url);
	ENGINE_API int DownloadPlugin(const char* downloadUrl, const char* destPath);
}