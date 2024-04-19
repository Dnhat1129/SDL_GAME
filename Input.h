// Input.h
#ifndef INPUT_H
#define INPUT_H

#include "SDL.h"

enum Axis {HORIZONTAL, VERTICAL};

class Input
{
public:
    static Input* GetInstance() {
        if (s_Instance != nullptr)
            return s_Instance;
        else
            return s_Instance = new Input();
    }
    void Listen();
    bool GetKeyDown(SDL_Scancode key);
    bool ListenMouse(SDL_Rect check);

    int GetAxisKey(Axis axis);

    SDL_Event GetEvent() { return luu; }

private:
    Input();
    void KeyUp();
    void KeyDown();

    //int nut, nut2;
    const Uint8* m_KeyStates;
    static Input* s_Instance;
    SDL_Event luu;
};

#endif
