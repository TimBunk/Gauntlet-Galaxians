#include "Player.h"

Player::Player(unsigned int sprite_id, unsigned int bullet_sprite_id, float speed, float screenHeight) : GameObject::GameObject(sprite_id)
{
	this->speed = speed;
	this->screenHeight = screenHeight;

	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	bullet = new GameObject(bullet_sprite_id);
	bullet->SetAlive(false);
	bulletSpeed = 480.0f;
}

Player::~Player()
{
	delete bullet;
}

void Player::Update(float deltaTime)
{
	if (bullet->IsAlive()) {
		glm::vec2 bulletPos = bullet->GetPos();
		bulletPos.y += bulletSpeed * deltaTime;
		if (bulletPos.y + (bullet->GetScale().y / 2.0f) >= screenHeight) {
			bullet->SetAlive(false);
		}
		else {
			bullet->SetPos(bulletPos);
		}
	}
}

float Player::GetSpeed()
{
	return speed;
}

void Player::SetSpeed(float speed)
{
	this->speed = speed;
}

GameObject * Player::GetBullet()
{
	return bullet;
}

void Player::ResetBullet()
{
	bullet->SetAlive(false);
}

void Player::Shoot()
{
	if (bullet->IsAlive() == false) {
		bullet->SetAlive(true);
		glm::vec2 bulletPos = GetPos();
		bulletPos.y += GetScale().y / 2.0f;
		bullet->SetPos(bulletPos);
	}
}

void Player::MarkForDraw()
{
	if (alive) {
		NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
		spriteManager->MarkSpriteForDraw(sprite_id);
		bullet->MarkForDraw();
	}
}
