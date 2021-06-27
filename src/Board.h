#pragma once
#include <fstream>
#include <vector>
#include <stdio.h>
#include <string>
#include "GameElement.h"
#include "Tile.h"
#include <unordered_map>
using namespace std;

class Board
{
public:

    bool inDebug, victory, defeat;
    float width;
    float height;
    int numMines;
    vector<vector<Tile>> tiles;
    unordered_map<int, string> minesMap;

    unsigned int flagCounter;
    int countNum;
    
    GameElement minusSign;
    GameElement counter[3];
    GameElement testButtons[4];
    GameElement faceButton;
    
    Board();
    Board(string testFile);
    
    void placeMines();
    void assignNeighbors();
    void countAdjMines();
    void setNumberSprites();
    void updateCounter();
    void resetBoard();
    void winGame();
    void loseGame();
    void toggleDebug();
    int countRevealedTiles();
};
