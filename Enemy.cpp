#include "Enemy.h"
#include "TextureManager.h"
#include "SDL.h"
#include "CollisionHandler.h"
#include "Warrior.h"
#include "GameObject.h"
#include "Camera.h"
#include "TileLayer.h"
#include <cmath>

ShortestPath* ShortestPath::s_Instance = nullptr;


void Enemy::Draw() {
	e_Animation->Draw(e_Transform->X, e_Transform->Y, e_Width, e_Height, e_Flip);

	int vitri_hp = 1.0 * e_HP / 10000 *75; if (vitri_hp > 75) vitri_hp = 75;
	TextureManager::GetInstance()->Draw("enemy_hp", e_Transform->X - 12, e_Transform->Y - 10, vitri_hp, 10);

	/*Vector2D cam = Camera::GetInstance()->GetPosition();
	SDL_Rect box = e_Collider->Get();
	box.x -= cam.X;
	box.y -= cam.Y;
	SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);*/
}

void Enemy::Update(float dt) {
	if (e_HP <= 0 && !isCurrentEnemyUpdated) {
		e_IsDie = true;
		CurrentEnemy++;
		e_HP = TDST[CurrentEnemy].HP;
		e_dame = TDST[CurrentEnemy].Dame;
		isCurrentEnemyUpdated = true;

		e_Transform->X = 300;
		e_Transform->Y = 100;
		
	}

	if (e_HP > 0) {
		isCurrentEnemyUpdated = false;
	}


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

	//Attack timer
	if (CollisionHandler::GetInstance()->CheckCollision(box_Enemy, box_Player)) {
		e_RigidBody->UnSetForce();
		e_IsAttacking = true;
		e_IsRunning = false; 
		e_IsFalling = false;
		if (player->CheckAttack() && e_Flip != player->GetFlip()) e_HP -= player->GetDame();
		if (player->GetKameAnimation()->GetCurrentFrame() == 2) {
			if (player->GetFlip() == SDL_FLIP_NONE) {
				if (e_Transform->X > m_Transform->X && e_Transform->X <= m_Transform->X + 600 && (abs(e_Transform->Y - m_Transform->Y) < 32.0f))
					e_HP -= 2*player->GetDame();
			}
			if (player->GetFlip() == SDL_FLIP_HORIZONTAL) {
				if (e_Transform->X < m_Transform->X && e_Transform->X >= m_Transform->X - 540 && (abs(e_Transform->Y - m_Transform->Y) < 32.0f))
					e_HP -= 2*player->GetDame();
			}
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
			if (abs(e_Transform->X - m_Transform->X) < 3.0f) {
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
			if (abs(e_Transform->Y - m_Transform->Y) < 3.0f) {
				e_IsJumping = false;
				e_IsFalling = false;
				e_IsRunning = true;
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
	if (e_IsJumping && !e_IsGrounded) {
		if (CollisionHandler::GetInstance()->MapCollision(e_Collider->Get()) && !e_IsGrounded) {
			e_IsJumping = false;
			e_IsRunning = false;
			e_IsFalling = true;
			e_Transform->Y += 3;
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

	if (CollisionHandler::GetInstance()->MapCollision(e_Collider->Get()) && !e_IsJumping)
	{
		e_Transform->Y = e_LastSafePosition.Y;
		e_IsGrounded = true;
	}
	else {
		e_IsGrounded = false;
	}

	//Check vuot ra khoi map
	if (e_Transform->X < 0 || e_Transform->X + e_Width > 1920) {
		e_Transform->X = e_LastSafePosition.X;
	}
	if (e_Transform->Y < 0 || e_Transform->Y + e_Height > 640) {
		e_Transform->Y = e_LastSafePosition.Y;
	}

	e_AnimationState();
	e_Animation->Update();

	e_time += dt;
	if (e_time > 500.0f) {
		e_Transform->X = m_Transform->X;
		e_Transform->Y = m_Transform->Y;
		e_time = 0.0f;
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
}

void Enemy::Clean() {
	TextureManager::GetInstance()->Drop("enemy");
}

void Enemy::Load() {
	std::cout << CurrentEnemy << std::endl;
	TextureManager::GetInstance()->Load("enemy", "LamGame/Picture/enemy/" + TDST[CurrentEnemy].Texture);
	TextureManager::GetInstance()->Load("enemy_hp", "LamGame/Picture/enemy/HP.png");

}