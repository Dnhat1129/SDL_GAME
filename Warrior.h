#ifndef WARRIOR_H
#define WARRIOR_H

#include "Character.h"
#include "RigidBody.h"
#include "Animation.h"

class Warrior : public Character {
    public:
        Warrior(Properties* props);

        virtual void Draw();
        virtual void Clean();
        virtual void Update(float dt);

    public:
        Animation* m_Animation;
        RigidBody* m_RigidBody;
};


#endif
