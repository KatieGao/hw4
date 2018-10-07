/*
 * Board.h
 *
 *  Created on: Sep 18, 2016
 *      Author: kempe
 */

/* The class Board encodes the state of the Scrabble board.
   It provides functions for displaying the board,
   and for returning the words (and score) that would be obtained by a move.
*/

#ifndef BOARD_H_
#define BOARD_H_

#include <string>
#include <set>
#include <map>
#include <vector>
#include "Tile.h"
#include "Square.h"
#include "Dictionary.h"
#include <utility>

class Board
{
	
public:
	/* constructor that initializes the board state with an empty board
           and the size and bonuses and start square based on the given file. */
	Board (std::string board_file_namey);

	~Board ();

	/* Returns a pointer to the Square object representing the
       (y,x) position of the board. Indexing starts at 1 here.
	   This is needed only to display the board. */
	Square * getSquare (size_t x, size_t y) const;

	/* Returns the number of rows of the board.*/
	size_t getRows() const;

	/* Returns the number of columns of the board.*/
	size_t getColumns() const;

	/* Reads config file */
	void readConfigFile(std::string config_file_name);

	/* Get the start x and y coordinate */
	std::pair<size_t, size_t> getStartingPoint() const;

	/* Ad dtile to the board */
	void placeTiles(std::vector<Tile*> tiles, bool isH, size_t startRow, size_t startCol);

	/* Check whether origin is available or not */
	bool isOriginTaken();

private:
	size_t nRow;
	size_t nCol;
	size_t startX;
	size_t startY;

	Square*** board = NULL;
};


#endif /* BOARD_H_ */
