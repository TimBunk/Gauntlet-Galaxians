#pragma once

#include "GameObject.h"
#include "SpriteSheet.h"
#include "Animation.h"
#include "Projectile.h"

struct BeingStats {
	int points = 0;
	float health = 0;
	float speed = 0;
	float damage = 0;
	float shootCooldown = 0.0f;
	float projectileSpeed = 0.0f;
	float projectileDamage = 0.0f;
	float invisibleTime = 0.0f;
};

class Being : public GameObject
{
public:
	Being(unsigned int startingSpriteIndex, SpriteSheet* spritesheet, BeingStats stats, unsigned int sprite_id);
	virtual ~Being();

	void Update(float deltaTime);
	bool IsShooter();
	Projectile* Shoot();

	void PlayCurrentAnimation(float deltaTime);
	void UpdateAnimation();
	void Chase(glm::vec2 dir, float deltaTime);

	void Damage(const float& amount);
	bool IsAlive();

	void GetPoints(int& points) const;
	void GetHealth(float& health) const;
	void GetSpeed(float& speed) const;
	void GetDamage(float& damage) const;

protected:
	BeingStats m_stats;

private:
	SpriteSheet* m_spriteSheet;
	std::vector<Animation> m_animations;
	unsigned int m_currentAnimation;
	unsigned int m_startingSpriteIndex;
	float m_shootTimer;
	float m_invisibleTimer;
};