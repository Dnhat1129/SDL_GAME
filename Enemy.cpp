#include "Enemy.h"
#include "TextureManager.h"
#include "SDL.h"
#include "CollisionHandler.h"
#include "Warrior.h"
#include "GameObject.h"

void Enemy::Draw() {
	e_Animation->Draw(e_Transform->X, e_Transform->Y, e_Width, e_Height, e_Flip);

	/*Vector2D cam = Camera::GetInstance()->GetPosition();
	SDL_Rect box = m_Collider->Get();
	box.x -= cam.X;
	box.y -= cam.Y;
	SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box)*/
}

void Enemy::Update(float dt) {
	e_IsRunning = false;
	e_RigidBody->UnSetForce();
	Warrior* player = Engine::GetInstance()->GetWarrior();
	Transform* m_Transform = player->GetPosition();
	Vector2D direction = m_Transform - e_Transform;
	
	/*
	//Run forward
	if (Input::GetInstance()->GetAxisKey(HORIZONTAL) == FORWARD && !e_IsAttacking) {
		e_RigidBody->ApplyForceX(FORWARD * RUN_FORCE);
		e_Flip = SDL_FLIP_NONE;
		e_IsRunning = true;
	}

	//Run backward
	if (Input::GetInstance()->GetAxisKey(HORIZONTAL) == BACKWARD && !e_IsAttacking) {
		e_RigidBody->ApplyForceX(BACKWARD * RUN_FORCE);
		e_Flip = SDL_FLIP_HORIZONTAL;
		e_IsRunning = true;
	}

	//Attack
	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_J)) {
		e_RigidBody->UnSetForce();
		e_IsAttacking = true;
	}

	//Jump
	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) && e_IsGrounded) {
		e_IsJumping = true;
		e_IsGrounded = false;
		e_RigidBody->ApplyForceY(UPWARD * e_JumpForce);
	}

	if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) && e_IsJumping && e_JumpTime > 0) {
		e_JumpTime -= dt;
		e_RigidBody->ApplyForceY(UPWARD * e_JumpForce);
	}
	else {
		e_IsJumping = false;
		e_JumpTime = JUMP_TIME;

	}

	//Fall
	if (e_RigidBody->Velocity().Y > 0 && !e_IsGrounded && !e_IsJumping) {

		e_IsFalling = true;
	}

	else e_IsFalling = false;

	//Attack timer
	if (e_IsAttacking && e_AttackTime > 0) {
		e_AttackTime -= dt;
	}
	else {
		e_IsAttacking = false;
		e_AttackTime = ATTACK_TIME;
	}


	//move on x 
	e_RigidBody->Update(dt);
	e_LastSafePosition.X = e_Transform->X;
	e_Transform->X += e_RigidBody->Position().X;
	e_Collider->Set(e_Transform->X, e_Transform->Y, 60, 75);

	if (CollisionHandler::GetInstance()->MapCollision(e_Collider->Get()))
	{
		e_Transform->X = e_LastSafePosition.X;

	}

	// move y
	e_RigidBody->Update(dt);
	e_LastSafePosition.Y = e_Transform->Y;
	e_Transform->Y += e_RigidBody->Position().Y;
	e_Collider->Set(e_Transform->X, e_Transform->Y, 60, 75);

	if (CollisionHandler::GetInstance()->MapCollision(e_Collider->Get()))
	{
		e_Transform->Y = e_LastSafePosition.Y;
		e_IsGrounded = true;
	}
	else {
		e_IsGrounded = false;
	}

	e_AnimationState();
	e_Animation->Update();
	*/
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
		e_Transform->Y -= 25;
		e_Animation->SetProps("enemy_jump", 0, 1, 100);
	}

	//fall
	if (e_IsFalling) {
		e_Animation->SetProps("enemy_fall", 0, 1, 100);
	}

	//attack
	if (e_IsAttacking) {
		e_Animation->SetProps("enemy_skill", 0, 11, 100);
	}
}

void Enemy::Clean() {
	TextureManager::GetInstance()->Drop("enemy");
}