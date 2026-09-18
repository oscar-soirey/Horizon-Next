#pragma once

#include "../../ui/editor_window.h"
#include "plugin_widget.h"

namespace hn::editor
{
	class DownloadPluginWindow : public EditorWindow {
		Q_OBJECT
	public:
		DownloadPluginWindow();

	private:
		void PopulateEntries(const std::vector<tools::PluginEntry>& entries);
	};
}