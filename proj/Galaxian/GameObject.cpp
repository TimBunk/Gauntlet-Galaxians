#include "GameObject.h"

GameObject::GameObject(unsigned int sprite_id)
{
	this->sprite_id = sprite_id;
	rotation = 0.0f;
	alive = true;
}

GameObject::~GameObject()
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->RemoveSprite(sprite_id);
}

glm::vec2 GameObject::GetPos()
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	glm::vec2 pos;
	spriteManager->GetSpritePosition(sprite_id, pos);
	return pos;
}

void GameObject::SetPos(glm::vec2 pos)
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	float newPos[2]{
		pos[0],
		pos[1]
	};
	spriteManager->MoveSprite(sprite_id, newPos);
}

float GameObject::GetAngle()
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	float angle = 0.0f;
	spriteManager->GetAngle(sprite_id, angle);
	return angle;
}

float GameObject::GetRot()
{
	return rotation;
}

void GameObject::SetRot(float rot)
{
	float diff = rot - rotation;
	rotation = rot;
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->RotateSprite(sprite_id, diff);
}

glm::vec2 GameObject::GetScale()
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	glm::vec2 scale;
	spriteManager->GetSpriteScale(sprite_id, scale);
	return scale;
}

void GameObject::SetScale(glm::vec2 scale)
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->SetSpriteScale(sprite_id, scale);
}

glm::vec2 GameObject::GetOrigin()
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	glm::vec2 origin;
	spriteManager->GetSpriteOrigin(sprite_id, origin);
	return origin;
}

void GameObject::SetOrigin(glm::vec2 origin)
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->SetSpriteOrigin(sprite_id, origin);
}

glm::vec4 GameObject::GetUvCoords()
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	glm::vec2 minUvCoords;
	glm::vec2 maxUvCoords;
	spriteManager->GetSpriteUVCoordinates(sprite_id, minUvCoords, maxUvCoords);
	return glm::vec4(minUvCoords.x, minUvCoords.y, maxUvCoords.x, maxUvCoords.y);
}

void GameObject::SetUvCoords(glm::vec4 uv)
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	glm::vec2 minUvCoords = glm::vec2(uv.x, uv.y);
	glm::vec2 maxUvCoords = glm::vec2(uv.z, uv.w);
	spriteManager->SetSpriteUVCoordinates(sprite_id, minUvCoords, maxUvCoords);
}

unsigned int GameObject::GetSprite_id()
{
	return sprite_id;
}

void GameObject::SetSprite_id(unsigned int sprite_id)
{
	this->sprite_id = sprite_id;
}

const NHTV::Vertex* GameObject::GetVertexData()
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	return spriteManager->GetVertexData(sprite_id);;
}

bool GameObject::IsAlive()
{
	return alive;
}

void GameObject::SetAlive(bool alive)
{
	this->alive = alive;
}

void GameObject::Update(float deltaTime)
{
}

bool GameObject::IsCollidingWith(GameObject * gameObject)
{
	glm::vec2 pos = GetPos();
	glm::vec2 origin = GetOrigin();
	glm::vec2 scale = GetScale();
	glm::vec2 topLeft = glm::vec2(pos.x - (origin.x * scale.x), pos.y + ((1.0f - origin.y) * scale.y));
	glm::vec2 bottomRight = glm::vec2(pos.x + ((1.0f - origin.x) * scale.x), pos.y - (origin.x * scale.y));

	glm::vec2 gameObjectPos = gameObject->GetPos();
	glm::vec2 gameObjectOrigin = gameObject->GetOrigin();
	glm::vec2 gameObjectScale = gameObject->GetScale();
	glm::vec2 gameObjectTopLeft = glm::vec2(gameObjectPos.x - (gameObjectOrigin.x * gameObjectScale.x), gameObjectPos.y + ((1.0f - gameObjectOrigin.y) * gameObjectScale.y));
	glm::vec2 gameObjectBottomRight = glm::vec2(gameObjectPos.x + ((1.0f - gameObjectOrigin.x) * gameObjectScale.x), gameObjectPos.y - (gameObjectOrigin.y * gameObjectScale.y));

	bool xCollision = bottomRight.x >= gameObjectTopLeft.x && gameObjectBottomRight.x >= topLeft.x;
	bool yCollision = bottomRight.y <= gameObjectTopLeft.y && gameObjectBottomRight.y <= topLeft.y;

	return xCollision && yCollision;
}

void GameObject::MarkForDraw()
{
	if (alive) {
		NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
		spriteManager->MarkSpriteForDraw(sprite_id);
	}
}
