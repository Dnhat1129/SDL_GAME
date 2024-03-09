#include "Warrior.h"
#include "TextureManager.h" 
#include "SDL.h" 
#include "Input.h" 
#include "Camera.h" 
#include "CollisionHandler.h" 


void Warrior::Draw() {
	m_Animation->Draw(m_Transform->X, m_Transform->Y, m_Width, m_Height, m_Flip);

	/*Vector2D cam = Camera::GetInstance()->GetPosition();
	SDL_Rect box = m_Collider->Get();
	box.x -= cam.X;
	box.y -= cam.Y;
	SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);*/
}

void Warrior::Update(float dt) {
	
	m_IsRunning = false;
	m_RigidBody->UnSetForce();
	
	//Run forward
	if (Input::GetInstance()->GetAxisKey(HORIZONTAL) == FORWARD && !m_IsAttacking) {
		m_RigidBody->ApplyForceX(FORWARD * RUN_FORCE);
		m_Flip = SDL_FLIP_NONE;
		m_IsRunning = true;

		//std::cout << m_Transform->X << " " << m_Transform->Y << std::endl;
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

	//Jump
	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) && m_IsGrounded) {
		m_IsJumping = true;
		m_IsGrounded = false;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
	}

	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) && m_IsJumping && m_JumpTime > 0) {
		m_JumpTime -= dt;
		m_RigidBody->ApplyForceY(UPWARD * m_JumpForce);
	}
	else {
		m_IsJumping = false;
		m_JumpTime = JUMP_TIME;

	}
	
	//Fall
	if (m_RigidBody->Velocity().Y > 0 && !m_IsGrounded && !m_IsJumping) {
	
		m_IsFalling = true;
	}
		
	else m_IsFalling = false;

	//Attack timer
	if (m_IsAttacking && m_AttackTime > 0) {
		m_AttackTime -= dt;
	}
	else {
		m_IsAttacking = false;
		m_AttackTime = ATTACK_TIME;
	}


	//move on x 
	m_RigidBody->Update(dt);
	m_LastSafePosition.X = m_Transform->X;
	m_Transform->X += m_RigidBody->Position().X;
	m_Collider->Set(m_Transform->X, m_Transform->Y, 60, 75);

	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get()))
	{
		m_Transform->X = m_LastSafePosition.X;
		
	}

	// move y
	m_RigidBody->Update(dt);
	m_LastSafePosition.Y = m_Transform->Y;
	m_Transform->Y += m_RigidBody->Position().Y;
	m_Collider->Set(m_Transform->X, m_Transform->Y, 60, 75);

	if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get()))
	{
		m_Transform->Y = m_LastSafePosition.Y;
		m_IsGrounded = true;
	}
	else {
		m_IsGrounded = false;
	}


	m_Origin->X = m_Transform->X + m_Width / 2;
	m_Origin->Y = m_Transform->Y + m_Height / 2;

	AnimationState();
	m_Animation->Update();
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
		m_Transform->Y -= 20;
			m_Animation->SetProps("player_jump", 0, 1, 100);
	}

	//fall
	if (m_IsFalling) {
		m_Animation->SetProps("player_fall", 0, 1, 100);
	}

	//attack
	if (m_IsAttacking) {
		m_Animation->SetProps("player_skill1", 0, 11, 100);
	}
}

void Warrior::Clean() {
	TextureManager::GetInstance()->Drop("player_right");
}