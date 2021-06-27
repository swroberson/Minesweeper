#include "Tile.h"
#include "TextureManager.h"
#include<SFML/Graphics.hpp>
#include <stdio.h>
#include <iostream>

Tile::Tile(TileState state, int x, int y, bool mine)
{
    this->state = state;
    Tile::setPos(x, y);
    this->mine = mine;
    this->flag = false;
    this->checked = false;
    this->adjMines = 0;
    for (unsigned int n = 0; n < 8; n++)
        this->neighbors[n] = nullptr;
    setSprites();
}

Tile& Tile::operator=(const Tile& rhs)
{
    this->state = rhs.state;
    this->position = rhs.position;
    this->xPos = rhs.xPos;
    this->flag = rhs.flag;
    this->mine = rhs.mine;
    this->flagSprite = rhs.flagSprite;
    this->mineSprite = rhs.mineSprite;
    this->hiddenSprite = rhs.hiddenSprite;
    this->revealedSprite = rhs.revealedSprite;
    this->numberSprite = rhs.numberSprite;
    
    return *this;
}


void Tile::setPos(sf::Vector2i position)
{
    this->position = position;
}

void Tile::setPos(int x, int y)
{
    this->xPos = x;
    this->yPos = y;
}

void Tile::setSprites()
{
    sf::Sprite hidden(TextureManager::GetTexture("tile_hidden"));
    sf::Sprite revealed(TextureManager::GetTexture("tile_revealed"));
    sf::Sprite flag(TextureManager::GetTexture("flag"));
    sf::Sprite mine(TextureManager::GetTexture("mine"));

    this->hiddenSprite = hidden;
    this->hiddenSprite.setColor(sf::Color(255,255,255,255));
    this->revealedSprite = revealed;
    this->revealedSprite.setColor(sf::Color(255,255,255,255));
    this->flagSprite = flag;
    this->flagSprite.setColor(sf::Color(255,255,255,0));
    this->mineSprite = mine;
    this->mineSprite.setColor(sf::Color(255,255,255,0));
    
}

void Tile::recursiveReveal()
{
    if (this->adjMines == 0 && !this->checked)
    {
        for (unsigned int n = 0; n < 8; n++)
        {
            if (this->neighbors[n] != nullptr && !this->neighbors[n]->checked && !this->neighbors[n]->mine && this->neighbors[n]->state == hidden && !this->neighbors[n]->flag)
            {
                this->neighbors[n]->hiddenSprite.setColor(sf::Color(255,255,255,0));
                this->neighbors[n]->setState(revealed);
                this->neighbors[n]->recursiveReveal();
            }
            else
            {
                continue;
            }
        }
    }

}

void Tile::setState(TileState state)
{
    this->state = state;
}

void Tile::leftClick()
{
    if (this->adjMines == 0 && state == Tile::hidden && !this->flag && !this->mine && !this->checked)
    {
        this->hiddenSprite.setColor(sf::Color(255,255,255,0));
        this->setState(revealed);
        this->recursiveReveal();
    }
    else if (state == Tile::hidden && !this->flag)
    {
        this->hiddenSprite.setColor(sf::Color(255,255,255,0));
        this->setState(revealed);
    }
    if (this->mine)
    {
        this->setState(revealed);
        this->mineSprite.setColor(sf::Color(255,255,255,255));
    }
}

void Tile::toggleFlag()
{
    if (!flag && state == Tile::hidden)
    {
        this->flagSprite.setColor(sf::Color(255,255,255,255));
        this->flag = true;
    }
    
    else
    {
        this->flagSprite.setColor(sf::Color(255,255,255,0));
        this->flag = false;
    }
}

void Tile::placeMine()
{
    if (!mine)
    {
        this->mineSprite.setColor(sf::Color(255,255,255,0));
        this->numberSprite.setColor(sf::Color(255,255,255,0));
        this->mine = true;
    }
}
