#pragma once

#include "SpriteManager.h"
#include "Collider.h"

class GameObject
{
public:
	GameObject(unsigned int sprite_id);
	void Duplicate(const GameObject* gameObject);
	virtual ~GameObject();

	void GetPos(glm::vec2& pos) const;
	void SetPos(const glm::vec2& pos);
	void GetVelocity(glm::vec2& velocity) const;
	void SetVelocity(const glm::vec2& velocity);
	void GetRot(float& rotation) const;
	void SetRot(const float& rotation);
	void GetScale(glm::vec2& scale) const;
	void SetScale(glm::vec2& scale);
	void GetOrigin(glm::vec2& origin) const;
	void SetOrigin(glm::vec2& origin);
	void GetUvCoords(glm::vec2& minUvCoords, glm::vec2& maxUvCoords) const;
	void SetUvCoords(glm::vec2& minUvCoords, glm::vec2& maxUvCoords);
	void GetColour(glm::vec4& colour) const;
	void SetColour(glm::vec4& colour);
	void GetSprite_id(unsigned int& sprite_id) const;
	void SetSprite_id(const unsigned int& sprite_id);
	void GetName(std::string& name) const;
	void SetName(const std::string& name);
	void SetColliderMatchesSprite(const bool& matches);

	bool DoesColliderMatchSprite();
	void ApplyPhysics();

	void GetCollisions(std::vector<Collider*>& colliders) const;
	void AddCollision(GameObject* gameObject);
	void AddCollision(Collider* collider);
	void ClearCollisions();

	void GetType_id(unsigned int& type_id) const;
	void SetType_id(const unsigned int &type_id);

	virtual bool IsAlive();
	void SetAlive(const bool& alive);

	virtual void Update(float deltaTime);
	Collider* GetCollider();
	bool IsCollidingWith(GameObject* gameObject);
	void MarkForDraw();

	unsigned int GetUnique_id();

private:
	static unsigned int unique_id;
	unsigned int m_unique_id;
	unsigned int m_sprite_id;
	unsigned int m_type_id;
	std::string m_name;
	float m_rotation;
	bool m_alive;

	glm::vec2 m_velocity;
	Collider* m_collider;
	bool m_colliderMatchesSprite;
};