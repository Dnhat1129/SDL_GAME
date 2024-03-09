#ifndef COLLISIONHANDLER_H
#define COLLISIONHANDLER_H

#include "SDL.h"
#include <vector>
#include "TileLayer.h"
#include "GameMap.h"

class CollisionHandler
{
	public: 
		bool CheckCollision(SDL_Rect a, SDL_Rect b);
		bool MapCollision(SDL_Rect a);

		inline static CollisionHandler* GetInstance() {
			if (s_Instance != nullptr)
				return s_Instance;
			else
				return s_Instance = new CollisionHandler();
		}
		
	private:
		CollisionHandler();
		TileMap m_CollisionTilemap;
		TileLayer* m_CollisionLayer;
		static CollisionHandler* s_Instance;


};

#endif // !COLLISIONHANDER_H


