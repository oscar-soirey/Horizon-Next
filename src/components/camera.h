#ifndef HGE_CAMERA_COMPONENT_H
#define HGE_CAMERA_COMPONENT_H

#include "scene_component.h"

struct BackendCamera;

namespace hge
{
	typedef enum {
		Perspective,
		Orthographic
	}Camera_Type_e;

	class ENGINE_API HGE_Camera : public HGE_SceneComponent {
	public:
		float fov_ortho_height;

		//exposer cette propriété (revoir les enums et structures dans la reflexion)
		Camera_Type_e type;

		explicit HGE_Camera(HGE_Actor* _parent);
		~HGE_Camera() override;

		void SetType(Camera_Type_e _type);

	private:
		BackendCamera* backend_;

		void FovChanged();

		void TransformModified() override;
		void PossessStateChanged(bool possessed, int p);
	};
}

#endif