#include "BulletManager.h"

BulletManager::BulletManager(unsigned int sprite_id, float bulletSpeed)
{
	this->sprite_id = sprite_id;
	this->bulletSpeed = bulletSpeed;
}

BulletManager::~BulletManager()
{
	DeleteAllBullets();
}

bool BulletManager::IsBulletCollidingWith(GameObject * gameObject)
{
	std::vector<GameObject*>::iterator it = bullets.begin();
	while (it != bullets.end()) {
		if ((*it)->IsCollidingWith(gameObject)) {
			delete (*it);
			it = bullets.erase(it);
			return true;
		}
		else {
			++it;
		}
	}
	return false;
}

void BulletManager::AddBullet(glm::vec2 position)
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	GameObject* bullet = new GameObject(spriteManager->DuplicateSprite(sprite_id));
	bullet->SetPos(position);
	bullets.push_back(bullet);
}

void BulletManager::UpdateBullets(float deltaTime)
{
	std::vector<GameObject*>::iterator it = bullets.begin();
	while (it != bullets.end()) {
		if ((*it)->GetPos().y + ((*it)->GetScale().y * 0.5f) < -360.0f) {
			delete (*it);
			it = bullets.erase(it);
		}
		else {
			glm::vec2 bulletPos = (*it)->GetPos();
			bulletPos.y -= bulletSpeed * deltaTime;
			(*it)->SetPos(bulletPos);
			++it;
		}
	}
}

void BulletManager::MarkBulletsForDraw()
{
	for (int i = 0; i < bullets.size(); i++) {
		bullets[i]->MarkForDraw();
	}
}

void BulletManager::DeleteAllBullets()
{
	std::vector<GameObject*>::iterator it = bullets.begin();
	while (it != bullets.end()) {
		delete (*it);
		it = bullets.erase(it);
	}
}
