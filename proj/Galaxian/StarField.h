#pragma once

#include "GameObject.h"

struct Star {
	GameObject* gameObject;
	float timer;
};

class StarField
{
public:
	StarField(unsigned int sprite_id, float maxLeft, float maxRight, float screenHeight, int starAmount);
	~StarField();

	void Update(float deltaTime);
	void MarkForDraw();

private:
	float screenHeight;
	float speed;
	float shineTime;
	std::vector<Star> stars;
};