#include "Player.h"

Player::Player(glm::vec4 uvSkull, unsigned int startingSpriteIndex, SpriteSheet* spritesheet, BeingStats stats) : Being::Being(startingSpriteIndex, spritesheet, stats, spritesheet->GetSprite(startingSpriteIndex))
{
	m_uvSkull = uvSkull;
	m_score = 0;
}

Player::~Player() 
{

}

void Player::Heal(const float & amount)
{
	m_stats.health += amount;
}

void Player::IncreaseScore(const int & amount)
{
	m_score += amount;
}


void Player::GetScore(int & amount) const
{
	amount = m_score;
}

void Player::ClearScore()
{
	m_score = 0;
}

bool Player::AddToInventory(GameObject * item)
{
	if (m_inventory.size() < 12) {
		m_inventory.push_back(item);
		OrganizeInventory();
		return true;
	}
	return false;
}

void Player::GetItems(std::vector<GameObject*>& inventory) const
{
	inventory = m_inventory;
}

bool Player::UsePotion()
{
	std::vector<GameObject*>::iterator it = m_inventory.begin();
	while (it != m_inventory.end()) {
		std::string name;
		(*it)->GetName(name);
		if (name == "potion") {
			delete (*it);
			m_inventory.erase(it);
			return true;
		}
		else {
			++it;
		}
	}
	return false;
}

bool Player::UseKey()
{
	std::vector<GameObject*>::iterator it = m_inventory.begin();
	while (it != m_inventory.end()) {
		std::string name;
		(*it)->GetName(name);
		if (name == "key") {
			delete (*it);
			m_inventory.erase(it);
			return true;
		}
		else {
			++it;
		}
	}
	return false;
}

void Player::ClearInventory()
{
	m_inventory.clear();
}

void Player::ShowSkull()
{
	SetUvCoords(glm::vec2(m_uvSkull.x, m_uvSkull.y), glm::vec2(m_uvSkull.z, m_uvSkull.w));
}

void Player::OrganizeInventory()
{
	std::vector<GameObject*>::iterator it = m_inventory.begin();
	std::vector<GameObject*> inventory;
	while (it != m_inventory.end()) {
		std::string name;
		(*it)->GetName(name);
		if (name == "key") {
			inventory.push_back((*it));
			it = m_inventory.erase(it);
		}
		else {
			++it;
		}
	}
	for (int i = 0; i < m_inventory.size(); i++) {
		inventory.push_back(m_inventory[i]);
	}
	m_inventory = inventory;
}