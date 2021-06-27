#pragma once
#include <SFML/Graphics.hpp>
class Tile
{
public:
    enum TileState {hidden, revealed};
    sf::Sprite flagSprite, mineSprite, hiddenSprite, revealedSprite, numberSprite;
    sf::Vector2i position;
    int xPos;
    int yPos;
    bool flag;
    bool mine;
    bool checked;
    TileState state;
    Tile* neighbors[8];
    unsigned int adjMines;
    
    Tile() {};
    Tile(TileState state, int x, int y, bool mine);
    Tile& operator=(const Tile& rhs);
    
    void setPos(sf::Vector2i position);
    void setPos(int x, int y);
    void setSprites();
    int countAdjMines();
    void recursiveReveal();
    void setState(TileState state);
    void leftClick();
    void toggleFlag();
    void placeMine();
};
