#ifndef HGE_MODULE_H
#define HGE_MODULE_H

#include "factory.h"

#include <unordered_map>

#define HGE_MODULE_FUNCTION extern "C" __declspec(dllexport)

//utilise la factory créée dans HGE_LINK_MODULE
#define HGE_MODULE_REGISTER(__class__) __factory__.emplace(#__class__, []() -> hge::HGE_Object* { return new __class__(); })

#define HGE_LINK_MODULE(__module_content__) extern "C" __declspec(dllexport) void FactoryRegisterClasses() \
{ std::unordered_map<std::string, hge::factory::ObjectConstructor> __factory__; \
	__module_content__; \
	hge::gamefactory::InsertFactory(__factory__); } \
	\
	extern "C" __declspec(dllexport) void FactoryUnregisterClasses() \
	{ \
	}

#endif