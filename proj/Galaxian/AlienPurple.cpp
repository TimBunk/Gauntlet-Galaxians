#include "AlienPurple.h"

AlienPurple::AlienPurple(unsigned int sprite_id1, unsigned int sprite_id2, GameObject* player, glm::vec2* parentPos, glm::vec2 offset, float startShootingPointY, float screenHeight) : Alien::Alien(sprite_id1, sprite_id2, player, parentPos, offset, startShootingPointY, screenHeight)
{
	attackPos1 = glm::vec2(0.0f, 0.0f);
	accelerationSpeed = 8.0f;
}

AlienPurple::~AlienPurple()
{

}

void AlienPurple::PrepareSpecialMovement()
{
	glm::vec2 playerPos = player->GetPos();
	glm::vec2 pos = GetPos();
	if (offset.x > 0.0f) {
		attackPos1 = glm::vec2(playerPos.x - 125.0f, screenHeight * 0.35f);
	}
	else {
		attackPos1 = glm::vec2(playerPos.x + 125.0f, screenHeight * 0.35f);
	}
	attackPos = playerPos - attackPos1;
	attackPos.y += GetScale().y * 2.0f;
	attackPos = glm::normalize(attackPos);
}

void AlienPurple::DoSpecialMovement()
{
	glm::vec2 acceleration;
	glm::vec2 pos = GetPos();
	if (GetPos().y > attackPos1.y) {
		acceleration = attackPos1 - pos;
		acceleration = glm::normalize(acceleration);
	}
	else {
		acceleration = attackPos;
	}
	acceleration *= accelerationSpeed;
	velocity += acceleration;
	velocity = glm::normalize(velocity);
	velocity *= speed;
}