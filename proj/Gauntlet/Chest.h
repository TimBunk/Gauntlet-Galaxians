#pragma once

#include "GameObject.h"

class Chest : public GameObject
{
public:
	Chest(int points, unsigned int sprite_id);
	~Chest();

	void GetPoints(int& points) const;

private:
	int m_points;
};