#pragma once

#ifdef EDITOR_BUILD
#define EDITOR_API __declspec(dllexport)
#else
#define EDITOR_API __declspec(dllimport)
#endif

namespace hn::editor
{
	class EditorApplication;
	class EditorMain;

	void InitSetEditorApplication(EditorApplication* app);
	EditorApplication* GetApplication();

	void InitSetMainWindow(EditorMain* window);
	EditorMain* GetMainWindow();
}