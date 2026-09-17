#pragma once

#include <QDockWidget>
#include "../utils/editor_dispatcher.h"

class QPlainTextEdit;

namespace hn::editor
{
	class ConsoleWidget : public QDockWidget {
	public:
		explicit ConsoleWidget(QWidget* parent = nullptr);

	protected:
		void closeEvent(QCloseEvent* event) override;

		QPlainTextEdit* console_widget_=nullptr;

	private:
		const EditorDispatcher<bool>& ED_Visible_={};
	};
}
