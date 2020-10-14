#pragma once

#include <glm/vec2.hpp>
#include <vector>

class GameObject;

class Collider
{
public:
	Collider(GameObject* gameObject);
	Collider();
	void Duplicate(const Collider* collider);
	~Collider();

	glm::vec2 ApplyPhysics();
	bool IsCollidingWith(GameObject* gameObject);
	bool IsCollidingWith(Collider* collider);
	void AddCollision(GameObject* gameObject);
	void AddCollision(Collider* collider);
	void GetCollisions(std::vector<Collider*>& colliders) const;
	void ClearCollisions();

	GameObject* GetGameObject();

	void GetLocalPos(glm::vec2& pos) const;
	void SetLocalPos(const glm::vec2& pos);
	void GetOrigin(glm::vec2& origin) const;
	void SetOrigin(const glm::vec2& origin);
	void GetScale(glm::vec2& scale) const;
	void SetScale(const glm::vec2& scale);
	bool IsKinematic();
	void SetKinematic(const bool& kinematic);
	bool IsSensor();
	void SetSensor(const bool& sensor);
	void GetVelocity(glm::vec2& velocity) const;
	void SetVelocity(const glm::vec2& velocity);

private:
	GameObject* m_gameObject;
	glm::vec2 m_velocity;
	glm::vec2 m_localPosition;
	glm::vec2 m_origin;
	glm::vec2 m_scale;
	std::vector<Collider*> m_collisions;
	bool m_kinematic;
	bool m_sensor;
};