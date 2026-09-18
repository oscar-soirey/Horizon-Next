#pragma once

#include <filesystem>

namespace hn::tools
{
	namespace fs = std::filesystem;

	inline void CopyDirectoryContents(const fs::path& source, const fs::path& destination)
	{
		fs::create_directories(destination);
		for (const auto& entry : fs::directory_iterator(source))
		{
			const auto& srcPath = entry.path();
			const auto dstPath = destination / srcPath.filename();

			if (fs::is_directory(srcPath))
				CopyDirectoryContents(srcPath, dstPath); // récursif
			else
				fs::copy_file(srcPath, dstPath, fs::copy_options::overwrite_existing);
		}
	}
}