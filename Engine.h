#ifndef ENGINE_H
#define ENGINE_H

#include <SDL.h>
#include <string>
#include <Windows.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include "GameMap.h"
#include "SoundManager.h"
#include "FontManager.h"

class Warrior; // Forward declaration
class Enemy;   // Forward declaration
class Boss;
class PlayPK;
class Menustage;


/// Screen
const int SCREEN_WIDTH = 960;
const int SCREEN_HEIGHT = 640;

class Engine {
public:
    Engine() {
        isMap1 = true; isMap2 = false;
        checkloadmap = false; checkloadmap = false;
        checkcontinue = false;
        score = 0; updatescore = false;
        gold = 0; goldplay = 0;
       // highscore[5] = {0,0,0,0,0};
    };
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
    void GetPrevious();
    void Luu();

    inline GameMap* GetMap() { return m_LevelMap; }
    inline bool IsRunning() { return m_IsRunning; }
    inline SDL_Renderer* GetRenderer() { return m_Renderer; }
    
    Warrior* GetWarrior() { return player; }
    Enemy* GetEnemy() { return enemy; }
    Boss* GetBoss() { return boss; }
    PlayPK* GetPlayPK() { return playPK; }
    Menustage* GetMenu() { return menu; }
    FontManager* GetFont() { return m_FontManager; }

    bool GetMap1() { return isMap1; } 
    bool GetMap2() { return isMap2; }
    bool GetoverWin() { return gameoverwin; }
    bool GetoverLose() { return gameoverlose; }

    int GetGold() { return gold; }

private:
    bool m_IsRunning = true;

    GameMap* m_LevelMap;
    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    static Engine* s_Instance;

    Warrior* player;
    Enemy* enemy;
    Boss* boss;
    PlayPK* playPK;
    Menustage* menu;

    FontManager* m_FontManager;
private:
    bool isMap1; bool isMap2; 
    bool checkloadmap; bool checkloadmap2; int CurrentEnemy; int CurrentBoss;
    bool checkcontinue;
    bool gameoverwin, gameoverlose;
    int score; bool updatescore;
    int gold; int goldplay;
    int highscore[5];
};

#endif // ENGINE_H
