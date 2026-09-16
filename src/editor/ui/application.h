#pragma once

class QApplication;

namespace hn::editor
{
	class EditorApplication {
	public:
		EditorApplication(int argc, char *argv[]);
		QApplication* GetApplication();

	private:
		QApplication* app_=nullptr;
	};
}