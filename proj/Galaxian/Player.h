#pragma once

#include "GameObject.h"

class Player : public GameObject
{
public:
	Player(unsigned int sprite_id, unsigned int bullet_sprite_id, float speed, float screenHeight);
	~Player();

	void Update(float deltaTime);

	float GetSpeed();
	void SetSpeed(float speed);
	GameObject* GetBullet();
	void ResetBullet();
	void Shoot();

	void MarkForDraw();

private:
	float speed;

	GameObject* bullet;
	float bulletSpeed;

	float screenHeight;
};