#ifndef __Tutorial1_H_
#define __Tutorial1_H_

#include <fstream>

#include "Application.h"
#include <glm/glm.hpp>

#include "TileCollisionManager.h"
#include "Explosion.h"

// Derived application class that wraps up all globals neatly
class Main : public NHTV::Application
{
public:

	Main();
	virtual ~Main();

protected:

	virtual bool onCreate(int a_argc, char* a_argv[]);
	virtual void onUpdate(float a_deltaTime);
	virtual void onDraw();
	virtual void onDestroy();

	void UpdatePlayer(float deltaTime);
	void UpdateProjectiles(float deltaTime);
	void UpdateEnemies(float deltaTime);
	void UpdateEnemyGenerators(float deltaTime);
	void UpdateExplosions(float deltaTime);
	void AddExplosion(const glm::vec2& pos, Animation animation, unsigned int sprite_id);

	void UsePotion();
	void OpenDoor(GameObject* door);

	void DeleteFromTiles(GameObject* gameObject);
	void DeleteEnemy(Being* enemy);
	void RemoveFromPickups(GameObject* gameObject);

	void NextLevel();
	void Restart();

private:
	//void LoadLevel(char* fileName);

	int m_iScreenWidth;
	int m_iScreenHeight;

	int m_fpsCount;
	float m_fpsTimer;
	float m_maxDeltaTime;
	
	SpriteSheet* m_spriteSheetBackground;
	SpriteSheet* m_spriteSheetEntities;
	LevelLoader* m_levelLoader;
	LevelInformation* m_levelInformation;
	TileCollisionManager* m_tcm;

	std::vector<Explosion*> m_explosions;
	Animation m_explosionProjectileAnimation;
	Animation m_explosionEnemyAnimation;

	bool m_nextLevel;
	float m_nextLevelTimer;
	glm::vec2 m_posExit;

	int m_currentLevel;
	int m_lastLevel;
	bool m_enterKey;
	int m_playerHealth;
	int m_playerScore;
	float m_playerDeathTime;
};

#endif // __Tutorial1_H_