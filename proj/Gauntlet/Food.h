#pragma once

#include "GameObject.h"

class Food : public GameObject
{
public:
	Food(float health, unsigned int sprite_id);
	~Food();

	void GetHealth(float& health) const;

private:
	float m_health;
};