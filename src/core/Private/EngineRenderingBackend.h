#pragma once

#include <vector>
#include <Ogre.h>
#include <Bites/OgreApplicationContext.h>
#include <Bites/OgreTrays.h>


#include "../../plugins/Private/SystemPlugin.h"


class HNKeyHandler;
class OgreLogRedirector;
class HNFrameListener;

namespace hn { class Engine; }




struct ImportedPlugin {
	std::string category;
	std::string path;
	//ressources=nullptr;
	hn::Private::SysPlugin* sys_plugin=nullptr;
};


//Viewport messages
struct viewport_labels_t {
	OgreBites::Label* label=nullptr;
	double time_remaning=1.0;
};


struct rendering_interface {
hn::Engine* engine=nullptr;

	OgreBites::ApplicationContext ctx{};
	Ogre::Root* root=nullptr;
	Ogre::SceneManager* scn_mng=nullptr;

	Ogre::OverlaySystem* overlay_system=nullptr;

	HNKeyHandler* key_handler=nullptr;
	HNFrameListener* listener=nullptr;

	//Logging
	Ogre::LogManager* log_manager=nullptr;
	OgreLogRedirector* ogre_log_redirector=nullptr;
	Ogre::Log* default_log=nullptr;

	//Debug UI
	OgreBites::TrayManager* tray_mgr = nullptr;
	std::vector<viewport_labels_t> viewport_labels;  //each print to viewport log messages

	rendering_interface(hn::Engine* engine);

	~rendering_interface();

	void log_to_viewport(const char* msg, double time);
};
