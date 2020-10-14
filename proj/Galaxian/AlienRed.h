#pragma once

#include "Alien.h"

class AlienRed : public Alien
{
public:
	AlienRed(unsigned int sprite_id1, unsigned int sprite_id2, GameObject* player, glm::vec2* parentPos, glm::vec2 offset, float startShootingPointY, float screenHeight);
	~AlienRed();

	void PrepareSpecialMovement();
	void DoSpecialMovement();

private:
	glm::vec2 attackPos1;
};