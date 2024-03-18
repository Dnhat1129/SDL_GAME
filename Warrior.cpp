#include "Warrior.h"
#include "TextureManager.h" 
#include "SDL.h" 
#include "Input.h" 
#include "Camera.h" 
#include "CollisionHandler.h" 
#include "Engine.h"
#include "Enemy.h"


void Warrior::Draw() {
	if (ssj) {
		ssj_Animation->Draw(m_Transform->X - 45, m_Transform->Y - 53, 150, 128, m_Flip); m_HP += 1;
	}
	else m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_Flip);

	/*Vector2D cam = Camera::GetInstance()->GetPosition();
	SDL_Rect box = m_Collider->Get();
	//m_Collider->Set(m_Transform->X, m_Transform->Y, 60, 75);
	box.x -= cam.X;
	box.y -= cam.Y;
	SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);*/
}

void Warrior::Update(float dt) {
	m_IsRunning = false;
	m_RigidBody->UnSetForce();
	
	if (m_IsDie) {
		m_IsRunning = false;
		m_IsJumping = false;
		m_IsFalling = false;
		m_IsAttacking = false;
	}
	else {
		//Run forward
		if (Input::GetInstance()->GetAxisKey(HORIZONTAL) == FORWARD && !m_IsAttacking) {
			m_RigidBody->ApplyForceX(FORWARD * RUN_FORCE);
			m_Flip = SDL_FLIP_NONE;
			m_IsRunning = true;
		}

		//Run backward
		if (Input::GetInstance()->GetAxisKey(HORIZONTAL) == BACKWARD && !m_IsAttacking) {
			m_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE);
			m_Flip = SDL_FLIP_HORIZONTAL;
			m_IsRunning = true;
		}

		//Attack
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_J)) {
			m_RigidBody->UnSetForce();
			m_IsAttacking = true;
		}

		//danh nhau 
		SDL_Rect box_Enemy = Engine::GetInstance()->GetEnemy()->GetBox();
		if (Engine::GetInstance()->GetEnemy()->CheckAttack()) {
			m_Time += dt;
			if (m_Time > 5) {
				m_Time = 0;
				m_HP -= 5;
			}
		}
		if (m_HP <= 0) m_IsDie = true;
		else m_IsDie = false;

		//Jump
		if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W)
			|| Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP))
			&& m_IsGrounded) {
			m_IsJumping = true;
			m_IsGrounded = false;
			m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
		}
		if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W)
			|| Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP))) {
			m_IsJumping = true;
			m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
		}
		else {
			m_IsJumping = false;
		}

		if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)) && m_IsJumping && !m_IsGrounded) {
			if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
				m_IsJumping = false;
				m_IsFalling = true;
				m_Transform->Y += 3;
			}
		}



		//Fall
		if (m_RigidBody->Velocity().Y > 0 && !m_IsGrounded && !m_IsJumping) {

			m_IsFalling = true;
		}

		else m_IsFalling = false;

		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_S) && !CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
			m_IsFalling = true;
		}

		//Attack timer
		if (m_IsAttacking && m_AttackTime > 0) {
			m_AttackTime -= dt;
		}
		else {
			m_IsAttacking = false;
			m_AttackTime = ATTACK_TIME;
		}

		//SSJ
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K)) {
			ssj_Animation->SetProps("player_ssj", 0, 7, 200);
			bool check = true;
			ssj = true;
			time_sjjkhi += dt; {
				if (time_sjjkhi > 9000) {
					ssj = false;
					time_sjjkhi = -9000;
				}
			}
			if (check) {
				ssj_Animation->Update();
				check = false;
			}
		}

		//move on x 
		m_RigidBody->Update(dt);
		m_LastSafePosition.X = m_Transform->X;
		m_Transform->X += m_RigidBody->Position().X;
		m_Collider->Set(static_cast<int>(m_Transform->X), static_cast<int>(m_Transform->Y), 60, 75);

		if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get()))
		{
			m_Transform->X = m_LastSafePosition.X;
		}

		// move y
		m_RigidBody->Update(dt);
		m_LastSafePosition.Y = m_Transform->Y;
		m_Transform->Y += m_RigidBody->Position().Y;
		m_Collider->Set(static_cast<int>(m_Transform->X), static_cast<int>(m_Transform->Y), 60, 75);

		if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get()) && !m_IsJumping)
		{
			m_Transform->Y = m_LastSafePosition.Y;
			m_IsGrounded = true;
		}
		else {
			m_IsGrounded = false;
		}

		//Check vuot ra khoi map
		if (m_Transform->X < 0 || m_Transform->X + m_Width > 1920) {
			m_Transform->X = m_LastSafePosition.X;
		}
		if (m_Transform->Y < 0 || m_Transform->Y + m_Height > 640) {
			m_Transform->Y = m_LastSafePosition.Y;
		}

		m_Origin->X = m_Transform->X + m_Width / 2;
		m_Origin->Y = m_Transform->Y + m_Height / 2;
	}

	AnimationState();
	m_Animation->Update();
	
	
	//std::cout << m_HP;
	//m_Transform->X = 22*32-64;
	//m_Transform->Y = 4*32-75;
}

void Warrior::AnimationState() {
	//dungim
	m_Animation->SetProps("player", 0, 3, 100);
	
	//run
	if (m_IsRunning) {
		m_Animation->SetProps("player_run", 0, 5, 100);
	}

	//Jump
	if (m_IsJumping) {
		m_Animation->SetProps("player_jump", 0, 1, 100);
	}

	//fall
	if (m_IsFalling) {
		m_Animation->SetProps("player_fall", 0, 1, 100);
	}

	//attack
	if (m_IsAttacking) {
		m_Animation->SetProps("player_skill1", 0, 11, 50);
	}
	if (m_IsDie) {
		m_Animation->SetProps("player_die", 0, 1, 100);
	}

	//ssj
	if (ssj) {
		
	}
}

void Warrior::Clean() {
	TextureManager::GetInstance()->Drop("player");
}

void Warrior::Load() {
	TextureManager::GetInstance()->Load("HPMN", "LamGame/Picture/hp + mana/mau + mana.png");
	TextureManager::GetInstance()->Load("HP", "LamGame/Picture/hp + mana/mau.png");
	TextureManager::GetInstance()->Load("MN", "LamGame/Picture/hp + mana/mana.png");
	TextureManager::GetInstance()->Load("player", "LamGame/Picture/Character/dungim_right.png");
	TextureManager::GetInstance()->Load("player_run", "LamGame/Picture/Character/run_right.png");
	TextureManager::GetInstance()->Load("player_skill1", "LamGame/Picture/Character/skill1_right.png");
	TextureManager::GetInstance()->Load("player_jump", "LamGame/Picture/Character/nhay.png");
	TextureManager::GetInstance()->Load("player_fall", "LamGame/Picture/Character/roi_xuong.png");
	TextureManager::GetInstance()->Load("player_bay", "LamGame/Picture/Character/bay.png");
	TextureManager::GetInstance()->Load("player_die", "LamGame/Picture/Character/die.png");

	TextureManager::GetInstance()->Load("player_ssj", "LamGame/Picture/Character/ssj.png");
}