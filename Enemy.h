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
  
    Enemy(Properties* e_props, int currentenemy);

    virtual void Draw();
    virtual void Clean();
    virtual void Update(float dt);
    void Load();
    void Luu();
    void SetContinue();

    SDL_Rect GetBox() { return e_Collider->Get(); }
    bool CheckAttack() { return e_IsAttacking; }
    int GetHP() { return e_HP; }
    int GetDame() { return e_dame; }
    bool GetIsDie() { return e_IsDie; }
    Transform* GetPosition() { return e_Transform; }

    bool GetIsRunning() { return e_IsRunning; }
    SDL_RendererFlip GetFlip() { return e_Flip; }
    bool GetComplete1() {return completemap1;}
    bool GetSkill() { return e_skill; }
    int GetCurrentEnemy() { 
        if (CurrentEnemy != TDST.size() - 1) return (CurrentEnemy+1);
        return -1;
    }

    /*void Reset() {
        e_Transform->X = 1184;
        e_Transform->Y = 437;
        CurrentEnemy = 0;
        e_HP = TDST[CurrentEnemy].HP;
        e_dame = TDST[CurrentEnemy].Dame;
        completemap1 = false;
        e_time = 0;
        e_skill = false;
        e_IsDie = false;

    }*/
    
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
    bool e_skill;

    float e_JumpForce;
    float e_time;
    int e_HP;
    int e_dame;

    Collider* e_Collider;

    Animation* e_Animation;
    RigidBody* e_RigidBody;

    Vector2D e_LastSafePosition;
private:
    int CurrentEnemy;
    std::vector <EnemyList> TDST;
    int e_max_HP;
    bool completemap1;
};

#endif
