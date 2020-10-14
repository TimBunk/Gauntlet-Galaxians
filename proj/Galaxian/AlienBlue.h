#pragma once

#include "Alien.h"

class AlienBlue : public Alien
{
public:
	AlienBlue(unsigned int sprite_id1, unsigned int sprite_id2, GameObject* player, glm::vec2* parentPos, glm::vec2 offset, float startShootingPointY, float screenHeight);
	~AlienBlue();

	void PrepareSpecialMovement();
	void DoSpecialMovement();

private:

};