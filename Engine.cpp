#include "Engine.h"
#include "TextureManager.h"
#include "Warrior.h"
#include "Input.h"
#include "Animation.h"
#include "Timer.h"
#include "MapParser.h"
#include <iostream>
#include "Camera.h"
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

    TextureManager::GetInstance()->Load("player", "LamGame/Picture/dungim_right.png");

    TextureManager::GetInstance()->Load("player_run", "LamGame/Picture/run_right.png");
    TextureManager::GetInstance()->Load("player_skill1", "LamGame/Picture/skill1_right.png");
    TextureManager::GetInstance()->Load("player_jump", "LamGame/Picture/fly + jump/nhay.png");
    TextureManager::GetInstance()->Load("player_fall", "LamGame/Picture/fly + jump/roi_xuong.png");

    TextureManager::GetInstance()->Load("enemy", "LamGame/Picture/enemy/so3.png");
    TextureManager::GetInstance()->Load("enemy_jump", "LamGame/Picture/enemy/so3_nhay.png");
    TextureManager::GetInstance()->Load("enemy_fall", "LamGame/Picture/enemy/so3_roi.png");
    TextureManager::GetInstance()->Load("enemy_run", "LamGame/Picture/enemy/so3_run.png");

    TextureManager::GetInstance()->Load("bg", "LamGame/Picture/Bg/background0.png");


    player = new Warrior(new Properties("player",100,100, 60, 75));
 
    enemy = new Enemy(new Properties("enemy", 200, 100, 60, 75));


    Camera::GetInstance()->SetTarget(player->GetOrigin());

    return m_IsRunning = true;
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(m_Renderer, 124, 218, 254, 255);
    SDL_RenderClear(m_Renderer);
    TextureManager::GetInstance()->Draw("bg", 0, 0, 1920, 1080);

    m_LevelMap->Render();
    player->Draw();
    enemy->Draw();


    SDL_RenderPresent(m_Renderer);
}

void Engine::Update()
{   
    
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