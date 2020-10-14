#include "EnemyGenerator.h"

EnemyGenerator::EnemyGenerator(unsigned int sprite_id, EnemyStats enemyStats, std::vector<glm::vec4> uvCoords, float health, float spawnTime) : GameObject::GameObject(sprite_id)
{
	m_enemyStats = enemyStats;
	m_uvCoords = uvCoords;
	m_health = health;
	m_maxHealth = health;
	m_spawnTime = spawnTime;
	m_spawn = false;
	m_currentTime = 0.0f;
}

EnemyGenerator::~EnemyGenerator()
{
}

void EnemyGenerator::Update(float deltaTime)
{
	if (m_spawn == false) {
		m_currentTime += deltaTime;
		if (m_currentTime >= m_spawnTime) {
			m_currentTime -= m_spawnTime;
			m_spawn = true;
		}
	}
}

Being * EnemyGenerator::GetNewEnemy(SpriteSheet* spriteSheet)
{
	m_spawn = false;
	Being* being = new Being(m_enemyStats.sprite_id, spriteSheet, m_enemyStats.stats, spriteSheet->GetSprite(m_enemyStats.sprite_id));
	being->SetType_id(m_enemyStats.type_id);
	return being;
}

bool EnemyGenerator::IsSpawnReady()
{
	return m_spawn;
}

bool EnemyGenerator::IsAlive()
{
	if (m_health > 0.0f) {
		return true;
	}
	return false;
}

void EnemyGenerator::Damage(const float & damage)
{
	m_health -= damage;
	if (m_health < 0.0f) {
		m_health = 0.0f;
	}
	float healthPercentage = m_health / m_maxHealth * 100.0f;
	int maxSprites = m_uvCoords.size();
	for (int i = maxSprites-1; maxSprites >= 0; i--) {
		float spritePercentage = (float)i / maxSprites * 100.0f;
		if (healthPercentage >= spritePercentage) {
			SetUvCoords(glm::vec2(m_uvCoords[i].x, m_uvCoords[i].y), glm::vec2(m_uvCoords[i].z, m_uvCoords[i].w));
			break;
		}
	}
}
