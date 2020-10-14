#include "GameObject.h"

unsigned int GameObject::unique_id = 0;

GameObject::GameObject(unsigned int sprite_id)
{
	m_sprite_id = sprite_id;
	m_name = "";
	m_type_id = 0;
	m_rotation = 0.0f;
	m_alive = true;
	m_collider = new Collider(this);
	m_colliderMatchesSprite = true;
	m_velocity = glm::vec2(0, 0);

	m_unique_id = GameObject::unique_id;
	GameObject::unique_id++;
}

void GameObject::Duplicate(const GameObject* gameObject)
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	m_sprite_id = spriteManager->DuplicateSprite(gameObject->m_sprite_id);
	m_type_id = gameObject->m_type_id;
	m_name = gameObject->m_name;
	m_rotation = gameObject->m_rotation;
	m_alive = gameObject->m_alive;
	m_velocity = gameObject->m_velocity;
	m_collider->Duplicate(gameObject->m_collider);
	m_colliderMatchesSprite = gameObject->m_colliderMatchesSprite;

	m_unique_id = GameObject::unique_id;
	GameObject::unique_id++;
}

GameObject::~GameObject()
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->RemoveSprite(m_sprite_id);
	delete m_collider;
}

void GameObject::GetPos(glm::vec2& pos) const
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->GetSpritePosition(m_sprite_id, pos);
}

void GameObject::SetPos(const glm::vec2& pos)
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	float newPos[2]{
		pos[0],
		pos[1]
	};
	spriteManager->MoveSprite(m_sprite_id, newPos);
}

void GameObject::GetVelocity(glm::vec2& velocity) const
{
	velocity = m_velocity;
}

void GameObject::SetVelocity(const glm::vec2& velocity)
{
	m_velocity = velocity;
	m_collider->SetVelocity(m_velocity);
}

void GameObject::GetRot(float& rotation) const
{
	rotation = m_rotation;
}

void GameObject::SetRot(const float& rotation)
{
	float diff = rotation - m_rotation;
	m_rotation = rotation;
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->RotateSprite(m_sprite_id, diff);
}

void GameObject::GetScale(glm::vec2& scale) const
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->GetSpriteScale(m_sprite_id, scale);
}

void GameObject::SetScale(glm::vec2& scale)
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->SetSpriteScale(m_sprite_id, scale);
	if (m_colliderMatchesSprite) {
		m_collider->SetScale(scale);
	}
}

void GameObject::GetOrigin(glm::vec2& origin) const
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->GetSpriteOrigin(m_sprite_id, origin);
}

void GameObject::SetOrigin(glm::vec2& origin)
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->SetSpriteOrigin(m_sprite_id, origin);
	if (m_colliderMatchesSprite) {
		m_collider->SetOrigin(origin);
	}
}

void GameObject::GetUvCoords(glm::vec2& minUvCoords, glm::vec2& maxUvCoords) const
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->GetSpriteUVCoordinates(m_sprite_id, minUvCoords, maxUvCoords);
}

void GameObject::SetUvCoords(glm::vec2& minUvCoords, glm::vec2& maxUvCoords)
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->SetSpriteUVCoordinates(m_sprite_id, minUvCoords, maxUvCoords);
}

void GameObject::GetColour(glm::vec4& colour) const
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->GetSpriteColour(m_sprite_id, colour);
}

void GameObject::SetColour(glm::vec4& colour)
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	spriteManager->SetSpriteColour(m_sprite_id, colour);
}

void GameObject::GetSprite_id(unsigned int& sprite_id) const
{
	sprite_id = m_sprite_id;
}

void GameObject::SetSprite_id(const unsigned int& sprite_id)
{
	m_sprite_id = sprite_id;
}

void GameObject::GetName(std::string & name) const
{
	name = m_name;
}

void GameObject::SetName(const std::string & name)
{
	m_name = name;
}

bool GameObject::DoesColliderMatchSprite()
{
	return m_colliderMatchesSprite;
}

void GameObject::SetColliderMatchesSprite(const bool& matches)
{
	m_colliderMatchesSprite = matches;
}

void GameObject::ApplyPhysics()
{
	SetPos(m_collider->ApplyPhysics());
}

void GameObject::GetCollisions(std::vector<Collider*>& colliders) const
{
	m_collider->GetCollisions(colliders);
}

void GameObject::AddCollision(GameObject* gameObject)
{
	m_collider->AddCollision(gameObject);
}

void GameObject::AddCollision(Collider * collider)
{
	m_collider->AddCollision(collider);
}

void GameObject::ClearCollisions()
{
	m_collider->ClearCollisions();
}

void GameObject::GetType_id(unsigned int& type_id) const
{
	type_id = m_type_id;
}

void GameObject::SetType_id(const unsigned int& type_id)
{
	m_type_id = type_id;
}

bool GameObject::IsAlive()
{
	return m_alive;
}

void GameObject::SetAlive(const bool& alive)
{
	this->m_alive = alive;
}

void GameObject::Update(float deltaTime)
{
}

Collider * GameObject::GetCollider()
{
	return m_collider;
}

bool GameObject::IsCollidingWith(GameObject* gameObject)
{
	return m_collider->IsCollidingWith(gameObject->GetCollider());
}

void GameObject::MarkForDraw()
{
	if (m_alive) {
		NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
		spriteManager->MarkSpriteForDraw(m_sprite_id);
	}
}

unsigned int GameObject::GetUnique_id()
{
	return m_unique_id;
}
