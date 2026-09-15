#ifndef HGE_BUILD_DLL
#define HGE_BUILD_DLL

#ifdef ENGINE_BUILD_DLL
#define ENGINE_API __declspec(dllexport)
#else
#define ENGINE_API __declspec(dllimport)
#endif

#endif