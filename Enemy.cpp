#include "Enemy.h"
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

ShortestPath* ShortestPath::s_Instance = nullptr; 

Enemy::Enemy(Properties* e_props, int currentenemy) : GameObject(e_props) {
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


	TDST.push_back(EnemyList(5000, "so1.png", 5));
	TDST.push_back(EnemyList(10000, "so2.png", 10));
	TDST.push_back(EnemyList(15000, "so3.png", 15));
	TDST.push_back(EnemyList(20000, "so4.png", 20));
	TDST.push_back(EnemyList(30000, "tdt.png", 30));

	CurrentEnemy = currentenemy;
	e_Flip = SDL_FLIP_NONE;
	e_JumpForce = JUMP_FORCE;
	e_max_HP = TDST[CurrentEnemy].HP;
	e_HP = TDST[CurrentEnemy].HP;
	e_time = 0;
	e_dame = TDST[CurrentEnemy].Dame;
	completemap1 = false;
	 


	e_Collider = new Collider();
	e_Collider->SetBuffer(-5, 0, 10, 0);

	e_RigidBody = new RigidBody();
	e_RigidBody->SetGravity(3.0f);

	e_Animation = new Animation();
	e_Animation->SetProps("enemy", 0, 3, 100);


}

void Enemy::Draw() {
	if (CurrentEnemy == TDST.size() - 1 && e_skill) {
		if (e_Flip == SDL_FLIP_NONE) e_Animation->Draw(e_Transform->X, e_Transform->Y, 200, e_Height, e_Flip);
		else e_Animation->Draw(e_Transform->X - 150, e_Transform->Y, 200, e_Height, e_Flip);
	}
	e_Animation->Draw(e_Transform->X, e_Transform->Y, e_Width, e_Height, e_Flip);

	int vitri_hp = 1.0 * e_HP / e_max_HP *75; if (vitri_hp > 75) vitri_hp = 75;
	
	TextureManager::GetInstance()->Draw("enemy_hp", e_Transform->X - 12, e_Transform->Y - 10, vitri_hp, 10);

	/*Vector2D cam = Camera::GetInstance()->GetPosition();
	SDL_Rect box = e_Collider->Get();
	box.x -= cam.X;
	box.y -= cam.Y;
	SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);*/
}

void Enemy::Update(float dt) {
	if (Engine::GetInstance()->GetMap1()) {
		if (e_HP <= 0 && CurrentEnemy != TDST.size() - 1) {
			e_IsDie = true;
		}
		else e_IsDie = false;


		if (e_HP < 0 && CurrentEnemy == TDST.size() - 1) {
			e_IsRunning = false;
			e_IsJumping = false;
			e_IsFalling = false;
			e_IsGrounded = false;
			e_IsAttacking = false;
			completemap1 = true;
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

			Point e_P, m_P;
			m_P.X = (m_Transform->X + 64) / 32;
			m_P.Y = (m_Transform->Y + 75) / 32;
			e_P.X = (e_Transform->X + 64) / 32;
			e_P.Y = (e_Transform->Y + 64) / 32;

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
						e_HP -= 5 * player->GetDame();
					if (player->GetFlip() == SDL_FLIP_NONE) e_RigidBody->ApplyForceX(5 * FORWARD);
					else if (player->GetFlip() == SDL_FLIP_HORIZONTAL) e_RigidBody->ApplyForceX(5 * BACKWARD);
				}
				else if (player->GetFlip() == SDL_FLIP_HORIZONTAL) {
					if (e_Transform->X < m_Transform->X && e_Transform->X >= m_Transform->X - 540 && (abs(e_Transform->Y - m_Transform->Y) < 32.0f))
						e_HP -= 5 * player->GetDame();
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
				std::vector<Point> duongdi = ShortestPath::GetInstance()->cungKhuDat(m_P, e_P);
				bool check = ShortestPath::GetInstance()->checkKD(m_P) != ShortestPath::GetInstance()->checkKD(e_P);
				if (!duongdi.empty() && check) {

					for (int i = 0; i < duongdi.size() - 1; i++) {
						if (i == duongdi.size()) {
							e_IsRunning = false;
							e_IsJumping = false;
							e_IsFalling = false;
						}

						if (i == 0) {
							e_IsJumping = false;
							e_IsFalling = false;
							e_IsRunning = true;


							if (abs(e_Transform->X - duongdi[0].X < 3.0f)) {
								e_IsRunning = false;
							}
							else if (e_Transform->X < duongdi[0].X) {
								e_IsRunning = true;
								e_RigidBody->ApplyForceX(FORWARD * RUN_FORCE);
							}
							else if (e_Transform->X > duongdi[0].X) {
								e_IsRunning = true;
								e_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE);
							}
							if (abs(e_Transform->X - duongdi[0].X < 3.0f)) {
								e_IsRunning = false;
							}
							e_RigidBody->Update(dt);
						}
						if (duongdi[i + 1].X == duongdi[i].X) {
							e_IsFalling = false;
							if (duongdi[i + 1].Y < duongdi[i].Y) {
								e_IsJumping = true;
								e_IsGrounded = false;
								e_RigidBody->ApplyForceY(UPWARD * e_JumpForce);
							}
							else if (duongdi[i + 1].Y > duongdi[i].Y) {
								e_IsFalling = true;
							}
							else {
								e_IsJumping = false;
								e_IsFalling = false;
							}
							e_RigidBody->Update(dt);
						}
						else if (duongdi[i + 1].Y == duongdi[i].Y) {
							e_IsJumping = false;
							e_IsFalling = false;
							e_IsRunning = true;
							if (duongdi[i + 1].X < duongdi[i].X) {
								e_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE);
							}
							else if (duongdi[i + 1].X > duongdi[i].X) {
								e_RigidBody->ApplyForceX(FORWARD * RUN_FORCE);
							}
							else {
								e_IsRunning = false;
							}
							e_RigidBody->Update(dt);
						}
					}
				}
				else if (!check && duongdi.empty()) {
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
						e_RigidBody->ApplyForceY(UPWARD * e_JumpForce);
					}
					e_RigidBody->Update(dt);
				}
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

			//Check vuot ra khoi map
			if (e_Transform->X < 5 || e_Transform->X + e_Width > 1915) {
				e_Transform->X = e_LastSafePosition.X;
			}
			if (e_Transform->Y < 5 || e_Transform->Y + e_Height > 635) {
				e_Transform->Y = e_LastSafePosition.Y;
			}

			e_AnimationState();
			e_Animation->Update();

			e_time += dt;
			if (e_time > 500.0f) {
				e_Transform->X = m_Transform->X;
				e_Transform->Y = m_Transform->Y;
				if (e_time > 600) e_time = 0.0f;
				e_skill = true;
			}
			else e_skill = false;
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

void Enemy::e_AnimationState() {
	//dungim
	e_Animation->SetProps("enemy", 0, 1, 100);

	//run
	if (e_IsRunning) {
		e_Animation->SetProps("enemy", 3, 5, 100);
	}

	//Jump
	if (e_IsJumping) {
		e_Animation->SetProps("enemy", 1, 1, 100);
	}

	//fall
	if (e_IsFalling) {
		e_Animation->SetProps("enemy", 2, 1, 100);
	}

	//attack
	if (e_IsAttacking) {
		e_Animation->SetProps("enemy", 4, 7, 50);
	}
	//skill
	if (e_skill && CurrentEnemy == TDST.size() - 1)  {
		e_Animation->SetProps("enemy", 5, 5, 200);
	}
}

void Enemy::Clean() {
	TextureManager::GetInstance()->Drop("enemy");
	TextureManager::GetInstance()->Drop("enemy_hp");
}

void Enemy::Load() {
	TextureManager::GetInstance()->Load("enemy", "LamGame/Picture/enemy/" + TDST[CurrentEnemy].Texture);
	TextureManager::GetInstance()->Load("enemy_hp", "LamGame/Picture/enemy/HP.png");

}

void Enemy::Luu() {
	std::ofstream out("LamGame/Picture/Pause/enemy.txt");
	if (out.is_open()) {
		out << e_Transform->X << std::endl << e_Transform->Y << std::endl;
		out << e_HP << std::endl << e_dame << std::endl;
		out << CurrentEnemy << std::endl << e_time << std::endl;
	}
	out.close();

}

void Enemy::SetContinue() {
	std::ifstream in("LamGame/Picture/Pause/enemy.txt");
	std::string line;
	int dem = 0;
	while (std::getline(in, line)) {
		dem++;
		std::stringstream iss(line);
		switch (dem) {
		case 1: iss >> e_Transform->X; break;
		case 2: iss >> e_Transform->Y; break;
		case 3: iss >> e_HP; break;
		case 4: iss >> e_dame; break;
		case 5: iss >> CurrentEnemy; break;
		case 6: iss >> e_time; break;
		default: break;
		}
	}
}