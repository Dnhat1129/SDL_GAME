#ifndef WARRIOR_H
#define WARRIOR_H

#include "Character.h"
#include "RigidBody.h"
#include "Animation.h"
#include "Collider.h"
#include "Vector2D.h"

#define JUMP_TIME 15.0f
#define JUMP_FORCE 10.0f

class Warrior : public Character {
    public:
        Warrior(Properties* props);

        virtual void Draw();
        virtual void Clean();
        virtual void Update(float dt);

    private:
        bool m_IsJumping;
        bool m_IsGrounded;

        float m_JumpTime;
        float m_JumpForce;

        Collider* m_Collider;

        Animation* m_Animation;
        RigidBody* m_RigidBody;

        Vector2D m_LastSafePosition;
};


#endif
