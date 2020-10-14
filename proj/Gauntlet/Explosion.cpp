#include "Explosion.h"

Explosion::Explosion(Animation& animation, unsigned int sprite_id) : GameObject::GameObject(sprite_id)
{
	m_animation = animation;
	m_lifeTime = m_animation.updateTime * (m_animation.uvCoords.size()-1);
	m_timer = 0.0f;
}

Explosion::~Explosion() 
{

}

void Explosion::UpdateAnimation(float deltaTime)
{
	m_timer += deltaTime;
	if (m_timer >= m_lifeTime) {
		SetAlive(false);
		return;
	}
	glm::vec4 uvCoords = m_animation.UpdateAnimation(deltaTime);
	SetUvCoords(glm::vec2(uvCoords.x, uvCoords.y), glm::vec2(uvCoords.z, uvCoords.w));
}