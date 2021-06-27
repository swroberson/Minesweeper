#include <SFML/Graphics.hpp>

class GameElement
{
public:
    sf::Texture texture;
    sf::Sprite sprite;
    int xPos;
    int yPos;
    
    void setPos(int x, int y);
    void setTexture(std::string textureName);
    void setSprite(std::string spriteName);
};
