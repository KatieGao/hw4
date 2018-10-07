/*
 * Move.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* The Move class family encodes a move made by one player, including its type
   (PASS, EXCHANGE, PLACE), the tiles used (and use for blanks),
   start square, and direction (if needed).
   Call Move::parseMove() to turn an entered string into its subclass of
   Move, then call apply() on that returned move to have it execute.*/

#ifndef MOVE_H_
#define MOVE_H_

// header files
#include <vector>
#include <string>
#include "Tile.h"
#include "Board.h"
#include "Bag.h"
#include "Dictionary.h"
#include "Player.h"
#include "Tile.h"

class Player; // forward declaration

class Move
{
public:
	Move(std::string expression, Player* player, Board* board, Bag* bag, Dictionary* dictionary);
	~Move();

	virtual bool checkValidity() = 0;
	virtual void execute() = 0;
	virtual bool commandParser() = 0;
	virtual int getScore() = 0;

protected:
	std::string command ; // user's command
	Player* user;

	Board* boardPtr ;
	Bag* bagPtr ;
	Dictionary* dictPtr ;

};

// Represents a pass move, where the player takes no action
class PassMove : public Move
{
public:

	/* Constructs a pass move. */
	PassMove(std::string expression, Player* player, Board* board, Bag* bag, Dictionary* dictionary);

	bool checkValidity();
	void execute();
	bool commandParser();
	int getScore();
};

// represents an exchange move, were a player replaces certain tiles
class ExchangeMove : public Move
{

public:

	/* Constructs a exchange move. */
	ExchangeMove(std::string expression, Player* player, Board* board, Bag* bag, Dictionary* dictionary);

	bool checkValidity();
	void execute();
	bool commandParser();
	int getScore();
private:
	std::string tilesWantExchange ;

};

// represents a place move, were a player places certain tiles
class PlaceMove : public Move
{

public:

	/* Constructs a place move. */
	PlaceMove(std::string expression, Player* player, Board* board, Bag* bag, Dictionary* dictionary);

	bool checkValidity();
	void execute();
	bool commandParser();
	int getScore() ;

private:
	std::string tilesWantPlace ; // command 
	size_t startRow, startCol; // startRow and startCol of the command
	bool isHorizontal ; // direction of the command
	std::vector<std::pair<int, bool>> bonusRecord; // store all bonus before place 
	
	// functions
	bool isLocationLegal();
	bool isWordLegal();
	std::vector<std::string> greedySearch(); //find all adjacent words
	std::string getWord(bool isHorizontal, size_t row, size_t col, std::string tiles);

	void countNewWordBonus();
	std::vector<int> getWordScore(size_t row, size_t col, bool isHorizontal, int bonus);
};


#endif /* MOVE_H_ */
