#pragma once

#include "LevelLoader.h"

class TileCollisionManager
{
public:
	TileCollisionManager(LevelInformation* levelInformation);
	~TileCollisionManager();

	void AddObjectToTile(const Object& obj, int xTile, int yTile);
	void RemoveObjectFromAllTiles(GameObject* gameObject);
	void GetTilePos(const glm::vec2& pos, int& xTile1, int& xTile2, int& yTile1, int& yTile2);
	void CheckCollisions(GameObject * gameObject, ObjectType type);
	bool IsTileEmpty(int xTile, int yTile);

private:
	void RemoveObjectFromTile(GameObject* gameObject, int xTile, int yTile);
	bool CheckCollisionsTile(GameObject* gameObject, int xIndex, int yIndex);
	LevelInformation* m_levelInformation;
};