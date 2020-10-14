#include "Application.h"
#include "Main.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

APPLICATION_ENTRY(Main);
#define DEFAULT_SCREENWIDTH 1280
#define DEFAULT_SCREENHEIGHT 720

Main::Main()
{
	
}

Main::~Main()
{

}

bool Main::onCreate(int a_argc, char* a_argv[])
{
	setBackgroundColor(NHTV::SColour(0x00, 0x00, 0x00, 0xFF));

	GetScreenSize(m_iScreenWidth, m_iScreenHeight);

	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	unsigned int backgrounds2Sprite = spriteManager->AddSprite("Gauntlet_images/backgrounds2.png", glm::vec2(512, 256), glm::vec2(0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_spriteSheetBackground = new SpriteSheet(512, 256, 32, 32, backgrounds2Sprite);
	unsigned int entities2Sprite = spriteManager->AddSprite("Gauntlet_images/entities2.png", glm::vec2(1120, 320), glm::vec2(0.5f, 0.5f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
	m_spriteSheetEntities = new SpriteSheet(1120, 320, 32, 32, entities2Sprite);

	m_levelLoader = new LevelLoader();

	m_posExit = glm::vec2(0, 0);
	m_nextLevel = false;
	m_nextLevelTimer = 0.0f;
	m_currentLevel = 1;
	m_lastLevel = 2;
	std::string levelName = "Gauntlet_levels/level";
	levelName += std::to_string(m_currentLevel);
	levelName += ".tmx";
	m_levelLoader->LoadLevelData(levelName.c_str(), m_spriteSheetBackground, m_spriteSheetEntities);
	m_levelInformation = m_levelLoader->GetLevelInfo();
	assert(m_levelInformation->player != NULL && "There is no player!");

	m_tcm = new TileCollisionManager(m_levelInformation);
	bool enterKey = false;
	float playerHealth;
	m_levelInformation->player->GetHealth(playerHealth);
	m_playerHealth = playerHealth;
	int playerScore;
	m_levelInformation->player->GetScore(playerScore);
	m_playerScore = playerScore;
	m_playerDeathTime = 0.0f;


	m_explosionProjectileAnimation.updateTime = 0.075f;
	glm::vec4 uvCoords;
	for (int i = 0; i < 5; i++) {
		m_spriteSheetEntities->GetUvCoorsSprite(334 + i, uvCoords);
		m_explosionProjectileAnimation.uvCoords.push_back(uvCoords);
	}
	m_explosionEnemyAnimation.updateTime = 0.075f;
	for (int i = 0; i < 7; i++) {
		m_spriteSheetEntities->GetUvCoorsSprite(328 + i, uvCoords);
		m_explosionEnemyAnimation.uvCoords.push_back(uvCoords);
	}

	m_fpsCount = 0;
	m_fpsTimer = 0.0f;
	m_maxDeltaTime = 1.0f / 10.0f;

	return true;
}

void Main::onUpdate(float a_deltaTime)
{
	if (a_deltaTime > m_maxDeltaTime) { a_deltaTime = m_maxDeltaTime; }
	m_fpsCount++;
	m_fpsTimer += a_deltaTime;
	if (m_fpsTimer >= 1.0f) {
		float playerHealth;
		int playerScore;
		m_levelInformation->player->GetHealth(playerHealth);
		m_levelInformation->player->GetScore(playerScore);
		printf("Fps: %i, player health: %i, score: %i\n", m_fpsCount, (int)playerHealth, playerScore);
		m_fpsTimer -= 1.0f;
		m_fpsCount = 0;
	}

	if (m_nextLevel == false) {
		UpdatePlayer(a_deltaTime);
		UpdateProjectiles(a_deltaTime);
		UpdateEnemies(a_deltaTime);
		UpdateEnemyGenerators(a_deltaTime);
	}
	else {
		// Move, rotate and scale the player for a second and the load the next level.
		m_nextLevelTimer += a_deltaTime;
		if (m_nextLevelTimer >= 1.0f) {
			NextLevel();
			return;
		}
		glm::vec2 pos;
		m_levelInformation->player->GetPos(pos);
		float speed;
		m_levelInformation->player->GetSpeed(speed);
		speed *= a_deltaTime;
		glm::vec2 diff = m_posExit - pos;
		if (diff.x != 0 || diff.y != 0) {
			float maxSpeed = glm::length(diff);
			if (speed > maxSpeed) {
				speed = maxSpeed;
			}
			diff = glm::normalize(diff);
			pos += diff * speed;
			m_levelInformation->player->SetPos(pos);
		}

		float rot;
		m_levelInformation->player->GetRot(rot);
		rot += 360.0f * a_deltaTime;
		m_levelInformation->player->SetRot(rot);

		glm::vec2 scale;
		m_levelInformation->player->GetScale(scale);
		scale.x -= m_levelInformation->tileWidth * a_deltaTime;
		scale.y -= m_levelInformation->tileHeight * a_deltaTime;
		m_levelInformation->player->SetScale(scale);
	}
	UpdateExplosions(a_deltaTime);

	glm::vec2 cameraPos;
	m_levelInformation->player->GetPos(cameraPos);
	SetCameraPos(cameraPos);

	/*if (m_levelInformation->player->IsAlive() == false) {
		float cameraScale;
		GetCameraScale(cameraScale);
		cameraScale += a_deltaTime * 0.25f;
		if (cameraScale > 1.3f) {
			cameraScale = 1.3f;
		}
		SetCameraScale(cameraScale);
	}*/

	// quit our application when escape is pressed
	if ( IsKeyDown(NHTV::KEY_ESCAPE) )
		destroy();
}

void Main::onDraw()
{
	clearScreen();

	float fDeltaT = GetDeltaTime();

	glm::vec2 cameraPos;
	m_levelInformation->player->GetPos(cameraPos);

	glm::vec2 objectPos;
	glm::vec2 objectScale;
	glm::vec2 objectOrigin;
	GameObject* gameObject = NULL;
	for (int i = 0; i < m_levelInformation->walls.size(); i++) {
		gameObject = m_levelInformation->walls[i];
		gameObject->GetPos(objectPos);
		gameObject->GetScale(objectScale);
		gameObject->GetOrigin(objectOrigin);
		if (objectPos.x + objectScale.x*objectOrigin.x > cameraPos.x - m_iScreenWidth * 0.5f && objectPos.x - objectScale.x*(1.0f - objectOrigin.x) < cameraPos.x + m_iScreenWidth * 0.5f && objectPos.y + objectScale.y*objectOrigin.y > cameraPos.y - m_iScreenHeight * 0.5f && objectPos.y - objectScale.y*(1.0f - objectOrigin.y) < cameraPos.y + m_iScreenHeight * 0.5f) {
			gameObject->MarkForDraw();
		}
	}
	for (int i = 0; i < m_levelInformation->backgroundObjects.size(); i++) {
		gameObject = m_levelInformation->backgroundObjects[i];
		gameObject->GetPos(objectPos);
		gameObject->GetScale(objectScale);
		gameObject->GetOrigin(objectOrigin);
		if (objectPos.x + objectScale.x*objectOrigin.x > cameraPos.x - m_iScreenWidth * 0.5f && objectPos.x - objectScale.x*(1.0f - objectOrigin.x) < cameraPos.x + m_iScreenWidth * 0.5f && objectPos.y + objectScale.y*objectOrigin.y > cameraPos.y - m_iScreenHeight * 0.5f && objectPos.y - objectScale.y*(1.0f - objectOrigin.y) < cameraPos.y + m_iScreenHeight * 0.5f) {
			gameObject->MarkForDraw();
		}
	}
	for (int i = 0; i < m_levelInformation->exits.size(); i++) {
		gameObject = m_levelInformation->exits[i];
		gameObject->GetPos(objectPos);
		gameObject->GetScale(objectScale);
		gameObject->GetOrigin(objectOrigin);
		if (objectPos.x + objectScale.x*objectOrigin.x > cameraPos.x - m_iScreenWidth * 0.5f && objectPos.x - objectScale.x*(1.0f - objectOrigin.x) < cameraPos.x + m_iScreenWidth * 0.5f && objectPos.y + objectScale.y*objectOrigin.y > cameraPos.y - m_iScreenHeight * 0.5f && objectPos.y - objectScale.y*(1.0f - objectOrigin.y) < cameraPos.y + m_iScreenHeight * 0.5f) {
			gameObject->MarkForDraw();
		}
	}
	for (int i = 0; i < m_levelInformation->doors.size(); i++) {
		gameObject = m_levelInformation->doors[i];
		gameObject->GetPos(objectPos);
		gameObject->GetScale(objectScale);
		gameObject->GetOrigin(objectOrigin);
		if (objectPos.x + objectScale.x*objectOrigin.x > cameraPos.x - m_iScreenWidth * 0.5f && objectPos.x - objectScale.x*(1.0f - objectOrigin.x) < cameraPos.x + m_iScreenWidth * 0.5f && objectPos.y + objectScale.y*objectOrigin.y > cameraPos.y - m_iScreenHeight * 0.5f && objectPos.y - objectScale.y*(1.0f - objectOrigin.y) < cameraPos.y + m_iScreenHeight * 0.5f) {
			gameObject->MarkForDraw();
		}
	}
	for (int i = 0; i < m_levelInformation->pickups.size(); i++) {
		gameObject = m_levelInformation->pickups[i];
		gameObject->GetPos(objectPos);
		gameObject->GetScale(objectScale);
		gameObject->GetOrigin(objectOrigin);
		if (objectPos.x + objectScale.x*objectOrigin.x > cameraPos.x - m_iScreenWidth * 0.5f && objectPos.x - objectScale.x*(1.0f - objectOrigin.x) < cameraPos.x + m_iScreenWidth * 0.5f && objectPos.y + objectScale.y*objectOrigin.y > cameraPos.y - m_iScreenHeight * 0.5f && objectPos.y - objectScale.y*(1.0f - objectOrigin.y) < cameraPos.y + m_iScreenHeight * 0.5f) {
			gameObject->MarkForDraw();
		}
	}
	for (int i = 0; i < m_levelInformation->enemyGenerators.size(); i++) {
		gameObject = m_levelInformation->enemyGenerators[i];
		gameObject->GetPos(objectPos);
		gameObject->GetScale(objectScale);
		gameObject->GetOrigin(objectOrigin);
		if (objectPos.x + objectScale.x*objectOrigin.x > cameraPos.x - m_iScreenWidth * 0.5f && objectPos.x - objectScale.x*(1.0f - objectOrigin.x) < cameraPos.x + m_iScreenWidth * 0.5f && objectPos.y + objectScale.y*objectOrigin.y > cameraPos.y - m_iScreenHeight * 0.5f && objectPos.y - objectScale.y*(1.0f - objectOrigin.y) < cameraPos.y + m_iScreenHeight * 0.5f) {
			gameObject->MarkForDraw();
		}
	}
	for (int i = 0; i < m_levelInformation->projectiles.size(); i++) {
		gameObject = m_levelInformation->projectiles[i];
		gameObject->GetPos(objectPos);
		gameObject->GetScale(objectScale);
		gameObject->GetOrigin(objectOrigin);
		if (objectPos.x + objectScale.x*objectOrigin.x > cameraPos.x - m_iScreenWidth * 0.5f && objectPos.x - objectScale.x*(1.0f - objectOrigin.x) < cameraPos.x + m_iScreenWidth * 0.5f && objectPos.y + objectScale.y*objectOrigin.y > cameraPos.y - m_iScreenHeight * 0.5f && objectPos.y - objectScale.y*(1.0f - objectOrigin.y) < cameraPos.y + m_iScreenHeight * 0.5f) {
			gameObject->MarkForDraw();
		}
	}
	for (int i = 0; i < m_levelInformation->enemies.size(); i++) {
		gameObject = m_levelInformation->enemies[i];
		gameObject->GetPos(objectPos);
		gameObject->GetScale(objectScale);
		gameObject->GetOrigin(objectOrigin);
		if (objectPos.x + objectScale.x*objectOrigin.x > cameraPos.x - m_iScreenWidth * 0.5f && objectPos.x - objectScale.x*(1.0f - objectOrigin.x) < cameraPos.x + m_iScreenWidth * 0.5f && objectPos.y + objectScale.y*objectOrigin.y > cameraPos.y - m_iScreenHeight * 0.5f && objectPos.y - objectScale.y*(1.0f - objectOrigin.y) < cameraPos.y + m_iScreenHeight * 0.5f) {
			gameObject->MarkForDraw();
		}
	}
	for (int i = 0; i < m_explosions.size(); i++) {
		gameObject = m_explosions[i];
		gameObject->GetPos(objectPos);
		gameObject->GetScale(objectScale);
		gameObject->GetOrigin(objectOrigin);
		if (objectPos.x + objectScale.x*objectOrigin.x > cameraPos.x - m_iScreenWidth * 0.5f && objectPos.x - objectScale.x*(1.0f - objectOrigin.x) < cameraPos.x + m_iScreenWidth * 0.5f && objectPos.y + objectScale.y*objectOrigin.y > cameraPos.y - m_iScreenHeight * 0.5f && objectPos.y - objectScale.y*(1.0f - objectOrigin.y) < cameraPos.y + m_iScreenHeight * 0.5f) {
			gameObject->MarkForDraw();
		}
	}

	m_levelInformation->player->MarkForDraw();
	// Draw ui
	std::vector<GameObject*> items;
	m_levelInformation->player->GetItems(items);
	glm::vec2 pos;
	m_levelInformation->player->GetPos(pos);
	pos.x -= (m_iScreenWidth * 0.5f) - 24.0f;
	pos.y -= m_iScreenHeight * 0.5f - 24.0f;
	for (int i = 0; i < items.size(); i++) {
		items[i]->SetPos(pos);
		items[i]->MarkForDraw();
		pos.x += 40;
	}
}

void Main::onDestroy()
{
	delete m_spriteSheetBackground;
	delete m_spriteSheetEntities;
	delete m_levelLoader;
	delete m_tcm;
}

void Main::UpdatePlayer(float deltaTime)
{
	if (m_levelInformation->player->IsAlive()) {
		// Damage the player over time for a small amount
		m_levelInformation->player->Damage(2.0f*deltaTime);

		// Use a potion if the player has one when the enter key is pressed
		if (IsKeyDown(NHTV::KEY_ENTER)) {
			if (m_enterKey == false) {
				m_enterKey = true;
				if (m_levelInformation->player->UsePotion()) {
					UsePotion();
				}
			}
		}
		else {
			m_enterKey = false;
		}

		glm::vec2 playerVelocity = glm::vec2(0, 0);
		float playerSpeed = 0;
		m_levelInformation->player->GetSpeed(playerSpeed);
		if (IsKeyDown(NHTV::KEY_RIGHT))
		{
			playerVelocity.x += playerSpeed * deltaTime;
		}
		if (IsKeyDown(NHTV::KEY_LEFT))
		{
			playerVelocity.x -= playerSpeed * deltaTime;
		}
		if (IsKeyDown(NHTV::KEY_UP))
		{
			playerVelocity.y += playerSpeed * deltaTime;
		}
		if (IsKeyDown(NHTV::KEY_DOWN))
		{
			playerVelocity.y -= playerSpeed * deltaTime;
		}
		m_levelInformation->player->SetVelocity(playerVelocity);
		m_levelInformation->player->Update(deltaTime);
		m_levelInformation->player->UpdateAnimation();
		if (IsKeyDown(NHTV::KEY_SPACE)) {
			m_levelInformation->player->PlayCurrentAnimation(deltaTime);
			m_levelInformation->player->SetVelocity(glm::vec2(0, 0));
			Projectile* p = m_levelInformation->player->Shoot();
			if (p != NULL) {
				m_levelInformation->projectiles.push_back(p);
			}
		}
		else if (playerVelocity.x != 0 || playerVelocity.y != 0) {
			m_levelInformation->player->PlayCurrentAnimation(deltaTime);
		}
		m_tcm->CheckCollisions(m_levelInformation->player, ObjectType::e_player);
		std::vector<Collider*> playerCollisions;
		m_levelInformation->player->GetCollisions(playerCollisions);
		for (int i = 0; i < playerCollisions.size(); i++) {
			GameObject* gameObject = playerCollisions[i]->GetGameObject();
			unsigned int type_id = 0;
			gameObject->GetType_id(type_id);
			if (type_id == ObjectType::e_generator) {
				float playerDamage = 0;
				m_levelInformation->player->GetDamage(playerDamage);
				EnemyGenerator* eg = static_cast<EnemyGenerator*>(gameObject);
				eg->Damage(playerDamage * deltaTime);
			}
			else if (type_id == ObjectType::e_food) {
				Food* f = static_cast<Food*>(gameObject);
				float healing;
				f->GetHealth(healing);
				m_levelInformation->player->Heal(healing);
				RemoveFromPickups(f);
				DeleteFromTiles(f);
			}
			else if (type_id == ObjectType::e_chest) {
				Chest* c = static_cast<Chest*>(gameObject);
				int points;
				c->GetPoints(points);
				m_levelInformation->player->IncreaseScore(points);
				RemoveFromPickups(c);
				DeleteFromTiles(c);
			}
			else if (type_id == ObjectType::e_potion || type_id == ObjectType::e_key) {
				GameObject* item = new GameObject(0);
				item->Duplicate(gameObject);
				m_levelInformation->player->AddToInventory(item);
				RemoveFromPickups(gameObject);
				DeleteFromTiles(gameObject);
			}
			else if (type_id == ObjectType::e_door && m_levelInformation->player->UseKey()) {
				OpenDoor(gameObject);
			}
			else if (type_id == ObjectType::e_exit) {
				//NextLevel();
				m_nextLevel = true;
				gameObject->GetPos(m_posExit);
				m_levelInformation->player->ClearCollisions();
				return;
			}
		}
		m_levelInformation->player->ClearCollisions();
	}
	// If the player is dead
	else {
		if (m_playerDeathTime == 0.0f) { 
			printf("Restarting please wait\n"); 
		}
		m_levelInformation->player->ShowSkull();
		m_playerDeathTime += deltaTime;
		if (m_playerDeathTime >= 3.0f) {
			Restart();
		}
	}
}

void Main::UpdateProjectiles(float deltaTime)
{
	std::vector<Projectile*>::iterator itProjectiles = m_levelInformation->projectiles.begin();
	while (itProjectiles != m_levelInformation->projectiles.end()) {
		(*itProjectiles)->Update(deltaTime);
		m_tcm->CheckCollisions((*itProjectiles), ObjectType::e_projectile);
		std::vector<Collider*> collisions;
		(*itProjectiles)->GetCollisions(collisions);
		bool markForDestroy = false;
		for (int i = 0; i < collisions.size(); i++) {
			GameObject* collision = collisions[i]->GetGameObject();
			unsigned int type_id;
			collision->GetType_id(type_id);
			unsigned int unique_id = collision->GetUnique_id();
			// Check if the projectile belongs to the player
			if ((*itProjectiles)->GetOwnersUnique_id() == m_levelInformation->player->GetUnique_id() && (type_id == ObjectType::e_generator || type_id == ObjectType::e_being)) {
				// Player projectile hits being
				if (type_id == ObjectType::e_being) {
					Being* b = static_cast<Being*>(collision);
					float projectileDamage = 0;
					(*itProjectiles)->GetDamage(projectileDamage);
					b->Damage(projectileDamage);
					markForDestroy = true;
					// If the being died increase the players score
					if (b->IsAlive() == false) {
						int enemyPoints;
						b->GetPoints(enemyPoints);
						m_levelInformation->player->IncreaseScore(enemyPoints);
					}
					break;
				}
				// Player projectile hits generator
				else if (type_id == ObjectType::e_generator) {
					EnemyGenerator* eg = static_cast<EnemyGenerator*>(collision);
					float projectileDamage = 0;
					(*itProjectiles)->GetDamage(projectileDamage);
					eg->Damage(projectileDamage);
					markForDestroy = true;
					break;
				}
			}
			else if ((type_id == ObjectType::e_being || type_id == ObjectType::e_player) && (*itProjectiles)->GetOwnersUnique_id() != unique_id) {
				Being* b = static_cast<Being*>(collision);
				float projectileDamage = 0;
				(*itProjectiles)->GetDamage(projectileDamage);
				b->Damage(projectileDamage);
				markForDestroy = true;
				break;
			}
			else if ((*itProjectiles)->GetOwnersUnique_id() != unique_id) {
				markForDestroy = true;
				break;
			}
		}
		(*itProjectiles)->ClearCollisions();
		if (markForDestroy) {
			glm::vec2 projectilePos;
			(*itProjectiles)->GetPos(projectilePos);
			AddExplosion(projectilePos, m_explosionProjectileAnimation.Duplicate(), m_spriteSheetEntities->GetSprite(328));
			DeleteFromTiles((*itProjectiles));
			itProjectiles = m_levelInformation->projectiles.erase(itProjectiles);
		}
		else {
			++itProjectiles;
		}
	}
}

void Main::UpdateEnemies(float deltaTime)
{
	std::vector<Being*>::iterator itEnemies = m_levelInformation->enemies.begin();
	while (itEnemies != m_levelInformation->enemies.end()) {
		if ((*itEnemies)->IsAlive() == false) {
			DeleteEnemy((*itEnemies));
			itEnemies = m_levelInformation->enemies.erase(itEnemies);
		}
		else {
			glm::vec2 playerPos;
			m_levelInformation->player->GetPos(playerPos);
			glm::vec2 enemyPos;
			(*itEnemies)->GetPos(enemyPos);
			if (m_levelInformation->player->IsAlive()) {
				(*itEnemies)->Chase(playerPos - enemyPos, deltaTime);
			}
			else {
				(*itEnemies)->Chase((playerPos - enemyPos) * -1, deltaTime);
			}
			(*itEnemies)->Update(deltaTime);
			(*itEnemies)->UpdateAnimation();
			(*itEnemies)->PlayCurrentAnimation(deltaTime);
			m_tcm->CheckCollisions((*itEnemies), ObjectType::e_being);
			std::vector<Collider*> collisions;
			(*itEnemies)->GetCollisions(collisions);
			if (m_levelInformation->player->IsAlive() && (*itEnemies)->IsShooter() && collisions.size() == 0) {
				Projectile* p = (*itEnemies)->Shoot();
				if (p != NULL) {
					m_levelInformation->projectiles.push_back(p);
				}
			}
			bool died = false;
			for (int i = 0; i < collisions.size(); i++) {
				GameObject* collision = collisions[i]->GetGameObject();
				unsigned int type_id;
				collision->GetType_id(type_id);
				if (type_id == ObjectType::e_player)
				{
					// If collding with the player then deal both damage to the player and enemy
					float playerDamage = 0;
					float enemyDamage = 0;
					m_levelInformation->player->GetDamage(playerDamage);
					(*itEnemies)->GetDamage(enemyDamage);
					m_levelInformation->player->Damage(enemyDamage * deltaTime);
					(*itEnemies)->Damage(playerDamage * deltaTime);
					if ((*itEnemies)->IsAlive() == false) {
						int enemyPoints;
						(*itEnemies)->GetPoints(enemyPoints);
						m_levelInformation->player->IncreaseScore(enemyPoints);
						DeleteEnemy((*itEnemies));
						itEnemies = m_levelInformation->enemies.erase(itEnemies);
						died = true;
					}
					break;
				}
			}
			// If the enemy died then do not clear because the enemy is already deleted
			if (died == false) {
				(*itEnemies)->ClearCollisions();
				++itEnemies;
			}
		}
	}
}

void Main::UpdateEnemyGenerators(float deltaTime)
{
	std::vector<EnemyGenerator*>::iterator itEnemyGenerators = m_levelInformation->enemyGenerators.begin();
	while (itEnemyGenerators != m_levelInformation->enemyGenerators.end()) {
		if ((*itEnemyGenerators)->IsAlive()) {
			(*itEnemyGenerators)->Update(deltaTime);
			if ((*itEnemyGenerators)->IsSpawnReady()) {
				// Spawn enemy
				glm::vec2 pos;
				(*itEnemyGenerators)->GetPos(pos);
				int tilePosX = pos.x / m_levelInformation->tileWidth;
				int tilePosY = std::abs((pos.y) / m_levelInformation->tileHeight);

				bool empty = false;
				if (m_tcm->IsTileEmpty(tilePosX - 1, tilePosY - 1)) {
					empty = true;
					pos.x -= m_levelInformation->tileWidth;
					pos.y += m_levelInformation->tileHeight;
				}
				else if (m_tcm->IsTileEmpty(tilePosX - 1, tilePosY + 1)) {
					empty = true;
					pos.x -= m_levelInformation->tileWidth;
					pos.y -= m_levelInformation->tileHeight;
				}
				else if (m_tcm->IsTileEmpty(tilePosX - 1, tilePosY)) {
					empty = true;
					pos.x -= m_levelInformation->tileWidth;
				}
				else if (m_tcm->IsTileEmpty(tilePosX + 1, tilePosY - 1)) {
					empty = true;
					pos.x += m_levelInformation->tileWidth;
					pos.y += m_levelInformation->tileHeight;
				}
				else if (m_tcm->IsTileEmpty(tilePosX + 1, tilePosY + 1)) {
					empty = true;
					pos.x += m_levelInformation->tileWidth;
					pos.y -= m_levelInformation->tileHeight;
				}
				else if (m_tcm->IsTileEmpty(tilePosX + 1, tilePosY)) {
					empty = true;
					pos.x += m_levelInformation->tileWidth;
				}
				else if (m_tcm->IsTileEmpty(tilePosX, tilePosY - 1)) {
					empty = true;
					pos.y += m_levelInformation->tileHeight;
				}
				else if (m_tcm->IsTileEmpty(tilePosX, tilePosY + 1)) {
					empty = true;
					pos.y -= m_levelInformation->tileHeight;
				}

				if (empty) {
					Being* b = (*itEnemyGenerators)->GetNewEnemy(m_spriteSheetEntities);
					b->SetPos(pos);
					m_levelInformation->enemies.push_back(b);

				}
			}
			++itEnemyGenerators;
		}
		else {
			glm::vec2 enemyGeneratorPos;
			(*itEnemyGenerators)->GetPos(enemyGeneratorPos);
			AddExplosion(enemyGeneratorPos, m_explosionEnemyAnimation.Duplicate(), m_spriteSheetEntities->GetSprite(334));
			DeleteFromTiles((*itEnemyGenerators));
			itEnemyGenerators = m_levelInformation->enemyGenerators.erase(itEnemyGenerators);
		}
	}
}

void Main::UpdateExplosions(float deltaTime)
{
	std::vector<Explosion*>::iterator itExplosions = m_explosions.begin();
	while (itExplosions != m_explosions.end()) {
		(*itExplosions)->UpdateAnimation(deltaTime);
		if ((*itExplosions)->IsAlive() == false) {
			delete (*itExplosions);
			itExplosions = m_explosions.erase(itExplosions);
		}
		else {
			++itExplosions;
		}
	}
}

void Main::AddExplosion(const glm::vec2& pos, Animation animation, unsigned int sprite_id)
{
	Explosion* e = new Explosion(animation, m_spriteSheetEntities->GetSprite(sprite_id));
	e->SetPos(pos);
	m_explosions.push_back(e);
}

void Main::UsePotion()
{
	glm::vec2 objectPos;
	glm::vec2 objectScale;
	glm::vec2 objectOrigin;
	glm::vec2 cameraPos;
	m_levelInformation->player->GetPos(cameraPos);
	std::vector<Being*>::iterator it = m_levelInformation->enemies.begin();
	while (it != m_levelInformation->enemies.end()) {
		(*it)->GetPos(objectPos);
		(*it)->GetScale(objectScale);
		(*it)->GetOrigin(objectOrigin);
		if (objectPos.x + objectScale.x*objectOrigin.x > cameraPos.x - m_iScreenWidth * 0.5f && objectPos.x - objectScale.x*(1.0f - objectOrigin.x) < cameraPos.x + m_iScreenWidth * 0.5f && objectPos.y + objectScale.y*objectOrigin.y > cameraPos.y - m_iScreenHeight * 0.5f && objectPos.y - objectScale.y*(1.0f - objectOrigin.y) < cameraPos.y + m_iScreenHeight * 0.5f) {
			DeleteEnemy((*it));
			it = m_levelInformation->enemies.erase(it);
		}
		else {
			++it;
		}
	}
}

void Main::OpenDoor(GameObject * door)
{
	glm::vec2 doorPos;
	door->GetPos(doorPos);
	int xTile = doorPos.x / m_levelInformation->tileWidth;
	int yTile = std::abs((doorPos.y) / m_levelInformation->tileHeight);
	// Delete the door
	for (int i = 0; i < m_levelInformation->doors.size(); i++) {
		if (m_levelInformation->doors[i] == door) {
			m_levelInformation->doors.erase(m_levelInformation->doors.begin() + i);
			DeleteFromTiles(door);
		}
	}
	// Look for neighbouring doors that also need to be opened
	for (int i = 0; i < m_levelInformation->tiles[yTile][xTile + 1].objects.size(); i++) {
		if (m_levelInformation->tiles[yTile][xTile + 1].objects[i].type == ObjectType::e_door) {
			OpenDoor(m_levelInformation->tiles[yTile][xTile + 1].objects[i].gameObject);
		}
	}
	for (int i = 0; i < m_levelInformation->tiles[yTile][xTile - 1].objects.size(); i++) {
		if (m_levelInformation->tiles[yTile][xTile - 1].objects[i].type == ObjectType::e_door) {
			OpenDoor(m_levelInformation->tiles[yTile][xTile - 1].objects[i].gameObject);
		}
	}
	for (int i = 0; i < m_levelInformation->tiles[yTile + 1][xTile].objects.size(); i++) {
		if (m_levelInformation->tiles[yTile + 1][xTile].objects[i].type == ObjectType::e_door) {
			OpenDoor(m_levelInformation->tiles[yTile + 1][xTile].objects[i].gameObject);
		}
	}
	for (int i = 0; i < m_levelInformation->tiles[yTile - 1][xTile].objects.size(); i++) {
		if (m_levelInformation->tiles[yTile - 1][xTile].objects[i].type == ObjectType::e_door) {
			OpenDoor(m_levelInformation->tiles[yTile - 1][xTile].objects[i].gameObject);
		}
	}
}

void Main::DeleteFromTiles(GameObject * gameObject)
{
	m_tcm->RemoveObjectFromAllTiles(gameObject);
	delete gameObject;
}

void Main::DeleteEnemy(Being * enemy)
{
	glm::vec2 enemyPos;
	enemy->GetPos(enemyPos);
	AddExplosion(enemyPos, m_explosionEnemyAnimation.Duplicate(), m_spriteSheetEntities->GetSprite(334));
	DeleteFromTiles(enemy);
}

void Main::RemoveFromPickups(GameObject * gameObject)
{
	for (int i = 0; i < m_levelInformation->pickups.size(); i++) {
		if (m_levelInformation->pickups[i] == gameObject) {
			m_levelInformation->pickups.erase(m_levelInformation->pickups.begin() + i);
			return;
		}
	}
}

void Main::NextLevel()
{
	m_nextLevel = false;
	m_nextLevelTimer = 0.0f;
	m_levelInformation->player->SetRot(0);
	m_levelInformation->player->SetScale(glm::vec2(m_levelInformation->tileWidth, m_levelInformation->tileHeight));

	m_currentLevel++;
	if (m_currentLevel > m_lastLevel) {
		m_currentLevel = 1;
	}
	m_levelLoader->DeleteCurrentLoadedLevel();
	std::string levelName = "Gauntlet_levels/level";
	levelName += std::to_string(m_currentLevel);
	levelName += ".tmx";
	m_levelLoader->LoadLevelData(levelName.c_str(), m_spriteSheetBackground, m_spriteSheetEntities);
	m_levelInformation->player->Heal(100.0f);
}

void Main::Restart()
{
	// Restart
	m_levelInformation->player->ClearInventory();
	m_levelInformation->player->ClearScore();
	m_playerDeathTime = 0.0f;
	m_levelInformation->player->Heal(500.0f);
	m_levelLoader->DeleteCurrentLoadedLevel();
	std::string levelName = "Gauntlet_levels/level";
	levelName += std::to_string(m_currentLevel);
	levelName += ".tmx";
	m_levelLoader->LoadLevelData(levelName.c_str(), m_spriteSheetBackground, m_spriteSheetEntities);
	// Play the animation so that the player does no longer look like a skull
	m_levelInformation->player->PlayCurrentAnimation(0.0f);
}