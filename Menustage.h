#ifndef MENUSTAGE_H
#define MENUSTAGE_H
#include "Vector2D.h"
#include "SDL.h"
#include "Input.h"

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

	bool GetPauseS() { return IsPauseS; }
	bool GetPauseP() { return IsPauseP; }

	bool GetContinue() { return IsContinue; }
	bool GetNewgame() { return IsPauseNewgame; }

	int GetVolume() { return volume; }
	int Getismua() { return ismua; }

	bool GetIsbxh() { return Isbxh; }

private:
	static Menustage* s_Instance;
	Input* chuot;

	SDL_Rect Continue;
	SDL_Rect nutpause;
	SDL_Rect NewGame;
	SDL_Rect LeaderBoards; SDL_Rect OK2;
	SDL_Rect Options; SDL_Rect Op1; SDL_Rect OK;
	SDL_Rect Credits;
	SDL_Rect Exit;
	SDL_Rect ModeStory; SDL_Rect ModePK;
	SDL_Rect Yes;
	SDL_Rect No;
	SDL_Rect mua;

private:
	bool IsMenu; bool IsMode; bool IsModePK;
	bool IsPlayStory;
	bool IsPlayPK;
	bool IsPauseS;
	bool IsPauseP;
	bool IsContinue;
	bool IsPauseNewgame;
	bool IsOption;
	bool Isbxh;

	int volume;
	int gold; bool ismua;
	Vector2D Cam;
};
#endif