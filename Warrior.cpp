#include "Warrior.h"
#include "TextureManager.h"
#include "SDL.h"
#include "Input.h"


Warrior::Warrior(Properties* props) : Character(props) {
    m_RigidBody = new RigidBody();
    m_Animation = new Animation();
}

void Warrior::Draw() {
    m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height);
}

void Warrior::Update(float dt) {

    m_Animation->SetProps("player", 1, 3, 200);
    m_RigidBody->UnSetForce();


    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_RIGHT)) {
        m_RigidBody->ApplyForceX(5 * FORWARD);
        m_Animation->SetProps("player_run", 1, 5, 100);
    }

    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_LEFT)) {
        m_RigidBody->ApplyForceX(5 * BACKWARD);
        m_Animation->SetProps("player_run", 1, 5, 100, SDL_FLIP_HORIZONTAL);
    }

    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_J)) {
        m_RigidBody->ApplyForceX(0);
        m_Animation->SetProps("player_skill1", 1, 11, 100);
    }

    SDL_Log("%f", dt);
    m_RigidBody->Update(dt);
    m_Transform->TranslateX(m_RigidBody->Position().X);
    //m_Transform->TranslateY(m_RigidBody->Position().Y);

    m_Animation->Update();
}

void Warrior::Clean() {
    TextureManager::GetInstance()->Clean();
}

