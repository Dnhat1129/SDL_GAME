#include "Boss.h"
#include "TextureManager.h"
#include "SDL.h"
#include "CollisionHandler.h"
#include "Warrior.h"
#include "GameObject.h"
#include "Camera.h"
#include "TileLayer.h"
#include <fstream>
#include <sstream>
#include <cmath>

Boss::Boss(Properties* e_props, int currentboss) : GameObject(e_props) {
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
	e_skill = false;


	Frieza.push_back(BossList(50000, "fide1.png", "skill_fide1.png", 50));
	Frieza.push_back(BossList(75000, "fide2.png", "skill_fide2.png", 75));
	Frieza.push_back(BossList(100000, "fide3.png", "skill_fide3.png", 100));

	CurrentBoss = currentboss;
	e_Flip = SDL_FLIP_NONE;
	e_max_HP = Frieza[CurrentBoss].HP;
	e_HP = Frieza[CurrentBoss].HP;
	e_time = 0;
	e_dame = Frieza[CurrentBoss].Dame;
	completemap2 = false;

	e_Collider = new Collider();
	e_Collider->SetBuffer(-5, 0, 10, 0);

	e_RigidBody = new RigidBody();
	e_RigidBody->SetGravity(3.0f);

	e_Animation = new Animation();
	e_Animation->SetProps("boss", 0, 3, 100);

	boss_skill = new Animation();
}

void Boss::Draw() {
	
	if (e_skill && e_time >= 0) {
		if (e_time >= 0 && e_time <= 50) skill_frame = 0;
		else if (e_time > 50 && e_time <= 100) skill_frame = 1;
		else if (e_time > 100 && e_time <= 150) skill_frame = 2;
		else if (e_time > 150 && e_time <= 175) skill_frame = 3;
		else if (e_time > 175 && e_time <= 200) skill_frame = 4;
		else {
			skill_frame = -1;
			TextureManager::GetInstance()->Drop("boss_skill");
		}
		if (e_Flip == SDL_FLIP_NONE && skill_frame >= 0)
			boss_skill->DrawKame(e_Transform->X, e_Transform->Y, 400, 150, skill_frame, e_Flip);
		else boss_skill->DrawKame(e_Transform->X, e_Transform->Y, 400, 150, skill_frame, e_Flip);
	}
	else e_Animation->Draw(e_Transform->X, e_Transform->Y, e_Width, e_Height, e_Flip);
	//std::cout << e_skill << " " << e_time << std::endl;
	int vitri_hp = 1.0 * e_HP / e_max_HP * 75; if (vitri_hp > 75) vitri_hp = 75;

	if (e_skill) {
		if (e_Flip == SDL_FLIP_HORIZONTAL) {
			TextureManager::GetInstance()->Draw("boss_hp", e_Transform->X - 12 + 350, e_Transform->Y + 65, vitri_hp, 10);
		}
			
		else TextureManager::GetInstance()->Draw("boss_hp", e_Transform->X - 12, e_Transform->Y + 65, vitri_hp, 10);
	}
	else TextureManager::GetInstance()->Draw("boss_hp", e_Transform->X - 12, e_Transform->Y -10, vitri_hp, 10);

	/*Vector2D cam = Camera::GetInstance()->GetPosition();
	SDL_Rect box = e_Collider->Get();
	box.x -= cam.X;
	box.y -= cam.Y;
	SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);*/
}

void Boss::Update(float dt) {
	if (Engine::GetInstance()->GetMap2() && !completemap2) {
		if (e_HP <= 0 && CurrentBoss != Frieza.size() - 1) {
			e_IsDie = true;
		}
		else e_IsDie = false;


		if (e_HP < 0 && CurrentBoss == Frieza.size() - 1) {
			e_IsRunning = false;
			e_IsJumping = false;
			e_IsFalling = false;
			e_IsGrounded = false;
			e_IsAttacking = false;
			completemap2 = true;
			Clean();
		}
		else {
			e_IsRunning = false;
			e_RigidBody->UnSetForce();
			Warrior* player = Engine::GetInstance()->GetWarrior();
			Transform* m_Transform = player->GetPosition();
			SDL_Rect box_Player = player->GetBox();
			SDL_Rect box_Enemy = e_Collider->Get();
			e_Flip = player->GetFlip();


			//Set Flip
			if (e_Transform->X < m_Transform->X) {
				e_Flip = SDL_FLIP_NONE;
			}
			else if (e_Transform->X > m_Transform->X) {
				e_Flip = SDL_FLIP_HORIZONTAL;
			}
			else e_Flip = player->GetFlip();

			if (player->GetKameFrame() == 2 && player->GetIsKame()) {
				if (player->GetFlip() == SDL_FLIP_NONE) {
					if (e_Transform->X > m_Transform->X && e_Transform->X <= m_Transform->X + 600 && (abs(e_Transform->Y - m_Transform->Y) < 32.0f))
						e_HP -= 3 * player->GetDame();
					if (player->GetFlip() == SDL_FLIP_NONE) e_RigidBody->ApplyForceX(5 * FORWARD);
					else if (player->GetFlip() == SDL_FLIP_HORIZONTAL) e_RigidBody->ApplyForceX(5 * BACKWARD);
				}
				else if (player->GetFlip() == SDL_FLIP_HORIZONTAL) {
					if (e_Transform->X < m_Transform->X && e_Transform->X >= m_Transform->X - 540 && (abs(e_Transform->Y - m_Transform->Y) < 32.0f))
						e_HP -= 3 * player->GetDame();
					if (player->GetFlip() == SDL_FLIP_NONE) e_RigidBody->ApplyForceX(10 * FORWARD);
					else if (player->GetFlip() == SDL_FLIP_HORIZONTAL) e_RigidBody->ApplyForceX(10 * BACKWARD);
				}

			}


			//Attack timer
			if (CollisionHandler::GetInstance()->CheckCollision(box_Enemy, box_Player)) {
				e_RigidBody->UnSetForce();
				e_IsAttacking = true;
				e_IsRunning = false;
				e_IsFalling = false;
				if (player->CheckAttack() && e_Flip != player->GetFlip()) {
					e_HP -= player->GetDame();
					if (player->GetFlip() == SDL_FLIP_NONE) e_RigidBody->ApplyForceX(2 * FORWARD);
					else if (player->GetFlip() == SDL_FLIP_HORIZONTAL) e_RigidBody->ApplyForceX(2 * BACKWARD);
					e_RigidBody->Update(dt);
				}
			}
			else {
				e_IsAttacking = false;
					//X
					if (abs(e_Transform->X - m_Transform->X) < 10.0f) {
						e_IsRunning = false;
					}

					else if (e_Transform->X < m_Transform->X) {
						e_IsRunning = true;
						e_RigidBody->ApplyForceX(FORWARD * RUN_FORCE);
					}
					else if (e_Transform->X > m_Transform->X) {
						e_IsRunning = true;
						e_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE);
					}
					e_RigidBody->Update(dt);

					//Y 
					e_RigidBody->Update(dt);
					if (abs(e_Transform->Y - m_Transform->Y) < 10.0f) {
						e_IsJumping = false;
						e_IsFalling = false;
						//e_IsRunning = true;
					}

					else if (e_Transform->Y < m_Transform->Y) {
						e_IsRunning = false;
						e_IsFalling = true;
					}
					else if (e_Transform->Y > m_Transform->Y) {
						e_IsJumping = true;
						e_IsGrounded = false;
						e_IsRunning = false;
						e_RigidBody->ApplyForceY(UPWARD * JUMP_FORCE);
					}
					e_RigidBody->Update(dt);
				}

			//Fall
			if (e_RigidBody->Velocity().Y > 0 && !e_IsGrounded && !e_IsJumping) {
				e_IsFalling = true;
			}

			else e_IsFalling = false;

			//move on x 
			e_RigidBody->Update(dt);
			e_LastSafePosition.X = e_Transform->X;
			e_Transform->X += e_RigidBody->Position().X;
			e_Collider->Set(static_cast<int>(e_Transform->X), static_cast<int>(e_Transform->Y), 60, 75);

			if (CollisionHandler::GetInstance()->MapCollision(e_Collider->Get()))
			{
				e_Transform->X = e_LastSafePosition.X;
			}

			// move y
			e_RigidBody->Update(dt);
			e_LastSafePosition.Y = e_Transform->Y;
			e_Transform->Y += e_RigidBody->Position().Y;
			e_Collider->Set(static_cast<int>(e_Transform->X), static_cast<int>(e_Transform->Y), 60, 75);

			if (e_IsJumping && !e_IsGrounded) {
				if (CollisionHandler::GetInstance()->MapCollision(e_Collider->Get()) && !e_IsGrounded) {
					e_IsJumping = false;
					e_IsRunning = false;
					e_IsFalling = true;
					e_Transform->Y += 3;
				}
			}
			if (CollisionHandler::GetInstance()->MapCollision(e_Collider->Get()) && !e_IsJumping)
			{
				e_Transform->Y = e_LastSafePosition.Y;
				e_IsGrounded = true;
			}
			else {
				e_IsGrounded = false;
			}

			//skill
			e_time += dt;
			if (e_time >= 0) {
				e_skill = true;
				boss_skill->SetProps("boss_skill", 0, 6, 6000);
				if (e_time > 200) e_time = -1000;

			}
			else e_skill = false;

			if (e_skill) {
				e_Transform->Y = m_Transform->Y - 75;
				if (e_Flip == SDL_FLIP_NONE && m_Transform->X - 350 > 0) {
					e_Collider->Set(static_cast<int>(m_Transform->X-350), static_cast<int>(e_Transform->Y), 60, 75);
					if (!CollisionHandler::GetInstance()->MapCollision(e_Collider->Get()))
					{
						e_Transform->X = m_Transform->X - 350;
					}
					else e_Flip = SDL_FLIP_HORIZONTAL;
				}
				else if (e_Flip == SDL_FLIP_HORIZONTAL && m_Transform->X + 350 < 1920) {
					e_Collider->Set(static_cast<int>(m_Transform->X+350), static_cast<int>(e_Transform->Y), 60, 75);
					if (!CollisionHandler::GetInstance()->MapCollision(e_Collider->Get()))
					{
						e_Transform->X = m_Transform->X;
					}
				}
			}

			//Check vuot ra khoi map
			if (e_Transform->X < 5 || e_Transform->X + e_Width > 1915) {
				e_Transform->X = e_LastSafePosition.X;
			}
			if (e_Transform->Y < 5 || e_Transform->Y + e_Height > 635) {
				e_Transform->Y = e_LastSafePosition.Y;
			}

			e_AnimationState();
			e_Animation->Update();
		}
	}
	else {
		e_IsRunning = false;
		e_IsJumping = false;
		e_IsFalling = false;
		e_IsGrounded = false;
		e_IsAttacking = false;
		e_IsDie = false;
		e_skill = false;
	}
}

void Boss::e_AnimationState() {
	//dungim
	e_Animation->SetProps("boss", 0, 1, 100);

	//run
	if (e_IsRunning) {
		e_Animation->SetProps("boss", 3, 5, 100);
	}

	//Jump
	if (e_IsJumping) {
		e_Animation->SetProps("boss", 1, 1, 100);
	}

	//fall
	if (e_IsFalling) {
		e_Animation->SetProps("boss", 2, 1, 100);
	}

	//attack
	if (e_IsAttacking) {
		e_Animation->SetProps("boss", 4, 7, 50);
	}
	//skill
	//if (e_skill && CurrentBoss != Frieza.size()) {
		//boss_skill->SetProps("boss_skill", 0, 5, 1000);
	//}
}

void Boss::Clean() {
	TextureManager::GetInstance()->Drop("boss");
	TextureManager::GetInstance()->Drop("boss_hp");
	TextureManager::GetInstance()->Drop("boss_skill");
}

void Boss::Load() {
	TextureManager::GetInstance()->Load("boss", "LamGame/Picture/Boss/" + Frieza[CurrentBoss].Texture);
	TextureManager::GetInstance()->Load("boss_skill", "LamGame/Picture/Boss/" + Frieza[CurrentBoss].skill_Texture);
	TextureManager::GetInstance()->Load("boss_hp", "LamGame/Picture/Boss/HP.png");
}

void Boss::Luu() {
	std::ofstream out("LamGame/Picture/Pause/boss.txt");
	if (out.is_open()) {
		out << e_Transform->X << std::endl << e_Transform->Y << std::endl;
		out << e_HP << std::endl << e_dame << std::endl;
		out << CurrentBoss << std::endl << e_time << std::endl;
	}
	out.close();

}

void Boss::SetContinue() {
	std::ifstream in("LamGame/Picture/Pause/boss.txt");
	std::string line;
	int dem = 0;
	while (std::getline(in, line)) {
		dem++;
		std::istringstream iss(line);
		switch (dem) {
		case 1: iss >> e_Transform->X; break;
		case 2: iss >> e_Transform->Y; break;
		case 3: iss >> e_HP; break;
		case 4: iss >> e_dame; break;
		case 5: iss >> CurrentBoss; break;
		case 6: iss >> e_time; break;
		default: break;
		}
	}
}