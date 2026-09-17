#include <iostream>
#include <qcoreapplication.h>

#include "common.h"
#include "core/engine.h"
#include "ui/application.h"
#include "window/main_window.h"
#include "widgets/viewport.h"


class EditorLogger : public hn::Logger {
public:
	void RedirectMessages(const std::string &buffer) override
	{
		hn::editor::GetApplication()->AppendMessage(buffer.c_str());
	}
};


int main(int argc, char *argv[])
{
	auto* app = new hn::editor::EditorApplication(argc, argv);
	hn::editor::InitSetEditorApplication(app);

	auto* main_window = new hn::editor::EditorMain();
	hn::editor::InitSetMainWindow(main_window);

	hn::editor::GetMainWindow()->show();


	auto* editor_logger = new EditorLogger();
	hn::Engine engine("jfiz", true, true, editor_logger, nullptr, hn::editor::GetMainWindow());


	while (!hn::editor::GetMainWindow()->ShouldClose())
	{
		QCoreApplication::processEvents();
		engine.ProgressOneFrame(0.001);
	}

	delete main_window;
	delete app;

	return 0;
}