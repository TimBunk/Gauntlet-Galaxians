#include "AlienBlue.h"

AlienBlue::AlienBlue(unsigned int sprite_id1, unsigned int sprite_id2, GameObject* player, glm::vec2* parentPos, glm::vec2 offset, float startShootingPointY, float screenHeight) : Alien::Alien(sprite_id1, sprite_id2, player, parentPos, offset, startShootingPointY, screenHeight)
{

}

AlienBlue::~AlienBlue()
{

}

void AlienBlue::PrepareSpecialMovement()
{
	attackPos = player->GetPos();
	dirPlayer = glm::normalize(attackPos - GetPos());
}

void AlienBlue::DoSpecialMovement()
{
	glm::vec2 acceleration = dirPlayer;
	acceleration *= accelerationSpeed;
	velocity += acceleration;
	velocity = glm::normalize(velocity);
	velocity *= speed;
}