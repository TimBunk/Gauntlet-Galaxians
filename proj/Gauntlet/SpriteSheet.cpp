#include "SpriteSheet.h"

SpriteSheet::SpriteSheet(int width, int height, int xOffset, int yOffset, unsigned int spriteID)
{
	m_width = width;
	m_height = height;
	m_xOffset = xOffset;
	m_yOffset = yOffset;
	m_spriteID = spriteID;
}

SpriteSheet::~SpriteSheet()
{
}

int SpriteSheet::GetRowCount()
{
	return m_height / m_yOffset;
}

int SpriteSheet::GetColumnCount()
{
	return m_width / m_xOffset;
}

unsigned int SpriteSheet::GetSprite(int index)
{
	NHTV::SpriteManager* spriteManager = NHTV::SpriteManager::GetInstance();
	unsigned int newSprite = spriteManager->DuplicateSprite(m_spriteID);

	glm::vec4 uvCoords;
	GetUvCoorsSprite(index, uvCoords);

	spriteManager->SetSpriteUVCoordinates(newSprite, glm::vec2(uvCoords.x, uvCoords.y), glm::vec2(uvCoords.z, uvCoords.w));
	spriteManager->SetSpriteScale(newSprite, glm::vec2(m_xOffset, m_yOffset));

	return newSprite;
}

void SpriteSheet::GetUvCoorsSprite(int index, glm::vec4 & uvCoords) const
{
	int xIndex = 0;
	int yIndex = 0;
	if (index != 0) {
		xIndex = index % (m_width / m_xOffset);
		yIndex = std::floor((float)index / ((float)m_width / (float)m_xOffset));
	}

	// minUVCoord is the bottomLeft and maxUVCoord is the topRight
	glm::vec2 minUVCoord = glm::vec2((float)xIndex * (float)m_xOffset / (float)m_width, 1.0f - (((float)yIndex + 1.0f) * (float)m_yOffset / (float)m_height));
	glm::vec2 maxUVCoord = glm::vec2(((float)xIndex + 1.0f) * (float)m_xOffset / (float)m_width, 1.0f - ((float)yIndex * (float)m_yOffset / (float)m_height));
	uvCoords = glm::vec4(minUVCoord, maxUVCoord);
}