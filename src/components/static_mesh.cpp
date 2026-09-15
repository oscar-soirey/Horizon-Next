#include "static_mesh.h"

#include <Ogre.h>

#include "core/actor.h"
#include "core/engine.h"
#include "../core/Private/EngineRenderingBackend.h"

namespace hn
{
	struct HN_StaticMesh::Internal {
		Ogre::Entity* entity=nullptr;
		Ogre::SceneNode* n=nullptr;
	};

	HN_StaticMesh::HN_StaticMesh(HGE_Actor* _parent):
	HGE_SceneComponent(_parent),
	backend_(new Internal())
	{}

	void HN_StaticMesh::MeshPathChanged()
	{
		auto* renderer = (rendering_interface*)parent_->GetEngine()->GetRenderingBackend();
		//Ouvrir ensuite avec mon propre filesystem
		backend_->entity = renderer->scn_mng->createEntity(mesh_path);
		backend_->entity->setCastShadows(true);
		backend_->n = renderer->scn_mng->getRootSceneNode()->createChildSceneNode();
		backend_->n->attachObject(backend_->entity);
		backend_->n->setScale(Ogre::Vector3(100.f, 100.f, 100.f));
		backend_->n->setPosition(Ogre::Vector3(0,0,0));
	}

	HN_StaticMesh::~HN_StaticMesh()
	{
		auto* renderer = (rendering_interface*)parent_->GetEngine()->GetRenderingBackend();
		renderer->scn_mng->destroySceneNode(backend_->n);
		renderer->scn_mng->destroyEntity(backend_->entity);
		delete backend_;
	}

	void HN_StaticMesh::TransformModified()
	{
		//Modifier le calcul de transform
		backend_->n->setPosition(Ogre::Vector3(relative_transform.location_.x,0,0));
	}
}
