#include <iostream>

#include "common.h"
#include "ui/application.h"
#include "ui/main_window.h"
#include "ui/title_bar.h"
#include "widgets/viewport.h"


class SettingsWindow : public QWidget
{
public:
	explicit SettingsWindow(QWidget *parent = nullptr)
			: QWidget(parent)
	{
		setWindowTitle("Settings");
		resize(500, 400);

		auto *layout = new QVBoxLayout(this);

		layout->addWidget(new QLabel("Settings"));
	}
};


int main(int argc, char *argv[])
{
	hn::editor::EditorApplication(argc, argv);

	auto* main_window = new hn::editor::EditorMain();
	hn::editor::InitSetMainWindow(main_window);

	hn::editor::GetMainWindow()->GetTabWidget()->addTab(
		new hn::editor::ViewportWidget(),
		"Main editor"
	);


	hn::editor::GetMainWindow()->show();

	while (!hn::editor::GetMainWindow()->ShouldClose())
	{
		QCoreApplication::processEvents();
	}

	return 0;
}