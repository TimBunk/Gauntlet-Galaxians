#include "Collider.h"
#include "utilities.h"
#include "GameObject.h"

Collider::Collider(GameObject * parent)
{
	m_gameObject = parent;
	m_localPosition = glm::vec2(0,0);
	parent->GetOrigin(m_origin);
	parent->GetScale(m_scale);
	m_velocity = glm::vec2(0, 0);

	m_kinematic = false;
	m_sensor = false;
}

Collider::Collider()
{
	m_gameObject = NULL;
	m_localPosition = glm::vec2(0, 0);
	m_origin = glm::vec2(0.5f, 0.5f);
	m_scale = glm::vec2(0, 0);
	m_velocity = glm::vec2(0, 0);

	m_kinematic = false;
	m_sensor = false;
}

void Collider::Duplicate(const Collider* collider)
{
	m_velocity = collider->m_velocity;
	m_localPosition = collider->m_localPosition;
	m_origin = collider->m_origin;
	m_scale = collider->m_scale;
	m_collisions = collider->m_collisions;
	m_kinematic = collider->m_kinematic;
	m_sensor = collider->m_sensor;
}

Collider::~Collider()
{
}

glm::vec2 Collider::ApplyPhysics()
{
	if (m_gameObject != NULL) {
		glm::vec2 gameObjectPos;
		m_gameObject->GetPos(gameObjectPos);
		return (gameObjectPos + m_velocity);
	}
	return (m_localPosition + m_velocity);
}

bool Collider::IsCollidingWith(GameObject * gameObject)
{
	return IsCollidingWith(gameObject->GetCollider());
}

bool Collider::IsCollidingWith(Collider * collider)
{
	glm::vec2 newPos = m_localPosition + m_velocity;
	if (m_gameObject != NULL) {
		glm::vec2 gameObjectPos;
		m_gameObject->GetPos(gameObjectPos);
		newPos += gameObjectPos;
	}
	glm::vec2 topLeft = glm::vec2(newPos.x - (m_origin.x * m_scale.x), newPos.y + ((1.0f - m_origin.y) * m_scale.y));
	glm::vec2 bottomRight = glm::vec2(newPos.x + ((1.0f - m_origin.x) * m_scale.x), newPos.y - (m_origin.x * m_scale.y));

	glm::vec2 gameObjectPos = collider->m_localPosition;
	if (collider->m_gameObject != NULL) {
		glm::vec2 colliderGameObjectPos;
		collider->m_gameObject->GetPos(colliderGameObjectPos);
		gameObjectPos += colliderGameObjectPos;
	}
	glm::vec2 gameObjectOrigin = collider->m_origin;
	glm::vec2 gameObjectScale = collider->m_scale;
	glm::vec2 gameObjectTopLeft = glm::vec2(gameObjectPos.x - (gameObjectOrigin.x * gameObjectScale.x), gameObjectPos.y + ((1.0f - gameObjectOrigin.y) * gameObjectScale.y));
	glm::vec2 gameObjectBottomRight = glm::vec2(gameObjectPos.x + ((1.0f - gameObjectOrigin.x) * gameObjectScale.x), gameObjectPos.y - (gameObjectOrigin.y * gameObjectScale.y));

	bool xCollision = bottomRight.x > gameObjectTopLeft.x && gameObjectBottomRight.x > topLeft.x;
	bool yCollision = bottomRight.y < gameObjectTopLeft.y && gameObjectBottomRight.y < topLeft.y;

	//return xCollision && yCollision;

	if (collider->IsSensor() || m_kinematic) {
		return xCollision && yCollision;
	}
	if (xCollision && yCollision) {
		glm::vec2 pos = m_localPosition;
		if (m_gameObject != NULL) {
			glm::vec2 gameObjectPos;
			m_gameObject->GetPos(gameObjectPos);
			pos += gameObjectPos;
		}
		glm::vec2 oldTopLeft = glm::vec2(pos.x - (m_origin.x * m_scale.x), pos.y + ((1.0f - m_origin.y) * m_scale.y));
		glm::vec2 oldBottomRight = glm::vec2(pos.x + ((1.0f - m_origin.x) * m_scale.x), pos.y - (m_origin.x * m_scale.y));
		bool xCollisionBefore = oldBottomRight.x > gameObjectTopLeft.x && gameObjectBottomRight.x > oldTopLeft.x;
		bool yCollisionBefore = oldBottomRight.y < gameObjectTopLeft.y && gameObjectBottomRight.y < oldTopLeft.y;

		glm::vec2 diff = glm::vec2(topLeft.x + m_velocity.x - gameObjectTopLeft.x, bottomRight.y + m_velocity.y - gameObjectBottomRight.y);

		if (xCollisionBefore == false) {
			if ((diff.x < -m_scale.x) == false && (diff.x > gameObjectScale.x) == false) {
				if (diff.x < 0) {
					m_velocity.x = 0;
					//m_velocity.x -= m_scale.x + diff.x;
				}
				if (diff.x >= 0) {
					m_velocity.x = 0;
					//m_velocity.x += gameObjectScale.x - diff.x;
				}
			}
		}
		if (yCollisionBefore == false) {
			if ((diff.y < -m_scale.y) == false && (diff.y > gameObjectScale.y) == false) {
				if (diff.y < 0) {
					m_velocity.y = 0;
					//m_velocity.y -= m_scale.y + diff.y;
				}
				if (diff.y >= 0) {
					m_velocity.y = 0;
					//m_velocity.y += gameObjectScale.y - diff.y;
				}
			}
		}
	}
	return xCollision && yCollision;
}

void Collider::AddCollision(GameObject * gameObject)
{
	m_collisions.push_back(gameObject->GetCollider());
}

void Collider::AddCollision(Collider * collider)
{
	m_collisions.push_back(collider);
}

void Collider::GetCollisions(std::vector<Collider*>& colliders) const
{
	colliders = m_collisions;
}

void Collider::ClearCollisions()
{
	m_collisions.clear();
}

GameObject * Collider::GetGameObject()
{
	return m_gameObject;
}

void Collider::GetLocalPos(glm::vec2& pos) const
{
	pos = m_localPosition;
}

void Collider::SetLocalPos(const glm::vec2& pos)
{
	m_localPosition = pos;
}

void Collider::GetOrigin(glm::vec2& origin) const
{
	origin = m_origin;
}

void Collider::SetOrigin(const glm::vec2& origin)
{
	m_origin = origin;
}

void Collider::GetScale(glm::vec2& scale) const
{
	scale = m_scale;
}

void Collider::SetScale(const glm::vec2& scale)
{
	m_scale = scale;
}

bool Collider::IsKinematic()
{
	return m_kinematic;
}

void Collider::SetKinematic(const bool& kinematic)
{
	m_kinematic = kinematic;
}

bool Collider::IsSensor()
{
	return m_sensor;
}

void Collider::SetSensor(const bool& sensor)
{
	m_sensor = sensor;
}

void Collider::GetVelocity(glm::vec2& velocity) const
{
	velocity = m_velocity;
}

void Collider::SetVelocity(const glm::vec2& velocity)
{
	m_velocity = velocity;
}