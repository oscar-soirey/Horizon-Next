#include "sprite.h"

#include "core/actor.h"

namespace hge
{
	struct HGE_Sprite::Internal {
		//HRL_id sprite;
	};

	HGE_Sprite::HGE_Sprite(HGE_Actor* _parent) : HGE_SceneComponent(_parent), backend_(new Internal())
	{
		HPROPERTY(material, Exposed, MaterialModified());

		//backend_->sprite = HRL_CreateMeshSprite(parent_->BackendGetSceneID());

		//subscribe to transform modified and physics mode changed
		parent_->ED_transform_modified.Subscribe([this]{ TransformModified(); });
	}

	HGE_Sprite::~HGE_Sprite()
	{
		//HRL_DeleteMesh(backend_->sprite);
		delete backend_;
	}

	void HGE_Sprite::TransformModified()
	{
		//HRL_SetMeshLocation(backend_->sprite, GetAbsoluteLocation().x,  GetAbsoluteLocation().y,  GetAbsoluteLocation().z);
		//HRL_SetMeshRotation(backend_->sprite, GetAbsoluteRotation().x, GetAbsoluteRotation().y, GetAbsoluteRotation().z);
		//HRL_SetMeshScale(backend_->sprite, GetAbsoluteScale().x, GetAbsoluteScale().y, GetAbsoluteScale().z);
	}

	void HGE_Sprite::MaterialModified()
	{
		//HRL_SetMeshMaterial(backend_->sprite, priv::runtime_ressources::AddMaterial(material.c_str()));
	}
}