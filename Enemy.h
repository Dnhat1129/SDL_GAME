#ifndef ENEMY_H
#define ENEMY_H

#include "GameObject.h"
#include "RigidBody.h"
#include "Animation.h"
#include "Collider.h"
#include "Vector2D.h"
#include <vector>
#include "Engine.h"
#include "TileLayer.h"
#include "ShortestPath.h"
#include "Warrior.h"

class Enemy : public GameObject {
public:
  
        Enemy(Properties* e_props) : GameObject(e_props) {
            e_TextureID = e_props->TextureID;
            e_Width = e_props->Width;
            e_Height = e_props->Height; 
            e_Flip = e_props->Flip;

            e_Transform = new Transform(e_props->X, e_props->Y);
    
            e_IsRunning = false;
            e_IsJumping = false;
            e_IsFalling = false;
            e_IsGrounded = false;
            e_IsAttacking = false;

            e_Flip = SDL_FLIP_NONE;
            e_JumpTime = JUMP_TIME;
            e_JumpForce = JUMP_FORCE;
            e_AttackTime = ATTACK_TIME;

            e_Collider = new Collider();
            e_Collider->SetBuffer(-5, 0, 10, 0);

            e_RigidBody = new RigidBody();
            e_RigidBody->SetGravity(3.0f);

            e_Animation = new Animation();
            e_Animation->SetProps("enemy", 0, 3, 100);
    }

    virtual void Draw();
    virtual void Clean();
    virtual void Update(float dt);
    void Load();
    SDL_Rect GetBox() { return e_Collider->Get(); }
    std::vector <Enemy> EnemyList;
    bool CheckAttack() { return e_IsAttacking; }

private: Transform* e_Transform;
       int e_Width, e_Height;
       std::string e_TextureID;
       SDL_RendererFlip e_Flip;

private:
    void e_AnimationState();

private:
    bool e_IsRunning;
    bool e_IsJumping;
    bool e_IsFalling;
    bool e_IsGrounded;
    bool e_IsAttacking;

    float e_JumpTime;
    float e_JumpForce;
    float e_AttackTime;

    Collider* e_Collider;

    Animation* e_Animation;
    RigidBody* e_RigidBody;

    Vector2D e_LastSafePosition;
    float e_time;
};

#endif
