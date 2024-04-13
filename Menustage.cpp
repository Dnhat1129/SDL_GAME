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

	chuot = Input::GetInstance();
	IsMenu = true; IsMode = false; IsModePK = false;
	IsPlayStory = false;
	IsPlayPK = false;
	IsPause = false;
	Cam = Vector2D(0, 0);
}

void Menustage::Draw() {
	TextureManager::GetInstance()->Draw("menu", Cam.X, 0, 960, 640);
	if (IsMode) TextureManager::GetInstance()->Draw("mode", 130 + Cam.X, 259, 700, 250);
	if (IsModePK) TextureManager::GetInstance()->Draw("ModePK", Cam.X, 0, 960, 640);
	if (IsPause) TextureManager::GetInstance()->Draw("pause", Cam.X, 0, 960, 640);
}

void Menustage::Update() {
	Cam.X = Camera::GetInstance()->GetPosition().X;
	if (IsMenu) {
		if (chuot->ListenMouse(NewGame)) {
			IsMode = true;
		}
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
}

void Menustage::UpdateStory() {
	if (IsPlayStory) {
		IsPauseNewgame = false;
		IsMenu = false;
		if (chuot->ListenMouse(nutpause)) {
			IsPause = true;
			IsPlayStory = false;
			IsContinue = false;
		}
	}
	if (IsPause) {
		if (chuot->ListenMouse(Continue)) {
			IsPause = false;
			IsMenu = false;
			IsContinue = true;
			IsPlayStory = true;
		}
		if (chuot->ListenMouse(NewGame)) {
			IsPauseNewgame = true;
			IsMenu = true;
			IsPause = false;
			IsPlayStory = true;
			IsPlayPK = false;
		}
	}
}

void Menustage::UpdatePK() {
	//std::cout << IsModePK << " " << IsPlayPK << " " << std::endl;
	IsPlayStory = false;
	if (IsModePK) {
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
				IsPause = true;
				IsPlayPK = false;
				IsContinue = false;
			}
		}
		if (IsPause) {
			if (chuot->ListenMouse(Continue)) {
				IsPause = false;
				IsMenu = false;
				IsContinue = true;
				IsPlayPK = true;
			}
			if (chuot->ListenMouse(NewGame)) {
				IsPauseNewgame = true;
				IsMenu = true;
				IsPause = false;
				IsPlayStory = false;
				IsPlayPK = false;
			}
		}
	}
}

void Menustage::Clean() {
	TextureManager::GetInstance()->Drop("menu");
	TextureManager::GetInstance()->Drop("mode");
	TextureManager::GetInstance()->Drop("pause");
	TextureManager::GetInstance()->Drop("ModePK");
}

void Menustage::Load() {
	TextureManager::GetInstance()->Load("menu", "LamGame/Picture/menu/menu1.png");
	TextureManager::GetInstance()->Load("mode", "LamGame/Picture/menu/mode.png");
	TextureManager::GetInstance()->Load("pause", "LamGame/Picture/menu/pause.png");
	TextureManager::GetInstance()->Load("ModePK", "LamGame/Picture/menu/PK.png");

}