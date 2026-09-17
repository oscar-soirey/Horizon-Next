#include "common.h"

#include "window/main_window.h"

namespace
{
	hn::editor::EditorApplication* application=nullptr;
	hn::editor::EditorMain* main_window=nullptr;
}

namespace hn::editor
{
	void InitSetEditorApplication(EditorApplication *app)
	{
		if (application)
		{
			return;
		}
		application = app;
	}

	EditorApplication *GetApplication()
	{
		return application;
	}

	void InitSetMainWindow(EditorMain *window)
	{
		if (main_window)
		{
			return;
		}
		main_window = window;
	}

	EditorMain *GetMainWindow()
	{
		return main_window;
	}
}
