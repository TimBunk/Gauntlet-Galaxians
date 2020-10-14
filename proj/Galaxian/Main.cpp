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

	// Load all of the sprites that we will be using
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();

	glm::vec2 size = glm::vec2(32.0f, 32.0f);
	glm::vec2 origin = glm::vec2(0.5f, 0.5f);
	unsigned int playerSprite = spriteManager->AddSprite("images/shooter.png", size, origin, glm::vec4(0, 0, 1, 1), glm::vec4(1, 1, 1, 1));
	size = glm::vec2(8, 8);
	unsigned int bulletSprite = spriteManager->AddSprite("images/verticalline.png", size, origin, glm::vec4(0, 0, 1, 1), glm::vec4(1, 1, 1, 1));
	bomb1Sprite = spriteManager->AddSprite("images/bomb-1.png", size, origin, glm::vec4(0, 0, 1, 1), glm::vec4(1, 1, 1, 1));
	bomb2Sprite = spriteManager->AddSprite("images/bomb-2.png", size, origin, glm::vec4(0, 0, 1, 1), glm::vec4(1, 1, 1, 1));

	size = glm::vec2(32, 32);
	unsigned int blueAlienSprite = spriteManager->AddSprite("images/galaxianA-1.png", size, origin, glm::vec4(0, 0, 1, 1), glm::vec4(1, 1, 1, 1));
	unsigned int purpleAlienSprite = spriteManager->AddSprite("images/galaxianC-1.png", size, origin, glm::vec4(0, 0, 1, 1), glm::vec4(1, 1, 1, 1));
	unsigned int redAlienSprite = spriteManager->AddSprite("images/galaxianB-1.png", size, origin, glm::vec4(0, 0, 1, 1), glm::vec4(1, 1, 1, 1));
	unsigned int yellowAlienSprite = spriteManager->AddSprite("images/galaxianD-1.png", size, origin, glm::vec4(0, 0, 1, 1), glm::vec4(1, 1, 1, 1));
	unsigned int blueAlien2Sprite = spriteManager->AddSprite("images/galaxianA-2.png", size, origin, glm::vec4(0, 0, 1, 1), glm::vec4(1, 1, 1, 1));
	unsigned int purpleAlien2Sprite = spriteManager->AddSprite("images/galaxianC-2.png", size, origin, glm::vec4(0, 0, 1, 1), glm::vec4(1, 1, 1, 1));
	unsigned int redAlien2Sprite = spriteManager->AddSprite("images/galaxianB-2.png", size, origin, glm::vec4(0, 0, 1, 1), glm::vec4(1, 1, 1, 1));
	unsigned int yellowAlien2Sprite = spriteManager->AddSprite("images/galaxianD-2.png", size, origin, glm::vec4(0, 0, 1, 1), glm::vec4(1, 1, 1, 1));
	// Create the player
	player = new Player(playerSprite, bulletSprite, 200.0f, m_iScreenHeight);
	player->SetPos(glm::vec2(0, -260));
	gameObjects.push_back(player);
	s_key = false;
	// Create the bulletManager
	bulletManager = new BulletManager(bulletSprite, 480.0f);
	// The maximumg left and right of the player and the aliens
	maxLeft = 341.0f - 640;
	maxRight = 939.0f - 640;
	starField = new StarField(bomb1Sprite, maxLeft, maxRight, m_iScreenHeight, 100);

	alienParentPos = glm::vec2(0, 360);
	aliensHorizontalSpeed = 40.0f;
	// Addd aliens
	glm::vec2 offset = glm::vec2(-182.0f, -300);
	// Add the blue aliens
	for (int i = 0; i < 3; i++) {
		std::vector<Alien*> rowBlueAliens;
		offset.x = -182.0f;
		for (int j = 0; j < 10; j++) {
			Alien* alien = new AlienBlue(spriteManager->DuplicateSprite(blueAlienSprite), spriteManager->DuplicateSprite(blueAlien2Sprite), player, &alienParentPos, offset, 420.0f, m_iScreenHeight);
			rowBlueAliens.push_back(alien);
			gameObjects.push_back(alien);
			offset.x += 40.0f;
		}
		offset.y += 40.0f;
		aliens.push_back(rowBlueAliens);
	}
	// Add the purple aliens
	std::vector<Alien*> rowPurpleAliens;
	offset.x = -142.0f;
	for (int i = 0; i < 8; i++) {
		Alien* alien = new AlienPurple(spriteManager->DuplicateSprite(purpleAlienSprite), spriteManager->DuplicateSprite(purpleAlien2Sprite), player, &alienParentPos, offset, 420.0f, m_iScreenHeight);
		rowPurpleAliens.push_back(alien);
		gameObjects.push_back(alien);
		offset.x += 40.0f;
	}
	aliens.push_back(rowPurpleAliens);
	// Add the red aliens
	std::vector<Alien*> rowRedAliens;
	offset.x = -102.0f;
	offset.y += 40.0f;
	for (int i = 0; i < 6; i++) {
		Alien* alien = new AlienRed(spriteManager->DuplicateSprite(redAlienSprite), spriteManager->DuplicateSprite(redAlien2Sprite), player, &alienParentPos, offset, 420.0f, m_iScreenHeight);
		rowRedAliens.push_back(alien);
		gameObjects.push_back(alien);
		offset.x += 40.0f;
	}
	aliens.push_back(rowRedAliens);
	// Add the yellow aliens
	std::vector<Alien*> rowYellowAliens;
	offset.x = -62.0f;
	offset.y += 40.0f;
	int shieldersOffset = 0;
	for (int i = 0; i < 2; i++) {
		std::vector<Alien*> shielders;
		for (int j = 0; j < 3; j++) {
			shielders.push_back(aliens[4][j + shieldersOffset]);
		}
		Alien* alien = new AlienYellow(spriteManager->DuplicateSprite(yellowAlienSprite), spriteManager->DuplicateSprite(yellowAlien2Sprite), shielders, player, &alienParentPos, offset, 420.0f, m_iScreenHeight);
		rowYellowAliens.push_back(alien);
		gameObjects.push_back(alien);
		offset.x += 120.0f;
		shieldersOffset += 3;
	}
	aliens.push_back(rowYellowAliens);

	// Set the attackTime
	attackTimer = 0.0f;
	attackTimeMin = 0.5f;
	attackTimeMax = 1.0f;
	attackTime = ((float)std::rand() / (float)RAND_MAX * (attackTimeMax - attackTimeMin)) + attackTimeMin;

	return true;
}

void Main::onUpdate(float a_deltaTime)
{
	CheckCollision();
	// Move the parent position of the aliens to the left or right
	alienParentPos.x += a_deltaTime * aliensHorizontalSpeed;
	for (int i = 0; i < aliens.size(); i++) {
		for (int j = 0; j < aliens[i].size(); j++) {
			if (aliens[i][j]->IsAlive()) {
				// If the most right or the most left alien reaches the maxLeft or maxRight then let the alienParent position move to the other direction
				if (aliens[i][j]->GetIdlePosition().x > maxRight) {
					aliensHorizontalSpeed *= -1.0f;
					alienParentPos.x -= aliens[i][j]->GetIdlePosition().x - maxRight;
					break;
				}
				else if (aliens[i][j]->GetIdlePosition().x < maxLeft) {
					aliensHorizontalSpeed *= -1.0f;
					alienParentPos.x -= aliens[i][j]->GetIdlePosition().x - maxLeft;
					break;
				}
			}

		}
	}
	// Move the player to the right
	if (player->IsAlive()) {
		if (IsKeyDown(NHTV::KEY_RIGHT))
		{
			glm::vec2 pos = player->GetPos();
			pos.x += player->GetSpeed() * a_deltaTime;
			if (pos.x > maxRight) {
				pos.x = maxRight;
			}
			player->SetPos(pos);
		}
		// Move the player to the left
		if (IsKeyDown(NHTV::KEY_LEFT))
		{
			glm::vec2 pos = player->GetPos();
			pos.x -= player->GetSpeed() * a_deltaTime;
			if (pos.x < maxLeft) {
				pos.x = maxLeft;
			}
			player->SetPos(pos);
		}
	}
	// Check if the s_key is being held down
	bool s_keyDown = IsKeyDown(NHTV::KEY_S);
	// Check if the s_key is being pressed and if so then shoot
	if (s_keyDown && s_key == false) {
		s_key = true;
		player->Shoot();
	}
	else if (s_keyDown == false) {
		s_key = false;
	}
	if (IsKeyDown(NHTV::KEY_SPACE)) {
		Restart();
	}


	// Update all the gameobjects and the bullets
	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->Update(a_deltaTime);
	}
	bulletManager->UpdateBullets(a_deltaTime);
	starField->Update(a_deltaTime);
	// Update the explosions
	std::vector<Explosion*>::iterator it = explosions.begin();
	while (it != explosions.end()) {
		// Delete the explosions if it is not alive anymore otherwise update it
		if ((*it)->IsAlive() == false) {
			delete (*it);
			it = explosions.erase(it);
		}
		else {
			(*it)->Update(a_deltaTime);
			++it;
		}
	}

	// Check if it is time for a alien to attack
	if (player->IsAlive()) {
		attackTimer += a_deltaTime;
		if (attackTimer >= attackTime) {
			attackTimer -= attackTime;
			Attack();
		}
	}

	// quit our application when escape is pressed
	if (IsKeyDown(NHTV::KEY_ESCAPE))
		destroy();
}

void Main::onDraw()
{
	clearScreen();

	float fDeltaT = GetDeltaTime();

	starField->MarkForDraw();
	for (int i = 0; i < explosions.size(); i++) {
		explosions[i]->MarkForDraw();
	}
	// Mark all gameobjects and bullets for draw
	for (int i = 0; i < gameObjects.size(); i++) {
		gameObjects[i]->MarkForDraw();
	}
	bulletManager->MarkBulletsForDraw();

	// Draw the borders of the game
	DrawLine(-m_iScreenWidth * 0.5f, -m_iScreenHeight * 0.5f, m_iScreenWidth * -0.5f, -m_iScreenHeight * 0.5f, NHTV::SColour(0x00, 0xFC, 0x00, 0xFF));
	DrawLine(maxLeft, -m_iScreenHeight * 0.5f, maxLeft, m_iScreenHeight * 0.5f, NHTV::SColour(0x00, 0xFC, 0x00, 0xFF));
	DrawLine(maxRight, -m_iScreenHeight * 0.5f, maxRight, m_iScreenHeight * 0.5f, NHTV::SColour(0x00, 0xFC, 0x00, 0xFF));
}

void Main::onDestroy()
{
	std::vector<GameObject*>::iterator it = gameObjects.begin();
	while (it != gameObjects.end()) {
		delete (*it);
		it = gameObjects.erase(it);
	}
	delete bulletManager;
	delete starField;
	std::vector<Explosion*>::iterator it2 = explosions.begin();
	while (it2 != explosions.end()) {
		delete (*it2);
		it2 = explosions.erase(it2);
	}
}

void Main::CheckCollision()
{
	GameObject* playerBullet = player->GetBullet();
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	if (player->IsAlive()) {
		for (int i = 0; i < aliens.size(); i++) {
			for (int j = 0; j < aliens[i].size(); j++) {
				if (aliens[i][j]->IsAlive()) {
					// Check if the playerbullet is collding with the alien
					if (playerBullet->IsAlive() && playerBullet->IsCollidingWith(aliens[i][j])) {
						// If the alien got hit then kill it and reset the bullet
						aliens[i][j]->SetAlive(false);
						explosions.push_back(new Explosion(spriteManager->DuplicateSprite(bomb1Sprite), spriteManager->DuplicateSprite(bomb2Sprite), aliens[i][j]->GetPos()));
						player->ResetBullet();
					}
					// Check if the alien is collding with the player
					if (aliens[i][j]->IsCollidingWith(player)) {
						// If the alien collided with the player then kill both the player and the alien and reset the bullet
						aliens[i][j]->SetAlive(false);
						player->ResetBullet();
						player->SetAlive(false);
						explosions.push_back(new Explosion(spriteManager->DuplicateSprite(bomb1Sprite), spriteManager->DuplicateSprite(bomb2Sprite), player->GetPos()));
						return;
					}
					// Check if the alien is ready to shoot it's bullet and if so then spawn a new bullet using the bulletmanager
					else if (aliens[i][j]->IsShootReady()) {
						bulletManager->AddBullet(aliens[i][j]->GetPos() + (aliens[i][j]->GetScale().y * 0.5f));
					}
				}
			}
		}
		// Check if a bullet is colldiing with the player
		if (bulletManager->IsBulletCollidingWith(player)) {
			// If the player got hit then reset the bullet and setalive to false
			player->ResetBullet();
			player->SetAlive(false);
			explosions.push_back(new Explosion(spriteManager->DuplicateSprite(bomb1Sprite), spriteManager->DuplicateSprite(bomb2Sprite), player->GetPos()));
		}
		if (player->IsAlive() == false) {
			for (int i = 0; i < aliens.size(); i++) {
				for (int j = 0; j < aliens[i].size(); j++) {
					aliens[i][j]->StopAttacking();
				}
			}
		}
	}
}

void Main::Attack()
{
	std::vector<int> availabelRows;
	int idleAliensAmount = 0;
	int aliveAliensAmount = 0;
	for (int i = 0; i < aliens.size(); i++) {
		bool rowAvailabel = false;
		// Check if the current row of aliens is availabel and if so then add it to the availabelRows
		// A row is avaiable when atleast one alien is alive and it is not currently attacking
		for (int j = 0; j < aliens[i].size(); j++) {
			if (aliens[i][j]->IsAlive()) {
				aliveAliensAmount++;
				if (aliens[i][j]->IsAttacking() == false) {
					// Whenever a alien is alive and is not attacking that means it is idle so we increase the amount of idleAliensAmount
					idleAliensAmount++;
					if (rowAvailabel == false) {
						rowAvailabel = true;
						availabelRows.push_back(i);
					}
				}
			}
		}
	}
	// Change the attack time by a random value between the minimum attack time and the maximum attack time and the amount of idleAliens
	// The less idleAliens there are the faster the attacktimer
	attackTime = ((float)std::rand() / (float)RAND_MAX * (attackTimeMax - attackTimeMin + (idleAliensAmount * 0.1f))) + attackTimeMin;

	if (availabelRows.size() > 0) {
		// Choose a random row
		int row = (float)std::rand() / (float)RAND_MAX * availabelRows.size();
		row = availabelRows[row];
		if (row == 4 || row == 5) {
			// Check if row 5 has any aliens alive and if any of those aliens are attacking
			bool row5Alive = false;
			bool row5Attacking = false;
			for (int i = 0; i < aliens[5].size(); i++) {
				if (aliens[5][i]->IsAlive()) {
					row5Alive = true;
					if (aliens[5][i]->IsAttacking()) {
						row5Attacking = true;
					}
				}
			}
			if (row5Alive && row5Attacking) {
				for (int i = 0; i < availabelRows.size(); i++) {
					// If any of the aliens on row 5 is attacking then choose a random other row that is not 4 or 5
					if (availabelRows[i] == 4 && i != 0) {
						row = ((float)std::rand() / (float)RAND_MAX) * (i - 1);
						row = availabelRows[row];
						break;
					}
					else if (availabelRows[i] == 5 && i != 0) {
						row = ((float)std::rand() / (float)RAND_MAX) * (i - 1);
						row = availabelRows[row];
						break;
					}
				}
			}
			// Make sure that none of the aliens on row 4 go alone when there is a alien alive on row 5
			if (availabelRows[0] == 4 && row == 4 && row5Attacking) {
				return;
			}
			else if (row5Alive) {
				row = 5;
			}
		}
		float random = (float)std::rand() / (float)RAND_MAX;
		// Let the alien on the most left side of the row attack first
		if (random > 0.5f) {
			for (int i = 0; i < aliens[row].size(); i++) {
				// Check which alien is availabel for attacking
				if (aliens[row][i]->IsAlive() && aliens[row][i]->IsAttacking() == false) {
					// Also check if there are less then 10 aliens alive and if so let the alien start raging
					if (aliveAliensAmount < 10) {
						aliens[row][i]->StartRage();
						return;
					}
					aliens[row][i]->StartAttacking();
					return;
				}
			}
		}
		// Let the alien on the most right side of the row attack first
		else {
			for (int i = aliens[row].size() - 1; i >= 0; i--) {
				// Check which alien is availabel for attacking
				if (aliens[row][i]->IsAlive() && aliens[row][i]->IsAttacking() == false) {
					// Also check if there are less then 10 aliens alive and if so let the alien start raging
					if (aliveAliensAmount < 10) {
						aliens[row][i]->StartRage();
						return;
					}
					aliens[row][i]->StartAttacking();
					return;
				}
			}
		}
	}
}

void Main::Restart()
{
	player->SetAlive(true);
	player->SetPos(glm::vec2(0, -260));
	player->ResetBullet();
	bulletManager->DeleteAllBullets();
	for (int i = 0; i < aliens.size(); i++) {
		for (int j = 0; j < aliens[i].size(); j++) {
			aliens[i][j]->Reset();
		}
	}
}
