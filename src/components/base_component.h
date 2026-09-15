#ifndef HGE_BASE_COMPONENT_H
#define HGE_BASE_COMPONENT_H

#include "../core/object.h"
#include "../core/api/build_dll.h"

namespace hge
{
	class HGE_Actor;

	class ENGINE_API HGE_Component : public HGE_Object {
	public:
		//allow actor to modify private members (util for modify parent_ member)
		friend class HGE_Actor;

		explicit HGE_Component(HGE_Actor* _parent);

		//ban the use of this method to use the constructor, or begin play
		void Init() final {}

	protected:
		HGE_Actor* parent_ = nullptr;
	};
}

#endif