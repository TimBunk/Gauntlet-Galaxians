#include "StarField.h"

StarField::StarField(unsigned int sprite_id, float maxLeft, float maxRight, float screenHeight, int starAmount)
{
	this->screenHeight = screenHeight;
	speed = 30.0f;
	shineTime = 0.5f;

	stars.resize(starAmount);
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	for (int i = 0; i < stars.size(); i++) {
		unsigned int duplicateSprite_id = spriteManager->DuplicateSprite(sprite_id);
		Star star;
		star.gameObject = new GameObject(duplicateSprite_id);
		star.gameObject->SetPos(glm::vec2(((float)std::rand() / (float)RAND_MAX * (maxRight - maxLeft)) + maxLeft, ((float)std::rand() / (float)RAND_MAX * screenHeight) - (screenHeight * 0.5f)));
		star.timer = (float)std::rand() / (float)RAND_MAX * shineTime;
		stars[i] = star;
		if ((float)std::rand() / (float)RAND_MAX > 0.5f) {
			glm::vec4 colour = glm::vec4((float)std::rand() / (float)RAND_MAX, (float)std::rand() / (float)RAND_MAX, (float)std::rand() / (float)RAND_MAX, 1.0f);
			spriteManager->SetSpriteColour(duplicateSprite_id, colour);
		}
		else {
			glm::vec4 colour = glm::vec4((float)std::rand() / (float)RAND_MAX, (float)std::rand() / (float)RAND_MAX, (float)std::rand() / (float)RAND_MAX, 0.0f);
			spriteManager->SetSpriteColour(duplicateSprite_id, colour);
		}
	}
}

StarField::~StarField()
{
	std::vector<Star>::iterator it = stars.begin();
	while (it != stars.end()) {
		delete (*it).gameObject;
		it = stars.erase(it);
	}
}

void StarField::Update(float deltaTime)
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	for (int i = 0; i < stars.size(); i++) {
		glm::vec2 starPos = stars[i].gameObject->GetPos();
		starPos.y -= speed * deltaTime;
		if (starPos.y < (screenHeight * -0.5f) - (stars[i].gameObject->GetScale().y / 0.5f)) {
			starPos.y = (screenHeight * 0.5f) + (stars[i].gameObject->GetScale().y / 0.5f);
		}
		stars[i].gameObject->SetPos(starPos);
		stars[i].timer += deltaTime;
		if (stars[i].timer >= shineTime) {
			stars[i].timer -= shineTime;
			glm::vec4 colour;
			spriteManager->GetSpriteColour(stars[i].gameObject->GetSprite_id(), colour);
			if (colour.a == 1.0f) {
				colour.a = 0.0f;
			}
			else {
				colour.a = 1.0f;
			}
			spriteManager->SetSpriteColour(stars[i].gameObject->GetSprite_id(), colour);
		}
	}
}

void StarField::MarkForDraw()
{
	for (int i = 0; i < stars.size(); i++) {
		stars[i].gameObject->MarkForDraw();
	}
}
