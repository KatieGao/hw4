#include "gameHeaderFiles.h"

using namespace std;

Table::Table(const int playerNum, const int handSize): playerNum(playerNum) {
	for (int i =0; i<playerNum; i++) {
		string name ;
		cout << "Player " << i+1 << " name : ";
		getline(cin, name);
		playersAtSeat.push_back(new Player(name, handSize));
	}
	// that's all for now (no drawing at initialization)
}

// destructor 
Table::	~Table()
{
	//delete each player
	for (auto it = playersAtSeat.rbegin(); it != playersAtSeat.rend(); ++it)
		delete *it;
}

Player* Table::accessPlayer(size_t index)
{

	if(index>= playerNum)
		index = index%playerNum; // it's a round tabel  !!
	return playersAtSeat[index];
}

	// get # of players
unsigned int Table::getTableSize()
{
	return playerNum;
}

void Table::showScoreBoard()
{
	int finishBonus = 0;
	int finisherIndex = -1;
	for (unsigned  it = 0; it<playerNum; ++it)
	{
		int temp = 0;
		set<Tile*> leftOver =  accessPlayer(it)->getHandTiles();
		if(leftOver.size() == 0)
			finisherIndex = it;
		//get penalty
		for (auto it = leftOver.begin(); it != leftOver.end() ; it++)
			temp += (*it)->getPoints();
		accessPlayer(it)->setScore( accessPlayer(it)->getScore() - temp); // make deduction
		finishBonus += temp;
	}

	if(finisherIndex > -1) // have a finisher
		accessPlayer(finisherIndex)->setScore( accessPlayer(finisherIndex)->getScore() + finishBonus);

	// display score 
	int winner, maxScore;
	for (unsigned  it = 0; it<playerNum; ++it)
	{
		if (it == 0)
		{
			winner = it; maxScore = accessPlayer(it)->getScore();
		}
		cout<<" Player "<<accessPlayer(it)->getID()<< " has score "<<accessPlayer(it)->getScore()<<endl;
		if(playersAtSeat[it]->getScore() > maxScore)
		{
			winner = it; maxScore = accessPlayer(it)->getScore();
		}
	}
	cout<<"winner is Player "<<accessPlayer(winner)->getID()<<" has score "<<maxScore<<endl;

}