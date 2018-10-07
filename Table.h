#ifndef TABLE_H_
#define TABLE_H_
#include "Player.h"


class Table
{
public :
	Table(const int playerNum, const int handSize);
	~Table();
	// access #i player
	Player* accessPlayer( size_t index);
	// get # of players
	unsigned int getTableSize();
	// show final results
	void showScoreBoard();
private:
	std::vector<Player*> playersAtSeat;
	unsigned int playerNum;
};



#endif