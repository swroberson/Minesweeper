#include "GameElement.h"
#include "TextureManager.h"


void GameElement::setPos(int x, int y)
{
    this->xPos = x;
    this->yPos = y;
}

void GameElement::setTexture(std::string textureName)
{
    this->texture = TextureManager::GetTexture(textureName);
}

void GameElement::setSprite(std::string spriteName)
{
    sf::Sprite newSprite(TextureManager::GetTexture(spriteName));
    this->sprite = newSprite;
}
