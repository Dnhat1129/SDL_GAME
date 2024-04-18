#include "Warrior.h"
#include "TextureManager.h" 
#include "SDL.h" 
#include "Input.h" 
#include "Camera.h" 
#include "CollisionHandler.h" 
#include "Engine.h"
#include "Enemy.h"
#include "Boss.h"
#include <fstream>
#include <sstream>

Warrior::Warrior(Properties* props) : GameObject(props) {
	m_TextureID = props->TextureID;
	m_Width = props->Width;
	m_Height = props->Height;
	m_Flip = props->Flip;

	m_Transform = new Transform(props->X, props->Y);

	m_IsRunning = false;
	m_IsJumping = false;
	m_IsFalling = false;
	m_IsGrounded = false;
	m_IsAttacking = false;
	m_IsKame = false;
	m_IsDie = false;
	ssj = false;
	checkload = 0;

	m_Flip = SDL_FLIP_NONE;
	m_HP = 1000000; max_HP = 1000000;
	m_dame = 1000;
	time_ssj = 0;
	time_kame = 0;
	kame_frame = 0;

	m_Collider = new Collider();
	m_Collider->SetBuffer(-10, 0, 15, 0);

	m_RigidBody = new RigidBody();
	m_RigidBody->SetGravity(3.0f);

	m_Animation = new Animation();
	m_Animation->SetProps("player", 0, 3, 100);

	kame_Animation = new Animation();

	ssj_Animation = new Animation();
}

void Warrior::Draw() {
	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_L)) 
		ssj_Animation->Draw(m_Transform->X - 45, m_Transform->Y - 53, 150, 128, m_Flip);
	else if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K) && time_kame >= 0) {
		if (time_kame >= 0 && time_kame <= 240) kame_frame = 0;
		else if (time_kame > 240 && time_kame <= 480) kame_frame = 1;
		else if (time_kame > 480 && time_kame <= 720) kame_frame = 2;
		else {
			kame_frame = -1;
			TextureManager::GetInstance()->Drop("player_kame");
			m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_Flip);
		}
			if (m_Flip == SDL_FLIP_NONE && kame_frame >= 0)
				kame_Animation->DrawKame(m_Transform->X, m_Transform->Y, 600, m_Height, kame_frame, m_Flip);
			else kame_Animation->DrawKame(m_Transform->X - 540, m_Transform->Y, 600, m_Height, kame_frame, m_Flip);
			 
		//if (m_Flip == SDL_FLIP_NONE) kame_Animation->Draw(m_Transform->X, m_Transform->Y, 600, m_Height, m_Flip);
		//else kame_Animation->Draw(m_Transform->X-540, m_Transform->Y, 600, m_Height, m_Flip);
	}
	else m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_Flip);
	
	float vitri_khung = m_Origin->X-480;
	if (vitri_khung < 0) vitri_khung = 0;
	if (m_Origin->X > 1440) vitri_khung = 960;
	TextureManager::GetInstance()->Draw("HPMN", static_cast<int>(vitri_khung), 0, 205, 150);
	int vitri_hp = 1.0 * m_HP / max_HP * 150; if (vitri_hp > 150) vitri_hp = 150;
	TextureManager::GetInstance()->Draw("HP", static_cast<int>(vitri_khung) + 43, 12, vitri_hp, 25);
	TextureManager::GetInstance()->Draw("MN", static_cast<int>(vitri_khung) + 43, 47, 137, 12);

	/*Vector2D cam = Camera::GetInstance()->GetPosition();
	SDL_Rect box = m_Collider->Get();
	//m_Collider->Set(m_Transform->X, m_Transform->Y, 60, 75);
	box.x -= cam.X;
	box.y -= cam.Y;
	SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);*/
}

void Warrior::Update(float dt) {
	//std::cout << m_Origin->X << " " << m_Origin->Y << std::endl;

	if (m_HP > max_HP) m_HP = max_HP;
	m_IsRunning = false;
	m_RigidBody->UnSetForce();
	
	if (m_HP <= 0) m_IsDie = true;
	else m_IsDie = false;

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
		if (time_kame < 0) {
			time_kame += dt;
			m_IsKame = false;
		}
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K) && !m_IsDie && time_kame >= 0) {
			kame_Animation->SetProps("player_kame", 0, 3, 5500);
			time_kame += dt;
			m_IsKame = true;

			if (time_kame > 720) {
				time_kame = -999;
			}
		}
		else m_IsKame = false;
		//std::cout << time_kame << std::endl;
		//SSJ
		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_L) && !ssj) {
			ssj_Animation->SetProps("player_ssj", 0, 7, 200);
			ssj_Animation->Update();
			time_ssj += dt;
		}

		if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_L) && !m_IsDie && time_ssj >= 200 && !ssj) {
			m_dame = 40;
			m_HP = 20000;
			max_HP = 20000;
			TextureManager::GetInstance()->Load("player", "LamGame/Picture/Character/gokussj.png");
			TextureManager::GetInstance()->Load("player_kame", "LamGame/Picture/Character/ssj_kame.png");
			checkload = 1;
			ssj = true;
		}
		if (ssj) {
			time_ssj += dt;
			if (time_ssj > 9000) {
				ssj = false;
				time_ssj = -9000;
			}
		}
		if (time_ssj < 0) {
			time_ssj += dt;
		}
		if (checkload == 1 && !ssj) {
			TextureManager::GetInstance()->Load("player", "LamGame/Picture/Character/base.png");
			TextureManager::GetInstance()->Load("player_kame", "LamGame/Picture/Character/base_kame.png");
			checkload = 0;
		}
		//danh nhau map 1
		Enemy* enemy = Engine::GetInstance()->GetEnemy();
		if (Engine::GetInstance()->GetMap1()) {
			if (enemy->CheckAttack()) {
				m_Time += dt;
				if (m_Time > 5) {
					m_Time = 0;
					m_HP -= enemy->GetDame();
					if (enemy->GetFlip() == SDL_FLIP_NONE) m_RigidBody->ApplyForceX(2 * FORWARD);
					else if (enemy->GetFlip() == SDL_FLIP_HORIZONTAL) m_RigidBody->ApplyForceX(2 * BACKWARD);
					m_RigidBody->Update(dt);
				}
			}

			if (Engine::GetInstance()->GetEnemy()->GetSkill()) {
				m_HP -= 5 * enemy->GetDame();
				if (enemy->GetFlip() == SDL_FLIP_NONE) m_RigidBody->ApplyForceX(5 * FORWARD);
				else if (enemy->GetFlip() == SDL_FLIP_HORIZONTAL) m_RigidBody->ApplyForceX(5 * BACKWARD);
				m_RigidBody->Update(dt);
			}
		}

		//danh nhau map2
		Boss* boss = Engine::GetInstance()->GetBoss();
		if (Engine::GetInstance()->GetMap2()) {
			if (boss->CheckAttack()) {
				m_Time += dt;
				if (m_Time > 5) {
					m_Time = 0;
					m_HP -= boss->GetDame();
					if (boss->GetFlip() == SDL_FLIP_NONE) m_RigidBody->ApplyForceX(2 * FORWARD);
					else if (boss->GetFlip() == SDL_FLIP_HORIZONTAL) m_RigidBody->ApplyForceX(2 * BACKWARD);
					m_RigidBody->Update(dt);
				}
			}

			if (boss->GetSkill() && boss->Gettime() >= 175 && boss->Gettime() <= 200) {
				m_HP -= 10 * boss->GetDame();
			}
		}

		//Jump
		if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W)
			|| Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP))
			&& m_IsGrounded) {
			m_IsJumping = true;
			m_IsGrounded = false;
			m_RigidBody->ApplyForceY(UPWARD * JUMP_FORCE);
		}
		if ((Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W)
			|| Input::GetInstance()->GetKeyDown(SDL_SCANCODE_UP))) {
			m_IsJumping = true;
			m_RigidBody->ApplyForceY(UPWARD * JUMP_FORCE);
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
		if (m_Transform->X < 5 || m_Transform->X + m_Width > 1915) {
			m_Transform->X = m_LastSafePosition.X;
		}
		if (m_Transform->Y < 5 || m_Transform->Y + m_Height > 635) {
			m_Transform->Y = m_LastSafePosition.Y;
		}

		m_Origin->X = m_Transform->X + m_Width / 2;
		m_Origin->Y = m_Transform->Y + m_Height / 2;
	}

	AnimationState();
	m_Animation->Update();
	//std::cout << m_Transform->X << " " << m_Transform->Y << std::endl;
	
}

void Warrior::AnimationState() {
	//dungim
	m_Animation->SetProps("player", 0, 3, 100);
	
	//run
	if (m_IsRunning) {
		m_Animation->SetProps("player", 3, 5, 100);
	}

	//Jump
	if (m_IsJumping) {
		m_Animation->SetProps("player", 1, 1, 100);
	}

	//fall
	if (m_IsFalling) {
		m_Animation->SetProps("player", 2, 1, 100);
	}

	//attack
	if (m_IsAttacking) {
		m_Animation->SetProps("player", 4, 10, 50);
	}
	if (m_IsDie) {
		m_Animation->SetProps("player", 5, 1, 100);
	}
}

void Warrior::Clean() {
	TextureManager::GetInstance()->Drop("player");
	TextureManager::GetInstance()->Drop("player_ssj");
	TextureManager::GetInstance()->Drop("HP");
	TextureManager::GetInstance()->Drop("HPMN");
	TextureManager::GetInstance()->Drop("MN");
	TextureManager::GetInstance()->Drop("player_kame");
}

void Warrior::Load() {
	TextureManager::GetInstance()->Load("player", "LamGame/Picture/Character/base.png");
	TextureManager::GetInstance()->Load("player_kame", "LamGame/Picture/Character/base_kame.png");

	TextureManager::GetInstance()->Load("HPMN", "LamGame/Picture/hp + mana/mau + mana.png");
	TextureManager::GetInstance()->Load("HP", "LamGame/Picture/hp + mana/mau.png");
	TextureManager::GetInstance()->Load("MN", "LamGame/Picture/hp + mana/mana.png");
	TextureManager::GetInstance()->Load("player_ssj", "LamGame/Picture/Character/bienssj.png");

}

void Warrior::Luu() {
	std::ofstream out("LamGame/Picture/Pause/player.txt");
	if (out.is_open()) {
		out << m_Transform->X << std::endl << m_Transform->Y << std::endl;
		out << m_HP << std::endl << m_MN << std::endl << m_dame << std::endl;
		out << ssj << std::endl << time_kame << std::endl;
	}
	out.close();

}

void Warrior::SetContinue() {
	std::ifstream in("LamGame/Picture/Pause/player.txt");
	std::string line;
	std::string value;
	int dem = 0;
	while (std::getline(in, line)) {
		dem++;
		std::stringstream iss(line);
		switch (dem) {
		case 1: iss >> m_Transform->X; break;
		case 2: iss >> m_Transform->Y; break;
		case 3: iss >> m_HP; break;
		case 4: iss >> m_MN; break;
		case 5: iss >> m_dame; break;
		case 6: iss >> ssj; break;
		case 7: iss >> time_kame; break;
		default: break;
		}
	}
}