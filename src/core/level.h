//
// Created by User on 28/01/2026.
//

#ifndef LEVEL_H
#define LEVEL_H


#include <vector>
#include "api/build_dll.h"

namespace hn
{
	class Engine;
}

namespace hge
{
	class HGE_Actor;

	class ENGINE_API HGE_Level {
		friend class hn::Engine;

	public:
		HGE_Actor* SpawnActor(const char* _className);
		void DestroyActor(HGE_Actor* _act);


		HGE_Actor* GetActorFromID(const char* id);
		const std::vector<HGE_Actor*>& GetActors() const;
		int CountActorsOfClass(const char* _className) const;

	private:
		std::vector<HGE_Actor*> actors_;

		HGE_Level()=default;
		~HGE_Level();

		void LoadFromFile(const char* _path, hn::Engine* engine);
	};
}


#endif //LEVEL_H
