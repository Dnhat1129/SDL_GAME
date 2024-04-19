#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <SDL_mixer.h>
#include <unordered_map>
#include <map>
#include <string>

class SoundManager {
public:
    SoundManager();
    ~SoundManager();

    static SoundManager* GetInstance() {
        if (s_Instance != nullptr)
            return s_Instance;
        else
            return s_Instance = new SoundManager();
    }
    void loadSound(const char* filePath, const char* soundName);
    void playSound(const char* soundName);
    void stopSound(const char* soundName);
    void setVolume(int volume);
    void UpdateSound();
    void Load();
    void clean();
private:
    static SoundManager* s_Instance;
    std::map<std::string, Mix_Chunk*> soundEffects{
        {"menu", nullptr},
        {"run", nullptr},
        {"attack", nullptr},
        {"kamehameha", nullptr},
        {"skill", nullptr }

    };
    std::map<std::string, int> channels;
};

#endif // SOUNDMANAGER_H