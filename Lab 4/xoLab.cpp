#include <iostream>
#include "xoHead.h"
#include <ctime> 

using namespace std;

int main() {
	cout << "Game start" << endl;
	srand(std::time(NULL));
	char letter;
	while (true) {
		std::cout << "Choose a character to play: (x or 0)" << std::endl;
		std::cin >> letter;
		if (letter == '0' || letter == 'x') break;
	}



	Game game = initGame(letter);

	if (game.isUserTurn) {
		while (true) {
			userTurn(&game);
			updateDisplay(game);
			if (updateGame(&game)) {
				break;
			}
			botTurn(&game);
			updateDisplay(game);
			if (updateGame(&game)) {
				break;
			}
		}
	}
	else {
		while (true) {
			botTurn(&game);
			updateDisplay(game);
			if (updateGame(&game)) {
				break;
			}
			userTurn(&game);
			updateDisplay(game);
			if (updateGame(&game)) {
				break;
			}
		}
	}
	switch (game.status) {
	case USER_WIN:
		updateDisplay(game);
		cout << "Your win";
		break;
	case BOT_WIN:
		updateDisplay(game);
		cout << "Bot win";
		break;
	case NOT_WIN:
		updateDisplay(game);
		cout << "Drow";
		break;
	}
	return  0;
}
