#include "gameHeaderFiles.h"
using namespace std;

int main(int argc, char* argv[]) {
	if (argc != 2) {
		cout << "Argumnet Error! \n";
		return -1;
	}

	string configFilePath = argv[1];
	unsigned int playerNum = 4;

	cout << "Number of Players\n";
	cin >> playerNum;
	cin.ignore();
	Game scrabble(configFilePath, playerNum);
	scrabble.runGame();
	
}