#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>
#include <string>
#include <Windows.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "GameMap.h"

/// Screen
const int SCREEN_WIDTH  = 960;
const int SCREEN_HEIGHT = 640;


class Engine {
public:
	Engine() {};
	static Engine* GetInstance() {
		if (s_Instance != nullptr)
			return s_Instance;
		else
			return s_Instance = new Engine();
	}
	bool Init();
	bool Clean();
	void Quit();

	void Update();
	void Render();
	void Events();

	inline GameMap* GetMap() { return m_LevelMap; }
	inline bool IsRunning() { return m_IsRunning; }
	inline SDL_Renderer* GetRenderer() { return m_Renderer; }
private:
	bool m_IsRunning;

	GameMap* m_LevelMap;
	SDL_Window* m_Window;
	SDL_Renderer* m_Renderer;
	static Engine* s_Instance;
	
};


#endif