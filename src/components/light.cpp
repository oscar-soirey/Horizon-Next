#include "light.h"

#include <core/actor.h>
#include <Ogre.h>

#include "core/engine.h"
#include "core/Private/EngineRenderingBackend.h"

using namespace Ogre;

struct BackendLight {
	//HRL_id light = HRL_INVALID_ID;  //default value
	Light* light=nullptr;
	SceneNode* lightNode=nullptr;
};

namespace hge
{
	HGE_Light::HGE_Light(HGE_Actor* _parent) : HGE_SceneComponent(_parent), backend_(new BackendLight())
	{
		HPROPERTY(color_, Exposed, ColorModified());
		HPROPERTY(attenuation_, Exposed, AttenuationModified());
		HPROPERTY(intensity_, Exposed, IntensityModified());


		auto* renderer = (rendering_interface*)_parent->GetEngine()->GetRenderingBackend();
		backend_->light = renderer->scn_mng->createLight("MainLight");
		backend_->lightNode = renderer->scn_mng->getRootSceneNode()->createChildSceneNode();
		backend_->lightNode->attachObject(backend_->light);

		//create HRL light
		//backend_->light = HRL_CreateLight(parent_->BackendGetSceneID(), HRL_POINT_LIGHT);

		//subscribe to transfrorm modified
		parent_->ED_transform_modified.Subscribe([this](){ TransformModified(); });
	}

	HGE_Light::~HGE_Light()
	{
		//HRL_DeleteLight(backend_->light);
		delete backend_;
	}


	void HGE_Light::TransformModified()
	{/*
		if (backend_->light == HRL_INVALID_ID)
		{
			LOG_ERROR("Try to call HGE_Light::LocationModified, but Init was never called\n");
			return;
		}
		HRL_SetLightLocation(backend_->light,
			GetAbsoluteLocation().x,
			GetAbsoluteLocation().y,
			GetAbsoluteLocation().z
			);
		HRL_SetLightRotation(backend_->light,
			GetAbsoluteRotation().x,
			GetAbsoluteRotation().y,
			GetAbsoluteRotation().z
		);*/

		backend_->lightNode->setPosition(
			GetAbsoluteLocation().x,
			GetAbsoluteLocation().y,
			GetAbsoluteLocation().z
		);
	}

	void HGE_Light::ColorModified()
	{
		//HRL_SetLightColor(backend_->light, color_.x, color_.y, color_.z);
	}

	void HGE_Light::AttenuationModified()
	{
		//HRL_SetLightAttenuation(backend_->light, attenuation_);
	}

	void HGE_Light::IntensityModified()
	{
		//HRL_SetLightIntensity(backend_->light, intensity_);
	}
}