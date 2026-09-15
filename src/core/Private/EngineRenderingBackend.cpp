#include "EngineRenderingBackend.h"

#include "input_manager.h"
#include "../engine.h"
#include "../log/redirect.h"
#include "../../scripting/lua_vm.h"
#include "../actor.h"
#include "../level.h"
#include "../../plugins/iplugin.h"




class HNKeyHandler : public OgreBites::InputListener
{
public:
	bool keyPressed(const OgreBites::KeyboardEvent& evt) override
	{
		if (evt.keysym.sym == OgreBites::SDLK_ESCAPE)
		{
			Ogre::Root::getSingleton().queueEndRendering();
		}
		return true;
	}
};


class HNFrameListener : public Ogre::FrameListener
{
public:
	Ogre::RenderWindow* window;
	hn::Engine* engine;
	OgreBites::TrayManager* tray_mgr;
	std::vector<viewport_labels_t>& viewport_labels;

	HNFrameListener(Ogre::RenderWindow* window, hn::Engine* engine, OgreBites::TrayManager* tray_mgr, std::vector<viewport_labels_t>& viewport_labels)
			: window(window), engine(engine), tray_mgr(tray_mgr), viewport_labels(viewport_labels)
	{
		window->setVSyncEnabled(true);
	}

	bool frameStarted(const Ogre::FrameEvent& evt) override
	{
		engine->current_delta_time_ = evt.timeSinceLastFrame;


		//refresh les debug messages sur le viewport
		for (auto it = viewport_labels.begin(); it != viewport_labels.end(); )
		{
			it->time_remaning -= evt.timeSinceLastFrame;
			if (it->time_remaning <= 0.0)
			{
				tray_mgr->destroyWidget(it->label);
				it = viewport_labels.erase(it);
			}
			else
			{
				++it;
			}
		}
		tray_mgr->frameRendered(evt);


		//Call lua Update global function
		engine->GetLuaVM()->Update(evt.timeSinceLastFrame);



		//update physics (before update actors)
		if (engine->game_tick_enabled_)
		{
			//call player controller tick (internal hidden function)
			//calls process input actor method (called before actor and physics update)
			PlayerControllersTick(evt.timeSinceLastFrame);

			//update physics, before actor ticks and update
			//physics::UpdateWorld(_deltatime);
		}

		if (engine->current_opened_level_)
		{
			for (const auto& a : engine->current_opened_level_->GetActors())
			{
				a->Update(evt.timeSinceLastFrame);
			}
		}

		if (engine->game_tick_enabled_)
		{
			//update actors
			if (engine->current_opened_level_)
			{
				for (const auto& a : engine->current_opened_level_->GetActors())
				{
					a->Tick(evt.timeSinceLastFrame);
				}
			}

			//reset just pressed keys (called after actors update)
			hge::priv::input::Tick();
		}


		for (const auto& [n, p]: engine->plugins_)
		{
			p->sys_plugin->GetPlugin()->Tick(evt.timeSinceLastFrame);
			if (engine->game_tick_enabled_)
			{
				p->sys_plugin->GetPlugin()->GameTick(evt.timeSinceLastFrame);
			}
		}


		return true;
	}
};



rendering_interface::rendering_interface(hn::Engine* engine): engine(engine)
{
	//créer le LogManager et le log avant Root, pour attacher le listener tôt
	log_manager = new Ogre::LogManager();
	default_log = log_manager->createLog("Ogre.log", true, false, false);
	// dernier param "false" = pas de suppress file output; mets true si tu veux zéro fichier Ogre.log

	ogre_log_redirector = new OgreLogRedirector(engine);
	default_log->addListener(ogre_log_redirector);

	ctx.initApp();

	//Creer une scene par défaut, a supprimer plus tard
	ctx.createDummyScene();

	key_handler = new HNKeyHandler();
	ctx.addInputListener(key_handler);



	// get a pointer to the already created root
	root = ctx.getRoot();
	scn_mng = root->createSceneManager();




	//Placeholder
	scn_mng->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));




	// register our scene with the RTSS
	Ogre::RTShader::ShaderGenerator* shadergen = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
	shadergen->addSceneManager(scn_mng);


	//Placeholder

	// without light we would just get a black screen
	Ogre::Light* light = scn_mng->createLight("MainLight");
	Ogre::SceneNode* lightNode = scn_mng->getRootSceneNode()->createChildSceneNode();
	lightNode->attachObject(light);
	lightNode->setPosition(20, 80, 50);



	//Create overlay system for debug
	overlay_system = Ogre::OverlaySystem::getSingletonPtr();
	if (!overlay_system)
	{
		overlay_system = new Ogre::OverlaySystem();
	}
	scn_mng->addRenderQueueListener(overlay_system);

	tray_mgr = new OgreBites::TrayManager("HNTrayMgr", ctx.getRenderWindow());
	ctx.addInputListener(tray_mgr);

	listener = new HNFrameListener(ctx.getRenderWindow(), engine, tray_mgr, viewport_labels);
	root->addFrameListener(listener);
}

rendering_interface::~rendering_interface()
{
	delete overlay_system;

	root->destroySceneManager(scn_mng);
	scn_mng=nullptr;
	root->shutdown();
	scn_mng=nullptr;

	default_log->removeListener(ogre_log_redirector);
	delete ogre_log_redirector;
	delete log_manager;
}

void rendering_interface::log_to_viewport(const char* msg, double time)
{
	//generer un id qui s'icrémente a chaque call
	static int label_counter = 0;
	std::string unique_name = "LogLabel_" + std::to_string(label_counter++);

	auto* label = tray_mgr->createLabel(OgreBites::TL_TOPLEFT, unique_name, msg, 200);
	viewport_labels.push_back({label, time});
	tray_mgr->moveWidgetToTray(label, OgreBites::TL_TOPLEFT);
}