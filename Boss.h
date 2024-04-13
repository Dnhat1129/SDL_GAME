#ifndef BOSS_H

#include "GameObject.h"
#include "RigidBody.h"
#include "Animation.h"
#include "Collider.h"
#include "Vector2D.h"
#include <vector>
#include "Engine.h"
#include "TileLayer.h"
#include "Warrior.h"
#include <string>

struct BossList {
    int HP;
    std::string Texture, skill_Texture;
    int Dame;
    BossList(int hp = 0, std::string texture = "", std::string skill_texture ="", int dame = 0) : 
        HP(hp), Texture(texture), Dame(dame), skill_Texture(skill_texture) {}
};

class Boss : public GameObject {
public:

    Boss(Properties* e_props, int currentboss);

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
    bool GetComplete2() { return completemap2; }
    bool GetSkill() { return e_skill; }
    int GetCurrentBoss() {
        if (CurrentBoss != Frieza.size() - 1) return (CurrentBoss + 1);
        return -1;
    }
    float Gettime() { return e_time; }

    /* Reset() {
        e_Transform->X = 1280;
        e_Transform->Y = 512;
        CurrentBoss = 0;
        e_HP = Frieza[CurrentBoss].HP;
        e_dame = Frieza[CurrentBoss].Dame;
        completemap2 = false;
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

    float e_time;
    int e_HP;
    int e_dame;
    int skill_frame;

    Collider* e_Collider;

    Animation* boss_skill;
    Animation* e_Animation;
    RigidBody* e_RigidBody;

    Vector2D e_LastSafePosition;
private:
    int CurrentBoss;
    std::vector <BossList> Frieza;
    int e_max_HP;
    bool completemap2;
};

#endif // !BOSS_H


