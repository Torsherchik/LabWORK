#include <iostream>


using namespace std;
enum Status {
	PLAY,            // ���� ������������
	USER_WIN,        // ����� �������
	BOT_WIN,         // ��� �������
	NOT_WIN          // �����. ���������� ���, �� � �� ���� ��� ��������� ������
};

struct Game {
	char bord[3][3];  // ������� ����
	bool isUserTurn;  // ��� ���. ���� ������������, �� isUserTurn = true
	char userChar;    // ������ ������� ������ ������������
	char botChar;     // ������ ������� ������ ���
	Status status;
};

Game initGame(char userChar) {
	Game game;
	//�������� �������
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			game.bord[i][j] = ' ';
		}
	}
	game.isUserTurn = true;//���������� ��� ���
	game.userChar = userChar; //������������� ������ ��� ������ (������� ���������� userChar)
	switch (game.userChar) { //������������� ������ ����
	case '0':
		game.botChar = 'x';
		break;
	case 'x':
		game.botChar = '0';
		break;
	}
	return game;
}

void updateDisplay(const Game game) {
	system("cls");
	cout << "0    1    2" << endl;
	cout << "--------------" << std::endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << game.bord[i][j] << "  | ";
		}
		cout << i << endl << "--------------" << endl;
	}
}

/*
��������� ��� ����. � ��������� ������ ��������������� ������ ������� ������ ���.
��� ������ ���������� ������, ������� ��� ��������� � ������� � ������ ������ ����� �������/������� � ������� ���� ���� ������. ���� �� ���� ��� ��� �����, ��� ������ ������� ���� ���� � �����. � ��������� ������� ��� ����� ��������.
*/

void botTurn(Game* game) {
	int absence = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (game->bord[i][j] != ' ') {
				absence++;
				if (absence > 1)
					break;
			}
		}
	}
	if (absence == 1) game->bord[1][1] = game->botChar;
	else {
		for (int i = 0; i < 3; i++) {
			int def = 0;
			int cl = 0;
			bool flag = false;
			for (int j = 0; j < 3; j++) {
				if (game->bord[i][j] == game->userChar) {
					cl++;
				}
				else if (game->bord[i][j] != game->botChar) {
					def = j;
					flag = true;
				}
			}
			if ((cl == 2) && flag) {
				game->bord[i][def] = game->botChar;
				return;
			}
		}
		
		for (int j = 0; j < 3; j++) {
			int def = 0;
			int cl = 0;
			bool flag = false;
			for (int i = 0; i < 3; i++) {
				if (game->bord[i][j] == game->userChar) {
					cl++;
				}
				else if (game->bord[i][j] != game->botChar) {
					def = i;
					flag = true;
				}
			}
			if ((cl == 2) && flag) {
				game->bord[def][j] = game->botChar;
				return;
			}
		}

		{
			int def = 0;
			int cl = 0;
			bool flag = false;
			for (int i = 0; i < 3; i++) {
				if (game->bord[i][i] == game->userChar) {
					cl++;
				}
				else if (game->bord[i][i] != game->botChar) {
					def = i;
					flag = true;
				}
			}
			if ((cl == 2) && flag) {
				game->bord[def][def] = game->botChar;
				return;
			}
		}

		{
			int def = 0;
			int cl = 0;
			bool flag = false;
			for (int i = 0; i < 3; i++) {
				if (game->bord[i][2 - i] == game->userChar) {
					cl++;
				}
				else  if (game->bord[i][2 - i] != game->botChar) {
					def = i;
					flag = true;
				}
			}
			if ((cl == 2) && flag) {
				game->bord[def][2 - def] = game->botChar;
				return;
			}
		}

		int i = 0, j = 0;
		while (game->bord[i][j] != ' ') {
			i = rand() % 3;
			j = rand() % 3;
		}
		game->bord[i][j] = game->botChar;
	}
}




void userTurn(Game* game) {
	int i, j;
	while (1) {
		cout << "Enter the x and y coordinates, respectively:";
		cin >> i >> j;

		if (i < 0 || j < 0 || i > 3 || j > 3 || &game->bord[i][j] == " ")
			cout << "Retry! ";
		else {
			game->bord[j][i] = game->userChar;
			break;
		}


	}
}
/*
������� ���������� ��� ���������� ��������� ���� ����� ���������� ����.
������� ��������� ����� ��������� ���� � ��������� game � ������� ��� �������
������.
������� ���������� true, ���� ���� ���������� ��� �����, ����� false.
*/

bool updateGame(Game* game) {
	int UserTriplet[4] = { 0,0,0,0 };
	int BOTTriplet[4] = { 0,0,0,0 };
	for (int i = 0; i < 3; i++) {
		UserTriplet[0] = 0;
		BOTTriplet[0] = 0;
		UserTriplet[1] = 0;
		BOTTriplet[1] = 0;
		for (int j = 0; j < 3; j++) {
			if (game->bord[i][j] != ' ') {
				game->bord[i][j] == game->userChar ? UserTriplet[0]++ : BOTTriplet[0]++;
			}
			if (game->bord[j][i] != ' ') {
				game->bord[j][i] == game->userChar ? UserTriplet[1]++ : BOTTriplet[1]++;
			}
		}
		if (game->bord[i][i] != ' ') {
			game->bord[i][i] == game->userChar ? UserTriplet[2]++ : BOTTriplet[2]++;
		}
		if (game->bord[i][2 - i] != ' ') {
			game->bord[i][2 - i] == game->userChar ? UserTriplet[3]++ : BOTTriplet[3]++;
		}
	}
	if ((UserTriplet[0] == 3) || (BOTTriplet[0] == 3) || (UserTriplet[1] == 3) || (BOTTriplet[1] == 3) || (UserTriplet[2] == 3) || (BOTTriplet[2] == 3) || (UserTriplet[3] == 3) || (BOTTriplet[3] == 3)) {
		game->status = ((UserTriplet[0] == 3 || UserTriplet[1] == 3 || UserTriplet[2] == 3 || UserTriplet[3] == 3) ? USER_WIN : BOT_WIN); // ����������� ����������
		return true; // ���� ����������
	}
	int DrawIndicator = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (game->bord[i][j] != ' ') {
				DrawIndicator++;
			}
		}
	}
	if (DrawIndicator == 9) {
		game->status = NOT_WIN;
		return true; //�����
	}
	game->status = PLAY;
	return false; // ���������� ����������

}