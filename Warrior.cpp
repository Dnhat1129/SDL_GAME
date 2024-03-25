#include "Warrior.h"
#include "TextureManager.h" 
#include "SDL.h" 
#include "Input.h" 
#include "Camera.h" 
#include "CollisionHandler.h" 
#include "Engine.h"
#include "Enemy.h"


void Warrior::Draw() {
	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K)) 
		ssj_Animation->Draw(m_Transform->X - 45, m_Transform->Y - 53, 150, 128, m_Flip);
	else if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_L) && time_kame > 0) {
			if (m_Flip == SDL_FLIP_NONE) player_kame->Draw(m_Transform->X, m_Transform->Y, 600, m_Height, m_Flip);
			else player_kame->Draw(m_Transform->X - 540, m_Transform->Y, 600, m_Height, m_Flip);
	}
	else m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_Flip);

	float vitri_khung = 0;
	if (m_Origin->X < 471) vitri_khung = 0;
	else if (m_Origin->X > 471 && m_Origin->X <= 1440) {
		vitri_khung = m_Transform->X - 480 + 30;
	}
	else vitri_khung = 1440 - 480;
	TextureManager::GetInstance()->Draw("HPMN", static_cast<int>(vitri_khung), 0, 350, 100);
	int vitri_hp = 1.0 * m_HP / 1000 * 150; if (vitri_hp > 150) vitri_hp = 150;
	TextureManager::GetInstance()->Draw("HP", static_cast<int>(vitri_khung) + 186, 12, vitri_hp, 25);
	TextureManager::GetInstance()->Draw("MN", static_cast<int>(vitri_khung) + 186, 47, 137, 12);

	/*Vector2D cam = Camera::GetInstance()->GetPosition();
	SDL_Rect box = m_Collider->Get();
	//m_Collider->Set(m_Transform->X, m_Transform->Y, 60, 75);
	box.x -= cam.X;
	box.y -= cam.Y;
	SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);*/
}

void Warrior::Update(float dt) {
	if (m_HP > 1000) m_HP = 1000;
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
		else m_IsAttacking = false;

		//KAMEHAMEHA 
		time_kame += dt;
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_L) && !m_IsDie && time_kame >= 0) {
			player_kame->SetProps("player_kame", 0, 3, 2000);
			if (time_kame >= 0) {
				player_kame->Update();
			}
			
			if (time_kame > 370) {
				player_kame->Reset();
				time_kame = -1000;
			}
		}

		//danh nhau 
		Enemy* enemy = Engine::GetInstance()->GetEnemy();
		/*SDL_Rect box_Enemy = enemy->GetBox();
		int e_HP = enemy->GetHP();
		if (CollisionHandler::GetInstance()->CheckCollision(box_Enemy, m_Collider->Get())) {
			if (m_IsAttacking) e_HP -= m_dame;
			std::cout << e_HP << std::endl;
		}*/

		if (Engine::GetInstance()->GetEnemy()->CheckAttack()) {
			m_Time += dt;
			if (m_Time > 5) {
				m_Time = 0;
				m_HP -= enemy->GetDame() ;
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

		if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) || Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP)) && m_IsJumping ) {
			if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get()) && !m_IsGrounded) {
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


		//SSJ
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K) && !m_IsDie && ssj) {
			ssj_Animation->SetProps("player_ssj", 0, 7, 200);
			m_HP += 1;
			ssj = true;
			time_sjjkhi += dt;
			if (time_sjjkhi > 9000) {
				ssj = false;
				time_sjjkhi = -9000;
			}
			ssj_Animation->Update();
		}
		//else delete ssj_Animation;

		

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
	
	//m_Transform->X = 32 * 30 - 64;
	//m_Transform->Y = 32 * 4 - 75;
	
	//std::cout << m_Transform->X << " " << m_Transform->Y << std::endl;
	
}

void Warrior::AnimationState() {
	//dungim
	m_Animation->SetProps("player", 0, 3, 100);
	
	//run
	if (m_IsRunning) {
		m_Animation->SetProps("player", 1, 5, 100);
	}

	//Jump
	if (m_IsJumping) {
		m_Animation->SetProps("player", 2, 1, 100);
	}

	//fall
	if (m_IsFalling) {
		m_Animation->SetProps("player", 3, 1, 100);
	}

	//attack
	if (m_IsAttacking) {
		m_Animation->SetProps("player", 4, 11, 50);
	}
	if (m_IsDie) {
		m_Animation->SetProps("player", 5, 1, 100);
	}

	//ssj
}

void Warrior::Clean() {
	TextureManager::GetInstance()->Drop("player");
}

void Warrior::Load() {

	TextureManager::GetInstance()->Load("HPMN", "LamGame/Picture/hp + mana/mau + mana.png");
	TextureManager::GetInstance()->Load("HP", "LamGame/Picture/hp + mana/mau.png");
	TextureManager::GetInstance()->Load("MN", "LamGame/Picture/hp + mana/mana.png");
	TextureManager::GetInstance()->Load("player", "LamGame/Picture/Character/base.png");

	TextureManager::GetInstance()->Load("player_ssj", "LamGame/Picture/Character/ssj.png");
	TextureManager::GetInstance()->Load("player_kame", "LamGame/Picture/Character/kame.png");

}