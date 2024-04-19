#include "SoundManager.h"
#include "Menustage.h"
#include "Engine.h"
#include "Warrior.h"
#include "Enemy.h"
#include "Boss.h"
#include "PlayPK.h"
SoundManager* SoundManager::s_Instance = nullptr;

SoundManager::SoundManager() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void SoundManager::loadSound(const char* filePath, const char* soundName) {
    Mix_Chunk* soundEffect = Mix_LoadWAV(filePath);
    if (soundEffect != nullptr) {
        soundEffects[soundName] = soundEffect;
    }
}

void SoundManager::playSound(const char* soundName) {
    Mix_PlayChannel(-1, soundEffects[soundName], 0);
}

void SoundManager::setVolume(int volume) {
    for (int i = 0; i < MIX_CHANNELS; ++i) {
        Mix_Volume(i, volume);
    }
}

void SoundManager::UpdateSound() {
    setVolume(Engine::GetInstance()->GetMenu()->GetVolume());

    if (music != nullptr) {
        Mix_PlayMusic(music, -1);
    }

    if (Engine::GetInstance()->GetWarrior()->CheckAttack() || Engine::GetInstance()->GetEnemy()->CheckAttack()
        || Engine::GetInstance()->GetPlayPK()->GetIsAttacking() || Engine::GetInstance()->GetBoss()->GetIsAttacking())
        playSound("attack");

    if (Engine::GetInstance()->GetWarrior()->GetIsRunning() || Engine::GetInstance()->GetEnemy()->GetIsRunning() 
        || Engine::GetInstance()->GetBoss()->GetIsRunning() || Engine::GetInstance()->GetPlayPK()->GetIsRunning())
        playSound("run");

    if (Engine::GetInstance()->GetWarrior()->GetIsKame() || Engine::GetInstance()->GetPlayPK()->GetIsSkill()) playSound("kamehameha");

    if (Engine::GetInstance()->GetBoss()->Gettime() >= 175 && Engine::GetInstance()->GetBoss()->Gettime() <= 200)
        playSound("skill");

}

void SoundManager::Load() {
    for (auto& pair : soundEffects) {
        std::string filePath = "LamGame/Sound/" + pair.first + ".mp3";
        loadSound(filePath.c_str(), pair.first.c_str());
    }
    music = Mix_LoadMUS("LamGame/Sound/menu.mp3");
}

void SoundManager::clean() {
    for (auto& pair : soundEffects) {
        if (pair.second != nullptr) {
            Mix_FreeChunk(pair.second);
            pair.second = nullptr;
        }
    }
    soundEffects.clear();
    Mix_FreeMusic(music);
    Mix_CloseAudio();
}