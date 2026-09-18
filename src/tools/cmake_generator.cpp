#include "cmake_generator.h"

#include <sstream>
#include <stdexcept>
#include <filesystem>

namespace hn::tools
{
    namespace
    {
        void AppendLine(
            std::ostringstream& stream,
            const std::string& line = ""
        )
        {
            stream << line << '\n';
        }


        void AppendComment(
            std::ostringstream& stream,
            const std::string& comment
        )
        {
            AppendLine(stream);
            AppendLine(stream, "# ============================================================");
            AppendLine(stream, "# " + comment);
            AppendLine(stream, "# ============================================================");
            AppendLine(stream);
        }
    }


    std::string GenerateGameCMake(
        const GameCMakeConfig& config
    )
    {
        if (config.game_name.empty())
        {
            throw std::invalid_argument(
                "Game name cannot be empty"
            );
        }

        if (config.editor_dir.empty())
        {
            throw std::invalid_argument(
                "Editor directory cannot be empty"
            );
        }


        const std::string editor_dir =
            std::filesystem::path(config.editor_dir).generic_string();


        std::ostringstream cmake;


        // ============================================================
        // PROJECT
        // ============================================================

        AppendLine(cmake,
            "cmake_minimum_required(VERSION 3.21)"
        );

        AppendLine(cmake);

        AppendLine(cmake,
            "project(" + config.game_name + " LANGUAGES C CXX)"
        );

        AppendLine(cmake);

        AppendLine(cmake,
            "set(CMAKE_CXX_STANDARD 20)"
        );

        AppendLine(cmake,
            "set(CMAKE_CXX_STANDARD_REQUIRED ON)"
        );

        AppendLine(cmake);

        AppendLine(cmake,
            "set(CMAKE_CONFIGURATION_TYPES Release CACHE STRING \"\" FORCE)"
        );


        // ============================================================
        // ENGINE
        // ============================================================

        AppendComment(
            cmake,
            "ENGINE"
        );

        AppendLine(cmake,
            "include_directories("
            "\"${CMAKE_SOURCE_DIR}/Engine/Include\""
            ")"
        );

        AppendLine(cmake);


        // ============================================================
        // GAME LIBRARY
        // ============================================================

        AppendComment(
            cmake,
            "GAME LIBRARY"
        );

        AppendLine(cmake,
            "add_library(" +
            config.game_name +
            " SHARED"
        );

        for (const auto& file : config.source_files)
        {
            AppendLine(
                cmake,
                "   ${CMAKE_SOURCE_DIR}/src/" + file
            );
        }

        AppendLine(cmake,
            ")"
        );

        AppendLine(cmake);


        // ============================================================
        // ENGINE LINK
        // ============================================================

        AppendComment(
            cmake,
            "ENGINE LINK"
        );

        AppendLine(cmake,
            "target_link_libraries(" +
            config.game_name +
            " PRIVATE"
        );

        AppendLine(cmake,
            "   \"${CMAKE_SOURCE_DIR}/Engine/horizon-next.lib\""
        );

        AppendLine(cmake,
            ")"
        );

        AppendLine(cmake);


        // ============================================================
        // ENGINE FILES
        // ============================================================

        if (config.copy_engine_dll)
        {
            AppendComment(
                cmake,
                "ENGINE FILES"
            );

            AppendLine(cmake,
                "add_custom_command(TARGET " +
                config.game_name +
                " POST_BUILD"
            );

            AppendLine(cmake,
                "   COMMAND ${CMAKE_COMMAND} -E copy_directory"
            );

            AppendLine(cmake,
                "   \"" +
                editor_dir +
                "/Editor/AutoBuild-Files\""
            );

            AppendLine(cmake,
                "   \"$<TARGET_FILE_DIR:" +
                config.game_name +
                ">\""
            );

            AppendLine(cmake,
                ")"
            );
        }


        AppendLine(cmake);

        return cmake.str();
    }
}