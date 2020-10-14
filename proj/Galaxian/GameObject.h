#pragma once

#include "SpriteManager.h"

class GameObject
{
public:
	GameObject(unsigned int sprite_id);
	virtual ~GameObject();

	virtual glm::vec2 GetPos();
	virtual void SetPos(glm::vec2 pos);
	virtual float GetAngle();
	virtual float GetRot();
	virtual void SetRot(float rot);
	virtual glm::vec2 GetScale();
	virtual void SetScale(glm::vec2 scale);
	virtual glm::vec2 GetOrigin();
	virtual void SetOrigin(glm::vec2 origin);
	virtual glm::vec4 GetUvCoords();
	virtual void SetUvCoords(glm::vec4 uv);
	virtual unsigned int GetSprite_id();
	virtual void SetSprite_id(unsigned int sprite_id);
	virtual const NHTV::Vertex* GetVertexData();

	virtual bool IsAlive();
	virtual void SetAlive(bool alive);

	virtual void Update(float deltaTime);
	virtual bool IsCollidingWith(GameObject* gameObject);
	virtual void MarkForDraw();

protected:
	unsigned int sprite_id;
	float rotation;
	bool alive;

private:
	
};