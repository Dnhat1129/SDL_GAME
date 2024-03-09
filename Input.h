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

    int GetAxisKey(Axis axis);

private:
    Input();
    void KeyUp();
    void KeyDown();

    const Uint8* m_KeyStates;
    static Input* s_Instance;
};

#endif
