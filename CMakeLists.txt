cmake_minimum_required(VERSION 3.21)
project(horizon-next
		LANGUAGES C CXX
)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
	add_compile_options($<$<COMPILE_LANGUAGE:C>:/std:c11>)
endif()

set(CMAKE_MESSAGE_LOG_LEVEL DEBUG)

include_directories(src)
include_directories(third-party)

set(OGRE_DIR "S:/Programmation/Libraries/OgreSDK/CMake")

message(STATUS "OGRE_DIR = ${OGRE_DIR}")
message(STATUS "${OGRE_STATIC}")
message(STATUS "${OGRE_PLUGIN_DIR}")
message(STATUS "${OGRE_MEDIA_DIR}")
message(STATUS "${OGRE_CONFIG_DIR}")

find_package(OGRE REQUIRED COMPONENTS Bites CONFIG)


file(GLOB LUA_SOURCE
		"${CMAKE_SOURCE_DIR}/third-party/lua/*.c"
)
#Remove lua main functions
list(REMOVE_ITEM LUA_SOURCE
		"${CMAKE_SOURCE_DIR}/third-party/lua/lua.c"
		"${CMAKE_SOURCE_DIR}/third-party/lua/luac.c"
)


# ============================================================
# HORIZON ENGINE
# ============================================================

add_library(horizon-next SHARED
		#Dependencies ============================================
		third-party/inih/ini.c
		third-party/tinyxml2/tinyxml2.cpp
		${LUA_SOURCE}

		#Source ==================================================
		src/core/engine.cpp
		src/core/filesystem.cpp
		src/core/level.cpp
		src/core/common.cpp
		src/core/object.cpp
		src/core/actor.cpp
		src/core/character.cpp
		src/core/data/ini_parser.cpp
		src/core/Private/input_manager.cpp
		src/core/Private/EngineRenderingBackend.cpp
		src/core/gameplay/input.cpp
		src/core/gameplay/player_controller.cpp
		src/core/log/redirect.cpp
		src/core/log/log.cpp

		#components
		src/components/base_component.cpp
		src/components/scene_component.cpp
		src/components/sprite.cpp
		src/components/light.cpp
		src/components/camera.cpp
		src/components/shapes/shape.cpp
		src/components/shapes/box.cpp
		src/components/shapes/capsule.cpp
		src/components/shapes/circle.cpp
		src/components/static_mesh.cpp

		#plugin internal
		src/plugins/iplugin.cpp
		src/plugins/Private/SystemPlugin.cpp

		#factory
		src/modules/factory.cpp
		src/modules/Private/SystemModule.cpp

		#scripting
		src/scripting/lua_vm.cpp
)

target_link_libraries(horizon-next
	PRIVATE
		OgreMain
		OgreBites
)


target_compile_definitions(horizon-next PRIVATE
		ENGINE_BUILD_DLL
		LUA_BUILD_AS_DLL
)

#engine version control
target_compile_definitions(horizon-next
	PRIVATE
		ENGINE_VERSION_YEAR=2026
		ENGINE_VERSION_MAJOR=1
		ENGINE_VERSION_MINOR=0
		ENGINE_VERSION_HINT="beta"
		ENGINE_VERSION_STR="Horizon-Next 2026.1.0 beta"
)


# ============================================================
# PLUGINS
# ============================================================


# FBX IMPORTER ===============================================
add_library(fbx-importer SHARED
		src-plugins/fbx-importer/fbx_importer.cpp
)

target_link_libraries(fbx-importer
	PRIVATE
		horizon-next
)

set_target_properties(fbx-importer PROPERTIES
		RUNTIME_OUTPUT_DIRECTORY
		"${CMAKE_BINARY_DIR}/Plugins/Import/FbxImporter"
)



# ============================================================
# GAME EXAMPLE SYMBOLS
# ============================================================

add_library(example-game SHARED
		game/src/module.cpp
		game/src/player.cpp
)

target_link_libraries(example-game
	PRIVATE
		horizon-next
)



# ============================================================
# GAME EXECUTABLE
# ============================================================
add_executable(game
	src/main.cpp


		###Placeholder
		src/editor/compilation/compilation.cpp
)

target_link_libraries(game
		PRIVATE
		horizon-next
)



# ============================================================
# EDITOR EXECUTABLE
# ============================================================
add_executable(editor
		src/editor/main.cpp
		src/editor/compilation/compilation.cpp
)

target_link_libraries(editor
		PRIVATE
		horizon-next
)




# ============================================================
# OGRE FILES
# ============================================================

file(GLOB OGRE_DLLS
		"S:/Programmation/Libraries/OgreSDK/bin/*.dll"
)

# DLLs OGRE à côté de l'exécutable
add_custom_command(TARGET example-game POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		${OGRE_DLLS}
		"$<TARGET_FILE_DIR:example-game>"
)

# Media OGRE à côté de l'exécutable
add_custom_command(TARGET example-game POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"S:/Programmation/Libraries/OgreSDK/Media"
		"$<TARGET_FILE_DIR:example-game>/Media"
)


# ============================================================
# ENGINE DLL À CÔTÉ DU GAME.EXE
# ============================================================

add_custom_command(TARGET horizon-next POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"$<TARGET_FILE:horizon-next>"
		"$<TARGET_FILE_DIR:example-game>"
)


# ============================================================
# PLUGINS
# ============================================================

add_custom_command(TARGET fbx-importer POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory
		"$<TARGET_FILE_DIR:example-game>/Plugins/Import/FBXImporter"

		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		"$<TARGET_FILE:fbx-importer>"
		"$<TARGET_FILE_DIR:example-game>/Plugins/Import/FBXImporter"
)