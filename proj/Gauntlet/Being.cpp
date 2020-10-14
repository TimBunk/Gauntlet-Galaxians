#include "Being.h"

Being::Being(unsigned int startingSpriteIndex, SpriteSheet* spritesheet, BeingStats stats, unsigned int sprite_id) : GameObject::GameObject(sprite_id)
{
	m_startingSpriteIndex = startingSpriteIndex;
	m_spriteSheet = spritesheet;
	m_stats = stats;
	m_currentAnimation = 0;

	glm::vec4 uvCoords;
	for (int i = 0; i < 8; i++) {
		Animation a;
		a.updateTime = 0.2f;
		spritesheet->GetUvCoorsSprite(startingSpriteIndex + i, uvCoords);
		a.uvCoords.push_back(uvCoords);
		spritesheet->GetUvCoorsSprite(startingSpriteIndex + i + 8, uvCoords);
		a.uvCoords.push_back(uvCoords);
		spritesheet->GetUvCoorsSprite(startingSpriteIndex + i + 16, uvCoords);
		a.uvCoords.push_back(uvCoords);
		m_animations.push_back(a);
	}
	m_shootTimer = 0.0f;
}

Being::~Being()
{
}

void Being::Update(float deltaTime)
{
	if (m_stats.shootCooldown != 0.0f && m_shootTimer < m_stats.shootCooldown) {
		m_shootTimer += deltaTime;
	}
	
	if (m_stats.invisibleTime != 0.0f) {
		m_invisibleTimer += deltaTime;
		if (m_invisibleTimer >= m_stats.invisibleTime * 2.0f) {
			if (m_invisibleTimer <= (m_stats.invisibleTime * 2.0f + m_stats.invisibleTime)) {
				SetColour(glm::vec4(1, 1, 1, 0));
			}
			else {
				SetColour(glm::vec4(1, 1, 1, 1));
				m_invisibleTimer -= (m_stats.invisibleTime * 2.0f + m_stats.invisibleTime);
			}
		}
	}
}

bool Being::IsShooter()
{
	if (m_stats.shootCooldown != 0.0f) {
		return true;
	}
	return false;
}

Projectile* Being::Shoot()
{
	if (m_stats.shootCooldown != 0.0f && m_shootTimer >= m_stats.shootCooldown) {
		m_shootTimer -= m_stats.shootCooldown;
		float degrees = ((float)m_currentAnimation * 45.0f) - 90.0f;
		float radians = glm::radians(degrees);
		glm::vec2 currentPos;
		GetPos(currentPos);
		glm::vec2 dir = glm::vec2(std::cos(radians), std::sin(radians));
		dir.y *= -1;
		dir = glm::normalize(dir);
		unsigned int sprite_id = m_spriteSheet->GetSprite(m_startingSpriteIndex + 24 + m_currentAnimation);
		Projectile* p = new Projectile(dir, m_stats.projectileSpeed, m_stats.projectileDamage, GetUnique_id(), sprite_id);
		p->SetPos(currentPos);
		return p;
	}
	return NULL;
}

void Being::PlayCurrentAnimation(float deltaTime)
{
	glm::vec4 uvCoords = m_animations[m_currentAnimation].UpdateAnimation(deltaTime);
	SetUvCoords(glm::vec2(uvCoords.x, uvCoords.y), glm::vec2(uvCoords.z, uvCoords.w));
}

void Being::UpdateAnimation()
{
	glm::vec2 vel;
	GetVelocity(vel);
	if (vel.x == 0.0f && vel.y == 0.0f) { return; }

	float degrees = glm::degrees(std::atan2(vel.y, vel.x));
	if (degrees >= 0) {
		// Moving right
		if (degrees < 22.5f) {
			m_currentAnimation = 2;
		}
		// Moving up-right
		else if (degrees >= 22.5f && degrees < 67.5f) {
			m_currentAnimation = 1;
		}
		// Moving up
		else if (degrees >= 67.5f && degrees < 112.5f) {
			m_currentAnimation = 0;
		}
		// Moving up-left
		else if (degrees >= 112.5f && degrees < 157.5f) {
			m_currentAnimation = 7;
		}
		// Moving left
		else if (degrees >= 157.5f && degrees <= 180.0f) {
			m_currentAnimation = 6;
		}
	}
	else {
		// Moving left
		if (degrees < -157.5f) {
			m_currentAnimation = 6;
		}
		// Moving down-left
		else if (degrees >= -157.5f && degrees < -112.5f) {
			m_currentAnimation = 5;
		}
		// Moving down
		else if (degrees >= -112.5f && degrees < -67.5f) {
			m_currentAnimation = 4;
		}
		// Moving down-right
		else if (degrees >= -67.5f && degrees < -22.5f) {
			m_currentAnimation = 3;
		}
		// Moving right
		else if (degrees >= -22.5f && degrees < 0) {
			m_currentAnimation = 2;
		}
	}
}

void Being::Chase(glm::vec2 dir, float deltaTime)
{
	glm::vec2 currentPos;
	GetPos(currentPos);
	dir = glm::normalize(dir);
	dir *= (m_stats.speed * deltaTime);
	SetVelocity(dir);
}

void Being::Damage(const float& amount) {
	m_stats.health -= amount;
	if (m_stats.health < 0) {
		m_stats.health = 0;
	}
}

bool Being::IsAlive() {
	if (m_stats.health <= 0) {
		return false;
	}
	return true;
}

void Being::GetPoints(int & points) const
{
	points = m_stats.points;
}

void Being::GetHealth(float& health) const
{
	health = m_stats.health;
}

void Being::GetSpeed(float& speed) const 
{
	speed = m_stats.speed;
}

void Being::GetDamage(float& damage) const 
{
	damage = m_stats.damage;
}