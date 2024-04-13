#include "Engine.h"
#include "TextureManager.h"
#include "Input.h"
#include "Animation.h"
#include "Timer.h"
#include "MapParser.h"
#include <iostream>
#include "Camera.h"
#include "Warrior.h"
#include "Enemy.h"
#include "Menustage.h"
#include "SoundManager.h"
#include "FontManager.h"
#include "CollisionHandler.h"
#include "Boss.h"
#include "PlayPK.h"

Engine* Engine::s_Instance = nullptr;

Warrior* player = nullptr;
Enemy* enemy = nullptr;
Boss* boss = nullptr;

Menustage* menu = nullptr;
SoundManager* sound = nullptr;
FontManager* m_FontManager = nullptr;
PlayPK* playPK = nullptr;

bool Engine::Init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);

    m_Window = SDL_CreateWindow("ADVENTURE SONGOKU", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);

    if (m_Window == nullptr) {
        SDL_Log("Failed to create Window: %s", SDL_GetError());
        return false;
    }

    SDL_Surface* iconSurface = IMG_Load("LamGame/Picture/menu/icon.png");
    if (!iconSurface) {
        SDL_Log("Failed to load icon: %s", SDL_GetError());
    }
    SDL_SetWindowIcon(m_Window, iconSurface);

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_Renderer == nullptr) {
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false;
    }

    menu = new Menustage();
    menu->Load();

    sound = new SoundManager();
    sound->Load();

    m_FontManager = new FontManager("LamGame/Font/Roboto.ttf", 24);
    m_FontManager->loadFont();

    TextureManager::GetInstance()->Load("mui_ten", "LamGame/Picture/maps/chiduong.png");
    

    player = new Warrior(new Properties("player", 100, 100, 60, 75));
    enemy = new Enemy(new Properties("enemy", 1184, 437, 60, 75),0);
    boss = new Boss(new Properties("boss", 1280, 512, 60, 75), 0);
    playPK = new PlayPK();

    player->Load();
    enemy->Load();
    boss->Load();
    playPK->Load();
    
   
    TextureManager::GetInstance()->Load("bg1", "LamGame/Picture/Bg/background0.png");
    TextureManager::GetInstance()->Load("bg2", "LamGame/Picture/Bg/rock.png");

    Camera::GetInstance()->SetTarget(player->GetOrigin());
    return m_IsRunning = true;
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);

    if (menu->GetIsMenu() || menu->GetIsMode() || menu->GetModePK() || menu->GetPause()) { menu->Draw(); }
    else if (menu->GetStory()) {
        TextureManager::GetInstance()->Draw("bg1", 0, 0, 1920, 1080);
        //map1
        m_LevelMap->Render();
        if (isMap1) enemy->Draw();
        player->Draw();

        //xong map 1
        if (enemy->GetComplete1() && !isMap2) {
            SDL_Color textColor = { 255, 255, 255, 255 };
            if (m_FontManager && m_FontManager->isFontLoaded()) {
                SDL_Texture* textTexture = m_FontManager->renderText("FIND WAY TO EXIT HERE !", textColor, m_FontManager->getFont(), m_Renderer);
                if (!isMap2 && !isMap1 && abs(player->GetPosition()->X - 1798) < 3 && abs(player->GetPosition()->Y - 180) < 3)
                    textTexture = m_FontManager->renderText("PRESS N TO CONTINUE !", textColor, m_FontManager->getFont(), m_Renderer);
                if (textTexture) {
                    SDL_Rect customRect = { 350, 0, 300, 40 };
                    SDL_RenderCopy(m_Renderer, textTexture, NULL, &customRect);
                    SDL_DestroyTexture(textTexture);
                }
            }
            TextureManager::GetInstance()->Draw("mui_ten", 1792, 64, 64, 64);
        }
        //map2
        if (isMap2) {
            boss->Draw();
        }
    }
    else if (menu->GetPK()) {
        TextureManager::GetInstance()->Draw("bg2", 0, 0, 1920, 1080);
        
        if (menu->GetModePK()) {
            playPK->UpdateModePK();
        }

        m_LevelMap->Render();
        playPK->Draw();
    }

    SDL_RenderPresent(m_Renderer);
}

void Engine::Update()
{
    menu->Update();
    sound->UpdateSound();
    menu->UpdateStory();
    menu->UpdatePK();


    if (menu->GetNewgame()) {
        CurrentEnemy = 0;
        Enemy* tempEnemy = new Enemy(new Properties("enemy", 1184, 437, 60, 75), 0);
        tempEnemy->Load();
        delete enemy;
        enemy = tempEnemy;

        CurrentBoss = 0;
        Boss* tempBoss = new Boss(new Properties("boss", 300, 100, 60, 75), 0);
        tempBoss->Load();
        delete boss;
        boss = tempBoss;

        Warrior* tempplayer = new Warrior(new Properties("player", 100, 100, 60, 75));
        tempplayer->Load();
        delete player;
        player = tempplayer;

        isMap1 = true;
        isMap2 = false;

        checkloadmap = false;
        checkloadmap2 = false;

        Camera::GetInstance()->SetTarget(player->GetOrigin());

        PlayPK* tempplayPK = new PlayPK();
        delete playPK;
        playPK = tempplayPK;
    }
    

    if (menu->GetStory()) {

            if (menu->GetContinue() && !checkcontinue) {
                player->SetContinue();
                if (isMap1) enemy->SetContinue();
                else if (isMap2) boss->SetContinue();
                checkcontinue = true;
            }

            if (!checkloadmap) {
                MapParser::GetInstance()->Load();
                m_LevelMap = MapParser::GetInstance()->GetMap("MAP");
                CollisionHandler::GetInstance()->Update();
                checkloadmap = true;
            }
            if (!menu->GetPause()) {
                isMap1 = !enemy->GetComplete1();
                if (!isMap2 && !isMap1 && abs(player->GetPosition()->X - 1798) < 3 && abs(player->GetPosition()->Y - 180) < 3 && Input::GetInstance()->GetKeyDown(SDL_SCANCODE_N))
                {
                    isMap2 = true;
                    isMap1 = false;
                }

                float dt = Timer::GetInstance()->GetDeltaTime();
                m_LevelMap->Update();
                player->Update(dt);
                player->Luu();

                if (isMap1) {
                    CurrentEnemy = enemy->GetCurrentEnemy();
                    if (enemy->GetIsDie() && CurrentEnemy >= 0) {
                        Enemy* tempEnemy = new Enemy(new Properties("enemy", 300, 100, 60, 75), CurrentEnemy);
                        tempEnemy->Load();
                        delete enemy;
                        enemy = tempEnemy;
                    }
                    enemy->Update(dt);
                    enemy->Luu();

                }
                if (isMap2) {
                    // enemy->Clean();
                    if (!checkloadmap2) {
                        player->Reset();
                        MapParser::GetInstance()->Load();
                        m_LevelMap = MapParser::GetInstance()->GetMap("MAP");
                        CollisionHandler::GetInstance()->Update();
                        checkloadmap2 = true;
                    }
                    CurrentBoss = boss->GetCurrentBoss();
                    if (boss->GetIsDie() && CurrentBoss >= 0) {
                        Boss* tempBoss = new Boss(new Properties("boss", 300, 100, 60, 75), CurrentBoss);
                        tempBoss->Load();
                        delete boss;
                        boss = tempBoss;
                    }

                    boss->Update(dt);
                    boss->Luu();
                }
                Camera::GetInstance()->Update(dt);
            }
    }
    else if (menu->GetModePK()) {
        playPK->UpdateModePK();
    }
    else if (menu->GetPK()) {

        if (!checkloadmap) {
            MapParser::GetInstance()->Load();
            m_LevelMap = MapParser::GetInstance()->GetMap("MAP");
            Camera::GetInstance()->SetTarget(playPK->GetOrigin());
            playPK->Load();
            checkloadmap = true;
        }

        if (!menu->GetPause()) {
            float dt = Timer::GetInstance()->GetDeltaTime();
            m_LevelMap->Update();
            playPK->Update(dt);

            Camera::GetInstance()->Update(dt);
        }
    }
}

void Engine::Events()
{
    Input::GetInstance()->Listen();
}

void Engine::Quit()
{
    m_IsRunning = false;
}

bool Engine::Clean()
{
    m_FontManager->closeFont();
    delete m_FontManager;
    m_FontManager = nullptr;
    
    player->Clean();
    delete player;
    player = nullptr;

    enemy->Clean();
    delete enemy;
    enemy = nullptr; 

    menu->Clean();
    delete menu;
    menu = nullptr;

    sound->clean();
    delete sound;
    sound = nullptr;

    playPK->Clean();
    delete playPK;
    playPK = nullptr;

    TextureManager::GetInstance()->Clean();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
    return true;
}
