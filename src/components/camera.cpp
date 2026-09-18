#include "camera.h"

#include <core/actor.h>

#include "../core/gameplay/player_controller.h"

#include <Ogre.h>

#include "core/engine.h"
#include "core/Private/EngineRenderingBackend.h"

using namespace Ogre;

struct BackendCamera {
	//HRL_id cam;
	hge::Camera_Type_e type;
	float fov;
	float ortho_height;

	SceneNode* camNode=nullptr;
	Camera* cam=nullptr;
};


namespace hge
{
	HGE_Camera::HGE_Camera(HGE_Actor* _parent) : HGE_SceneComponent(_parent), backend_(new BackendCamera())
	{
		HPROPERTY(fov_ortho_height, Exposed, FovChanged());

		//backend_->cam = HRL_CreateCamera(parent_->BackendGetSceneID(), HRL_PERSPECTIVE);		parent_->ED_transform_modified.Subscribe([this](){ TransformModified(); });

		auto* renderer = (rendering_interface*)_parent->GetEngine()->GetRenderingBackend();
		backend_->camNode = renderer->scn_mng->getRootSceneNode()->createChildSceneNode();

		// create the camera
		backend_->cam = renderer->scn_mng->createCamera("myCam");
		backend_->cam->setNearClipDistance(5); // specific to this sample
		backend_->cam->setAutoAspectRatio(true);
		backend_->cam->setFOVy(Degree(70.f));
		backend_->camNode->attachObject(backend_->cam);
		backend_->camNode->setPosition(0, 47, 222);


		//pareil que avec la propriété type, exposer les propriétés
		backend_->type = Perspective;
		backend_->fov = 60.0f;
		backend_->ortho_height = 1000.f;
		//HRL_SetCameraPerspectiveFov(backend_->cam, backend_->fov);
		//HRL_SetCameraNearPlane(backend_->cam, 0.001f);
		//HRL_SetCameraFarPlane(backend_->cam, 10000.f);

		parent_->ED_transform_modified.Subscribe([this](){ TransformModified(); });
		parent_->ED_possess_state_changed.Subscribe([this](bool possessed, int pc) { PossessStateChanged(possessed, pc); });
	}

	HGE_Camera::~HGE_Camera()
	{
		//HRL_DeleteCamera(backend_->cam);
		delete backend_;
	}

	void HGE_Camera::SetType(Camera_Type_e _type)
	{/*
		HRL_ECameraType type = HRL_ORTHO;
		if (_type == Perspective) type = HRL_PERSPECTIVE;

		HRL_SetCameraType(backend_->cam, type);
		backend_->type = _type;
		if (_type == Perspective)
		{
			HRL_SetCameraPerspectiveFov(backend_->cam, backend_->fov);
		}
		else
		{
			//si y'a un bug ca vient peut etre du near plane, qui doit être en ortho négatif (-10000; 10000) exemple
			HRL_SetCameraOrthoVertical(backend_->cam, backend_->ortho_height);
		}*/
	}

	void HGE_Camera::FovChanged()
	{/*
		if (backend_->type == Orthographic)
		{
			HRL_SetCameraOrthoVertical(backend_->cam, fov_ortho_height);
		}
		else
		{
			HRL_SetCameraPerspectiveFov(backend_->cam, fov_ortho_height);
		}*/
	}

	void HGE_Camera::TransformModified()
	{
		//HRL_SetCameraLocation(backend_->cam, GetAbsoluteLocation().x, GetAbsoluteLocation().y, GetAbsoluteLocation().z);
		//HRL_SetCameraRotation(backend_->cam, GetAbsoluteRotation().x, GetAbsoluteRotation().y, GetAbsoluteRotation().z);
	}

	void HGE_Camera::PossessStateChanged(bool possessed, int p)
	{
		if (possessed)
		{
			//HRL_SetViewportCamera(GetPlayerViewportBackend(p), backend_->cam);
		}
		else
		{
			//HRL_SetViewportCamera(GetPlayerViewportBackend(p), HRL_INVALID_ID);
		}
	}
}