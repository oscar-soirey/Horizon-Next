#pragma once

#include "scene_component.h"
#include <string>


namespace hn
{
	using namespace hge;

	class ENGINE_API HN_StaticMesh : public HGE_SceneComponent {
	public:
		std::string mesh_path;

		explicit HN_StaticMesh(HGE_Actor* _parent);
		~HN_StaticMesh() override;

	private:
		struct Internal;
		Internal* backend_=nullptr;

		void MeshPathChanged();
		void TransformModified() override;
	};
}