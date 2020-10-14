#pragma once

#include "SpriteManager.h"

class SpriteSheet
{
public:
	SpriteSheet(int width, int height, int xOffset, int yOffset, unsigned int spriteID);
	~SpriteSheet();

	int GetRowCount();
	int GetColumnCount();
	unsigned int GetSprite(int index);
	void GetUvCoorsSprite(int index, glm::vec4& uvCoords) const;

private:
	int m_width, m_height;
	int m_xOffset, m_yOffset;
	int m_spriteID;
};