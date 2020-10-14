#include "AlienYellow.h"

AlienYellow::AlienYellow(unsigned int sprite_id1, unsigned int sprite_id2, std::vector<Alien*> shielders, GameObject* player, glm::vec2* parentPos, glm::vec2 offset, float startShootingPointY, float screenHeight) : Alien::Alien(sprite_id1, sprite_id2, player, parentPos, offset, startShootingPointY, screenHeight)
{
	this->shielders = shielders;
	for (int i = 0; i < shielders.size(); i++) {
		shielders[i]->Follow(this);
	}
	attackPos1 = glm::vec2(0.0f, 0.0f);
	accelerationSpeed = 10.0f;
}

AlienYellow::~AlienYellow()
{

}

void AlienYellow::PrepareSpecialMovement()
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

void AlienYellow::DoSpecialMovement()
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

void AlienYellow::StartAttacking()
{
	newAlienState = AlienState::diving;
	startAttackingPos = GetPos();
	int aliveCount = 0;
	for (int i = 0; i < shielders.size();i++) {
		if (shielders[i]->IsAlive()) {
			aliveCount++;
		}
	}
	// If all shielders(3) are alive then let the middle one attack and the left or right
	if (aliveCount == 3) {
		if (newAlienState == rage) {
			shielders[1]->StartRage();
		}
		else {
			shielders[1]->StartAttacking();
		}
		float randomValue = (float)std::rand() / (float)RAND_MAX;

		if (randomValue >= 0.5f) {
			if (newAlienState == rage) {
				shielders[2]->StartRage();
			}
			else {
				shielders[2]->StartAttacking();
			}
		}
		else {
			if (newAlienState == rage) {
				shielders[0]->StartRage();
			}
			{
				shielders[0]->StartAttacking();
			}
		}
	}
	// If not all shielders are alive then move whoever is alive
	else {
		for (int i = 0; i < shielders.size(); i++) {
			if (shielders[i]->IsAlive()) {
				if (newAlienState == rage) {
					shielders[i]->StartRage();
				}
				else {
					shielders[i]->StartAttacking();
				}
			}
		}
	}
}

void AlienYellow::StartRage()
{
	newAlienState = AlienState::rage;
	StartAttacking();
}

void AlienYellow::StopAttacking()
{
	if (currentAlienState == AlienState::rage || newAlienState == AlienState::rage) {
		currentAlienState = AlienState::attacking;
		newAlienState = AlienState::attacking;
	}
	for (int i = 0; i < shielders.size(); i++) {
		shielders[i]->StopAttacking();
	}
}
