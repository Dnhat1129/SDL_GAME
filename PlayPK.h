#ifndef PLAYPK_H
#define PLAYPK_H

#include "RigidBody.h"
#include "Animation.h"
#include "Collider.h"
#include "Vector2D.h"
#include "GameObject.h"
#include <vector>
#include "Point.h"
#include "SDL.h"


struct PlayerPK {
    bool Isbuy; int Gia;
    int HP;
    int Dame;
    int skill_frame, attack_frame;
    int skill_frame_width, skill_frame_height;
    std::string Path, skill_path;
    SDL_Rect choose;

    PlayerPK(bool isbuy = 0, int gia = 0, int hp = 0, int dame = 0, int skillframe = 0, int attackframe = 0, int skillframewidth = 0, int skillframeheight = 0, std::string path = "", std::string skillpath = "", SDL_Rect chose = { 0,0,0,0 }) :
        Isbuy(isbuy), Gia(gia), HP(hp), Dame(dame), skill_frame(skillframe), attack_frame(attackframe), skill_frame_width(skillframewidth), skill_frame_height(skillframeheight), Path(path), skill_path(skillpath), choose(chose) {}
};

class PlayPK
{
public:
    PlayPK();
    static PlayPK* GetInstance() {
        if (s_Instance != nullptr)
            return s_Instance;
        else
            return s_Instance = new PlayPK();
    }
    void Update(float dt);
    void Draw();
    void Load();
    void Clean();

    void UpdateModePK(float dt);
    void Luu();
    void SetContinue();
    void SetPrevious();

    Point* GetOrigin() { return Cam; }
    bool Checkback() { return checkback; }
    bool Checkstart() { return checkstart; }

    Warrior* GetPlayer1() { return player1; }
    Warrior* GetPlayer2() { return player2; }
    std::vector<Warrior> CharacterList;

    SDL_Rect GetChoose1() { return p1_choose; }
    SDL_Rect GetChoose2() { return p2_choose; }

    bool GetIsRunning() { return p1_IsRunning || p2_IsRunning; }
    bool GetIsAttacking() { return p1_IsAttacking || p2_IsAttacking; }
    bool GetIsSkill() { return p1_IsSkill || p2_IsSkill; }

    int Getluugia() { return luugia; }

    void AnimationState(std::string id);
    
private:
    static PlayPK* s_Instance;
    Warrior* player1;
    Warrior* player2;
    Point* Cam;
    SDL_Rect Back = { 430, 15, 85, 35 }; bool checkback;
    SDL_Rect Start = { 430, 582, 85, 35 }; bool checkstart;
    std:: vector <PlayerPK> PKList;
    float delay;
    int luugia;

//player1
private: 
    bool p1_IsRunning;
    bool p1_IsJumping;
    bool p1_IsFalling;
    bool p1_IsGrounded;
    bool p1_IsAttacking;

    Collider* p1_Collider;

    Animation* p1_Animation;
    RigidBody* p1_RigidBody;
    Vector2D p1_LastSafePosition;

    bool p1_IsDie;
    int p1_HP; int p1_max_HP;
    int p1_MN;
    int p1_dame;
    float p1_Time;
    int kc1;
    int p1_skill_width;
    int p1_skill_height;
    bool p1_Isbuy;

    bool p1_IsSkill; int p1_skill_frame, p1_attack_frame; int p1_currentframe;
    float p1_time_skill;
    Animation* p1_skill_Animation;

    Transform* p1_Transform;
    int p1_Width, p1_Height;
    std::string p1_TextureID;
    SDL_RendererFlip p1_Flip;
    Point* p1_Origin;
    int chon1;
    SDL_Rect p1_choose;

    std::string p1_path;
    std::string p1_skill_path;

//player2
private:
    bool p2_IsRunning;
    bool p2_IsJumping;
    bool p2_IsFalling;
    bool p2_IsGrounded;
    bool p2_IsAttacking;

    Collider* p2_Collider;

    Animation* p2_Animation;
    RigidBody* p2_RigidBody;
    Vector2D p2_LastSafePosition;

    bool p2_IsDie;
    int p2_HP; int p2_max_HP;
    int p2_MN;
    int p2_dame;
    float p2_Time;
    int kc2;
    int p2_skill_width;
    int p2_skill_height;
    bool p2_Isbuy;

    bool p2_IsSkill; int p2_skill_frame, p2_attack_frame; int p2_currentframe;
    float p2_time_skill;
    Animation* p2_skill_Animation;

    Transform* p2_Transform;
    int p2_Width, p2_Height;
    std::string p2_TextureID;
    SDL_RendererFlip p2_Flip;
    Point* p2_Origin;
    int chon2;
    SDL_Rect p2_choose;

    std::string p2_path;
    std::string p2_skill_path;
};

#endif // !PLAYPK_H