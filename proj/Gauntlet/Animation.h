#pragma once

#include <vector>

#include "SpriteManager.h"

struct Animation
{
public:
	std::vector<glm::vec4> uvCoords;
	unsigned int currentSprite = 0;
	float updateTime = 0.0f;

	glm::vec4 UpdateAnimation(float deltaTime) {
		currentTime += deltaTime;
		if (currentTime >= updateTime) {
			currentTime -= updateTime;
			currentSprite++;
			if (currentSprite == uvCoords.size()) {
				currentSprite = 0;
			}
		}
		return uvCoords[currentSprite];
	}
	Animation Duplicate() {
		NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
		Animation a;
		a.uvCoords = uvCoords;
		a.updateTime = updateTime;
		a.currentTime = currentTime;
		return a;
	}
private:
	float currentTime = 0.0f;
};