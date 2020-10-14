#include "Alien.h"

Alien::Alien(unsigned int sprite_id1, unsigned int sprite_id2, GameObject* player, glm::vec2* parentPos, glm::vec2 offset, float startShootingPointY, float screenHeight) : GameObject::GameObject(sprite_id1)
{
	this->sprite_id1 = sprite_id1;
	this->sprite_id2 = sprite_id2;
	this->player = player;
	this->parentPos = parentPos;
	this->offset = offset;
	this->startShootingPointY = startShootingPointY;
	this->screenHeight = screenHeight;

	currentAlienState = AlienState::idle;
	newAlienState = AlienState::idle;

	danceOffset = glm::vec2(1.0f, 1.0f);
	attackPos = glm::vec2(0.0f, 0.0f);
	dirPlayer = glm::vec2(0.0f, 0.0f);

	speed = 150.0f;
	accelerationSpeed = 15.0f;
	rotationSpeed = 180.0f;

	velocity = glm::vec2(0.0f, 0.0f);
	startAttackingPos = glm::vec2(0.0f, 0.0f);

	changeSpriteTime = 0.25f;
	timer = (float)std::rand() / (float)RAND_MAX * changeSpriteTime;

	// Set the pos
	glm::vec2 pos = glm::vec2(parentPos->x, parentPos->y);
	pos += offset;
	SetPos(pos);

	if ((float)std::rand() / (float)RAND_MAX > 0.5f) {
		danceOffset.x *= -1;
	}

	shootTimer = 0.0f;
	shootBulletTime = 0.4f;
	shoot = false;
	shootCount = 0;

	leader = NULL;
	offsetLeader = glm::vec2(0.0f, 0.0f);
	leaderAlive = false;
}

Alien::~Alien()
{

}

void Alien::Update(float deltaTime)
{
	switch (currentAlienState)
	{
	case idle:
		Idle(deltaTime);
		// Before diving wait for the sprite_id to be equal to the sprite_id2
		if (newAlienState == AlienState::diving && sprite_id == sprite_id2) {
			currentAlienState = newAlienState;
		}
		else if (newAlienState == AlienState::rage) {
			currentAlienState = AlienState::diving;
		}
		break;
	case diving:
		Diving(deltaTime);
		if (newAlienState == AlienState::attacking) {
			currentAlienState = newAlienState;
			PrepareSpecialMovement();
		}
		else if (newAlienState == AlienState::rage) {
			currentAlienState = newAlienState;
			PrepareSpecialMovement();
		}
		break;
	case attacking:
		Attack(deltaTime);
		if (newAlienState == AlienState::returning) {
			currentAlienState = newAlienState;
		}
		break;
	case returning:
		Returning(deltaTime);
		if (newAlienState == AlienState::idle) {
			currentAlienState = newAlienState;
		}
		break;
	case rage:
		Attack(deltaTime);
		break;
	}
}

void Alien::Idle(float deltaTime)
{
	// Set position
	glm::vec2 pos = GetIdlePosition();
	if (sprite_id == sprite_id1) {
		pos += danceOffset;
	}
	SetPos(pos);
	ChangeSprite(deltaTime);
}

void Alien::Diving(float deltaTime)
{
	float rot = 0.0f;
	// If there is a leader then copy the rotation and positon of the leader. The positon has a offset from the leader
	if (leader != NULL && leaderAlive && leader->IsAttacking()) {
		rot = leader->GetRot();
		SetRot(rot);
		glm::vec2 pos = leader->GetPos();
		pos += offsetLeader;
		SetPos(pos);
		velocity = glm::vec2(0.0f, 0.0f);
	}
	else {
		// Start rotating and set the velocity towards that rotatiion
		rot = GetRot();
		if (offset.x < 0.0f) {
			rot += rotationSpeed * deltaTime;
		}
		else {
			rot -= rotationSpeed * deltaTime;
		}
		SetRot(rot);
		float rad = glm::radians(rot + 90.0f);
		velocity = glm::vec2(glm::cos(rad), glm::sin(rad));
		velocity = glm::normalize(velocity);
		velocity *= speed;
		glm::vec2 pos = GetPos();
		pos += (velocity * deltaTime);
		SetPos(pos);
	}
	// If rotated 180 degrees then start preparing the special movement
	if (rot >= 180.0f || rot <= -180.0f) {
		glm::vec2 playerPos = player->GetPos();
		attackPos = glm::vec2(playerPos.x, playerPos.y);
		dirPlayer = glm::normalize(attackPos - GetPos());
		SetRot(-180.0f);
		if (newAlienState != AlienState::rage) {
			newAlienState = AlienState::attacking;
		}
	}
}

void Alien::Attack(float deltaTime)
{
	glm::vec2 pos = GetPos();
	// If this alien has a leader and it is alive then follow it else doSpecialMovement
	if (leader != NULL && leaderAlive) {
		velocity = glm::vec2(0.0f, 0.0f);
		pos = leader->GetPos();
		pos += offsetLeader;
	}
	else {
		DoSpecialMovement();
	}
	// Rotate the alien towards the player
	RotateTowardsPlayer(deltaTime);
	// If the alien goes past the shootingPoint then it will start shooting up to 3 times
	if (pos.y <= startShootingPointY) {
		if (shootCount == 0) { shoot = true; shootCount++; }
		shootTimer += deltaTime;
		if (shootCount < 3 && shootTimer >= shootBulletTime) {
			shootTimer -= shootBulletTime;
			shootCount++;
			shoot = true;
		}
	}
	// Add the velocity to the position and set the position
	pos += (velocity * deltaTime);
	SetPos(pos);
	// If the alien movies out of the screen then set it's position to the top of the screen
	if (pos.y < (GetScale().y * -0.5f) - screenHeight * 0.5f) {
		pos.y = screenHeight *0.5f + (GetScale().y * 0.5f);
		SetPos(pos);
		// If rage is true then do not return the alien to its original position but instead continue attacking
		if (currentAlienState == AlienState::rage) {
			if (leader != NULL) {
				leaderAlive = leader->IsAlive();
			}
			shootCount = 0;
			shootTimer = 0.0f;
			pos.x = offset.x + parentPos->x;
			glm::vec2 playerPos = player->GetPos();
			attackPos = glm::vec2(playerPos.x, playerPos.y);
			dirPlayer = glm::normalize(attackPos - pos);
			SetPos(pos);
			SetRot(-180.0f);
			PrepareSpecialMovement();
			return;
		}
		SetRot(180.0f);
		newAlienState = AlienState::returning;
	}
}

void Alien::Returning(float deltaTime)
{
	glm::vec2 pos = GetPos();
	pos.x = parentPos->x + offset.x;
	velocity = glm::vec2(0.0f, -speed * deltaTime);
	float targetY = parentPos->y + offset.y;
	pos += velocity;
	float beginRotatingPointY = GetScale().y * 2.0f;
	if (pos.y <= targetY + beginRotatingPointY) {
		float diff = 0.0f;
		if (offset.x > 0.0f) {
			diff = (targetY + beginRotatingPointY) - pos.y;
		}
		else {
			diff = pos.y - (targetY + beginRotatingPointY);
		}
		float percentage = diff / beginRotatingPointY;
		SetRot(percentage * 180.0f + 180.0f);
	}
	if (pos.y <= targetY) {
		newAlienState = AlienState::idle;
		SetRot(0.0f);
		shootCount = 0;
		shootTimer = 0.0f;
	}
	SetPos(pos);
}

void Alien::PrepareSpecialMovement()
{

}

void Alien::DoSpecialMovement()
{
}

void Alien::ChangeSprite(float deltaTime)
{
	timer += deltaTime;
	// Change the sprite whenver the timer is greater then the changeSpriteTime
	if (timer >= changeSpriteTime) {
		timer -= changeSpriteTime;
		if (sprite_id == sprite_id1) {
			glm::vec2 pos = glm::vec2(parentPos->x, parentPos->y);
			pos += offset;
			sprite_id = sprite_id2;
			SetPos(pos);
		}
		else {
			glm::vec2 pos = glm::vec2(parentPos->x, parentPos->y);
			pos += offset;
			sprite_id = sprite_id1;
			danceOffset.x *= -1;
			pos += danceOffset;
			SetPos(pos);
		}
	}
}

void Alien::StartAttacking()
{
	newAlienState = AlienState::diving;
	startAttackingPos = GetPos();
	if (leader != NULL) {
		leaderAlive = leader->IsAlive();
	}
}

void Alien::StartRage()
{
	newAlienState = AlienState::rage;
	startAttackingPos = GetPos();
	if (leader != NULL) {
		leaderAlive = leader->IsAlive();
	}
}

void Alien::StopAttacking()
{
	if (currentAlienState == AlienState::rage || newAlienState == AlienState::rage) {
		currentAlienState = AlienState::attacking;
		newAlienState = AlienState::attacking;
	}
}

void Alien::RotateTowardsPlayer(float deltaTime)
{
	glm::vec2 pos = GetPos();
	glm::vec2 playerPos = player->GetPos();
	if (pos.y > playerPos.y) {
		glm::vec2 diff = playerPos - pos;

		float degrees = glm::degrees(std::atan2(diff.y, diff.x));
		degrees = glm::clamp(degrees, -180.0f, 0.0f);
		if (degrees < -180.0f) { degrees = -180.0f; }
		else if (degrees > 0.0f) { degrees = 0.0f; }

		float currentRot = GetRot();
		float diffRot = currentRot - (degrees - 90.0f);

		float rotSpeed = rotationSpeed * deltaTime;
		if (glm::abs(rotSpeed) > glm::abs(diffRot)) {
			rotSpeed = glm::abs(diffRot);
		}
		if (diffRot > 0.0f) {
			SetRot(currentRot - rotSpeed);
		}
		else if (diffRot < 0.0f) {
			SetRot(currentRot + rotSpeed);
		}
	}
}

bool Alien::IsShootReady()
{
	if (shoot) {
		shoot = false;
		return true;
	}
	return false;
}

bool Alien::IsAttacking()
{
	if (currentAlienState != AlienState::idle) {
		return true;
	}
	return false;
}

void Alien::Follow(Alien * leader)
{
	this->leader = leader;
	offsetLeader = GetPos() - leader->GetPos();
	leaderAlive = leader->IsAlive();
}

glm::vec2 Alien::GetIdlePosition()
{
	glm::vec2 idlePos;
	idlePos.x = parentPos->x;
	idlePos.y = parentPos->y;
	idlePos += offset;
	return idlePos;
}

void Alien::Reset()
{
	currentAlienState = AlienState::idle;
	newAlienState = AlienState::idle;
	glm::vec2 pos = glm::vec2(parentPos->x, parentPos->y);
	pos += offset;
	SetPos(pos);
	SetRot(0);
	shootCount = 0;
	shootTimer = 0.0f;
	SetAlive(true);
}
