#include "TileCollisionManager.h"

TileCollisionManager::TileCollisionManager(LevelInformation* levelInformation)
{
	m_levelInformation = levelInformation;
}

TileCollisionManager::~TileCollisionManager()
{
}

void TileCollisionManager::AddObjectToTile(const Object& obj, int xTile, int yTile)
{
	int index = -1;
	for (int i = 0; i < m_levelInformation->tiles[yTile][xTile].objects.size(); i++) {
		if (obj.gameObject == m_levelInformation->tiles[yTile][xTile].objects[i].gameObject) {
			return;
		}
		if (index == -1 && m_levelInformation->tiles[yTile][xTile].objects[i].gameObject == NULL) {
			index = i;
		}
	}
	if (index == -1) {
		m_levelInformation->tiles[yTile][xTile].objects.push_back(obj);
	}
	else {
		m_levelInformation->tiles[yTile][xTile].objects[index] = obj;
	}
}

void TileCollisionManager::RemoveObjectFromAllTiles(GameObject * gameObject)
{
	glm::vec2 pos;
	gameObject->GetPos(pos);
	int xTile1;
	int xTile2;
	int yTile1;
	int yTile2;
	GetTilePos(pos, xTile1, xTile2, yTile1, yTile2);

	RemoveObjectFromTile(gameObject, xTile1, yTile1);
	RemoveObjectFromTile(gameObject, xTile1, yTile2);
	RemoveObjectFromTile(gameObject, xTile2, yTile1);
	RemoveObjectFromTile(gameObject, xTile2, yTile2);
}

void TileCollisionManager::GetTilePos(const glm::vec2 & pos, int & xTile1, int & xTile2, int & yTile1, int & yTile2)
{
	float tilePosX = pos.x / m_levelInformation->tileWidth;
	float tilePosY = std::abs((pos.y) / m_levelInformation->tileHeight);
	xTile1 = std::floor(tilePosX);
	xTile2 = std::ceil(tilePosX);
	yTile1 = std::floor(tilePosY);
	yTile2 = std::ceil(tilePosY);
}

void TileCollisionManager::CheckCollisions(GameObject * gameObject, ObjectType type)
{
	glm::vec2 pos;
	gameObject->GetPos(pos);
	int xTile1;
	int xTile2;
	int yTile1;
	int yTile2;
	GetTilePos(pos, xTile1, xTile2, yTile1, yTile2);

	RemoveObjectFromTile(gameObject, xTile1, yTile1);
	RemoveObjectFromTile(gameObject, xTile1, yTile2);
	RemoveObjectFromTile(gameObject, xTile2, yTile1);
	RemoveObjectFromTile(gameObject, xTile2, yTile2);

	CheckCollisionsTile(gameObject, xTile1, yTile1);
	CheckCollisionsTile(gameObject, xTile1 - 1, yTile1);
	CheckCollisionsTile(gameObject, xTile1, yTile1 - 1);
	CheckCollisionsTile(gameObject, xTile1 - 1, yTile1 - 1);

	CheckCollisionsTile(gameObject, xTile2, yTile1);
	CheckCollisionsTile(gameObject, xTile2 + 1, yTile1);
	CheckCollisionsTile(gameObject, xTile2, yTile1 - 1);
	CheckCollisionsTile(gameObject, xTile2 + 1, yTile1 - 1);

	CheckCollisionsTile(gameObject, xTile1, yTile2);
	CheckCollisionsTile(gameObject, xTile1 - 1, yTile2);
	CheckCollisionsTile(gameObject, xTile1, yTile2 + 1);
	CheckCollisionsTile(gameObject, xTile1 - 1, yTile2 + 1);

	CheckCollisionsTile(gameObject, xTile2, yTile2);
	CheckCollisionsTile(gameObject, xTile2 + 1, yTile2);
	CheckCollisionsTile(gameObject, xTile2, yTile2 + 1);
	CheckCollisionsTile(gameObject, xTile2 + 1, yTile2 + 1);

	gameObject->ApplyPhysics();
	gameObject->GetPos(pos);
	GetTilePos(pos, xTile1, xTile2, yTile1, yTile2);
	Object obj{ type, gameObject };

	if (xTile1 >= 0 && xTile1 < m_levelInformation->tiles[0].size()) {
		if (yTile1 >= 0 && yTile1 < m_levelInformation->tiles.size()) {
			AddObjectToTile(obj, xTile1, yTile1);
		}
		if (yTile2 >= 0 && yTile2 < m_levelInformation->tiles.size()) {
			AddObjectToTile(obj, xTile1, yTile2);
		}
	}
	if (xTile2 >= 0 && xTile2 < m_levelInformation->tiles[0].size()) {
		if (yTile1 >= 0 && yTile1 < m_levelInformation->tiles.size()) {
			AddObjectToTile(obj, xTile2, yTile1);
		}
		if (yTile2 >= 0 && yTile2 < m_levelInformation->tiles.size()) {
			AddObjectToTile(obj, xTile2, yTile2);
		}
	}
}

bool TileCollisionManager::IsTileEmpty(int xTile, int yTile)
{
	if (xTile >= 0 && xTile < m_levelInformation->width && yTile >= 0 && yTile < m_levelInformation->height) {
		for (int i = 0; i < m_levelInformation->tiles[yTile][xTile].objects.size();i++) {
			if (m_levelInformation->tiles[yTile][xTile].objects[i].gameObject != NULL) {
				return false;
			}
		}
		return true;
	}
	return false;
}

void TileCollisionManager::RemoveObjectFromTile(GameObject * gameObject, int xTile, int yTile)
{
	Tile& tile = m_levelInformation->tiles[yTile][xTile];
	for (int i = 0; i < tile.objects.size(); i++) {
		if (gameObject == tile.objects[i].gameObject) {
			tile.objects[i].gameObject = NULL;
			tile.objects[i].type = ObjectType::e_nothing;
			break;
		}
	}
	m_levelInformation->tiles[yTile][xTile] = tile;
}

bool TileCollisionManager::CheckCollisionsTile(GameObject * gameObject, int xIndex, int yIndex)
{
	if (xIndex <= 0 && yIndex <= 0) { return false; }
	int rows = m_levelInformation->tiles.size();
	if (yIndex >= rows) { return false; }
	int columns = m_levelInformation->tiles[yIndex].size();
	if (xIndex >= columns) { return false; }
	Tile& tile = m_levelInformation->tiles[yIndex][xIndex];
	bool collision = false;
	for (int i = 0; i < tile.objects.size(); i++) {
		if (tile.objects[i].gameObject != NULL && tile.objects[i].type != ObjectType::e_nothing && gameObject != tile.objects[i].gameObject) {

			glm::vec2 pos;
			tile.objects[i].gameObject->GetPos(pos);
			int xTile1;
			int xTile2;
			int yTile1;
			int yTile2;
			GetTilePos(pos, xTile1, xTile2, yTile1, yTile2);

			if ((xTile1 == xIndex || xTile2 == xIndex) && (yTile1 == yIndex || yTile2 == yIndex)) {
				if (gameObject->IsCollidingWith(tile.objects[i].gameObject)) {
					gameObject->AddCollision(tile.objects[i].gameObject);
					if (gameObject->GetCollider()->IsSensor() == false) {
						collision = true;
					}
				}
			}
			else {
				tile.objects[i].gameObject = NULL;
				tile.objects[i].type = ObjectType::e_nothing;
			}
		}
	}
	m_levelInformation->tiles[yIndex][xIndex] = tile;
	return collision;
}
