#include "LevelLoader.h"

LevelLoader::LevelLoader()
{
	m_levelInformation = new LevelInformation();

	ghostStats.stats.points = 10;
	ghostStats.stats.health = 12.0f;
	ghostStats.stats.speed = 150.0f;
	ghostStats.stats.damage = 10.0f;
	ghostStats.sprite_id = 140;
	ghostStats.type_id = ObjectType::e_being;

	demonStats.stats.points = 20;
	demonStats.stats.health = 9.0f;
	demonStats.stats.speed = 150.0f;
	demonStats.stats.damage = 10.0f;
	demonStats.stats.shootCooldown = 2.0f;
	demonStats.stats.projectileSpeed = 300.0f;
	demonStats.stats.projectileDamage = 4.0f;
	demonStats.sprite_id = 175;
	demonStats.type_id = ObjectType::e_being;

	warriorStats.stats.points = 30;
	warriorStats.stats.health = 20.0f;
	warriorStats.stats.speed = 150.0f;
	warriorStats.stats.damage = 10.0f;
	warriorStats.sprite_id = 210;
	warriorStats.type_id = ObjectType::e_being;

	mageStats.stats.points = 40;
	mageStats.stats.health = 20.0f;
	mageStats.stats.speed = 150.0f;
	mageStats.stats.damage = 30.0f;
	mageStats.stats.invisibleTime = 3.0f;
	mageStats.sprite_id = 245;
	mageStats.type_id = ObjectType::e_being;

	deathStats.stats.points = 50;
	deathStats.stats.health = 500.0f;
	deathStats.stats.speed = 200.0f;
	deathStats.stats.damage = 36.0f;
	deathStats.sprite_id = 280;
	deathStats.type_id = ObjectType::e_being;
}

LevelLoader::~LevelLoader()
{
	DeleteCurrentLoadedLevel();
	delete m_levelInformation->player;
	delete m_levelInformation;
}

bool LevelLoader::LoadLevelData(const char * level, SpriteSheet* spriteSheetBackground, SpriteSheet* spriteSheetEntities)
{
	FILE* file = fopen(level, "rb");
	if (file != NULL) {
		TiXmlDocument doc(level);
		doc.LoadFile();

		TiXmlNode *xmlNodeMap = doc.FirstChild("map");
		TiXmlElement* xmlElementMap = xmlNodeMap->ToElement();

		std::stringstream width(xmlElementMap->Attribute("width"));
		width >> m_levelInformation->width;
		std::stringstream height(xmlElementMap->Attribute("height"));
		height >> m_levelInformation->height;
		std::stringstream tileWidth(xmlElementMap->Attribute("tilewidth"));
		tileWidth >> m_levelInformation->tileWidth;
		std::stringstream tileHeight(xmlElementMap->Attribute("tileheight"));
		tileHeight >> m_levelInformation->tileHeight;

		for (int i = 0; i < m_levelInformation->height; i++) {
			std::vector<Tile> tiles;
			for (int j = 0; j < m_levelInformation->width; j++) {
				Tile t;
				tiles.push_back(t);
			}
			m_levelInformation->tiles.push_back(tiles);
		}

		// Loop through childeren of the map
		for (TiXmlNode* child = xmlNodeMap->FirstChild(); child; child = child->NextSibling()) {
			if (child->Value()[0] == 'l') {
				TiXmlElement* xmlElementChild = child->ToElement();
				std::string layerName = xmlElementChild->Attribute("name");
				TiXmlElement* xmlElementData = child->FirstChildElement("data");
				const char* text = xmlElementData->GetText();
				if (layerName == "background") {
					LoadLayer(&LevelLoader::AddToBackground, text, spriteSheetBackground);
				}
				else if (layerName == "shadows") {
					LoadLayer(&LevelLoader::AddToShadows, text, spriteSheetBackground);
				}
				else if (layerName == "entities") {
					LoadLayer(&LevelLoader::AddToEntities, text, spriteSheetEntities);
				}
			}
		}
	}
	else {
		return false;
	}
	fclose(file);
	return true;
}

void LevelLoader::DeleteCurrentLoadedLevel()
{
	std::vector<GameObject*>::iterator it = m_levelInformation->backgroundObjects.begin();
	while (it != m_levelInformation->backgroundObjects.end()) {
		delete (*it);
		++it;
	}
	m_levelInformation->backgroundObjects.clear();
	std::vector<GameObject*>::iterator it2 = m_levelInformation->walls.begin();
	while (it2 != m_levelInformation->walls.end()) {
		delete (*it2);
		++it2;
	}
	m_levelInformation->walls.clear();
	std::vector<Being*>::iterator it3 = m_levelInformation->enemies.begin();
	while (it3 != m_levelInformation->enemies.end()) {
		delete (*it3);
		++it3;
	}
	m_levelInformation->enemies.clear();
	std::vector<Projectile*>::iterator it4 = m_levelInformation->projectiles.begin();
	while (it4 != m_levelInformation->projectiles.end()) {
		delete (*it4);
		++it4;
	}
	m_levelInformation->projectiles.clear();
	std::vector<EnemyGenerator*>::iterator it5 = m_levelInformation->enemyGenerators.begin();
	while (it5 != m_levelInformation->enemyGenerators.end()) {
		delete (*it5);
		++it5;
	}
	m_levelInformation->enemyGenerators.clear();
	std::vector<GameObject*>::iterator it6 = m_levelInformation->pickups.begin();
	while (it6 != m_levelInformation->pickups.end()) {
		delete (*it6);
		++it6;
	}
	m_levelInformation->pickups.clear();
	std::vector<GameObject*>::iterator it7 = m_levelInformation->doors.begin();
	while (it7 != m_levelInformation->doors.end()) {
		delete (*it7);
		++it7;
	}
	m_levelInformation->doors.clear();
	std::vector<GameObject*>::iterator it8 = m_levelInformation->exits.begin();
	while (it8 != m_levelInformation->exits.end()) {
		delete (*it8);
		++it8;
	}
	m_levelInformation->exits.clear();

	for (int i = 0; i < m_levelInformation->tiles.size(); i++) {
		m_levelInformation->tiles[i].clear();
	}
	m_levelInformation->tiles.clear();
}

void LevelLoader::LoadLayer(void(LevelLoader::*LoadLevelPart)(const glm::vec2&, unsigned int, SpriteSheet*), const char * text, SpriteSheet * spriteSheet)
{
	glm::vec2 pos = glm::vec2(0, 0);
	for (int i = 0; i < strlen(text); i++) {
		if (text[i] != '0' && text[i] != ',') {
			std::string number = "";
			for (int j = i; j < strlen(text); j++) {
				i = j;
				if (text[j] == ',') {
					break;
				}
				number += text[j];
			}
			int spriteNum = std::stoi(number);
			if (spriteNum != 0) {
				(this->*LoadLevelPart)(pos, spriteNum, spriteSheet);
			}
		}
		else if (text[i] != ',') {
			continue;
		}
		pos.x += m_levelInformation->tileWidth;
		if (pos.x >= m_levelInformation->width * m_levelInformation->tileWidth) {
			pos.x = 0.0f;
			pos.y -= m_levelInformation->tileHeight;
		}
	}
}

void LevelLoader::AddToBackground(const glm::vec2& pos, unsigned int spriteNum, SpriteSheet * spriteSheetBackground)
{
	// If the spriteNum is between 16 and 113 that means it is a wall otherwise it is a floor
	GameObject* gameObject = NULL;
	if (spriteNum > 16 && spriteNum < 113) {
		gameObject = new GameObject(spriteSheetBackground->GetSprite(spriteNum - 1));
		m_levelInformation->walls.push_back(gameObject);
		AddToTiles(pos, gameObject, ObjectType::e_wall);
	}
	else {
		gameObject = new GameObject(spriteSheetBackground->GetSprite(spriteNum - 1));
		gameObject->SetPos(glm::vec2(pos.x, pos.y));
		m_levelInformation->backgroundObjects.push_back(gameObject);
	}
}

void LevelLoader::AddToShadows(const glm::vec2& pos, unsigned int spriteNum, SpriteSheet * spriteSheet)
{
	GameObject* gameObject = new GameObject(spriteSheet->GetSprite(spriteNum - 1));
	gameObject->SetPos(glm::vec2(pos.x, pos.y));
	m_levelInformation->backgroundObjects.push_back(gameObject);
}

void LevelLoader::AddToEntities(const glm::vec2& pos, unsigned int spriteNum, SpriteSheet * spriteSheet)
{
	// Player
	if ((spriteNum > 128 && spriteNum < 153) || (spriteNum > 163 && spriteNum < 188) || (spriteNum > 198 && spriteNum < 223) || (spriteNum > 233 && spriteNum < 258)) {
		if (m_levelInformation->player == NULL) {
			BeingStats playerStats;
			playerStats.health = 500.0f;
			playerStats.speed = 200.0f;
			playerStats.damage = 36.0f;
			playerStats.shootCooldown = 0.333f;
			playerStats.projectileSpeed = 300.0f;
			playerStats.projectileDamage = 18.0f;
			glm::vec4 uvsSkullSprite;
			spriteSheet->GetUvCoorsSprite(161 - 129, uvsSkullSprite);
			Player* player = new Player(uvsSkullSprite, 129 - 129, spriteSheet, playerStats);
			m_levelInformation->player = player;
			AddToTiles(pos, player, ObjectType::e_player);
		}
		else {
			AddToTiles(pos, m_levelInformation->player, ObjectType::e_player);
		}
	}
	// Ghost
	else if (spriteNum > 268 && spriteNum < 293) {
		Being* ghost = new Being(269 - 129, spriteSheet, ghostStats.stats, spriteSheet->GetSprite(ghostStats.sprite_id));
		m_levelInformation->enemies.push_back(ghost);
		AddToTiles(pos, ghost, ObjectType::e_being);
	}
	// Ghost generator
	else if (spriteNum > 300 && spriteNum < 304) {
		std::vector<glm::vec4> allUvCoords;
		for (int i = 303; i > 300; i--) {
			glm::vec4 uvCoords;
			spriteSheet->GetUvCoorsSprite(i-129, uvCoords);
			allUvCoords.push_back(uvCoords);
		}
		EnemyGenerator* ghostGenerator = new EnemyGenerator(spriteSheet->GetSprite(301-129), ghostStats, allUvCoords, 26.0f, 1.5f);
		m_levelInformation->enemyGenerators.push_back(ghostGenerator);
		AddToTiles(pos, ghostGenerator, ObjectType::e_generator);
	}
	// Demon
	else if (spriteNum > 303 && spriteNum < 328) {
		Being* demon = new Being(304 - 129, spriteSheet, demonStats.stats, spriteSheet->GetSprite(demonStats.sprite_id));
		m_levelInformation->enemies.push_back(demon);
		AddToTiles(pos, demon, ObjectType::e_being);
	}
	// Demon generator
	else if (spriteNum > 335 && spriteNum < 339) {
		std::vector<glm::vec4> allUvCoords;
		for (int i = 338; i > 335; i--) {
			glm::vec4 uvCoords;
			spriteSheet->GetUvCoorsSprite(i - 129, uvCoords);
			allUvCoords.push_back(uvCoords);
		}
		EnemyGenerator* demonGenerator = new EnemyGenerator(spriteSheet->GetSprite(336 - 129), demonStats, allUvCoords, 26.0f, 1.5f);
		m_levelInformation->enemyGenerators.push_back(demonGenerator);
		AddToTiles(pos, demonGenerator, ObjectType::e_generator);
	}
	// Warrior
	else if (spriteNum > 338 && spriteNum < 363) {
		Being* warrior = new Being(339 - 129, spriteSheet, warriorStats.stats, spriteSheet->GetSprite(warriorStats.sprite_id));
		m_levelInformation->enemies.push_back(warrior);
		AddToTiles(pos, warrior, ObjectType::e_being);
	}
	// Warrior generator
	else if (spriteNum > 370 && spriteNum < 374) {
		std::vector<glm::vec4> allUvCoords;
		for (int i = 373; i > 370; i--) {
			glm::vec4 uvCoords;
			spriteSheet->GetUvCoorsSprite(i - 129, uvCoords);
			allUvCoords.push_back(uvCoords);
		}
		EnemyGenerator* warriorGenerator = new EnemyGenerator(spriteSheet->GetSprite(371 - 129), warriorStats, allUvCoords, 26.0f, 1.5f);
		m_levelInformation->enemyGenerators.push_back(warriorGenerator);
		AddToTiles(pos, warriorGenerator, ObjectType::e_generator);
	}
	// Mage
	else if (spriteNum > 373 && spriteNum < 398) {
		Being* mage = new Being(374 - 129, spriteSheet, mageStats.stats, spriteSheet->GetSprite(mageStats.sprite_id));
		m_levelInformation->enemies.push_back(mage);
		AddToTiles(pos, mage, ObjectType::e_being);
	}
	// Mage generator
	else if (spriteNum > 405 && spriteNum < 409) {
		std::vector<glm::vec4> allUvCoords;
		for (int i = 408; i > 405; i--) {
			glm::vec4 uvCoords;
			spriteSheet->GetUvCoorsSprite(i - 129, uvCoords);
			allUvCoords.push_back(uvCoords);
		}
		EnemyGenerator* mageGenerator = new EnemyGenerator(spriteSheet->GetSprite(406 - 129), mageStats, allUvCoords, 26.0f, 1.5f);
		m_levelInformation->enemyGenerators.push_back(mageGenerator);
		AddToTiles(pos, mageGenerator, ObjectType::e_generator);
	}
	// Death
	else if (spriteNum > 408 && spriteNum < 433) {
		Being* death = new Being(409 - 129, spriteSheet, deathStats.stats, spriteSheet->GetSprite(deathStats.sprite_id));
		m_levelInformation->enemies.push_back(death);
		AddToTiles(pos, death, ObjectType::e_being);
	}
	// Food
	else if (spriteNum >= 444 && spriteNum <= 448) {
		Food* food;
		if (spriteNum == 444 || spriteNum == 445) {
			food = new Food(50.0f, spriteSheet->GetSprite(spriteNum - 129));
		}
		else if (spriteNum == 446) {
			food = new Food(20.0f, spriteSheet->GetSprite(spriteNum - 129));
		}
		else {
			food = new Food(30.0f, spriteSheet->GetSprite(spriteNum - 129));
		}
		m_levelInformation->pickups.push_back(food);
		AddToTiles(pos, food, ObjectType::e_food);
	}
	// Key
	else if (spriteNum == 449) {
		GameObject* key = new GameObject(spriteSheet->GetSprite(spriteNum - 129));
		key->SetName("key");
		m_levelInformation->pickups.push_back(key);
		AddToTiles(pos, key, ObjectType::e_key);
	}
	// potion
	else if (spriteNum == 450) {
		GameObject* potion = new GameObject(spriteSheet->GetSprite(spriteNum - 129));
		potion->SetName("potion");
		m_levelInformation->pickups.push_back(potion);
		AddToTiles(pos, potion, ObjectType::e_potion);
	}
	// Chest
	else if (spriteNum >= 451 && spriteNum <= 453) {
		Chest* chest = new Chest(100, spriteSheet->GetSprite(spriteNum - 129));
		m_levelInformation->pickups.push_back(chest);
		AddToTiles(pos, chest, ObjectType::e_chest);
	}
	// Door
	else if (spriteNum == 454 || spriteNum == 455) {
		GameObject* door = new GameObject(spriteSheet->GetSprite(spriteNum - 129));
		m_levelInformation->doors.push_back(door);
		AddToTiles(pos, door, ObjectType::e_door);
	}
	// Exit
	else if (spriteNum == 456) {
		GameObject* exit = new GameObject(spriteSheet->GetSprite(spriteNum - 129));
		m_levelInformation->exits.push_back(exit);
		AddToTiles(pos, exit, ObjectType::e_exit);
	}
}

void LevelLoader::AddToTiles(glm::vec2 pos, GameObject * gameObject, ObjectType type)
{
	gameObject->SetPos(pos);
	gameObject->SetType_id(type);
	Object object { type, gameObject };
	m_levelInformation->tiles[std::abs(pos.y) / m_levelInformation->tileHeight][pos.x / m_levelInformation->tileWidth].objects.push_back(object);
}

LevelInformation* LevelLoader::GetLevelInfo()
{
	return m_levelInformation;
}