#ifndef MENUSTAGE_H
#define MENUSTAGE_H
#include "Vector2D.h"
#include "SDL.h"
#include "Input.h"
#include "Vector2D.h"

class Menustage
{
public:
	Menustage();

	static Menustage* GetInstance() {
		if (s_Instance != nullptr)
			return s_Instance;
		else
			return s_Instance = new Menustage();
	}

	void Draw();
	void Clean();
	void Update();
	void Load();

	void UpdateStory();
	void UpdatePK();

	bool GetIsMenu() { return IsMenu; }
	bool GetIsMode() { return IsMode; }

	bool GetStory() { return IsPlayStory; }
	bool GetModePK() { return IsModePK; }
	bool GetPK() { return IsPlayPK; }

	bool GetPause() { return IsPause; }
	bool GetContinue() { return IsContinue; }
	bool GetNewgame() { return IsPauseNewgame; }

private:
	static Menustage* s_Instance;
	Input* chuot;

	SDL_Rect Continue;
	SDL_Rect nutpause;
	SDL_Rect NewGame;
	SDL_Rect LeaderBoards;
	SDL_Rect Options;
	SDL_Rect Credits;
	SDL_Rect Exit;
	SDL_Rect ModeStory; SDL_Rect ModePK;

private:
	bool IsMenu; bool IsMode; bool IsModePK;
	bool IsPlayStory;
	bool IsPlayPK;
	bool IsPause;
	bool IsContinue;
	bool IsPauseNewgame;
	Vector2D Cam;
};
#endif