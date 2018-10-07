/*
 * Player.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* Encodes the state of a player, mostly the name, tiles, points. 
   The main non-trivial ability of this class is to produce a vector
   of Tile* from a string describing tiles to place/exchange. */

#ifndef PLAYER_H_
#define PLAYER_H_

#include <string>
#include <set>
#include <vector>

#include "Tile.h"
#include "Bag.h"
#include "Dictionary.h"
#include "Board.h"
#include "Move.h"

class Player
{

public:
	/* Constructor giving the player the given name, and setting their points to 0.
	   Does not give the player any tiles.
	*/
	Player (std::string const & name, size_t maxTiles);

	/* Destructor for a player. Deletes all the tiles the player still has. */
	~Player ();
	/* Reads a move string, finds the corresponding tiles in the player's hand, and
	   removes and returns those tiles in the order they appear in the move string..
	*/
	std::vector<Tile*> takeTiles (std::string const & tilesToRemove, bool resolveBlanks);
	// Adds all the tiles in the vector to the player's hand.
	void addTiles (std::vector<Tile*> const & tilesToAdd);

	//get move input from user
	bool makeMove(std::string command, Bag* bag, Dictionary* dict, Board* board);

	/* Reads a move string and confirms that the player has the tiles necessary to
	   execute this move.
	 */
	bool hasTiles(std::string const & tiles) ;
	//overload
	bool hasTiles(std::string const & tiles, bool isPlace);

	void setScore(int newScore);

	int getScore();

	//get max tiles 
	size_t getMaxTiles();

	//get current tiles in a vector
	std::set<Tile*> getHandTiles();

	//get current tiles in a map
	std::map<char, int> getCurrentTiles();

	// get player name
	std::string getID();

	

private:
	std::string name;

	unsigned int score ;
	
	size_t maxTiles;

	/* returns all tiles in hand */
	std::vector<Tile*> tilesAtHand;

	/* a map of tiles and their numbers */
	std::map<char, int> currentTiles;

	/* check validity of a move */
	bool checkAndExecute(std::string input, Bag* bag, Dictionary* dict, Board* board);

	/* Returns the map of tiles and their numbers the player currently holds. */
	void countHandTiles();
};


#endif /* PLAYER_H_ */
