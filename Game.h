#ifndef GAME_H_
#define GAME_H_

#include <iostream>
#include <bits/stdc++.h>

#include "Bag.h"
#include "Dictionary.h"
#include "Table.h"
#include "ConsolePrinter.h" 
#include "Board.h" 

class Game {
private:
    // conf. vars
    int handSize;
    std::string tileFile;
    std::string dictFile;
    std::string boardFile;
    uint32_t seed;

    
    Table* currentPlayers = NULL;
    Dictionary* wordReference = NULL;
    Bag* tilesBag = NULL;
    ConsolePrinter* displayGame;
    Board* board = NULL;
    

    void readConfigFile(std::string config_file);

    std::string stripSpaces(std::string name);

public:
    Game(std::string config_file, int no_players); // constructor 
    ~Game(); //destructor

    void runGame();

    void displayGameConfig();

};


#endif 