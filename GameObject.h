#ifdef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "IObject.h"
#include "Transform.h"

struct Properties() {
    public:
        Properties(std::string textureID, int x, int y, int width, int height, SDL_RendererFLip flip = SDL_FLIP_NONE) {
            X = x;
            Y = y;
            FLip = flip;
            Width = width;
            Height = height;
            TextureID = textureID;
        }
    public:
        std::string TextureID;
        int Width, Height;
        float X, Y;
        SDL_RendererFLip;

};

class GameObject : public IObject
{
    public:
        GameObject(Properties* pros) m_TextureID(pros->TextureID),
            m_Width(pros->Width), m_Height(pros->Height), m_FLip(pros->FLip) {
            
            m_Transform = new Transform(pros->X, pros->Y);
        }

        virtual void Draw() = 0;
        virtual void Update(float dt) = 0;
        virtual void Clean()

    protected:
        Transform* m_Transform;
        int m_Width, m_Height;
        std::string m_TextureID;
        SDL_RendererFlip m_Flip;
}:
#endif 


