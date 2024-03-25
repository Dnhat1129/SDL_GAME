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

Engine* Engine::s_Instance = nullptr;
Warrior* player = nullptr;
Enemy* enemy = nullptr;

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

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (m_Renderer == nullptr) {
        SDL_Log("Failed to create Renderer: %s", SDL_GetError());
        return false;
    }

    if (!MapParser::GetInstance()->Load()) {
        std::cout << "Failed to load" << std::endl;
    }

    m_LevelMap = MapParser::GetInstance()->GetMap("MAP");

    player = new Warrior(new Properties("player", 100, 100, 60, 75));
    enemy = new Enemy(new Properties("enemy", 1184, 437, 60, 75));

    player->Load();

    enemy->Load();

    TextureManager::GetInstance()->Load("bg", "LamGame/Picture/Bg/background0.png");


    
    
    Camera::GetInstance()->SetTarget(player->GetOrigin());

    return m_IsRunning = true;
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);
    TextureManager::GetInstance()->Draw("bg", 0, 0, 1920, 1080);
    m_LevelMap->Render();
    enemy->Draw();
    player->Draw();
    
    

    SDL_RenderPresent(m_Renderer);
}

void Engine::Update()
{   
    if (enemy->GetIsDie()) {
        enemy->Load();
    }
    float dt = Timer::GetInstance()->GetDeltaTime();
    m_LevelMap->Update();
    player->Update(dt);
    enemy->Update(dt);
    Camera::GetInstance()->Update(dt);
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
    TextureManager::GetInstance()->Clean();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();
    return true;
}
