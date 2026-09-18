#include "common.h"

#include "window/main_window.h"

#include <filesystem>
#include <fstream>

#include <nlohmann/json.hpp>

#include "modules/Private/SystemModule.h"
#include "tools/cmake_generator.h"
#include "tools/compilation/compilation.h"
#include "window/loading_window.h"

namespace
{
	hn::editor::EditorRessources* ressources= new hn::editor::EditorRessources();
}

namespace hn::editor
{
	EditorRessources* GetEditorRessources()
	{
		return ressources;
	}

	void InitSetEditorApplication(EditorApplication *app)
	{
		if (ressources->application)
		{
			return;
		}
		ressources->application = app;
	}

	EditorApplication *GetApplication()
	{
		return ressources->application;
	}

	void InitSetMainWindow(EditorMain *window)
	{
		if (ressources->main_window)
		{
			return;
		}
		ressources->main_window = window;
	}

	EditorMain *GetMainWindow()
	{
		return ressources->main_window;
	}


	int OpenProjectRessource(const char *path)
	{
		auto* loading_window = new LoadingWindow();
		loading_window->show();

		delete ressources->current_project;

		namespace fs = std::filesystem;

		fs::path project_path = path;
		fs::path build_path = project_path / "build";
		fs::path ressources_path = project_path / "content";
		fs::path src_path = project_path / "src";

		//Placeholder
		fs::path project_main_file = project_path / "game.json";

		if (!exists(project_main_file))
		{
			std::cout << "Error while loading project" << std::endl;
			return -1;
		}

		std::string main_file_content = EditorGetFileContent(project_main_file);
		const nlohmann::json j = nlohmann::json::parse(main_file_content);

		const bool auto_cmake = j.at("auto-cmake").get<bool>();
		fs::path cmakelists_file = project_path / "CMakeLists.txt";
		if (auto_cmake)
		{
			if (!exists(cmakelists_file))
			{
				//Create CMakeLists.txt and populate it
				std::ofstream file(cmakelists_file);

				if (!file)
				{
					std::cout << "error while trying to create CMakeLists.txt file" << std::endl;
					return -1;
				}

				file << tools::GenerateGameCMake({"MyNewGame", {"module.cpp"},fs::current_path().string()});
			}
			else
			{
				std::ofstream file(cmakelists_file);

				if (!file)
				{
					std::cout << "error while trying to open CMakeLists.txt file" << std::endl;
					return -1;
				}

				file << tools::GenerateGameCMake({"MyNewGame", {"module.cpp"}, fs::current_path().string()});
			}
		}
		else
		{
			if (!exists(cmakelists_file))
			{
				std::cout << "Project loading error : cmake auto is false but CMakeLists.txt was not found at the root branch" << std::endl;
				return -1;
			}
		}


		if (!exists(build_path))
		{
			std::string p_str = project_path.string();

			//build n'existe pas, on va génerer et compiler
			if (tools::GenerateProjectFiles(p_str.c_str()) != 0)
			{
				std::cout << "Error while generating project files" << std::endl;
				return -1;
			}

			if (tools::CompileGame(p_str.c_str(), 6) != 0)
			{
				std::cout << "Error while generating compiling project" << std::endl;
				return -1;
			}
		}

		fs::path shared_path;
#ifdef _WIN32
		//Charger la dll et verifier si elle est valide
		 shared_path = build_path / "Release" / std::string(std::string("MyNewGame") + ".dll");
#endif

		std::string shared_path_str = shared_path.string();
		auto* module = new hn::Private::SysModule(shared_path_str.c_str());
		if (!module)
		{
			std::cout << "Module loading error" << std::endl;
			return -2;
		}
		module->RegisterFactory();

		ressources->current_project = new CommonProject(path, ressources_path.string());

		//4 charger le dossier Assets
		ressources->current_project->ressources_path = ressources_path.string();

		loading_window->close();

		return 0;
	}

	CommonProject* GetCurrentProject()
	{
		return ressources->current_project;
	}


	std::string EditorGetFileContent(const std::filesystem::path& path)
	{
		std::ifstream file(path, std::ios::in | std::ios::binary);
		if (!file)
		{
			std::cout << "Failed to open file: " << path.string() << std::endl;
			return {};
		}

		std::ostringstream buffer;
		buffer << file.rdbuf();
		return buffer.str();
	}
}
