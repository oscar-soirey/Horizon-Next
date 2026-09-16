#include "common.h"

#include "ui/main_window.h"

namespace
{
	hn::editor::EditorMain* main_window=nullptr;
}

namespace hn::editor
{
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
