#include "Chest.h"

Chest::Chest(int points, unsigned int sprite_id) : GameObject::GameObject(sprite_id)
{
	m_points = points;
}

Chest::~Chest()
{
}

void Chest::GetPoints(int & points) const
{
	points = m_points;
}
