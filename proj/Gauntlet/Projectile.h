#pragma once

#include "GameObject.h"

class Projectile : public GameObject
{
public:
	Projectile(glm::vec2 direction, float speed, float damage, unsigned int ownersUnique_id, unsigned int sprite_id);
	~Projectile();

	void Update(float deltaTime);

	unsigned int GetOwnersUnique_id();
	void GetDamage(float& damage) const;

private:
	glm::vec2 m_direction;
	float m_speed;
	float m_damage;
	unsigned int m_ownersUnique_id;
};