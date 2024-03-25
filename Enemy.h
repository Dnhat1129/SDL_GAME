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
#include <string>

struct EnemyList {
    int HP;
    std::string Texture;
    int Dame;
    EnemyList(int hp = 0, std::string texture ="", int dame = 0) : HP(hp), Texture (texture), Dame(dame) {}
};

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
            e_IsDie = false;

            
            TDST.push_back(EnemyList(5000, "so1.png", 5));
            TDST.push_back(EnemyList(10000, "so2.png", 10));

            e_Flip = SDL_FLIP_NONE;
            e_JumpForce = JUMP_FORCE;
            e_HP = TDST[0].HP;
            e_time = 0;
            e_dame = TDST[0].Dame;
            isCurrentEnemyUpdated = false;
            

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
    bool CheckAttack() { return e_IsAttacking; }
    int GetHP() { return e_HP; }
    int GetDame() { return e_dame; }
    bool GetIsDie() { return e_IsDie; }
    Transform* GetPosition() { return e_Transform; }

    
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
    bool e_IsDie;


    float e_JumpForce;
    float e_time;
    int e_HP;
    int e_dame;

    Collider* e_Collider;

    Animation* e_Animation;
    RigidBody* e_RigidBody;

    Vector2D e_LastSafePosition;
private:
    int CurrentEnemy = 0;
    bool isCurrentEnemyUpdated;
    std::vector <EnemyList> TDST;
};

#endif
