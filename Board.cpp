#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include <utility>

#include "Bag.h"
#include "Tile.h"
#include "Square.h"
#include "Dictionary.h"
#include "Table.h"
#include "ConsolePrinter.h" 
#include "Game.h"
#include "Player.h"
#include "Board.h"
#include "Move.h"

using namespace std;

Board::Board (string config_file_name) {
	readConfigFile(config_file_name);
}

Board::~Board() { // Will delete the entire board
	for (unsigned int i = 0; i < nRow; i++)
	{
		for (unsigned int j = 0; j < nCol; j++)
			delete board[i][j];
		delete [] board[i];
	}
	delete [] board;
}

void Board::readConfigFile(string config_file_name) {
	fstream config;
	config.open(config_file_name);

	if (!config) {
		cout<<"Unable to open the board config file\n";
		exit(-1);
	}

	//get board size 
	config >> nCol; // get x
	config >> nRow; // get y
	
	//get starting point
	config >> startX; // start X
	config >> startY; //start Y

	board = new Square** [nRow];
	
	for (unsigned int i = 0; i < nRow; i++)
		board[i] = new Square* [nCol];
	
	string oneRow;
	getline(config, oneRow);

	for (unsigned int row = 0; row < nRow ; row++) {
		getline(config, oneRow);
		for (unsigned int col =0; col<nCol; col++) {

			bool flag = false;
			int wordBonus = 1;
			int letterBonus = 1;
			bool isStart = false;
			switch(oneRow[col]) {
				case '2':
					letterBonus = 2;
					break;
				case '3':
					letterBonus = 3;	
					break;
				case 'd':
					wordBonus = 2;
					break;
				case 't':
					wordBonus = 3;
					break;
				default:
					break;

			}
			if(col == startY-1 and row == startX-1)
				isStart = true;

			if (!flag )
				board[row][col] =  new Square(letterBonus, wordBonus, isStart);
		}
	}
}

size_t Board::getRows() const {
	return nRow;
}

size_t Board::getColumns() const {
	return nCol;
}


Square * Board::getSquare (size_t row, size_t col) const {
	if (row>nCol or col >nRow or row < 1 or col < 1)
		return NULL;
	else
		return board[row - 1][col-1];
}

pair<size_t, size_t> Board::getStartingPoint() const {
	pair<size_t,size_t>start = make_pair(startX, startY);
	return start;
}

void Board::placeTiles(std::vector<Tile*> tilesForPlace, bool isHorizontal, size_t startRow, size_t startCol) {

	if(isHorizontal) {
		for(auto it = tilesForPlace.begin(); it != tilesForPlace.end(); it++) {

			while(getSquare(startRow, startCol)->isOccupied())
				startCol++;

			getSquare(startRow, startCol)->placeTile(*it);
			startCol++;
		}
	} else {
		for(auto it = tilesForPlace.begin(); it != tilesForPlace.end(); it++) {
			while(getSquare(startRow, startCol)->isOccupied())
				startRow++;

			getSquare(startRow, startCol)->placeTile(*it);
			startRow++;
		}
	}
}

bool Board::isOriginTaken()
{
	return getSquare(startX, startY)->isOccupied();
}