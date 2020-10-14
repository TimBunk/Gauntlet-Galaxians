#pragma once

#include "GameObject.h"
#include "SpriteSheet.h"
#include "Animation.h"

class Explosion : public GameObject
{
public:
	Explosion(Animation& animation, unsigned int sprite_id);
	~Explosion();

	void UpdateAnimation(float deltaTime);

private:
	Animation m_animation;
	float m_timer;
	float m_lifeTime;
};