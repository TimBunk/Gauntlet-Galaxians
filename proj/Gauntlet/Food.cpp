#include "Food.h"

Food::Food(float health, unsigned int sprite_id) : GameObject::GameObject(sprite_id)
{
	m_health = health;
}

Food::~Food()
{
}

void Food::GetHealth(float & health) const
{
	health = m_health;
}
