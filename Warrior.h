#ifndef WARRIOR_H
#define WARRIOR_H

#include "RigidBody.h"
#include "Animation.h"
#include "Collider.h"
#include "Vector2D.h"
#include "GameObject.h"
#include <vector>

#define JUMP_TIME 15.0f
#define JUMP_FORCE 10.0f

#define RUN_FORCE 4.0f
#define ATTACK_TIME 20.0f

class Warrior : public GameObject {
    
    public:
        Warrior(Properties* props) : GameObject(props) {
            m_TextureID = props->TextureID;
            m_Width = props->Width;
            m_Height = props->Height;
            m_Flip = props->Flip;

            m_Transform = new Transform(props->X, props->Y);

            m_IsRunning = false;
            m_IsJumping = false;
            m_IsFalling = false;
            m_IsGrounded = false;
            m_IsAttacking = false;
            m_IsKame = false;
            m_IsDie = false;
            ssj = true;

            m_Flip = SDL_FLIP_NONE;
            m_JumpForce = JUMP_FORCE;
            m_HP = 1000;
            m_dame = 10;
            time_sjjkhi = 0;
            time_kame = 0;

            m_Collider = new Collider();
            m_Collider->SetBuffer(-10, 0, 15, 0);

            m_RigidBody = new RigidBody();
            m_RigidBody->SetGravity(3.0f);

            m_Animation = new Animation();
            m_Animation->SetProps("player_right", 0, 3, 100);

            ssj_Animation = new Animation();
            player_kame = new Animation();
        }
        virtual void Draw();
        virtual void Clean();
        virtual void Update(float dt);
        void Load();

        bool CheckAttack() { return m_IsAttacking; }
        Transform* GetPosition() { return m_Transform; }
        SDL_Rect GetBox() { return m_Collider->Get(); }
        SDL_RendererFlip GetFlip() { return m_Flip; }
        int GetHP() { return m_HP; }
        int GetDame() { return m_dame; }
        Animation* GetKameAnimation() { return player_kame; }

    private: 
        void AnimationState();
    private:
        bool m_IsRunning;
        bool m_IsJumping;
        bool m_IsFalling;
        bool m_IsGrounded;
        bool m_IsAttacking;


        float m_JumpForce;
        
        
        Collider* m_Collider;

        Animation* m_Animation;
        RigidBody* m_RigidBody;
        

        Vector2D m_LastSafePosition;
    private:
        bool m_IsDie;
        int m_HP;
        int m_MN;
        int m_dame;
        float m_Time;

        bool ssj, khi;

        bool m_IsKame;
        Animation* ssj_Animation;
        float time_sjjkhi;
        Animation* player_kame;
        float time_kame;


        std::vector<Warrior> CharacterList;

    protected:
        Transform* m_Transform;
        int m_Width, m_Height;
        std::string m_TextureID;
        SDL_RendererFlip m_Flip;
};

#endif
