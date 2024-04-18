// Input.cpp
#include "Input.h"
#include "Engine.h"
#include "Menustage.h"

Input* Input::s_Instance = nullptr;

Input::Input() {
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}
void Input::Listen() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT: Engine::GetInstance()->Quit(); break;
            case SDL_KEYDOWN: KeyDown(); break;
            case SDL_KEYUP: KeyUp(); break;
        
            case SDL_MOUSEBUTTONDOWN:
                luu.type = SDL_MOUSEBUTTONDOWN;
                break;
            default: 
                luu.type = SDL_FIRSTEVENT;
                break;
        }
    }
}

bool Input::GetKeyDown(SDL_Scancode key) {
    return (m_KeyStates[key] == 1);
}

void Input::KeyDown() {
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}

void Input::KeyUp() {
    m_KeyStates = SDL_GetKeyboardState(nullptr);
}

int Input::GetAxisKey(Axis axis) {
    switch (axis) {
        case HORIZONTAL:
            if (GetKeyDown(SDL_SCANCODE_D) || GetKeyDown(SDL_SCANCODE_RIGHT)) {
                return 1;
            }
            if (GetKeyDown(SDL_SCANCODE_A) || GetKeyDown(SDL_SCANCODE_LEFT)) {
                return -1;
            }
            break;
            
        case VERTICAL:
            if (GetKeyDown(SDL_SCANCODE_W) || GetKeyDown(SDL_SCANCODE_UP)) {
                return 1;
            }
            if (GetKeyDown(SDL_SCANCODE_S) || GetKeyDown(SDL_SCANCODE_DOWN)) {
                return -1;
            }
         
        default: 
            return 0;
    }
}

bool Input::ListenMouse(SDL_Rect check) {
        if (luu.type == SDL_MOUSEBUTTONDOWN) {
            int mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            //std::cout << mouseX << " " << mouseY << std::endl;
            if (mouseX >= check.x && mouseX <= check.x + check.w &&
                mouseY >= check.y && mouseY <= check.y + check.h) {
                return true;
            }
        }
    return false;
}