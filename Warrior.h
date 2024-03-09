#ifndef WARRIOR_H
#define WARRIOR_H

#include "RigidBody.h"
#include "Animation.h"
#include "Collider.h"
#include "Vector2D.h"
#include "GameObject.h"

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

            m_Flip = SDL_FLIP_NONE;
            m_JumpTime = JUMP_TIME;
            m_JumpForce = JUMP_FORCE;
            m_AttackTime = ATTACK_TIME;

            m_Collider = new Collider();
            m_Collider->SetBuffer(-5, 10, 15, 60);

            m_RigidBody = new RigidBody();
            m_RigidBody->SetGravity(3.0f);

            m_Animation = new Animation();
            m_Animation->SetProps("player_right", 0, 3, 100);
        }
        virtual void Draw();
        virtual void Clean();
        virtual void Update(float dt);
        Transform* GetPosition() { return m_Transform; }
        SDL_Rect GetBox() { return m_Collider->Get(); }

    private: 
        void AnimationState();
    private:
        bool m_IsRunning;
        bool m_IsJumping;
        bool m_IsFalling;
        bool m_IsGrounded;
        bool m_IsAttacking;

        float m_JumpTime;
        float m_JumpForce;
        float m_AttackTime;

        Collider* m_Collider;

        Animation* m_Animation;
        RigidBody* m_RigidBody;

        Vector2D m_LastSafePosition;

    protected:
        Transform* m_Transform;
        int m_Width, m_Height;
        std::string m_TextureID;
        SDL_RendererFlip m_Flip;
};

#endif
