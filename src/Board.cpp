#include "Board.h"
#include "Tile.h"
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <sstream>
#include <string>
#include <SFML/Graphics.hpp>
#include "TextureManager.h"
#include "Random.h"
using namespace std;

void Board::placeMines()
{
    
    for (int n = 0; n < numMines; n++)
    {
        int x = Random::Int(0, width - 1);
        int y = Random::Int(0, height - 1);
        
        if (tiles[x][y].mine)
            n--;
        else
        {
            tiles[x][y].placeMine();
            minesMap [n] = (to_string(x) + ", " + to_string(y));
        }
    }
}

void Board::assignNeighbors()
{
    for (int n = 0; n < width; n++)
        for (int m = 0; m < height; m++)
        {
            if (n == 0 || m == 0) // top row and left column
            {
                if (n != width - 1)
                    tiles[n][m].neighbors[2] = &tiles[n + 1][m];      // right
                if (m != height - 1)
                {
                    if(n != width - 1)
                        tiles[n][m].neighbors[3] = &tiles[n + 1][m + 1];  // down/right
                    tiles[n][m].neighbors[4] = &tiles[n][m + 1];      // down
                }
                if (n == 0 && m != 0)
                {
                    tiles[n][m].neighbors[0] = &tiles[n][m - 1];      // up
                    tiles[n][m].neighbors[1] = &tiles[n + 1][m - 1];  // up/right
                }
                if (n != 0 && m == 0)
                {
                    tiles[n][m].neighbors[5] = &tiles[n - 1][m + 1];  // down/left
                    tiles[n][m].neighbors[6] = &tiles[n - 1][m];      // left
                }
            }
            else if (n == width - 1 || m == height - 1) // bottom row and right column
            {
                tiles[n][m].neighbors[6] = &tiles[n - 1][m];      // left
                if (m != 0)
                {
                    tiles[n][m].neighbors[0] = &tiles[n][m - 1];      // up
                    tiles[n][m].neighbors[7] = &tiles[n - 1][m - 1];  // up/left
                }
                
                if (n != width - 1 && m == height - 1) // if not bottom row, just right column
                {
                    tiles[n][m].neighbors[1] = &tiles[n + 1][m - 1];  // up/right
                    tiles[n][m].neighbors[2] = &tiles[n + 1][m];      // right
                }
                if (n == width - 1 && m != height - 1) // if not right column, just bottom row
                {
                    tiles[n][m].neighbors[4] = &tiles[n][m + 1];      // down
                    tiles[n][m].neighbors[5] = &tiles[n - 1][m + 1];  // down/left
                }
            }
            if (n != 0 && n != width - 1 && m != 0 && m != height - 1)
            {
                tiles[n][m].neighbors[0] = &tiles[n][m - 1];      // up
                tiles[n][m].neighbors[1] = &tiles[n + 1][m - 1];  // up/right
                tiles[n][m].neighbors[2] = &tiles[n + 1][m];      // right
                tiles[n][m].neighbors[3] = &tiles[n + 1][m + 1];  // down/right
                tiles[n][m].neighbors[4] = &tiles[n][m + 1];      // down
                tiles[n][m].neighbors[5] = &tiles[n - 1][m + 1];  // down/left
                tiles[n][m].neighbors[6] = &tiles[n - 1][m];      // left
                tiles[n][m].neighbors[7] = &tiles[n - 1][m - 1];  // up/left
            }
        }
}

void Board::countAdjMines()
{
    for (unsigned int n = 0; n < width; n++)
        for (unsigned int m = 0; m < height; m++)
        {
            for (unsigned int i = 0; i < 8; i++)
            {
                    for (auto& iter: minesMap)
                    {
                        string posStr = (iter.second);
                        int x = stoi(posStr.substr(0, posStr.find(',')));
                        int y = stoi(posStr.substr(posStr.find(' '), posStr.length()));
                        if (tiles[n][m].neighbors[i] != nullptr && tiles[n][m].neighbors[i] == &tiles[x][y] && &tiles[n][m].neighbors[i]->mine)
                            tiles[n][m].adjMines++;
                    }
                if (tiles[n][m].adjMines > 0)
                {
                    sf::Sprite num(TextureManager::GetTexture("number_" + to_string(tiles[n][m].adjMines)));
                    tiles[n][m].numberSprite = num;
                    tiles[n][m].numberSprite.setPosition(sf::Vector2f(n * 32, m * 32));
                    tiles[n][m].numberSprite.setColor(sf::Color(255,255,255,255));
                }
            }
    }
}


Board::Board()
{
    victory = false;
    defeat = false;
    inDebug = false;
    string line;
    ifstream file("boards/config.cfg");
    file >> this->width;
    file >> this->height;
    file >> this->numMines;
    
    for (unsigned int n = 0; n < width; n++)
    {
        vector<Tile> temp;
        for (unsigned int m = 0; m < height; m++)
        {
            Tile tile(Tile::hidden, n * 32, m * 32, false);
            temp.push_back(tile);
        }
        tiles.push_back(temp);
    }
    
    placeMines();
    assignNeighbors();
    countAdjMines();
    for (int n = 0; n < width; n++)
        for (int m = 0; m < height; m++)
        {
            tiles[n][m].flagSprite.setPosition(sf::Vector2f(n * 32, m * 32));
            tiles[n][m].mineSprite.setPosition(sf::Vector2f(n * 32, m * 32));
            tiles[n][m].revealedSprite.setPosition(sf::Vector2f(n * 32, m * 32));
            tiles[n][m].hiddenSprite.setPosition(sf::Vector2f(n * 32, m * 32));
        }
    testButtons[0].setTexture("debug");
    testButtons[0].setSprite("debug");
    for (int n = 1; n < 4; n++)
    {
        testButtons[n].setTexture("test_" + to_string(n));
        testButtons[n].setSprite("test_" + to_string(n));
    }
    for (int n = 0; n < 4; n++)
    {
        testButtons[n].sprite.setPosition(sf::Vector2f(n * 64 + (32 * width - 64 * 4), height * 32));
        testButtons[n].setPos(n * 64 + (32 * width - 64 * 4), height * 32);
    }
    
    this->faceButton.setTexture("face_happy");
    this->faceButton.setSprite("face_happy");
    faceButton.sprite.setPosition((width / 2 - 1) * 32, height * 32);
    
    int digits[3];
    
    digits[0] = numMines / 100;
    digits[1] = numMines / 10 % 10;
    digits[2] = numMines % 10;
    
    for (unsigned int n = 0; n < 3; n++)
    {
        counter[n].setTexture("digits");
        counter[n].setSprite("digits");
        counter[n].sprite.setTextureRect(sf::IntRect(21 * digits[n], 0, 21, 32));
        counter[n].sprite.setPosition(n * 21 + 21, height * 32);
    }
}

Board::Board(string testFile)
{
    victory = false;
    defeat = false;
    inDebug = false;
    string row;
    this->numMines = 0;
    unsigned int numLines = 0;
    ifstream file("boards/" + testFile);

        if (file.is_open())
        {
            string line;
            while(getline(file, line))
            {
                istringstream stream(line);
                getline(stream, row);
                for (unsigned int n = 0; n < row.length(); n++)
                {
                    if (row.at(n) == '1')
                    {
                        numMines++;
                        minesMap [numMines] = (to_string(n) + ", " + to_string(numLines));
                    }
                }
                numLines++;
            }
        }
    
    this->width = row.length();
    this->height = numLines;
    for (unsigned int n = 0; n < width; n++)
    {
        vector<Tile> temp;
        for (unsigned int m = 0; m < height; m++)
        {
            
            Tile tile(Tile::hidden, n * 32, m * 32, false);
            temp.push_back(tile);
        }
        tiles.push_back(temp);
    }
    int num = 0;
    for (auto& iter: minesMap)
    {
        string xStr = (iter.second);
        int x = stoi(xStr.substr(0, xStr.find(',')));
        int y = stoi(xStr.substr(xStr.find(' '), xStr.length()));
        tiles[x][y].placeMine();
    }
    
    assignNeighbors();
    countAdjMines();
    toggleDebug();
    
    for (int n = 0; n < width; n++)
        for (int m = 0; m < height; m++)
        {
            tiles[n][m].flagSprite.setPosition(sf::Vector2f(n * 32, m * 32));
            tiles[n][m].mineSprite.setPosition(sf::Vector2f(n * 32, m * 32));
            tiles[n][m].revealedSprite.setPosition(sf::Vector2f(n * 32, m * 32));
            tiles[n][m].hiddenSprite.setPosition(sf::Vector2f(n * 32, m * 32));
            tiles[n][m].numberSprite.setPosition(sf::Vector2f(n * 32, m * 32));
        }
    testButtons[0].setTexture("debug");
    testButtons[0].setSprite("debug");
    for (int n = 1; n < 4; n++)
    {
        testButtons[n].setTexture("test_" + to_string(n));
        testButtons[n].setSprite("test_" + to_string(n));
    }
    for (int n = 0; n < 4; n++)
    {
        testButtons[n].sprite.setPosition(sf::Vector2f(n * 64 + (32 * width - 64 * 4), height * 32));
        testButtons[n].setPos(n * 64 + (32 * width - 64 * 4), height * 32);
    }
    
    this->faceButton.setTexture("face_happy");
    this->faceButton.setSprite("face_happy");
    faceButton.sprite.setPosition((width / 2 - 1) * 32, height * 32);
    
    int digits[3];
    
    digits[0] = numMines / 100;
    digits[1] = numMines / 10 % 10;
    digits[2] = numMines % 10;
    
    for (unsigned int n = 0; n < 3; n++)
    {
        counter[n].setTexture("digits");
        counter[n].setSprite("digits");
        counter[n].sprite.setTextureRect(sf::IntRect(21 * digits[n], 0, 21, 32));
        counter[n].sprite.setPosition(n * 21 + 21, height * 32);
    }   
}


void Board::updateCounter()
{
    int digits[3];
    digits[0] = this->countNum / 100;
    digits[1] = this->countNum / 10 % 10;
    digits[2] = this->countNum % 10;
    

    for (unsigned int n = 0; n < 3; n++)
    {
        counter[n].setTexture("digits");
        counter[n].setSprite("digits");
        counter[n].sprite.setTextureRect(sf::IntRect(21 * abs(digits[n]), 0, 21, 32));
        counter[n].sprite.setPosition(n * 21 + 21, height * 32);
    }
    
    if (countNum < 0)
    {
        this->minusSign.setTexture("digits");
        this->minusSign.setSprite("digits");
        this->minusSign.sprite.setTextureRect(sf::IntRect(21 * 10, 0, 21, 32));
        this->minusSign.sprite.setPosition(0, height * 32);
    }
    if (countNum > 0)
    {
        minusSign.sprite.setColor(sf::Color(255,255,255,0));
    }
}

void Board::loseGame()
{
    faceButton.sprite.setTexture(TextureManager::GetTexture("face_lose"));
    this->defeat = true;
    for (unsigned int n = 0; n < this->width; n++)
        for (unsigned int m = 0; m < this->height; m++)
            if (this->tiles[n][m].mine == true)
            {
                this->tiles[n][m].mineSprite.setColor(sf::Color(255,255,255,255));
            }
}
void Board::winGame()
{
    faceButton.sprite.setTexture(TextureManager::GetTexture("face_win"));
    this->victory = true;
    for (unsigned int n = 0; n < this->width; n++)
        for (unsigned int m = 0; m < this->height; m++)
            if (tiles[n][m].mine == true)
            {
                if (!tiles[n][m].flag && tiles[n][m].state == Tile::hidden)
                    tiles[n][m].toggleFlag();
                
            }
}
void Board::toggleDebug()
{
    if (!this->inDebug)
    {
        for (unsigned int n = 0; n < this->width; n++)
            for (unsigned int m = 0; m < this->height; m++)
                if (this->tiles[n][m].mine == true)
                {
                    this->tiles[n][m].mineSprite.setColor(sf::Color(255,255,255,255));
                    this->inDebug = true;
                }
    }
    else
    {
        for (unsigned int n = 0; n < this->width; n++)
            for (unsigned int m = 0; m < this->height; m++)
                if (this->tiles[n][m].mine == true)
                {
                    this->tiles[n][m].mineSprite.setColor(sf::Color(255,255,255,0));
                    this->inDebug = false;
                }
    }
}

int Board::countRevealedTiles()
{
    int numRevealed = 0;
    for (unsigned int n = 0; n < width; n++)
        for (unsigned int m = 0; m < height; m++)
        {
            if (tiles[n][m].state == Tile::revealed)
                numRevealed++;
        }
    return numRevealed;
}

