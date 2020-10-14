#pragma once

#include "Being.h"

struct EnemyStats {
	BeingStats stats;
	unsigned int type_id;
	unsigned int sprite_id;
};

class EnemyGenerator : public GameObject
{
public:
	EnemyGenerator(unsigned int sprite_id, EnemyStats enemyStats, std::vector<glm::vec4> uvCoords, float health, float spawnTime);
	~EnemyGenerator();

	void Update(float deltaTime);
	Being* GetNewEnemy(SpriteSheet* spriteSheet);

	bool IsSpawnReady();
	bool IsAlive();
	void Damage(const float& damage);

private:
	std::vector<glm::vec4> m_uvCoords;
	float m_health;
	float m_maxHealth;

	EnemyStats m_enemyStats;
	float m_spawnTime;
	float m_currentTime;
	bool m_spawn;
};