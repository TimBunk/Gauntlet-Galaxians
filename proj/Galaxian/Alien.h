#pragma once

#include "GameObject.h"

enum AlienState {
	idle,
	diving,
	attacking,
	returning,
	rage
};

class Alien : public GameObject
{
public:
	Alien(unsigned int sprite_id1, unsigned int sprite_id2, GameObject* player, glm::vec2* parentPos, glm::vec2 offset, float startShootingPointY, float screenHeight);
	virtual ~Alien();

	virtual void Update(float deltaTime);
	virtual void StartAttacking();
	virtual void StartRage();
	virtual void StopAttacking();

	bool IsShootReady();
	bool IsAttacking();
	void Follow(Alien* leader);
	glm::vec2 GetIdlePosition();

	void Reset();

protected:
	void Idle(float deltaTime);
	void Diving(float deltaTime);
	void Attack(float deltaTime);
	void Returning(float deltaTime);

	virtual void PrepareSpecialMovement();
	virtual void DoSpecialMovement();

	void ChangeSprite(float deltaTime);
	void RotateTowardsPlayer(float deltaTime);

	AlienState currentAlienState;
	AlienState newAlienState;
	float screenHeight;

	Alien* leader;
	glm::vec2 offsetLeader;
	bool leaderAlive;

	GameObject* player;
	glm::vec2 attackPos;
	glm::vec2 dirPlayer;
	glm::vec2* parentPos;
	glm::vec2 offset;
	glm::vec2 danceOffset;

	float speed;
	float accelerationSpeed;
	float rotationSpeed;

	glm::vec2 velocity;
	glm::vec2 startAttackingPos;

	unsigned int sprite_id1;
	unsigned int sprite_id2;

private:
	float timer;
	float changeSpriteTime;

	float shootTimer;
	float shootBulletTime;
	float startShootingPointY;
	bool shoot;
	int shootCount;
};