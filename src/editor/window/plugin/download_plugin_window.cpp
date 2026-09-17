#include "download_plugin_window.h"
#include "../../../tools/network/access_repo.h"

#include <QVBoxLayout>

namespace hn::editor
{
	DownloadPluginWindow::DownloadPluginWindow()
		: EditorWindow("Download Plugins")
	{
		resize(600, 400);

		std::vector<tools::PluginEntry> plugin_entries = tools::FetchPluginManifest("https://raw.githubusercontent.com/oscar-soirey/Horizon-Next/main/registry/plugin-registry.json");
		PopulateEntries(plugin_entries);
	}

	void DownloadPluginWindow::PopulateEntries(const std::vector<tools::PluginEntry>& entries)
	{
		for (const auto& entry : entries)
		{
			auto* widget = new PluginWidget(entry, central);
			layout->addWidget(widget);
		}

		layout->addStretch();
	}
}
