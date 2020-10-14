#include "Projectile.h"

Projectile::Projectile(glm::vec2 direction, float speed, float damage, unsigned int ownersUnique_id, unsigned int sprite_id) : GameObject::GameObject(sprite_id)
{
	m_direction = direction;
	m_speed = speed;
	m_damage = damage;
	m_ownersUnique_id = ownersUnique_id;
	GetCollider()->SetSensor(true);
}

Projectile::~Projectile()
{
}

void Projectile::Update(float deltaTime)
{
	SetVelocity(m_direction * m_speed * deltaTime);
}

unsigned int Projectile::GetOwnersUnique_id()
{
	return m_ownersUnique_id;
}

void Projectile::GetDamage(float& damage) const
{
	damage = m_damage;
}
