#pragma once

#include "Being.h"

class Player : public Being
{
public:
	Player(glm::vec4 uvSkull, unsigned int startingSpriteIndex, SpriteSheet* spritesheet, BeingStats stats);
	~Player();

	void Heal(const float& amount);
	void IncreaseScore(const int& amount);
	void GetScore(int& amount) const;
	void ClearScore();
	
	bool AddToInventory(GameObject* item);
	void GetItems(std::vector<GameObject*>& inventory) const;
	bool UsePotion();
	bool UseKey();
	void ClearInventory();

	void ShowSkull();

private:
	void OrganizeInventory();
	std::vector<GameObject*> m_inventory;
	int m_score;
	glm::vec4 m_uvSkull;
};