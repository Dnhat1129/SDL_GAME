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

SoundManager::~SoundManager() {
    for (auto& pair : soundEffects) {
        Mix_FreeChunk(pair.second);
    }
    Mix_CloseAudio();
}

void SoundManager::loadSound(const char* filePath, const char* soundName) {
    Mix_Chunk* soundEffect = Mix_LoadWAV(filePath);
    if (soundEffect != nullptr) {
        soundEffects[soundName] = soundEffect;
    }
}

void SoundManager::playSound(const char* soundName) {
    int channel = Mix_GroupAvailable(-1);
    if (channel >= 0) {
        Mix_PlayChannel(channel, soundEffects[soundName], 0);
        channels[soundName] = channel;
    }
}

void SoundManager::stopSound(const char* soundName) {
    if (channels.find(soundName) != channels.end()) {
        int channel = channels[soundName];
        Mix_HaltChannel(channel);
        channels.erase(soundName);
    }
}

void SoundManager::setVolume(int volume) {
    for (int i = 0; i < MIX_CHANNELS; ++i) {
        Mix_Volume(i, volume);
    }
}

void SoundManager::UpdateSound() {
    setVolume(Engine::GetInstance()->GetMenu()->GetVolume());
    if (Menustage::GetInstance()->GetIsMenu()) playSound("menu");
    else stopSound("menu");
    if (Engine::GetInstance()->GetWarrior()->CheckAttack() || Engine::GetInstance()->GetEnemy()->CheckAttack()
        || Engine::GetInstance()->GetPlayPK()->GetIsAttacking() || Engine::GetInstance()->GetBoss()->GetIsAttacking())
        playSound("attack");
    else stopSound("attack");
    if (Engine::GetInstance()->GetWarrior()->GetIsRunning() || Engine::GetInstance()->GetEnemy()->GetIsRunning() 
        || Engine::GetInstance()->GetBoss()->GetIsRunning() || Engine::GetInstance()->GetPlayPK()->GetIsRunning())
        playSound("run");
    else stopSound("run");
    if (Engine::GetInstance()->GetWarrior()->GetIsKame() || Engine::GetInstance()->GetPlayPK()->GetIsSkill()) playSound("kamehameha");
    else stopSound("kamehameha");
    if (Engine::GetInstance()->GetBoss()->Gettime() >= 175 && Engine::GetInstance()->GetBoss()->Gettime() <= 200)
        playSound("skill");
    else stopSound("skill");
}

void SoundManager::Load() {
    for (auto& pair : soundEffects) {
        std::string filePath = "LamGame/Sound/" + pair.first + ".mp3";
        loadSound(filePath.c_str(), pair.first.c_str());
    }
}

void SoundManager::clean() {
    for (auto& pair : soundEffects) {
        if (pair.second != nullptr) {
            Mix_FreeChunk(pair.second);
            pair.second = nullptr;
        }
    }
    soundEffects.clear();
    channels.clear();
}