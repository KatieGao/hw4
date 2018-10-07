#include "gameHeaderFiles.h"

using namespace std;

Move::Move(std::string expression, Player* player, Board* board, Bag* bag, Dictionary* dictionary): command(expression), user(player), boardPtr(board), bagPtr(bag),  dictPtr(dictionary) {

}

Move::~Move() {

}

PassMove::PassMove(std::string expression, Player* player, Board* board, Bag* bag, Dictionary* dictionary): Move( expression, player, board, bag, dictionary) {

}

bool PassMove::checkValidity() {
	return true;
}

void PassMove::execute() {

}

bool PassMove::commandParser() {
	return true;
}

int PassMove::getScore() {
	return 0;
}

ExchangeMove::ExchangeMove(std::string expression, Player* player, Board* board, Bag* bag, Dictionary* dictionary): Move( expression, player, board, bag, dictionary) {

}

bool ExchangeMove::checkValidity() {
	if (!commandParser()) return false;
	
	std::map<char, int> tempMap = user->getCurrentTiles();
	
	if(tilesWantExchange.size() == 0) {
		cout << "Specify the tiles that needs to be exchanged\n";
		return false;
	}

	if(!user->hasTiles(tilesWantExchange))
		return false;
		
	return true;
}

void ExchangeMove::execute() {	
	vector<Tile*> tilesForExchange = user->takeTiles(tilesWantExchange, false);
	bagPtr->addTiles(tilesForExchange);
	vector<Tile*> newTiles = bagPtr->drawTiles(tilesWantExchange.size());
	user->addTiles(newTiles);
	cout << "New tiles: ";
	
	for(auto it = newTiles.begin(); it!=newTiles.end(); it++)
		cout<<(*it)->getLetter()<<", ";
	cout<<endl;
}

bool ExchangeMove::commandParser() {
	stringstream ss(command);

	string move;
	ss>>move>>tilesWantExchange;
	if(tilesWantExchange == "") {	
		cout << "tiles error\n";
		return false;
	}
	return true;
}

int ExchangeMove::getScore() {
	return 0;
}

PlaceMove::PlaceMove(std::string expression, Player* player, Board* board, Bag* bag, Dictionary* dictionary): Move( expression, player, board, bag, dictionary) {}

bool PlaceMove::checkValidity() {
	if (!commandParser())
		return false;
	
	if (!user->hasTiles(tilesWantPlace, true))
		return false;

	if (!isLocationLegal())
		return false;

	if (!isWordLegal())
		return false;

	return true;
}

void PlaceMove::execute() {
	countNewWordBonus();

	vector<Tile*> tilesForPlace = user->takeTiles(tilesWantPlace, true);//place
	
	if(tilesForPlace.size() == user->getMaxTiles()) {
		user->setScore(user->getScore() + 50);
	}

	boardPtr->placeTiles(tilesForPlace, isHorizontal, startRow, startCol);

	vector<Tile*> newTiles = bagPtr->drawTiles(tilesForPlace.size() ) ;
	user->addTiles(newTiles );
	
	cout << "New Tiles: ";
	for(auto it = newTiles.begin(); it!=newTiles.end(); it++)
		cout << (*it)->getLetter() << ", ";
	cout<<endl;
}

bool PlaceMove::commandParser() {
	stringstream ss(command);
	string move;
	ss >> move;
	char direction;
	ss >> direction;

	if (direction == '-')
		isHorizontal = true;
	else if (direction == '|')
		isHorizontal = false;
	else {
		cout << "DIRECTION NOT VALID\n";
		return false;
	}

	ss>>startRow>>startCol;

	if(startRow == 0 || startCol == 0) {
		cout << "INVALID START POINT\n";
		return false;
	}
	ss >> tilesWantPlace;
	
	if (tilesWantPlace.empty()) {
		cout << "TILES ERROR\n";
		return false;
	}
	return true;
}

bool PlaceMove::isLocationLegal() {
	if(startRow < 1 || startCol < 1 || startRow > boardPtr->getRows() || startCol > boardPtr->getColumns()) {
		cout<<"Location not in bound\n";
		return false;
	}

	size_t tilesNum = 0;

	for (size_t i = 0; i < tilesWantPlace.size(); i++)
		if (tilesWantPlace[i] != '?')
			tilesNum++;
	pair<size_t, size_t> boardStartPoint = boardPtr->getStartingPoint();
	
	if(!boardPtr->isOriginTaken()) {
		if(isHorizontal) {
			if(startRow != boardStartPoint.first) {
				cout << "Must use origin\n";
				return false;
			}
			if(startCol > boardStartPoint.second) {
				cout << "Must use origin\n";
				return false;
			}
			if(startCol+tilesNum < boardStartPoint.second ) {
				cout << "Must use origin\n";
				return false;
			}
		}
		else {
			if(startCol != boardStartPoint.second) {
				cout << "Must use origin\n";
				return false;
			}
			if(startRow > boardStartPoint.first) {
				cout << "Must use origin\n";
				return false;
			}
			if(startRow+tilesNum < boardStartPoint.first ) {
				cout <<"Must use origin\n";
				return false;
			}
		}
	}

	if(boardPtr->getSquare(startRow, startCol)->isOccupied()) {
		cout << "the location you chose is occupied\n";
		return false;
	}
	if (isHorizontal) {
		size_t startColCopy = startCol;
		while (tilesNum > 0) {
			if(startColCopy > boardPtr->getColumns()) {
				cout << "Out of bound\n";
				return false;
			}
			if(!boardPtr->getSquare(startRow, startColCopy)->isOccupied())
				tilesNum --;
			startColCopy ++;
		}
	} else {
		size_t startRowCopy = startRow;
		while (tilesNum > 0) {
			if(startRowCopy > boardPtr->getRows()) {
				cout << "Out of bound\n";
				return false;
			}
			if(!boardPtr->getSquare(startRowCopy, startCol)->isOccupied())
				tilesNum --;
			startRowCopy ++;
		}
		
	}
	return true;
}

bool PlaceMove::isWordLegal() {
	vector<string> words = greedySearch();
	if(words.size() == 0 and boardPtr->isOriginTaken()) {
		cout << "no adjacent letter\n";
		return false;
	}
	for (auto it = words.begin(); it != words.end(); it++) {
		if(!dictPtr->isLegalWord(*it)) {
			cout<<*it<<" is not a legal word\n";
			return false;
		}
	}
	return true;
}

vector<string> PlaceMove::greedySearch() {
	vector<string> words;
	if (isHorizontal) {
		string temp = getWord(true, startRow, startCol, tilesWantPlace);
		if (temp != tilesWantPlace)
			words.push_back(temp);
		size_t colCopy = startCol;
		for(size_t it = 0; it < tilesWantPlace.size(); it++) {
			if(tilesWantPlace[it] == '?')
				continue;

			while( colCopy <= boardPtr->getColumns() && boardPtr->getSquare(startRow, colCopy)->isOccupied())
				colCopy++;
			string temp = getWord(false, startRow, colCopy, tilesWantPlace.substr(it, 1));
			if(temp !=  tilesWantPlace.substr(it, 1))
				words.push_back( temp);
			colCopy++;
		}
	} else {
		string temp = getWord(false, startRow, startCol, tilesWantPlace);
		if (temp != tilesWantPlace)
			words.push_back(temp);

		size_t rowCopy = startRow;
		for(size_t it = 0; it < tilesWantPlace.size(); it++) {
			if(tilesWantPlace[it] == '?')
				continue;

			while( rowCopy <= boardPtr->getRows() && boardPtr->getSquare(rowCopy, startCol)->isOccupied())
				rowCopy++;
			string temp = getWord(true, rowCopy, startCol, tilesWantPlace.substr(it, 1));
			if(temp !=  tilesWantPlace.substr(it, 1))
				words.push_back( temp);
			rowCopy++;
		}
	}
	return words;
}

string PlaceMove::getWord(bool isHorizontal, size_t startRow, size_t startCol, string tilesWantPlace) {
	string word;
	if (isHorizontal) {
		size_t colCopy = startCol;
		if(colCopy > 1) {
			colCopy--;
			while( colCopy >= 1 && boardPtr->getSquare(startRow, colCopy)->isOccupied())
				colCopy--;
			colCopy++;
		}

		for(size_t it = 0; it < tilesWantPlace.size(); it++) {
			if(tilesWantPlace[it] == '?')
				continue;

			if(colCopy > boardPtr->getColumns() || colCopy < 1)
				break;
			while( colCopy <= boardPtr->getColumns() && boardPtr->getSquare(startRow, colCopy)->isOccupied()) {
				word += boardPtr->getSquare(startRow, colCopy)->getLetter();
				colCopy++;
			}
			word += tilesWantPlace[it];
			colCopy++;
		}
		while(colCopy >= 1 && colCopy <=  boardPtr->getColumns() && boardPtr->getSquare(startRow, colCopy)->isOccupied()) {
			word += boardPtr->getSquare(startRow, colCopy)->getLetter();
			colCopy++;
		}

	} else {
		size_t rowCopy = startRow;
		if(rowCopy > 1) {
			rowCopy --;
			while( rowCopy >= 1 && boardPtr->getSquare(rowCopy, startCol)->isOccupied())
				rowCopy--;
			rowCopy ++;
		}


		for(size_t it = 0; it < tilesWantPlace.size(); it++) {
			if(tilesWantPlace[it] == '?')
				continue;
			if(rowCopy > boardPtr->getRows() || rowCopy < 1)
				break;
			while( rowCopy <= boardPtr->getRows() && boardPtr->getSquare(rowCopy, startCol)->isOccupied()) {
				word += boardPtr->getSquare(rowCopy, startCol)->getLetter();
				rowCopy++;
			}
			word += tilesWantPlace[it];
			rowCopy++;
		}

		while(rowCopy >= 1 &&  rowCopy <= boardPtr->getRows() && boardPtr->getSquare(rowCopy, startCol)->isOccupied()) {
			word += boardPtr->getSquare(rowCopy, startCol)->getLetter();
			rowCopy++;
		}
	}

	return word;
}

void PlaceMove::countNewWordBonus() {
	vector<pair<int, bool>>recorder;
	if(isHorizontal) {
		int i = -1;
		while ( startCol+i > 1 and boardPtr->getSquare(startRow, startCol + i)->isOccupied())
			i--;
		i++;
		while (i<0) {
			recorder.push_back(make_pair(1, true));
			i++;
		}

		for (unsigned int it = 0; it < tilesWantPlace.size() ; it++) {
			if (tilesWantPlace[it] == '?')
				continue;
			if (startCol + i > boardPtr->getColumns() || startCol + i < 1)
				break;

			while( startCol + i <= boardPtr->getColumns() && boardPtr->getSquare(startRow, startCol + i)->isOccupied()) {
				recorder.push_back(make_pair(1, true));
				i++;
			}

			int letterBonus = boardPtr->getSquare(startRow, startCol + i)->getLMult();
			int wordBonus  = boardPtr->getSquare(startRow, startCol + i)->getWMult();
			if (letterBonus == 1 && wordBonus == 1)
				recorder.push_back(make_pair(1, false));
			else if (letterBonus == 1)
				recorder.push_back(make_pair(10+wordBonus, false));
			else
				recorder.push_back(make_pair(letterBonus,false));
			i++;
		}
		
		while (startCol + i <= boardPtr->getColumns() && boardPtr->getSquare(startRow, startCol + i)->isOccupied()) {
			recorder.push_back(make_pair(1, true));
			i++;
		}
	} else {
		int i = -1;
		while ( startRow+i > 1 and boardPtr->getSquare(startRow + i, startCol)->isOccupied())
			i--;
		i++;
		while (i<0) {
			recorder.push_back(make_pair(1, true));
			i++;
		}
		for (unsigned int it = 0; it < tilesWantPlace.size() ; it++) {
			if(tilesWantPlace[it] == '?')
				continue;

			if(startRow + i > boardPtr->getRows() || startRow + i < 1)
				break;

			while( startRow + i <= boardPtr->getRows() && boardPtr->getSquare(startRow + i, startCol)->isOccupied()) {
				recorder.push_back(make_pair(1, true));
				i++;
			}
			int letterBonus = boardPtr->getSquare(startRow + i, startCol)->getLMult();
			int wordBonus  = boardPtr->getSquare(startRow + i, startCol)->getWMult();
			if (letterBonus == 1 and wordBonus == 1)
				recorder.push_back(make_pair(1, false));
			else if (letterBonus == 1)
				recorder.push_back(make_pair(10+wordBonus, false));
			else
				recorder.push_back(make_pair(letterBonus, false));
			i++;
		}
		while (startRow + i <= boardPtr->getRows() &&  boardPtr->getSquare(startRow+i, startCol)->isOccupied()) {
			recorder.push_back(make_pair(1, true));
			i++;
		}
	}
	
	bonusRecord = recorder;
}

int PlaceMove::getScore() {
	int score = 0;
	int wordBonus = 1;

	vector<int> wordScore = getWordScore(startRow, startCol, isHorizontal, 1);
	if (wordScore.size() <= 1)
		score = 0;
	else {
		for (unsigned int i = 0; i<wordScore.size(); i++) {
			if(bonusRecord[i].first < 10 )
				score += bonusRecord[i].first * wordScore[i];
			else {
				score += wordScore[i];
				wordBonus *= (bonusRecord[i].first - 10);
			}
		}
		score = wordBonus*score;
	}

	cout << "New Word: "<<getWord(isHorizontal, startRow, startCol, "") << ", Score: " << score << endl;
	if (isHorizontal) {
		int i = -1;
		while ( startCol+i >= 1 && boardPtr->getSquare(startRow, startCol+i)->isOccupied())
			i--;
		i++;
		for (unsigned int it = 0; it<bonusRecord.size(); it++) {	
			int wordBonus = 1;
			if(!bonusRecord[it].second) {
				vector<int> wordScore;
				if(bonusRecord[it].first < 10) 
					wordScore = getWordScore(startRow, startCol+i, false, bonusRecord[it].first);
				else {
					wordBonus *= (bonusRecord[it].first - 10);
					wordScore = getWordScore(startRow, startCol+i, false, 1);
				}
				if(wordScore.size() > 1) {
					int tempScore = 0;
					for (unsigned int i = 0; i<wordScore.size(); i++)
						tempScore += wordScore[i];

					cout << "New Word: "<< getWord(!isHorizontal, startRow, startCol+i, "") << ", Score: " << tempScore << " Bonus: " << wordBonus<<endl;
					score += wordBonus*tempScore;
				}
			}
			i++;
		}
	} else {
		int i = -1;
		while( startRow + i >= 1 && boardPtr->getSquare(startRow+i, startCol)->isOccupied())
			i--;
		i ++;
		for (unsigned int it = 0; it<bonusRecord.size(); it++) {
			int wordBonus = 1;
			if(!bonusRecord[it].second) {
				vector<int> wordScore;
				if(bonusRecord[it].first < 10) 
					wordScore = getWordScore(startRow+i, startCol, true, bonusRecord[it].first);
				else {
					wordBonus *= (bonusRecord[it].first - 10);
					wordScore = getWordScore(startRow+i, startCol, true, 1);
				}
				if(wordScore.size() > 1) {
					int tempScore = 0;
					for (unsigned int i = 0; i<wordScore.size(); i++)
						tempScore += wordScore[i];

					cout<<"New Word " << getWord(!isHorizontal, startRow+i, startCol, "") << ", Score: " << tempScore << " Bonus: " << wordBonus << endl;
					score += wordBonus*tempScore;
				}
			}
			i++;
		}
	}
	cout << "Round's Score: " << score << endl;
	return score;
}

vector<int> PlaceMove::getWordScore(size_t row, size_t col, bool isHorizontal, int bonus) {
	vector<int> score ;
	if(isHorizontal) {
		int i = -1;
		while ( col+i >= 1 and boardPtr->getSquare(row, col + i)->isOccupied())
			i--;
		i++;
		while(col+i <=boardPtr->getColumns() && boardPtr->getSquare(row, col + i)->isOccupied()) {
			if(i==0 and bonus< 10)
				score.push_back( bonus*boardPtr->getSquare(row, col + i)->getScore() );
			else
				score.push_back( boardPtr->getSquare(row, col + i)->getScore());
			i++;
		}
	} else {
		int i = -1;
		while ( row+i >= 1 and boardPtr->getSquare(row+i, col)->isOccupied())
			i--;
		i++;

		while(row+i <=boardPtr->getRows() and boardPtr->getSquare(row+i, col)->isOccupied()) {
			if(i==0 and bonus< 10)
				score.push_back(bonus*boardPtr->getSquare(row+i, col)->getScore() );
			else
				score.push_back(boardPtr->getSquare(row+i, col)->getScore());
			i++;
		}
	}
	return score;
}