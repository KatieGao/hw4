all:
	g++ -g -Wall --std=c++11 Game.cpp main.cpp Bag.cpp Table.cpp Dictionary.cpp Player.cpp Board.cpp ConsolePrinter.cpp Move.cpp -o scrabble