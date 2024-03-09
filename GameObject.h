#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Transform.h"
#include "SDL.h"
#include "Point.h"

struct Properties {
    public:
        Properties (std::string textureID, float x, float y, int width, int height, SDL_RendererFlip flip = SDL_FLIP_NONE) {
            X = x;
            Y = y;
            Flip = flip;
            Width = width;
            Height = height;
            TextureID = textureID;
        }
    public:
        std::string TextureID;
        int Width, Height;
        float X, Y;
        SDL_RendererFlip Flip;

};

class GameObject 
{
    public:
        GameObject(Properties* props) {

        float px = (props->X + props->Width)/2;
        float py = (props->Y + props->Height) / 2;
        m_Origin = new Point(px,py);
        }

        inline Point* GetOrigin() { return m_Origin; }

        virtual void Draw() = 0;
        virtual void Update(float dt) = 0;
        virtual void Clean() = 0;

    protected:
        Point* m_Origin;

        
};
#endif


