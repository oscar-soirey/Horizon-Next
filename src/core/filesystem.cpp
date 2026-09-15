#include "filesystem.h"

#include <fstream>
#include <iostream>


static bool useRawFiles;

namespace hn
{
	void InitFilesystem(bool _useRawFiles)
	{
		useRawFiles = _useRawFiles;
	}

	std::string GetFileContent(const char *_path, size_t *_outSize, bool _forceRawFile)
	{

		std::filesystem::path p = _path;
		std::filesystem::path path = "Assets" / p;

		if (useRawFiles || _forceRawFile)
		{
			std::ifstream file(path, std::ios::binary | std::ios::ate);
			if (!file)
			{
				printf("Error while openning file (file doesn't exists) : \n");
				return "";
			}

			//lit le fichier et stocke la taille
			std::streamsize size = file.tellg();
			//revient au debut du fichier
			file.seekg(0, std::ios::beg);

			std::string content(size, '\0');
			if (!file.read(&content[0], size))
			{
				printf("Error while openning file (corrupted file) : \n");
				return "";
			}

			if (_outSize)
			{
				*_outSize = (size_t)size;
			}
			return content;
		}
		else
		{
			printf("Fonction pas encore implémentée\n");
			return "";
		}
	}


	std::string GetFileContent(std::filesystem::path _path, size_t *_outSize, bool _forceRawFile)
	{
		return GetFileContent(_path.c_str(), _outSize, _forceRawFile);
	}


	std::filesystem::path GetParentPath(const char *path)
	{
		std::filesystem::path asset_full_path(path);
		return asset_full_path.parent_path();
	}
}
