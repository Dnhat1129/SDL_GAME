#include "PlayPK.h"
#include "TextureManager.h"
#include "SDL.h"
#include "Input.h"
#include "Camera.h"
#include "CollisionHandler.h"
#include "Engine.h"
#include "Warrior.h"
#include "Animation.h"
#include <fstream>
#include <sstream>

PlayPK* PlayPK::s_Instance = nullptr;

PlayPK::PlayPK() {
	//choose and set 
	PKList.push_back(PlayerPK(20000, 25, 3, 10, 600, 75, "goku.png", "goku_kame.png",  {375, 96, 70, 70} ));
	PKList.push_back(PlayerPK(20000, 25, 3, 10, 600, 75, "gokussj.png", "ssj_kame.png", { 515, 96, 70, 70 }));

	PKList.push_back(PlayerPK(20000, 25, 5, 7, 400, 150, "fide1.png", "skill_fide1.png", { 306, 192, 70, 70 }));
	PKList.push_back(PlayerPK(20000, 25, 5, 7, 400, 150, "fide2.png", "skill_fide2.png", { 445, 192, 70, 70 }));
	PKList.push_back(PlayerPK(20000, 25, 5, 7, 400, 150, "fide3.png", "skill_fide3.png", { 585, 192, 70, 70 }));

	PKList.push_back(PlayerPK(20000, 25, 5, 7, 400, 75, "so1.png", "", { 198, 288, 70, 70 }));
	PKList.push_back(PlayerPK(20000, 25, 5, 7, 400, 75, "so2.png", "", { 325, 288, 70, 70 }));
	PKList.push_back(PlayerPK(20000, 25, 5, 7, 400, 75, "so3.png", "", { 446, 288, 70, 70 }));
	PKList.push_back(PlayerPK(20000, 25, 5, 7, 400, 75, "so4.png", "", { 565, 288, 70, 70 }));
	PKList.push_back(PlayerPK(20000, 25, 5, 7, 200, 75, "tdt.png", "tdt_skill.png", { 682, 288, 70, 70 }));

	checkback = 0;
	checkstart = 0;

	//player1
	Properties* luu1 = new Properties("player1", 688, 436, 60, 75);
	player1 = new Warrior(luu1);

	p1_TextureID = luu1->TextureID;
	p1_Width = luu1->Width;
	p1_Height = luu1->Height;
	p1_Flip = luu1->Flip;

	p1_Transform = new Transform(luu1->X, luu1->Y);
	p1_Origin = new Point(luu1->X + luu1->Width/2 , luu1->Y + luu1->Height/2);

	p1_IsRunning = false;
	p1_IsJumping = false;
	p1_IsFalling = false;
	p1_IsGrounded = false;
	p1_IsAttacking = false;
	p1_IsSkill = false;
	p1_IsDie = false;

	p1_Flip = SDL_FLIP_NONE;
	p1_HP = 10000; p1_max_HP = 10000;
	p1_dame = 10;
	p1_time_skill = 0;
	p1_skill_frame = 5;
	p1_attack_frame = 10;
	p1_currentframe = 0;
	kc1 = 64;

	chon1 = 0;
	p1_path = "";
	p1_skill_path = "";

	p1_Collider = new Collider();
	p1_Collider->SetBuffer(-10, 0, 15, 0);

	p1_RigidBody = new RigidBody();
	p1_RigidBody->SetGravity(3.0f);

	p1_Animation = new Animation();
	p1_Animation->SetProps("player1", 0, 3, 100);

	p1_skill_Animation = new Animation();

	//player2
	Properties* luu2 = new Properties("player2", 1184, 436, 60, 75);
	player2 = new Warrior(luu2);

	p2_TextureID = luu2->TextureID;
	p2_Width = luu2->Width;
	p2_Height = luu2->Height;
	p2_Flip = luu2->Flip;

	p2_Transform = new Transform(luu2->X, luu2->Y);
	p2_Origin = new Point(luu2->X + luu2->Width / 2, luu2->Y + luu2->Height / 2);

	p2_IsRunning = false;
	p2_IsJumping = false;
	p2_IsFalling = false;
	p2_IsGrounded = false;
	p2_IsAttacking = false;
	p2_IsSkill = false;
	p2_IsDie = false;

	p2_Flip = SDL_FLIP_HORIZONTAL;
	p2_HP = 10000; p2_max_HP = 10000;
	p2_dame = 10;
	p2_time_skill = 0;
	p2_skill_frame = 5;
	p2_attack_frame = 10;
	p2_currentframe = 0;
	kc2 = 64;

	chon2 = 0;
	p2_path = "";
	p2_skill_path = "";

	p2_Collider = new Collider();
	p2_Collider->SetBuffer(-10, 0, 15, 0);

	p2_RigidBody = new RigidBody();
	p2_RigidBody->SetGravity(3.0f);

	p2_Animation = new Animation();
	p2_Animation->SetProps("player2", 0, 3, 100);

	p2_skill_Animation = new Animation();

	Cam = new Point((p1_Origin->X + p2_Origin->X) / 2, (p1_Origin->Y + p2_Origin->Y) / 2);
	delay = 0;
}

void PlayPK::UpdateModePK(float dt) {
	Vector2D Cam = Camera::GetInstance()->GetPosition();

	if (Input::GetInstance()->ListenMouse(Back)) checkback = 1;
	else checkback = 0;
	if (checkback || checkstart) return;
	if (!chon1) {
		for (int i = 0; i < PKList.size(); i++) {
			if (Input::GetInstance()->ListenMouse(PKList[i].choose)) {
				p1_HP = PKList[i].HP;
				p1_dame = PKList[i].Dame;
				p1_skill_frame = PKList[i].skill_frame;
				p1_attack_frame = PKList[i].attack_frame;
				p1_path = PKList[i].Path;
				p1_skill_path = PKList[i].skill_path;
				p1_skill_width = PKList[i].skill_frame_width;
				p1_skill_height = PKList[i].skill_frame_height;
				
				p1_choose = PKList[i].choose;
				delay += dt;
				if (delay > 10) {
					chon1 = 1;
					delay = 0;
				}
			}
		}
	}

	

	else if (!chon2) {
		for (int i = 0; i < PKList.size(); i++) {
			if (Input::GetInstance()->ListenMouse(PKList[i].choose)) {
				p2_HP = PKList[i].HP;
				p2_dame = PKList[i].Dame;
				p2_skill_frame = PKList[i].skill_frame;
				p2_attack_frame = PKList[i].attack_frame;
				p2_path = PKList[i].Path;
				p2_skill_path = PKList[i].skill_path;
				p2_choose = PKList[i].choose;
				p2_skill_width = PKList[i].skill_frame_width;
				p2_skill_height = PKList[i].skill_frame_height;

				delay += dt;
				if (delay > 10) {
					chon2 = 1;
					chon1 = 0;
					delay = 0;
				}
			}
		}
	}
	TextureManager::GetInstance()->Draw("muiten1", Cam.X + p1_choose.x, p1_choose.y - 70, 70, 50);
	TextureManager::GetInstance()->Draw("muiten2", Cam.X + p2_choose.x, p2_choose.y - 70, 70, 50);

	if ((!chon1 && chon2) || (chon1 && !chon2) || (chon1 && chon2)) {
		if (Input::GetInstance()->ListenMouse(Start)) {
			checkstart = 1;
		}
		else checkstart = 0;
	}
}

void PlayPK::Draw() {
	//player1
	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K) && p1_time_skill >= 0) {
		for (int i = 0; i < p1_skill_frame; i++) {
			if (p1_time_skill >= i * 100 && p1_time_skill < (i+1)*100) p1_currentframe = i;
			if (p1_time_skill >= p1_skill_frame * 100) p1_currentframe = -1;
		}
		if (p1_skill_height > 75) {
			if (p1_Flip == SDL_FLIP_NONE && p1_currentframe >= 0)
				p1_skill_Animation->DrawKame(p1_Transform->X, p1_Transform->Y - 75, p1_skill_width, p1_skill_height, p1_currentframe, p1_Flip);
			else p1_skill_Animation->DrawKame(p1_Transform->X - (p1_skill_width - 60), p1_Transform->Y - 75, p1_skill_width, p1_skill_height, p1_currentframe, p1_Flip);
		}
		else {
			if (p1_Flip == SDL_FLIP_NONE && p1_currentframe >= 0)
				p1_skill_Animation->DrawKame(p1_Transform->X, p1_Transform->Y, p1_skill_width, p1_skill_height, p1_currentframe, p1_Flip);
			else p1_skill_Animation->DrawKame(p1_Transform->X - (p1_skill_width - 60), p1_Transform->Y, p1_skill_width, p1_skill_height, p1_currentframe, p1_Flip);
		}
	}
	else p1_Animation->Draw(p1_Transform->X, p1_Transform->Y, p1_Width, p1_Height, p1_Flip);

	float vitri_khung1 = 0;
	if (Cam->X <= 480) vitri_khung1 = 0;
	else if (Cam->X > 480 && Cam->X <= 1440) vitri_khung1 = Cam->X - 480;
	else vitri_khung1 = 960;
	TextureManager::GetInstance()->Draw("P1HPMN", static_cast<int>(vitri_khung1), 0, 205, 150);
	int vitri_hp1 = 1.0 * p1_HP / p1_max_HP * 150; if (vitri_hp1 > 150) vitri_hp1 = 150;
	TextureManager::GetInstance()->Draw("P1HP", static_cast<int>(vitri_khung1) + 43, 12, vitri_hp1, 25);
	TextureManager::GetInstance()->Draw("P1MN", static_cast<int>(vitri_khung1) + 43, 47, 137, 12);
	//player2
	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_KP_2) && p2_time_skill >= 0) {
		for (int i = 0; i < p2_skill_frame; i++) {
			if (p2_time_skill >= i * 100 && p2_time_skill < (i + 1) * 100) p2_currentframe = i;
			if (p1_time_skill >= p2_skill_frame * 100) p2_currentframe = -1;
		}

		if (p2_skill_height > 75) {
			if (p2_Flip == SDL_FLIP_NONE && p2_currentframe >= 0)
				p2_skill_Animation->DrawKame(p2_Transform->X, p2_Transform->Y-75, p2_skill_width, p2_skill_height, p2_currentframe, p2_Flip);
			else p2_skill_Animation->DrawKame(p2_Transform->X - (p2_skill_width - 60), p2_Transform->Y-75, p2_skill_width, p2_skill_height, p2_currentframe, p2_Flip);
		}
		else {
			if (p2_Flip == SDL_FLIP_NONE && p2_currentframe >= 0)
				p2_skill_Animation->DrawKame(p2_Transform->X, p2_Transform->Y, p2_skill_width, p2_skill_height, p2_currentframe, p2_Flip);
			else p2_skill_Animation->DrawKame(p2_Transform->X - (p2_skill_width - 60), p2_Transform->Y, p2_skill_width, p2_skill_height, p2_currentframe, p2_Flip);
		}
	}
	else p2_Animation->Draw(p2_Transform->X, p2_Transform->Y, p2_Width, p2_Height, p2_Flip);

	float vitri_khung2 = vitri_khung1 + 755;
	TextureManager::GetInstance()->Draw("P2HPMN", static_cast<int>(vitri_khung2), 0, 205, 70,SDL_FLIP_HORIZONTAL);
	int vitri_hp2 = 1.0 * p2_HP / p2_max_HP * 150; if (vitri_hp2 > 150) vitri_hp2 = 150;
	TextureManager::GetInstance()->Draw("P2HP", static_cast<int>(vitri_khung2) + 10 + 150-vitri_hp2, 10, vitri_hp2, 25, SDL_FLIP_HORIZONTAL);
	TextureManager::GetInstance()->Draw("P2MN", static_cast<int>(vitri_khung2) + 22, 47, 137, 12, SDL_FLIP_HORIZONTAL);
}

void PlayPK::Update(float dt) {
	//player1
	if (p1_HP > p1_max_HP) p1_HP = p1_max_HP;
	p1_IsRunning = false;
	p1_RigidBody->UnSetForce();

	if (p1_HP <= 0) p1_IsDie = true;
	else p1_IsDie = false;

	if (p1_IsDie) {
		p1_IsRunning = false;
		p1_IsJumping = false;
		p1_IsFalling = false;
		p1_IsAttacking = false;
	}
	else {
		//Run forward
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_D) && !p1_IsAttacking) {
			p1_RigidBody->ApplyForceX(FORWARD * RUN_FORCE);
			p1_Flip = SDL_FLIP_NONE;
			p1_IsRunning = true;
		}

		//Run backward
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_A) && !p1_IsAttacking) {
			p1_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE);
			p1_Flip = SDL_FLIP_HORIZONTAL;
			p1_IsRunning = true;
		}

		//Attack
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_J)) {
			p1_RigidBody->UnSetForce();
			p1_IsAttacking = true;
		}
		else p1_IsAttacking = false;

		//Skill
		if (p1_time_skill < 0) {
			p1_time_skill += dt;
			p1_IsSkill = false;
		}
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K) && !p1_IsDie && p1_time_skill >= 0) {
			p1_skill_Animation->SetProps("player1_skill", 0, p1_skill_frame, 5500);
			p1_time_skill += dt;
			p1_IsSkill = true;

			if (p1_time_skill >= p1_skill_frame * 100) {
				p1_time_skill = -200* p1_skill_frame;
			}
		}
		else p1_IsSkill = false;
		
		//danh nhau
		if (p2_IsAttacking && abs(p1_Transform->X - p2_Transform->X) <= kc2 && abs(p1_Transform->Y - p2_Transform->Y) <= kc2) {
				p1_Time += dt;
				if (p1_Time > 5) {
					p1_Time = 0;
					p1_HP -= 2*p2_dame;
					if (p2_Flip == SDL_FLIP_NONE) p1_RigidBody->ApplyForceX(2 * FORWARD);
					else if (p2_Flip == SDL_FLIP_HORIZONTAL) p1_RigidBody->ApplyForceX(2 * BACKWARD);
					p1_RigidBody->Update(dt);
				}
		}

		if (p2_IsSkill && abs(p1_Transform->X - p2_Transform->X) <= p2_skill_width 
			&& abs(p1_Transform->Y - p2_Transform->Y) <= p2_skill_height && p2_currentframe == p2_skill_frame - 1) {
				p1_HP -= 2*p2_dame;
				if (p2_Flip == SDL_FLIP_NONE) p1_RigidBody->ApplyForceX(5 * FORWARD);
				else if (p2_Flip == SDL_FLIP_HORIZONTAL) p1_RigidBody->ApplyForceX(5 * BACKWARD);
				p1_RigidBody->Update(dt);
		}

		//Jump
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W)
			&& p1_IsGrounded) {
			p1_IsJumping = true;
			p1_IsGrounded = false;
			p1_RigidBody->ApplyForceY(UPWARD * JUMP_FORCE);
		}
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W)) {
			p1_IsJumping = true;
			p1_RigidBody->ApplyForceY(UPWARD * JUMP_FORCE);
		}
		else {
			p1_IsJumping = false;
		}

		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) && p1_IsJumping) {
			if (CollisionHandler::GetInstance()->MapCollision(p1_Collider->Get()) && !p1_IsGrounded) {
				p1_IsJumping = false;
				p1_IsFalling = true;
				p1_Transform->Y += 3;
			}
		}



		//Fall
		if (p1_RigidBody->Velocity().Y > 0 && !p1_IsGrounded && !p1_IsJumping) {

			p1_IsFalling = true;
		}

		else p1_IsFalling = false;

		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_S) && !CollisionHandler::GetInstance()->MapCollision(p1_Collider->Get())) {
			p1_IsFalling = true;
		}

		//move on x 
		p1_RigidBody->Update(dt);
		p1_LastSafePosition.X = p1_Transform->X;
		p1_Transform->X += p1_RigidBody->Position().X;
		p1_Collider->Set(static_cast<int>(p1_Transform->X), static_cast<int>(p1_Transform->Y), 60, 75);

		if (CollisionHandler::GetInstance()->MapCollision(p1_Collider->Get()))
		{
			p1_Transform->X = p1_LastSafePosition.X;
		}

		// move y
		p1_RigidBody->Update(dt);
		p1_LastSafePosition.Y = p1_Transform->Y;
		p1_Transform->Y += p1_RigidBody->Position().Y;
		p1_Collider->Set(static_cast<int>(p1_Transform->X), static_cast<int>(p1_Transform->Y), 60, 75);

		if (CollisionHandler::GetInstance()->MapCollision(p1_Collider->Get()) && !p1_IsJumping)
		{
			p1_Transform->Y = p1_LastSafePosition.Y;
			p1_IsGrounded = true;
		}
		else {
			p1_IsGrounded = false;
		}

		//Check vuot ra khoi map
		if (p1_Transform->X < 5 || p1_Transform->X + p1_Width > 1915) {
			p1_Transform->X = p1_LastSafePosition.X;
		}
		if (p1_Transform->Y < 5 || p1_Transform->Y + p1_Height > 635) {
			p1_Transform->Y = p1_LastSafePosition.Y;
		}
	}

	AnimationState("player1");
	p1_Animation->Update();
	p1_Origin->X = p1_Transform->X + p1_Width;
	p1_Origin->Y = p1_Transform->Y + p1_Height;

	//player2
	if (p2_HP > p2_max_HP) p2_HP = p2_max_HP;
	p2_IsRunning = false;
	p2_RigidBody->UnSetForce();

	if (p2_HP <= 0) p2_IsDie = true;
	else p2_IsDie = false;

	if (p2_IsDie) {
		p2_IsRunning = false;
		p2_IsJumping = false;
		p2_IsFalling = false;
		p2_IsAttacking = false;
	}
	else {
		//Run forward
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_RIGHT) && !p2_IsAttacking) {
			p2_RigidBody->ApplyForceX(FORWARD * RUN_FORCE);
			p2_Flip = SDL_FLIP_NONE;
			p2_IsRunning = true;
		}

		//Run backward
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_LEFT) && !p2_IsAttacking) {
			p2_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE);
			p2_Flip = SDL_FLIP_HORIZONTAL;
			p2_IsRunning = true;
		}

		//Attack
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_KP_1)) {
			p2_RigidBody->UnSetForce();
			p2_IsAttacking = true;
		}
		else p2_IsAttacking = false;

		//Skill
		if (p2_time_skill < 0) {
			p2_time_skill += dt;
			p2_IsSkill = false;
		}
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_KP_2) && !p1_IsDie && p2_time_skill >= 0) {
			p2_skill_Animation->SetProps("player2_skill", 0, p2_skill_frame, 5500);
			p2_time_skill += dt;
			p2_IsSkill = true;

			if (p2_time_skill >= p2_skill_frame * 100) {
				p2_time_skill = -200 * p2_skill_frame;
			}
		}
		else p2_IsSkill = false;

		//danh nhau
		if (p1_IsAttacking && abs(p1_Transform->X - p2_Transform->X) <= kc1 && abs(p1_Transform->Y - p2_Transform->Y) <= kc1) {
				p2_Time += dt;
				if (p2_Time > 5) {
					p2_Time = 0;
					p2_HP -= p1_dame;
					if (p1_Flip == SDL_FLIP_NONE) p2_RigidBody->ApplyForceX(2 * FORWARD);
					else if (p1_Flip == SDL_FLIP_HORIZONTAL) p2_RigidBody->ApplyForceX(2 * BACKWARD);
					p2_RigidBody->Update(dt);
				}
		}

		if (p1_IsSkill && abs(p1_Transform->X - p2_Transform->X) <= p1_skill_width 
			&& abs(p1_Transform->Y - p2_Transform->Y) <= p1_skill_height && p1_currentframe == p1_skill_frame-1) {
				p2_HP -= p1_dame;
				if (p1_Flip == SDL_FLIP_NONE) p2_RigidBody->ApplyForceX(5 * FORWARD);
				else if (p1_Flip == SDL_FLIP_HORIZONTAL) p2_RigidBody->ApplyForceX(5 * BACKWARD);
				p2_RigidBody->Update(dt);
		}
		
		//Jump
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)
			&& p2_IsGrounded) {
			p2_IsJumping = true;
			p2_IsGrounded = false;
			p2_RigidBody->ApplyForceY(UPWARD * JUMP_FORCE);
		}
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)) {
			p2_IsJumping = true;
			p2_RigidBody->ApplyForceY(UPWARD * JUMP_FORCE);
		}
		else {
			p2_IsJumping = false;
		}

		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP) && p2_IsJumping) {
			if (CollisionHandler::GetInstance()->MapCollision(p2_Collider->Get()) && !p2_IsGrounded) {
				p2_IsJumping = false;
				p2_IsFalling = true;
				p2_Transform->Y += 3;
			}
		}



		//Fall
		if (p2_RigidBody->Velocity().Y > 0 && !p2_IsGrounded && !p2_IsJumping) {

			p2_IsFalling = true;
		}

		else p2_IsFalling = false;

		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_DOWN) && !CollisionHandler::GetInstance()->MapCollision(p2_Collider->Get())) {
			p2_IsFalling = true;
		}

		//move on x 
		p2_RigidBody->Update(dt);
		p2_LastSafePosition.X = p2_Transform->X;
		p2_Transform->X += p2_RigidBody->Position().X;
		p2_Collider->Set(static_cast<int>(p2_Transform->X), static_cast<int>(p2_Transform->Y), 60, 75);

		if (CollisionHandler::GetInstance()->MapCollision(p2_Collider->Get()))
		{
			p2_Transform->X = p2_LastSafePosition.X;
		}

		// move y
		p2_RigidBody->Update(dt);
		p2_LastSafePosition.Y = p2_Transform->Y;
		p2_Transform->Y += p2_RigidBody->Position().Y;
		p2_Collider->Set(static_cast<int>(p2_Transform->X), static_cast<int>(p2_Transform->Y), 60, 75);

		if (CollisionHandler::GetInstance()->MapCollision(p2_Collider->Get()) && !p2_IsJumping)
		{
			p2_Transform->Y = p2_LastSafePosition.Y;
			p2_IsGrounded = true;
		}
		else {
			p2_IsGrounded = false;
		}

		//Check vuot ra khoi map
		if (p2_Transform->X < 5 || p2_Transform->X + p2_Width > 1915) {
			p2_Transform->X = p2_LastSafePosition.X;
		}
		if (p2_Transform->Y < 5 || p2_Transform->Y + p2_Height > 635) {
			p2_Transform->Y = p2_LastSafePosition.Y;
		}
	}

	AnimationState("player2");
	p2_Animation->Update();
	p2_Origin->X = p2_Transform->X + p2_Width;
	p2_Origin->Y = p2_Transform->Y + p2_Height;

	Cam->X = (p1_Origin->X + p2_Origin->X) / 2;
	Cam->Y = (p1_Origin->Y + p2_Origin->Y) / 2;

}

void PlayPK::AnimationState(std::string id) {
	//player1
	if (id == "player1") {
		//dungim
		p1_Animation->SetProps(id, 0, 3, 100);

		//run
		if (p1_IsRunning) {
			p1_Animation->SetProps(id, 3, 5, 100);
		}

		//Jump
		if (p1_IsJumping) {
			p1_Animation->SetProps(id, 1, 1, 100);
		}

		//fall
		if (p1_IsFalling) {
			p1_Animation->SetProps(id, 2, 1, 100);
		}

		//attack
		if (p1_IsAttacking) {
			p1_Animation->SetProps(id, 4, p1_attack_frame, 50);
		}
		//if (p1_IsDie) {
			//p1_Animation->SetProps(id, 5, 1, 100);
		//}
	}

	//player2
	if (id == "player2") {
		//dungim
		p2_Animation->SetProps(id, 0, 3, 100);

		//run
		if (p2_IsRunning) {
			p2_Animation->SetProps(id, 3, 5, 100);
		}

		//Jump
		if (p2_IsJumping) {
			p2_Animation->SetProps(id, 1, 1, 100);
		}

		//fall
		if (p2_IsFalling) {
			p2_Animation->SetProps(id, 2, 1, 100);
		}

		//attack
		if (p2_IsAttacking) {
			p2_Animation->SetProps(id, 4, p2_attack_frame, 50);
		}
		//if (p2_IsDie) {
			//p2_Animation->SetProps(id, 5, 1, 100);
		//}
	}
}

void PlayPK::Load() {
	TextureManager::GetInstance()->Load("player1", "LamGame/Picture/ModePK/" + p1_path);
	TextureManager::GetInstance()->Load("player1_skill", "LamGame/Picture/ModePK/" + p1_skill_path);

	TextureManager::GetInstance()->Load("player2", "LamGame/Picture/ModePK/" + p2_path);
	TextureManager::GetInstance()->Load("player2_skill", "LamGame/Picture/ModePK/" + p2_skill_path);

	TextureManager::GetInstance()->Load("P1HPMN", "LamGame/Picture/hp + mana/mau + mana.png");
	TextureManager::GetInstance()->Load("P1HP", "LamGame/Picture/ModePK/mau.png");
	TextureManager::GetInstance()->Load("P1MN", "LamGame/Picture/ModePK/mana.png");

	TextureManager::GetInstance()->Load("P2HPMN", "LamGame/Picture/ModePK/mau + mana.png");
	TextureManager::GetInstance()->Load("P2HP", "LamGame/Picture/ModePK/mau.png");
	TextureManager::GetInstance()->Load("P2MN", "LamGame/Picture/ModePK/mana.png");

	TextureManager::GetInstance()->Load("muiten1", "LamGame/Picture/ModePK/chon1.png");
	TextureManager::GetInstance()->Load("muiten2", "LamGame/Picture/ModePK/chon2.png");
}

void PlayPK::Clean() {
	TextureManager::GetInstance()->Drop("player1");
	TextureManager::GetInstance()->Drop("player1_skill");

	TextureManager::GetInstance()->Drop("player2");
	TextureManager::GetInstance()->Drop("player2_skill");

	TextureManager::GetInstance()->Drop("P1HPMN");
	TextureManager::GetInstance()->Drop("P1HP");
	TextureManager::GetInstance()->Drop("P1MN");

	TextureManager::GetInstance()->Drop("P2HPMN");
	TextureManager::GetInstance()->Drop("P2HP");
	TextureManager::GetInstance()->Drop("P2MN");
}

void PlayPK::Luu() {
	std::ofstream out("LamGame/PK.txt");
	if (out.is_open()) {
		out << p1_Transform->X << std::endl << p1_Transform->Y << std::endl;
		out << p1_HP << std::endl << p1_MN << std::endl << p1_dame << std::endl;
		out << p1_IsSkill << std::endl << p1_time_skill << std::endl;
		out << p1_skill_frame << std::endl << p1_attack_frame << std::endl;
		out << p1_path << std::endl << p1_skill_path << std::endl;
		out << p1_skill_width << std::endl << p1_skill_height << std::endl;

		out << p2_Transform->X << std::endl << p2_Transform->Y << std::endl;
		out << p2_HP << std::endl << p2_MN << std::endl << p2_dame << std::endl;
		out << p2_IsSkill << std::endl << p2_time_skill << std::endl;
		out << p2_skill_frame << std::endl << p2_attack_frame << std::endl;
		out << p2_path << std::endl << p2_skill_path << std::endl;
		out << p2_skill_width << std::endl << p2_skill_height << std::endl;
	}
	out.close();

}

void PlayPK::SetContinue() {
	std::ifstream in("LamGame/PK.txt");
	std::string line;
	std::string value;
	int dem = 0;
	while (std::getline(in, line)) {
		dem++;
		std::stringstream iss(line);
		switch (dem) {
		case 1: iss >> p1_Transform->X; break;
		case 2: iss >> p1_Transform->Y; break;
		case 3: iss >> p1_HP; break;
		case 4: iss >> p1_MN; break;
		case 5: iss >> p1_dame; break;
		case 6: iss >> p1_IsSkill; break;
		case 7: iss >> p1_time_skill; break;
		case 8: iss >> p1_skill_frame; break;
		case 9: iss >> p1_attack_frame; break;
		case 10: iss >> p1_path; break;
		case 11: iss >> p1_skill_path; break;
		case 12: iss >> p1_skill_width; break;
		case 13: iss >> p1_skill_height; break;

		case 14: iss >> p2_Transform->X; break;
		case 15: iss >> p2_Transform->Y; break;
		case 16: iss >> p2_HP; break;
		case 17: iss >> p2_MN; break;
		case 18: iss >> p2_dame; break;
		case 19: iss >> p2_IsSkill; break;
		case 20: iss >> p2_time_skill; break;
		case 21: iss >> p2_skill_frame; break;
		case 22: iss >> p2_attack_frame; break;
		case 23: iss >> p2_path; break;
		case 24: iss >> p2_skill_path; break;
		case 25: iss >> p2_skill_width; break;
		case 26: iss >> p2_skill_height; break;
		default: break;
		}
	}
}