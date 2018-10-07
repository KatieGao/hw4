#include "gameHeaderFiles.h"
using namespace std;

Player::Player(string const & name, size_t maxTiles): name(name), maxTiles(maxTiles) {
    this->score = 0;
}

Player::~Player() {
    for(auto tileIter = tilesAtHand.begin(); tileIter != tilesAtHand.end(); ++tileIter)
        delete *tileIter;
}

void Player::countHandTiles() {
    map<char, int> tempMap;
     
    for (auto i = tilesAtHand.begin(); i != tilesAtHand.end(); i++) {
        char value = (*i)->getLetter();

        auto it = tempMap.find(value);
        if(it == tempMap.end())
            tempMap[value] = 1;
        else
            it->second += 1;
    }
    currentTiles = tempMap;
}

bool Player::hasTiles(string const & tiles) {
    countHandTiles();
    map<char, int> tempMap = currentTiles;

    for (unsigned int i = 0; i<tiles.size(); i++) {
        auto it = tempMap.find(tiles[i]);
        if (it == tempMap.end()) {
            cout << "you don't have " << tiles[i] << endl;
            return false;
        } else {
            it->second -= 1;
            if (it->second < 0) {
                cout << "you don't have enough " << tiles[i] << endl;
                return false;
            }
        }
    }
    return true;
}

bool Player::hasTiles(string const & tiles, bool isPlace) {
    string cp = tiles;
    for (size_t i = 0; i< cp.size() ; i++) {
        if (cp[i] == '?')
            cp.erase(i+1, 1);
    }
    return (hasTiles(cp));
}

vector<Tile*> Player::takeTiles (string const & tilesToRemove, bool resolveBlanks) {
    vector<Tile*> tilesDrawn;
    size_t i = 0;
    size_t whitespaceCounter = 0;
    countHandTiles();

    if (!resolveBlanks) { 
        while(!tilesAtHand.empty() && i < tilesToRemove.size()) {
            unsigned int j = 0;

            while (j < tilesAtHand.size()) {
                if(tilesAtHand[j]->getLetter() == tilesToRemove[i]) {
                    Tile * tile = tilesAtHand[j];
                    tilesDrawn.push_back(tile);
                    tilesAtHand.erase(tilesAtHand.begin() + j);
                    break;
                }
                j++;
            }
            i++;
        }
    } else {
        while(!tilesAtHand.empty() && i < tilesToRemove.size()) {
            unsigned int j = 0;
            while (j < tilesAtHand.size()) {
                if(tilesAtHand[j]->getLetter() == tilesToRemove[i]) 
                {
                    Tile * tile = tilesAtHand[j];
                    if (tilesToRemove[i] == '?') {
                        tile->useAs(tilesToRemove[i+1]);
                        i++;
                        whitespaceCounter++;
                    }
                    tilesDrawn.push_back(tile); 
                    tilesAtHand.erase(tilesAtHand.begin() + j);
                    break;
                }
                j++;
            }
            i++;
        }
    }
    countHandTiles();

    return tilesDrawn;
}

void Player::addTiles (vector<Tile*> const & tilesToAdd) {

    for(auto tileIter = tilesToAdd.begin(); tileIter != tilesToAdd.end(); ++tileIter)
        tilesAtHand.push_back(*tileIter);

    countHandTiles();
}

string Player::getID() {
    return this->name;
}

bool Player::makeMove(string command, Bag* bag, Dictionary* dict, Board* board) {
    transform(command.begin(), command.end(), command.begin(), ::tolower);
    return checkAndExecute(command, bag, dict, board);
}

bool Player::checkAndExecute(string input, Bag* bag, Dictionary* dict, Board* board) {
    string EXCHANGE = "exchange";
    string PASS = "pass";
    string PLACE = "place";
    if (input.substr(0,EXCHANGE.size()) == EXCHANGE) {
        ExchangeMove move(input, this, board, bag, dict);
        if( !move.checkValidity())
            return false;
        move.execute();
        score += move.getScore();

    } else if (input.substr(0,PLACE.size()) == PLACE) {
        PlaceMove move(input, this, board, bag, dict);
        if( !move.checkValidity())
            return false;
        move.execute();
        score += move.getScore();
    } else if (input.substr(0,PASS.size()) == PASS) {
        PassMove move(input, this, board, bag, dict);
        if( !move.checkValidity())
            return false;
        move.execute();
        score += move.getScore();
    } else {
        cout << "COMMAND NOT FOUND\n";
        return false;
    }
    return true;
}

map<char, int> Player::getCurrentTiles() {
    countHandTiles();
    return currentTiles;
}


size_t Player::getMaxTiles() {
    return maxTiles;    
}

set<Tile*> Player::getHandTiles() {
     set<Tile*> s (tilesAtHand.begin(), tilesAtHand.end()) ;
     return s;
}

void Player::setScore(int newScore) {
    score = newScore;
}

int Player::getScore() {
    return score;
}