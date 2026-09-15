/**
 *
 */

#pragma once


#include <string>
#include "api/build_dll.h"
#include <filesystem>

namespace hn
{
	/**
	 * 
	 * @param _useRawFiles Use raw file or archive ?
	 */
	void ENGINE_API InitFilesystem(bool _useRawFiles);

	/**
	 * 
	 * @param _path Relative path from package folder or archive
	 * @param _outSize set the variable to file size (in byte)
	 * @param _forceRawFile force to use raw file, usefull for configuration files
	 * @return Content of the file
	 */
	std::string ENGINE_API GetFileContent(const char* _path, size_t* _outSize, bool _forceRawFile = false);
	std::string ENGINE_API GetFileContent(std::filesystem::path _path, size_t* _outSize, bool _forceRawFile = false);

	std::filesystem::path GetParentPath(const char* path);
}