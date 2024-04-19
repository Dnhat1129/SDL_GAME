#ifndef WARRIOR_H
#define WARRIOR_H

#include "RigidBody.h"
#include "Animation.h"
#include "Collider.h"
#include "Vector2D.h"
#include "GameObject.h"
#include <vector>
#include "FontManager.h"
#include <string>

#define JUMP_TIME 15.0f
#define JUMP_FORCE 10.0f

#define RUN_FORCE 4.0f
#define ATTACK_TIME 20.0f

class Warrior : public GameObject {
    
    public:
        Warrior(Properties* props);
        virtual void Draw();
        virtual void Clean();
        virtual void Update(float dt);
        void Load();
        void Luu();
        void SetContinue();

        bool CheckAttack() { return m_IsAttacking; }
        Transform* GetPosition() { return m_Transform; }
        SDL_Rect GetBox() { return m_Collider->Get(); }
        SDL_RendererFlip GetFlip() { return m_Flip; }
        int GetHP() { return m_HP; }
        int GetDame() { return m_dame; }
        int GetKameFrame() { return kame_frame; }
        bool GetIsKame() { return m_IsKame; }
        bool GetIsRunning() { return m_IsRunning; }

        int Getdauthan() { return dau_than; }

        void Reset() {
            m_Transform->X = 100;
            m_Transform->Y = 100;

            m_Origin->X = 80;
            m_Origin->Y = 87.5;
        }

    private: 
        void AnimationState();
    private:
        bool m_IsRunning;
        bool m_IsJumping;
        bool m_IsFalling;
        bool m_IsGrounded;
        bool m_IsAttacking;


        
        
        Collider* m_Collider;

        Animation* m_Animation;
        RigidBody* m_RigidBody;
        

        Vector2D m_LastSafePosition;
    private:
        bool m_IsDie;
        int m_HP; int max_HP;
        int m_MN;
        int m_dame;
        float m_Time;

        bool ssj;
        int checkload;

        bool m_IsKame; int kame_frame;
        Animation* ssj_Animation;
        float time_ssj;
        float time_kame;
        Animation* kame_Animation;
        int dau_than; float time_dau_than; float time_dau_than_max;

    protected:
        Transform* m_Transform;
        int m_Width, m_Height;
        std::string m_TextureID;
        SDL_RendererFlip m_Flip;
};

#endif 
