#ifndef HGE_LIGHT_COMPONENT_H
#define HGE_LIGHT_COMPONENT_H

#include "scene_component.h"

struct BackendLight;

namespace hge
{
	class ENGINE_API HGE_Light : public HGE_SceneComponent {
	public:
		HGE_Vec3 color_;
		float attenuation_;
		float intensity_;

		explicit HGE_Light(HGE_Actor* _parent);
		~HGE_Light() override;

	private:
		BackendLight* backend_;

		void TransformModified() override;

		void ColorModified();
		void AttenuationModified();
		void IntensityModified();
	};
}

#endif