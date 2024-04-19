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
#include <fstream>
#include <algorithm>

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
    enemy = new Enemy(new Properties("enemy", 1184, 437, 60, 75), 0);
    boss = new Boss(new Properties("boss", 1280, 512, 60, 75), 0);
    playPK = new PlayPK();

    player->Load();
    enemy->Load();
    boss->Load();
    playPK->Load();

    playPK->SetPrevious();

    TextureManager::GetInstance()->Load("bg1", "LamGame/Picture/Bg/background0.png");
    TextureManager::GetInstance()->Load("bg2", "LamGame/Picture/Bg/rock.png");

    sound->playMusic();
    Camera::GetInstance()->SetTarget(player->GetOrigin());
    return m_IsRunning = true;
}

void Engine::Render()
{
    float dt = Timer::GetInstance()->GetDeltaTime();
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);

    if (menu->GetIsMenu() || menu->GetIsMode() || menu->GetModePK() || menu->GetPauseP() || menu->GetPauseS() || gameoverwin || gameoverlose) { 
        menu->Draw(); 
        if (gameoverwin || gameoverlose) {
            SDL_Color textColor = { 255, 255, 0, 255 };
            if (m_FontManager && m_FontManager->isFontLoaded()) {
                SDL_Texture* textTexture1 = m_FontManager->renderText(std::to_string(goldplay), textColor, m_FontManager->getFont(), m_Renderer);
                
                if (textTexture1) {
                    SDL_Rect customRect1 = { 455, 305, 40, 30 };
                    SDL_RenderCopy(m_Renderer, textTexture1, NULL, &customRect1);
                    SDL_DestroyTexture(textTexture1);
                }
                
                SDL_Texture* textTexture2 = m_FontManager->renderText(std::to_string(score), textColor, m_FontManager->getFont(), m_Renderer);
                if (textTexture2) {
                    SDL_Rect customRect2 = { 455, 340, 40, 30 };
                    SDL_RenderCopy(m_Renderer, textTexture2, NULL, &customRect2);
                    SDL_DestroyTexture(textTexture2);
                }
            }
        }
    }

    if (menu->GetIsbxh()) {
        SDL_Color textColor = { 255, 255, 255, 255 };
        if (m_FontManager && m_FontManager->isFontLoaded()) {
            SDL_Texture* textTexture1 = m_FontManager->renderText(std::to_string(highscore[0]), textColor, m_FontManager->getFont(), Engine::GetInstance()->GetRenderer());
            if (textTexture1) {
                SDL_Rect customRect = { 390, 355, 50, 30 };
                SDL_RenderCopy(Engine::GetInstance()->GetRenderer(), textTexture1, NULL, &customRect);
                SDL_DestroyTexture(textTexture1);
            }

            SDL_Texture* textTexture2 = m_FontManager->renderText(std::to_string(highscore[1]), textColor, m_FontManager->getFont(), Engine::GetInstance()->GetRenderer());
            if (textTexture2) {
                SDL_Rect customRect = { 390, 380, 50, 30 };
                SDL_RenderCopy(Engine::GetInstance()->GetRenderer(), textTexture2, NULL, &customRect);
                SDL_DestroyTexture(textTexture2);
            }

            SDL_Texture* textTexture3 = m_FontManager->renderText(std::to_string(highscore[2]), textColor, m_FontManager->getFont(), Engine::GetInstance()->GetRenderer());
            if (textTexture3) {
                SDL_Rect customRect = { 390, 405, 50, 30 };
                SDL_RenderCopy(Engine::GetInstance()->GetRenderer(), textTexture3, NULL, &customRect);
                SDL_DestroyTexture(textTexture3);
            }

            SDL_Texture* textTexture4 = m_FontManager->renderText(std::to_string(highscore[3]), textColor, m_FontManager->getFont(), Engine::GetInstance()->GetRenderer());
            if (textTexture4) {
                SDL_Rect customRect = { 390, 430, 50, 30 };
                SDL_RenderCopy(Engine::GetInstance()->GetRenderer(), textTexture4, NULL, &customRect);
                SDL_DestroyTexture(textTexture4);
            }

            SDL_Texture* textTexture5 = m_FontManager->renderText(std::to_string(highscore[4]), textColor, m_FontManager->getFont(), Engine::GetInstance()->GetRenderer());
            if (textTexture5) {
                SDL_Rect customRect = { 390, 455, 50, 30 };
                SDL_RenderCopy(Engine::GetInstance()->GetRenderer(), textTexture5, NULL, &customRect);
                SDL_DestroyTexture(textTexture5);
            }
        }
    }
    
    if (menu->GetStory()) {
        TextureManager::GetInstance()->Draw("bg1", 0, 0, 1920, 1080);
        //map1
        m_LevelMap->Render();
        if (isMap1) {
            enemy->Draw();
        }

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

        player->Draw();
     
        //map2
        if (isMap2) {
            boss->Draw();
        }
    }
    else if (menu->GetModePK()) {
        playPK->UpdateModePK(dt);
    }
    else if (menu->GetPK()) {
        TextureManager::GetInstance()->Draw("bg2", 0, 0, 1920, 1080);

        m_LevelMap->Render();
        playPK->Draw();
    }

    SDL_RenderPresent(m_Renderer);
}

void Engine::Update()
{
    gameoverlose = player->GetHP() <= 0;
    gameoverwin = player->GetHP() > 0 && boss->GetComplete2();
    menu->Update();
    
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
        checkcontinue = false;

        Camera::GetInstance()->SetTarget(player->GetOrigin());

        PlayPK* tempplayPK = new PlayPK();
        delete playPK;
        playPK = tempplayPK;

        gameoverwin = false;
        gameoverlose = false;
        updatescore = false;
        goldplay = 0;
        playPK->SetPrevious();
        Engine::GetPrevious();
    }

    if (gameoverlose || gameoverwin) {
        if (!updatescore) {
            score -= (10 - player->Getdauthan()) * 100;
            gold += goldplay;
            for (int i = 0; i < 5; i++) {
                if (highscore[i] <= score) {
                    highscore[i] = score;
                    break;
                }
            }
            std::sort(highscore, highscore + 5, [](int a, int b) {
                return a > b;
                });
            }
            updatescore = true;
            return;
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
        if (!menu->GetPauseS()) {
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
                    goldplay += 100;
                    score += 1000;
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
                    goldplay += 200;
                    score += 2000;
                }

                boss->Update(dt);
                boss->Luu();
            }
            Camera::GetInstance()->Update(dt);
        }
    }
    else if (menu->GetModePK()) {
        playPK->Luu();
    }
    else if (menu->GetPK()) {

        if (menu->GetContinue() && !checkcontinue) {
            playPK->SetContinue();
            playPK->Load();
            checkcontinue = true;
        }

        if (!checkloadmap) {
            MapParser::GetInstance()->Load();
            m_LevelMap = MapParser::GetInstance()->GetMap("MAP");
            CollisionHandler::GetInstance()->Update();
            Camera::GetInstance()->SetTarget(playPK->GetOrigin());
            playPK->Load();
            checkloadmap = true;
        }

        if (!menu->GetPauseP()) {
            float dt = Timer::GetInstance()->GetDeltaTime();
            m_LevelMap->Update();
            playPK->Update(dt);
            playPK->Luu();
            Camera::GetInstance()->Update(dt);
        }
    }
    if (menu->Getismua()) gold -= playPK->Getluugia();
    sound->UpdateSound();
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

void Engine::GetPrevious() {
    std::ifstream in("LamGame/Picture/previous.txt");
    std::string line;
    int dem = 0;
    while (std::getline(in, line)) {
        dem++;
        std::stringstream iss(line);
        switch (dem) {
        case 1: iss >> gold; break;
        case 2: iss >> highscore[0]; break;
        case 3: iss >> highscore[1]; break;
        case 4: iss >> highscore[2]; break;
        case 5: iss >> highscore[3]; break;
        case 6: iss >> highscore[4]; break;
        default: break;
        }
    }
    in.close();
}

void Engine::Luu() {
    std::ofstream out("LamGame/Picture/previous.txt");
    out << gold << std::endl;
    if (out.is_open()) {
        for (int i = 0; i < 5; i++) {
            out << highscore[i] << std::endl;
        }
    }
    out.close();
}