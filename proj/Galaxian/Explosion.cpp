#include "Explosion.h"

Explosion::Explosion(unsigned int sprite_id1, unsigned int sprite_id2, glm::vec2 pos) : GameObject::GameObject(sprite_id1)
{
	this->sprite_id2 = sprite_id2;
	SetPos(pos);
	SetScale(glm::vec2(32, 32));

	m_timer = 0.0f;
	changeTime = 0.25f;
}

Explosion::~Explosion()
{
}

void Explosion::Update(float deltaTime)
{
	m_timer += deltaTime;
	if (m_timer >= changeTime * 2.0f) {
		SetAlive(false);
	}
	else if (m_timer >= changeTime) {
		glm::vec2 pos = GetPos();
		sprite_id = sprite_id2;
		SetPos(pos);
		SetScale(glm::vec2(32, 32));
	}
}
