#ifndef HGE_SPRITE_COMPONENT_H
#define HGE_SPRITE_COMPONENT_H

#include "scene_component.h"
#include <string>

namespace hge
{
	class ENGINE_API HGE_Sprite : public HGE_SceneComponent {
	public:
		std::string material;

		explicit HGE_Sprite(HGE_Actor* _parent);
		~HGE_Sprite() override;

	private:
		struct Internal;
		Internal* backend_=nullptr;

		void MaterialModified();

		void TransformModified() override;
	};
}

#endif
