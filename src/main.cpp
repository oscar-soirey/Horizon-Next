#include "core/engine.h"
#include "core/actor.h"
#include "core/level.h"
#include "modules/module.h"
#include "core/gameplay/player_controller.h"
#include "core/gameplay/input.h"
#include "core/log/log.h"
#include "modules/Private/SystemModule.h"

#include <iostream>

#include "scripting/lua_vm.h"


#include "tools/compilation/compilation.h"
#include "tools/network/access_repo.h"


#define DEBUG printf("%d\n", __LINE__)

int main()
{
	hn::Engine engine("default_engine.ini", true, true, nullptr, nullptr);

	std::cout << hn::GetEngineVersion() << std::endl;

	auto* module = new hn::Private::SysModule("example-game.dll");
	module->RegisterFactory();

	int player_controller = hge::CreatePlayer();

	engine.CreateLevel("scene.xml");

	engine.StartMainLoop();


	return 0;





	/*
	// also need to tell where we are
	SceneNode* camNode = scnMgr->getRootSceneNode()->createChildSceneNode();

	// create the camera
	Camera* cam = scnMgr->createCamera("myCam");
	cam->setNearClipDistance(5); // specific to this sample
	cam->setAutoAspectRatio(true);
	cam->setFOVy(Degree(70.f));
	camNode->attachObject(cam);
	camNode->setPosition(0, 47, 222);

	Ogre::ColourValue fadeColour(0.9, 0.9, 0.9);

	// and tell it to render into the main window
	ctx.getRenderWindow()->addViewport(cam)->setBackgroundColour(fadeColour);

	//fog
	scnMgr->setFog(Ogre::FOG_EXP2, fadeColour, 0.002);


	for (int i=0; i<0; i++)
	{
		Ogre::Entity* ninjaEntity =
			scnMgr->createEntity("scifi_girl_v.01.mesh");

		ninjaEntity->setCastShadows(true);

		SceneNode* n = scnMgr->getRootSceneNode()->createChildSceneNode();
		n->attachObject(ninjaEntity);
		n->setScale(Vector3(100.f, 100.f, 100.f));
		n->setPosition(Vector3(2.f*(float)i, 0.f, 2.f*(float)i));
		ninjaNode.push_back(n);
	}

	Ogre::Entity* character =
		scnMgr->createEntity("UnarmedWalkForward.mesh");
	Ogre::SceneNode* characterNode =
		scnMgr->getRootSceneNode()->createChildSceneNode();
	characterNode->attachObject(character);
	characterNode->setPosition(0, 0, 0);
	if (character->hasSkeleton())
	{
		Ogre::SkeletonInstance* skeleton =
				character->getSkeleton();

		Ogre::AnimationStateSet* animations =
				character->getAllAnimationStates();

		Ogre::AnimationStateIterator iterator =
				animations->getAnimationStateIterator();

		while (iterator.hasMoreElements())
		{
			Ogre::AnimationState* animation =
					iterator.getNext();

			std::cout << animation->getAnimationName() << "ANIM" << std::endl;
		}

		anim = character->getAnimationState("mixamo.com");
		anim->setEnabled(true);
		anim->setLoop(true);
		// Ton personnage possède une armature
	}



	//creer le sol
	Plane plane(Vector3::UNIT_Y, 0);
	MeshManager::getSingleton().createPlane(
				"ground", RGN_DEFAULT,
				plane,
				1500, 1500, 20, 20,
				true,
				1, 5, 5,
				Vector3::UNIT_Z);

	Entity* groundEntity = scnMgr->createEntity("ground");
	scnMgr->getRootSceneNode()->createChildSceneNode()->attachObject(groundEntity);
	groundEntity->setCastShadows(false);
	groundEntity->setMaterialName("Examples/Rockwall");

	scnMgr->setAmbientLight(ColourValue(0, 0, 0));
	scnMgr->setShadowTechnique(ShadowTechnique::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED);


	Light* directionalLight = scnMgr->createLight("DirectionalLight");
	directionalLight->setType(Light::LT_DIRECTIONAL);
	directionalLight->setDiffuseColour(ColourValue(0.4, 0.4, 0.4));
	directionalLight->setSpecularColour(ColourValue(0.4, 0.4, 0.4));
	SceneNode* directionalLightNode = scnMgr->getRootSceneNode()->createChildSceneNode();
	directionalLightNode->attachObject(directionalLight);
	directionalLightNode->setDirection(Vector3(0, -1, 1));

	scnMgr->setSkyBox(true, "Examples/CloudyNoonSkyBox", 300, false);
	//scnMgr->setSkyDome(true, "Examples/CloudySky", 40, 8);

	/*Ogre::Plane skyPlane;
	skyPlane.d = 1000;
	skyPlane.normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
	scnMgr->setSkyPlane(true, skyPlane, "Examples/SpaceSkyPlane", 1500, 75);


	//gere sa boucle de rendu
	MyFrameListener listener{ctx.getRenderWindow()};
	root->addFrameListener(&listener);

	root->startRendering();

	ctx.closeApp();

*/

}
