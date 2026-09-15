#include "compilation.h"

#include <cstdlib>
#include <cstdio>
#include <iostream>
#include <string>
#include <windows.h>
#include <filesystem>

extern "C" {
#include <lua/lua.h>
#include <lua/lauxlib.h>
#include <lua/lualib.h>
}

namespace fs = std::filesystem;



int RunCommand(const char* command, const char* working_directory);


#ifdef _WIN32
int RunCommand(
    const char* command,
    const char* working_directory
)
{
    SECURITY_ATTRIBUTES security_attributes{};
    security_attributes.nLength = sizeof(SECURITY_ATTRIBUTES);
    security_attributes.bInheritHandle = TRUE;

    HANDLE stdout_read = nullptr;
    HANDLE stdout_write = nullptr;

    if (!CreatePipe(
        &stdout_read,
        &stdout_write,
        &security_attributes,
        0))
    {
        return -1;
    }

    // Le processus enfant ne doit hériter que de l'écriture.
    SetHandleInformation(
        stdout_read,
        HANDLE_FLAG_INHERIT,
        0
    );

    STARTUPINFOA startup_info{};
    startup_info.cb = sizeof(STARTUPINFOA);
    startup_info.dwFlags |= STARTF_USESTDHANDLES;

    startup_info.hStdOutput = stdout_write;
    startup_info.hStdError = stdout_write;

    PROCESS_INFORMATION process_info{};

    char command_line[1024];
    strcpy_s(command_line, command);

    BOOL result = CreateProcessA(
        nullptr,
        command_line,
        nullptr,
        nullptr,
        TRUE,       // hérite des handles
        0,
        nullptr,
        working_directory,
        &startup_info,
        &process_info
    );

    if (!result)
    {
        CloseHandle(stdout_read);
        CloseHandle(stdout_write);

        return -1;
    }

    // Le parent n'a plus besoin de l'écriture.
    CloseHandle(stdout_write);
    stdout_write = nullptr;

    // Lire la sortie du processus
    char buffer[4096];
    DWORD bytes_read;

    while (ReadFile(
        stdout_read,
        buffer,
        sizeof(buffer) - 1,
        &bytes_read,
        nullptr
    ) && bytes_read > 0)
    {
        buffer[bytes_read] = '\0';

        std::cout << buffer;
    }

    CloseHandle(stdout_read);

    // Attendre que CMake soit terminé
    WaitForSingleObject(
        process_info.hProcess,
        INFINITE
    );

    DWORD exit_code = 0;

    GetExitCodeProcess(
        process_info.hProcess,
        &exit_code
    );

    CloseHandle(process_info.hProcess);
    CloseHandle(process_info.hThread);

    return static_cast<int>(exit_code);
}
#endif




namespace hn::tools
{
    int MakeArchive(const char *assets_path)
    {
        return -1;
    }

    int CompileLuaFiles(const char *assets_path)
    {
        //callback lua_Writer (lambda non-capturante => convertible en pointeur de fonction C)
        auto writer = [](lua_State* /*L*/, const void* p, size_t sz, void* ud) -> int {
            FILE* f = static_cast<FILE*>(ud);
            size_t written = fwrite(p, 1, sz, f);
            return (written != sz) ? 1 : 0;
        };

        fs::path assetsDir(assets_path);

        if (!fs::exists(assetsDir) || !fs::is_directory(assetsDir)) {
            std::cerr << "Dossier introuvable: " << assetsDir << "\n";
            return 1;
        }

        int total = 0, success = 0;

        for (const auto& entry : fs::recursive_directory_iterator(assetsDir)) {
            if (!entry.is_regular_file()) continue;
            if (entry.path().extension() != ".lua") continue;

            ++total;
            fs::path luaPath = entry.path();

            lua_State* L = luaL_newstate();
            if (!L) {
                std::cerr << "Impossible de creer l'etat Lua\n";
                continue;
            }
            luaL_openlibs(L);

            if (luaL_loadfile(L, luaPath.string().c_str()) != LUA_OK) {
                std::cerr << "Erreur de compilation " << luaPath << " : "
                          << lua_tostring(L, -1) << "\n";
                lua_close(L);
                continue;
            }

            fs::path outPath = luaPath;
            outPath.replace_extension(".luac");

            FILE* out = fopen(outPath.string().c_str(), "wb");
            if (!out) {
                std::cerr << "Impossible d'ouvrir " << outPath << " en ecriture\n";
                lua_close(L);
                continue;
            }

            int dumpResult = lua_dump(L, writer, out, /*strip=*/0);

            fclose(out);
            lua_close(L);

            if (dumpResult != 0) {
                std::cerr << "Erreur lors du dump de " << luaPath << "\n";
                fs::remove(outPath);
                continue;
            }

            std::cout << "OK: " << luaPath << " -> " << outPath << "\n";
            ++success;
        }

        std::cout << "\n" << success << "/" << total << " fichiers compiles avec succes.\n";
        return (success == total) ? 0 : 1;
    }


    int GenerateProjectFiles(const char *project_main_path, const char* generator)
    {
        std::filesystem::create_directories(
            std::filesystem::path(project_main_path) / "build"
        );

        std::string command =
            R"(cmake -S . -B build -G ")" +
            std::string(generator) +
            R"(" -A x64)";

        std::filesystem::path working_directory = std::filesystem::path(project_main_path) / "build";
        std::string working_directory_str = working_directory.string();

        int result = RunCommand( command.c_str(), project_main_path);
        std::cout << "Exit code: " << result << std::endl;
        return result;
    }

    int CompileGame(const char* project_main_path, int max_workers)
	{
	    std::string command = "cmake --build build --config Release --parallel " + std::to_string(max_workers);
        int result = RunCommand( command.c_str(), project_main_path);
        std::cout << "Exit code: " << result << std::endl;
        return result;
	}
}