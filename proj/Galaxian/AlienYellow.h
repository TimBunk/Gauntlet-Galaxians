#pragma once

#include "Alien.h"

class AlienYellow : public Alien
{
public:
	AlienYellow(unsigned int sprite_id1, unsigned int sprite_id2, std::vector<Alien*> shielders, GameObject* player, glm::vec2* parentPos, glm::vec2 offset, float startShootingPointY, float screenHeight);
	~AlienYellow();

	void PrepareSpecialMovement();
	void DoSpecialMovement();

	void StartAttacking();
	void StartRage();
	void StopAttacking();

private:
	std::vector<Alien*> shielders;
	glm::vec2 attackPos1;
};