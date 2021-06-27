#include <stdio.h>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Random.h"
#include "Board.h"

using namespace sf;

void drawBoard(RenderWindow& window, Board& board)
{
    
    for (unsigned int n = 0; n < board.width; n++)
        for (unsigned int m = 0; m < board.height; m++)
        {
            window.draw(board.tiles[n][m].revealedSprite);
            window.draw(board.tiles[n][m].numberSprite);
            window.draw(board.tiles[n][m].hiddenSprite);
            window.draw(board.tiles[n][m].flagSprite);
            window.draw(board.tiles[n][m].mineSprite);
        }
    for (unsigned int n = 0; n < 4; n++)
        window.draw(board.testButtons[n].sprite);
    
    window.draw(board.faceButton.sprite);
    
    for (unsigned int n = 0; n < 3; n++)
    window.draw(board.counter[n].sprite);
    
    window.draw(board.minusSign.sprite);
}

int main()
{
    Board *game = new Board();
    RenderWindow *window = new RenderWindow(sf::VideoMode(game->width * 32, (game->height * 32) + 88), "Minesweeper");
    int tilesRemaining;
    int flagCounter = 0;
    while (window->isOpen())
    {
        Event event;
        bool debug = game->inDebug;
        while (window->pollEvent(event) && event.type != Event::MouseMoved)
        {
            if (event.type == Event::Closed)
                window->close();
            
            
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                Vector2i click = sf::Mouse::getPosition(*window);
                if (click.y >= 0 && click.y < game->height * 32 && !game->defeat && !game->victory)
                {
                    if (click.x >= 0 && click.x <= game->width * 32)
                    {
                        int x = (click.x / 32);
                        int y = (click.y / 32);
                        game->tiles[x][y].leftClick();
                        
                        tilesRemaining = game->countRevealedTiles();
                        
                        if (game->tiles[x][y].mine)
                            game->loseGame();
                        
                        if (game->width * game->height - tilesRemaining == game->numMines)
                        {
                            game->winGame();
                            
                            flagCounter = 0;
                            for (unsigned int n = 0; n < game->width; n++)
                                for (unsigned int m = 0; m < game->height; m++)
                                {
                                    if (game->tiles[n][m].flag)
                                        flagCounter++;
                                }
                            game->countNum = game->numMines - flagCounter;
                            game->updateCounter();
                        }
                    }
                }
                if (click.y > game->height * 32 && click.y <= game->height * 32 + 88 && click.y > game->height * 32 &&  click.y < game->height * 32 + 64)
                {
                    // button interface
                    if (click.x >= 0 && click.x <= game->width * 32)
                    {
                        // Face button is clicked
                        if (click.x >= (game->width / 2 - 1) * 32 && click.x < (game->width / 2 - 1) * 32 + 64 )
                        {
                            debug = game->inDebug;
                            delete game;
                            delete window;
                            game = new Board();
                            window = new RenderWindow(sf::VideoMode(game->width * 32, (game->height * 32) + 88), "Minesweeper");
                            tilesRemaining = (game->width * game->height);
                            flagCounter = 0;
                            if (debug)
                                game->toggleDebug();
                        }
                        // test button clicked
                        for (unsigned int n = 0; n < 4; n++)
                            if (click.x > game->testButtons[n].xPos && click.x < game->testButtons[n].xPos + 64)
                            {
                                if (n == 0)
                                {
                                    if (!game->defeat && !game->victory)
                                    {
                                        debug = game->inDebug;
                                        game->toggleDebug();
                                    }
                                }
                                
                                else
                                {
                                    debug = game->inDebug;
                                    delete game;
                                    game = new Board("testboard" + to_string(n) + ".brd");
                                    delete window;
                                    window = new RenderWindow(sf::VideoMode(game->width * 32, (game->height * 32) + 88), "Minesweeper");
                                    if (!debug)
                                        game->toggleDebug();
                                }
                            }
                    }
                }
            }
            // right click
            if (Mouse::isButtonPressed(Mouse::Right) && !game->defeat && !game->victory)
            {
                Vector2i click = sf::Mouse::getPosition(*window);
                if (click.y >= 0 && click.y < game->height * 32)
                {
                    if (click.x >= 0 && click.x <= game->width * 32) // tile right clicked
                    {
                        int x = (click.x / 32);
                        int y = (click.y / 32);
                        if (game->tiles[x][y].state == Tile::hidden)
                        {
                            game->tiles[x][y].toggleFlag();
                            if (game->tiles[x][y].flag)
                                flagCounter++;
                            else if (!game->tiles[x][y].flag)
                                flagCounter--;
                            game->countNum = game->numMines - flagCounter;
                            game->updateCounter();
                        }
                        
                    }
                }
            }
            
        }
        window->clear(sf::Color::White);
        drawBoard(*window, *game);
        window->display();
    }
    

    return 0;
}

