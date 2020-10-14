#pragma once

#include "GameObject.h"

class Explosion : public GameObject
{
public:
	Explosion(unsigned int sprite_id1, unsigned int sprite_id2, glm::vec2 pos);
	~Explosion();

	void Update(float deltaTime);

private:
	unsigned int sprite_id2;
	float m_timer;
	float changeTime;

};