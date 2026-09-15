#ifndef HGE_PLAYER_CONTROLLER_H
#define HGE_PLAYER_CONTROLLER_H

#include "../api/build_dll.h"
#include <cstdint>

namespace hge
{
	class HGE_Actor;

	/**
	 * Create player controller
	 * @return ID of the new PlayerController
	 */
	ENGINE_API int  CreatePlayer();
	ENGINE_API void DeletePlayer(int pc);
	ENGINE_API void PossessActor(int pc, HGE_Actor* act);
	ENGINE_API void UnpossessActor(int pc);
	ENGINE_API HGE_Actor* GetPossessedActor(int pc);

	//ajouter une gestion auto
	ENGINE_API void SetPlayerViewportSize(int pc,
		float x, float y,
		float _width, float _height
	);
	ENGINE_API uint32_t GetPlayerViewportBackend(int pc);

	ENGINE_API int GetPlayerCount();
}

#endif