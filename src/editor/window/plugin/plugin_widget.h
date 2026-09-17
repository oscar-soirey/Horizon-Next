#pragma once

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include "../../../tools/network/plugin_entry.h"

namespace hn::editor
{
	class PluginWidget : public QWidget {
		Q_OBJECT
	public:
		PluginWidget(const tools::PluginEntry& entry, bool can_download);

	private:
		QLabel* m_iconLabel = nullptr;
		QLabel* m_nameLabel = nullptr;
		QLabel* m_categoryLabel = nullptr;
		QLabel* m_versionLabel = nullptr;
		QLabel* m_descriptionLabel = nullptr;
		QPushButton* m_downloadButton = nullptr;
		QPushButton* m_repairButton = nullptr;
	};
}
