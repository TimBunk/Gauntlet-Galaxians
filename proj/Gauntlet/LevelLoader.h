#pragma once

#include <fstream>
#include <sstream>
#include <tinyxml.h>

#include "SpriteSheet.h"
#include "Player.h"
#include "EnemyGenerator.h"
#include "Food.h"
#include "Chest.h"

enum ObjectType {
	e_nothing,
	e_wall,
	e_projectile,
	e_being,
	e_player,
	e_generator,
	e_food,
	e_chest,
	e_potion,
	e_key,
	e_door,
	e_exit
};

struct Object {
	ObjectType type = e_nothing;
	GameObject* gameObject = NULL;
};

struct Tile {
	std::vector<Object> objects;
};

struct LevelInformation {
	int width = 0;
	int height = 0;
	int tileWidth = 0;
	int tileHeight = 0;
	Player* player = NULL;
	std::vector<std::vector<Tile>> tiles;
	std::vector<Projectile*> projectiles;
	std::vector<Being*> enemies;
	std::vector<EnemyGenerator*> enemyGenerators;
	std::vector<GameObject*> pickups;
	std::vector<GameObject*> exits;
	std::vector<GameObject*> doors;
	std::vector<GameObject*> walls;
	// BackgroundObjects contians floors and shadows
	std::vector<GameObject*> backgroundObjects;
};

class LevelLoader
{
public:
	LevelLoader();
	~LevelLoader();

	bool LoadLevelData(const char* level, SpriteSheet* spriteSheetBackground, SpriteSheet* spriteSheetEntities);
	void DeleteCurrentLoadedLevel();
	void LoadLayer(void(LevelLoader::*LoadLevelPart)(const glm::vec2&, unsigned int, SpriteSheet*), const char* text, SpriteSheet* spriteSheet);
	void AddToBackground(const glm::vec2& pos, unsigned int spriteNum, SpriteSheet* spriteSheet);
	void AddToShadows(const glm::vec2& pos, unsigned int spriteNum, SpriteSheet* spriteSheet);
	void AddToEntities(const glm::vec2& pos, unsigned int spriteNum, SpriteSheet* spriteSheet);

	void AddToTiles(glm::vec2 pos, GameObject* gameObject, ObjectType type);

	LevelInformation* GetLevelInfo();

private:
	LevelInformation* m_levelInformation;
	EnemyStats ghostStats;
	EnemyStats demonStats;
	EnemyStats warriorStats;
	EnemyStats mageStats;
	EnemyStats deathStats;
};