#pragma once

#include "GameObject.h"

class BulletManager
{
public:
	BulletManager(unsigned int sprite_id, float bulletSpeed);
	~BulletManager();

	bool IsBulletCollidingWith(GameObject* gameObject);

	void AddBullet(glm::vec2 position);
	void UpdateBullets(float deltaTime);
	void MarkBulletsForDraw();

	void DeleteAllBullets();

private:
	unsigned int sprite_id;
	float bulletSpeed;
	std::vector<GameObject*> bullets;
};