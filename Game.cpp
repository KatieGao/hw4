#include "gameHeaderFiles.h"

using namespace std;

Game::Game(string configFile, int playerNum) {
	readConfigFile(configFile);
	
	tilesBag = new Bag(tileFile, seed);
	
	currentPlayers = new Table(playerNum, handSize);

	wordReference = new Dictionary(dictFile);

	board = new Board(boardFile);

	displayGame = new ConsolePrinter();
}	

Game::~Game() {
	delete tilesBag;
	delete currentPlayers;
	delete wordReference; 
	delete board;
	delete displayGame;
}

void Game::runGame() {
	for (unsigned int i = 0; i < currentPlayers->getTableSize(); i++) {
		Player* playerr = currentPlayers->accessPlayer(i);
		playerr->addTiles(tilesBag->drawTiles(handSize));
	}

	int totalMoves = 0;
	unsigned int passCounter = 0;
	while (true) {
		bool isValid ;
		Player* playerr = currentPlayers->accessPlayer(totalMoves);
		do {
			displayGame->printBoard(*board);
			displayGame->printHand(*playerr);

			string command;

			cout << playerr->getID() << "'s Turn:" << endl;
			cout<<"Please input command: \n";
			
			getline(cin, command);
			
			cout << "Entered Command :: " << command << endl;
			if(command == "") {
				cout << "EOF, Game terminated\n";
				exit(1);
			}
			if (command == "PASS" || command == "pass")
				passCounter ++;
			else
				passCounter = 0;

			isValid = playerr->makeMove(command, tilesBag, wordReference, board);

		} while (!isValid);

		totalMoves ++;
		cout<<"Total score for player: "<<playerr->getID()<<" is "<<playerr->getScore()<<endl;
		if(playerr->getHandTiles().size() == 0 || passCounter >= currentPlayers->getTableSize())
			break;

	}
	currentPlayers->showScoreBoard();
}


void Game::readConfigFile(string configFile) {
	fstream config;
	config.open(configFile);
	if (!config) {
		cout << "Unable to open the file\n";
		exit(-1);
	}
	string delimiter = ":";
	for (string temp = ""; getline(config, temp); ) {
		int index = 0;
		temp = stripSpaces(temp);
		switch(temp[0]) {
			case 'H':
				index = temp.find(delimiter);
				handSize = stoi(temp.substr(index+1));
				break;
			case 'D':
				index = temp.find(delimiter);				
				dictFile = temp.substr(index+1);
				break;
			case 'T':
				index = temp.find(delimiter);
				tileFile = temp.substr(index+1);
				break;
			case 'B':
				index = temp.find(delimiter);
				boardFile = temp.substr(index+1);
				break;
			case 'S':
				index = temp.find(delimiter);
				seed = stoi(temp.substr(index+1));
				break;
			default:
				break;

		}

	}
}

string Game::stripSpaces(string nameWithSpace) {

	nameWithSpace.erase(remove(nameWithSpace.begin(), nameWithSpace.end(), ' '), nameWithSpace.end());

	nameWithSpace.erase(remove(nameWithSpace.begin(), nameWithSpace.end(), '\t'), nameWithSpace.end());

	return nameWithSpace;
}

void Game::displayGameConfig() {
	cout << "GAME CONFIG INFO: \n";
	cout << "HANDSIZE:[" <<  handSize << endl;
	cout << "TILES:[" << tileFile << endl;
	cout << "DICTIONARY:[" << dictFile << endl;
	cout << "BOARD:[" << boardFile << endl;
	cout << "SEED:[" << seed << endl;
}


