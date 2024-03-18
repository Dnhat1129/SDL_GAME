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

	/*Vector2D cam = Camera::GetInstance()->GetPosition();
	SDL_Rect box = e_Collider->Get();
	box.x -= cam.X;
	box.y -= cam.Y;
	SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);*/
}

void Enemy::Update(float dt) {
	e_IsRunning = false;
	e_RigidBody->UnSetForce();
	Warrior* player = Engine::GetInstance()->GetWarrior();
	Transform* m_Transform = player->GetPosition();
	SDL_Rect box_Player = player->GetBox();
	SDL_Rect box_Enemy = e_Collider->Get();
	e_Flip = player->GetFlip();

	Point e_P, m_P;
	m_P.X = (m_Transform->X+64)/32;
	m_P.Y = (m_Transform->Y+75)/32;
	e_P.X = (e_Transform->X+64)/32;
	e_P.Y = (e_Transform->Y+64)/32;
	
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
	}
	else {
		e_IsAttacking = false;
		std::vector<Point> duongdi = ShortestPath::GetInstance()->cungKhuDat(m_P, e_P);
		for (int i = 0; i < duongdi.size(); i++) {
			//std::cout << "Buoc " << i << " : " << (duongdi[i].X + 64) / 32 << " " << (duongdi[i].Y + 75) / 32 << std::endl;
		}
		//std::cout << ShortestPath::GetInstance()->checkKD(m_P) << " " << ShortestPath::GetInstance()->checkKD(e_P) << std::endl;
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
					

					//std::cout << e_IsRunning << " " << e_Transform->X << " " << duongdi[0].X << std::endl;
					if (abs(e_Transform->X - duongdi[0].X < 3.0f) && check != 1) {
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
				}
				if (duongdi[i + 1].X == duongdi[i].X) {
					e_IsFalling = false;
					//e_IsRunning = false;
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
				}
			}
		}
		else if (!check) {
			//X
			e_RigidBody->Update(dt);
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
			else if (e_Transform->X > m_Transform->X) {
				e_IsJumping = true;
				e_IsGrounded = false;
				e_IsRunning = false;
				e_RigidBody->ApplyForceY(UPWARD * e_JumpForce);
			}
		}
	}
	if (e_IsJumping && !e_IsGrounded) {
		if (CollisionHandler::GetInstance()->MapCollision(e_Collider->Get())) {
			e_IsJumping = false;
			e_IsRunning = false;
			e_IsFalling = true;
			e_Transform->Y += 3;
		}
	}

	if (e_IsAttacking && e_AttackTime > 0) {
		e_AttackTime -= dt;
	}
	else {
		e_IsAttacking = false;
		e_AttackTime = ATTACK_TIME;
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

	//std::cout << e_IsRunning << " " << e_IsAttacking << " " << e_IsFalling << " " << e_IsJumping << " " << e_IsGrounded << std::endl;
}

void Enemy::e_AnimationState() {
	//dungim
	e_Animation->SetProps("enemy", 0, 1, 100);

	//run
	if (e_IsRunning) {
		e_Animation->SetProps("enemy_run", 0, 5, 100);
	}

	//Jump
	if (e_IsJumping) {
		e_Animation->SetProps("enemy_jump", 0, 1, 100);
	}

	//fall
	if (e_IsFalling) {
		e_Animation->SetProps("enemy_fall", 0, 1, 100);
	}

	//attack
	if (e_IsAttacking) {
		e_Animation->SetProps("enemy_skill", 0, 7, 50);
	}

}

void Enemy::Clean() {
	TextureManager::GetInstance()->Drop("enemy");
}

void Enemy::Load() {

	TextureManager::GetInstance()->Load("enemy", "LamGame/Picture/enemy/so3.png");
	TextureManager::GetInstance()->Load("enemy_jump", "LamGame/Picture/enemy/so3_nhay.png");
	TextureManager::GetInstance()->Load("enemy_fall", "LamGame/Picture/enemy/so3_roi.png");
	TextureManager::GetInstance()->Load("enemy_run", "LamGame/Picture/enemy/so3_run.png");
	TextureManager::GetInstance()->Load("enemy_skill", "LamGame/Picture/enemy/so3_skill.png");
}