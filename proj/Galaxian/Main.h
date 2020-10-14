#pragma once

#include "Application.h"
#include <glm/glm.hpp>
#include <iostream>

#include "Player.h"
#include "AlienBlue.h"
#include "AlienPurple.h"
#include "AlienRed.h"
#include "AlienYellow.h"
#include "BulletManager.h"
#include "StarField.h"
#include "Explosion.h"

// Derived application class that wraps up all globals neatly
class Main : public NHTV::Application
{
public:

	Main();
	virtual ~Main();

private:

	virtual bool onCreate(int a_argc, char* a_argv[]);
	virtual void onUpdate(float a_deltaTime);
	virtual void onDraw();
	virtual void onDestroy();

	void CheckCollision();
	void Attack();

	void Restart();

	float attackTimer;
	float attackTimeMin, attackTimeMax;
	float attackTime;

	int m_iScreenWidth;
	int m_iScreenHeight;

	float maxLeft, maxRight;

	std::vector<GameObject*> gameObjects;
	Player* player;
	bool s_key;

	BulletManager* bulletManager;
	StarField* starField;

	std::vector<Explosion*> explosions;
	unsigned int bomb1Sprite, bomb2Sprite;

	std::vector<std::vector<Alien*>> aliens;
	glm::vec2 alienParentPos;
	float aliensHorizontalSpeed;
};