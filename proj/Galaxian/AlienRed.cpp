#include "AlienRed.h"

AlienRed::AlienRed(unsigned int sprite_id1, unsigned int sprite_id2, GameObject* player, glm::vec2* parentPos, glm::vec2 offset, float startShootingPointY, float screenHeight) : Alien::Alien(sprite_id1, sprite_id2, player, parentPos, offset, startShootingPointY, screenHeight)
{
	attackPos1 = glm::vec2(0.0f, 0.0f);
	accelerationSpeed = 10.0f;
}

AlienRed::~AlienRed()
{

}

void AlienRed::PrepareSpecialMovement()
{
	glm::vec2 playerPos = player->GetPos();
	glm::vec2 pos = GetPos();
	if (offset.x > 0.0f) {
		attackPos1 = glm::vec2(playerPos.x - 150.0f, screenHeight * 0.35f);
		playerPos.x -= 75.0f;
	}
	else {
		attackPos1 = glm::vec2(playerPos.x + 150.0f, screenHeight * 0.35f);
		playerPos.x += 75.0f;
	}
	attackPos = playerPos - attackPos1;
	attackPos = glm::normalize(attackPos);
}

void AlienRed::DoSpecialMovement()
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