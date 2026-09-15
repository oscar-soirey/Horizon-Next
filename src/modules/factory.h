#ifndef HGE_FACTORY_H
#define HGE_FACTORY_H

#include "../core/object.h"

#include <unordered_map>
#include <string>
#include <functional>

namespace hge
{

	namespace factory
	{
		using ObjectConstructor = std::function<HGE_Object*()>;
		using Factory = std::unordered_map<std::string, ObjectConstructor>;
	}

	namespace gamefactory
	{
		//object factory
		factory::ObjectConstructor ENGINE_API GetObjectConstructor(const char* _name);
		factory::Factory ENGINE_API GetFactory();
		void ENGINE_API InsertFactory(factory::Factory _moduleFactory);
		void ENGINE_API ClearFactory();
	}

}

#endif