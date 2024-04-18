#include "Menustage.h"
#include "TextureManager.h"
#include "Engine.h"
#include "Input.h"
#include "Camera.h"
#include "PlayPK.h"
#include "Warrior.h"

Menustage* Menustage::s_Instance = nullptr;

Menustage::Menustage() {
	nutpause = {0, 80, 70, 70};
	Continue = { 182, 256, 598, 54 };
	NewGame = { 182, 322, 598, 54 };

	ModeStory = {189,391,200,42};
	ModePK = {571,391,200,42};

	LeaderBoards = { 182, 388, 598, 54 };

	Options = { 182, 454, 598, 54 };

	Credits = { 182, 520, 598, 54 };

	Exit = { 182, 586, 598, 54 };

	Yes = { 374, 516, 66, 24 };
	No = { 521, 516, 54, 24 };

	chuot = Input::GetInstance();
	IsMenu = true; IsMode = false; IsModePK = false;
	IsPlayStory = false;
	IsPlayPK = false;
	IsPauseP = false;
	IsPauseS = false;
	Cam = Vector2D(0, 0);
}

void Menustage::Draw() {
	TextureManager::GetInstance()->Draw("menu", Cam.X, 0, 960, 640);
	if (IsMode) TextureManager::GetInstance()->Draw("mode", 130 + Cam.X, 259, 700, 250);
	if (IsModePK) TextureManager::GetInstance()->Draw("ModePK", Cam.X, 0, 960, 640);
	if (IsPauseP || IsPauseS) TextureManager::GetInstance()->Draw("pause", Cam.X, 0, 960, 640);
	if (Engine::GetInstance()->Getover()) TextureManager::GetInstance()->Draw("over", Cam.X, 0, 960, 640);
}

void Menustage::Update() {
	Cam.X = Camera::GetInstance()->GetPosition().X;

	if (IsMenu) {
		if (chuot->ListenMouse(NewGame)) {
			IsMode = true;

		}
		if (chuot->ListenMouse(Exit)) Engine::GetInstance()->Quit();
	}

	if (IsMode || IsModePK || IsPauseS || IsPauseP) {
		IsMenu = true;
	}
	if (IsPlayStory || IsPlayPK) {
		IsMenu = false;
	}

	if (IsMode) {
		if (chuot->ListenMouse(ModeStory)) {
			IsPlayStory = true;
			IsMode = false;
		}
		if (chuot->ListenMouse(ModePK)) {
			IsModePK = true;
			IsMode = false;
		}
	}

	if ((Engine::GetInstance()->Getover())) {
		//IsMenu = true;
		IsModePK = false;
		IsMode = false;
		IsPlayStory = false;
		IsPlayPK = false;
		if (chuot->ListenMouse(Yes) || chuot->ListenMouse(No)) {
			IsMenu = true;
			IsPauseNewgame = true;
		}
	}
}

void Menustage::UpdateStory() {
	if (IsPlayStory) {
		IsMenu = false;
		IsPauseNewgame = false;
		if (chuot->ListenMouse(nutpause)) {
			IsPauseS = true;
			IsPlayStory = false;
			IsContinue = false;
		}
	}
	if (IsPauseS) {
		if (chuot->ListenMouse(Continue)) {
			IsPauseS = false;
			IsMenu = false;
			IsContinue = true;
			IsPlayStory = true;
		}
		if (chuot->ListenMouse(NewGame)) {
			IsPauseS = false;
			IsMenu = true;
			IsPlayStory = false;
			IsPlayPK = false;
			IsPauseNewgame = true;
		}
	}
	
}

void Menustage::UpdatePK() {
	if (IsModePK) {
		IsMenu = false;
		IsPauseNewgame = false;
		if (Engine::GetInstance()->GetPlayPK()->Checkback()) {
			IsMenu = true;
			IsModePK = false;
			IsPauseNewgame = true;
		}
		if (Engine::GetInstance()->GetPlayPK()->Checkstart()) {
			IsPlayPK = true;
			IsModePK = false;
		}
	}
	else {
		if (IsPlayPK) {
			IsMenu = false;
			if (chuot->ListenMouse(nutpause)) {
				IsPauseP = true;
				IsPlayPK = false;
				IsContinue = false;
			}
		}
		if (IsPauseP) {
			if (chuot->ListenMouse(Continue)) {
				IsPauseP = false;
				IsMenu = false;
				IsContinue = true;
				IsPlayPK = true;
			}
			if (chuot->ListenMouse(NewGame)) {
				IsPauseNewgame = true;
				IsPauseP = false;
				IsMenu = true;
				IsPlayStory = false;
				IsPlayPK = false;
			}
		}
	}
	if (IsModePK || IsPlayPK) {
		IsPlayStory = false;
	}
}

void Menustage::Clean() {
	TextureManager::GetInstance()->Drop("menu");
	TextureManager::GetInstance()->Drop("mode");
	TextureManager::GetInstance()->Drop("pause");
	TextureManager::GetInstance()->Drop("ModePK");
	TextureManager::GetInstance()->Drop("over");
}

void Menustage::Load() {
	TextureManager::GetInstance()->Load("menu", "LamGame/Picture/menu/menu1.png");
	TextureManager::GetInstance()->Load("mode", "LamGame/Picture/menu/mode.png");
	TextureManager::GetInstance()->Load("pause", "LamGame/Picture/menu/pause.png");
	TextureManager::GetInstance()->Load("ModePK", "LamGame/Picture/menu/PK.png");
	TextureManager::GetInstance()->Load("over", "LamGame/Picture/menu/over.png");
}