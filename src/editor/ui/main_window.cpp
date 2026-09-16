#include "main_window.h"

namespace hn::editor
{
	EditorMain::EditorMain()
	{
		//Configuration

		setWindowFlags(Qt::FramelessWindowHint | Qt::Window);

		resize(1300, 900);

		auto *central = new QWidget();
		auto *layout = new QVBoxLayout(central);

		layout->setContentsMargins(0, 0, 0, 0);
		layout->setSpacing(0);

		// =========================
		// Title bar
		// =========================

		auto *titleBar = new TitleBar(this);

		layout->addWidget(titleBar);

		// =========================
		// Tabs
		// =========================

		tabs_ = new QTabWidget();
		layout->addWidget(tabs_);

		setCentralWidget(central);
	}


	bool EditorMain::ShouldClose() const
	{
		return should_close_;
	}

	QTabWidget *EditorMain::GetTabWidget() const
	{
		return tabs_;
	}

	void EditorMain::closeEvent(QCloseEvent *event)
	{
		should_close_ = true;
		event->accept();
	}
}
