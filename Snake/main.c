#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h> 

#include <conio.h>
#include <windows.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77
#define WAITING_GAME 300
#define WAITING_OPENING 1000
#define CLEAR "\033[H\033[J" // Clears the screen
#define LEN_WIN width * 2

unsigned int width = 0;
unsigned int height = 0;


struct Node {
	int x;
	int y;
	struct Node* next;

}Node;

struct Snake {
	char direction;
	int len;
	struct Node* tail;
	struct Node* head;

}Snake;

void printScreen(char **board)
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			printf("%c", board[i][j]);
		}
		printf("\n");
	}
}

char** initGame() {

	//Set board:
	printf("Enter width: \n");
	scanf("%u", &width);
	if (width <= 2) {
		printf("width must be higher than 2\n");
		return NULL;
	}
	printf("Enter height: \n");
	scanf("%u", &height);
	if (height <= 2) {
		printf("height must be higher than 2\n");
		return NULL;
	}

	char** board = (char**)malloc(sizeof(char*) * height);
	if (!board) {
		printf("board malloc failed error\n");
		return NULL;
	}
	for (int i = 0; i < height; i++) {
		char* row = (char*)malloc(sizeof(char) * width);
		if (!row) {
			printf("row malloc failed error\n");
			for (int j = 0; j < i; j++) {
				free(board[j]);
			}
			free(board);
			return NULL;
		}
		board[i] = row;
	}

	for (int i = 0; i < width; i++) {
		board[0][i] = '-';
	}

	for (int i = 1; i < height - 1; i++) {
		board[i][0] = '|';
		for (int j = 1; j < width - 1; j++) {
			board[i][j] = ' ';
		}
		board[i][width - 1] = '|';
	}

	for (int i = 0; i < width; i++) {
		board[height - 1][i] = '-';
	}

	return board;
}

struct Snake* initSnake(char** board) {

	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
	if (!node) {
		return NULL;
	}
	node->x = height / 2;
	node->y = width / 2; 
	node->next = NULL;


	board[node->x][node->y] = 'Q';

	struct Snake* snake = (struct Snake*)malloc(sizeof(struct Snake));
	if (!snake) {
		return NULL;
	}
	snake->direction = 'D';
	snake->head = node;
	snake->tail = node;
	snake->len = 1;

	return snake;

}


void setStar(char** board) {
	srand(time(NULL));

	int countAvailable = 0;
	int x = 0;
	while (!countAvailable) {
		x = 1 + rand() % (height - 2);
		countAvailable = 0;
		for (int i = 1; i <= width - 2; i++) {
			if (board[x][i] == ' ') {
				countAvailable++;
			}
		}
	}
	int y = 1 + rand() % (countAvailable);
	int realY = 1;

	while (y > 1 && realY < width - 2) {
		while (realY < width - 2 && board[x][realY] != ' ') {
			realY++;
		}
		realY++;
		y--;
	}

	while (realY < width - 2 && board[x][realY] != ' ') {
		realY++;
	}
	if (realY > width - 2) {
		printf("Error setting star");
		return;
	}


	board[x][realY] = '*';
}
bool isEatStar(struct Snake* snake, char** board) {
	if (board[snake->head->x][snake->head->y] == '*') {
		return true;
	}
	return false;
}

bool isCollision(struct Snake* snake, char** board) {
	if (board[snake->head->x][snake->head->y] == 'O') {
		return true;
	}
	return false;
}

void setSnakeOnBoard(struct Snake* snake, char** board) {
	if ((snake->head != NULL)
		&& (snake->head->x < height) 
		&& (snake->head->y < width)) {
		board[snake->head->x][snake->head->y] = 'Q';
	}

}

bool step(char** board, char direction, struct Snake* snake) {
	char prevHead = (snake->len == 1) ? ' ' : 'O';
	board[snake->head->x][snake->head->y] = prevHead;
	struct Node* prevTail = snake->tail;

	if (snake->tail->next != NULL) {
		snake->tail = snake->tail->next;
	}
	int xLastTail = prevTail->x;
	int yLastTail = prevTail->y;

	prevTail->next = NULL;
	prevTail->x = snake->head->x;
	prevTail->y = snake->head->y;

	if (direction == 'D') {
		prevTail->x += 1;
	}
	else if (direction == 'U') {
		prevTail->x -= 1;
	}
	else if (direction == 'R') {
		prevTail->y += 1;
	}
	else if (direction == 'L') {
		prevTail->y -= 1;
	}

	snake->head->next = prevTail;
	snake->head = prevTail;
	snake->head->next = NULL;
	bool isEat = (isEatStar(snake, board)) ? true : false;
	bool isCol = isCollision(snake, board);
	board[xLastTail][yLastTail] = (isEat) ? 'O' : ' ';

	struct Node* lastTail = NULL;
	if (isEat) {
		lastTail = malloc(sizeof(Node));
		if (lastTail != NULL) {
			lastTail->x = xLastTail;
			lastTail->y = yLastTail;
			lastTail->next = snake->tail;
		}
		snake->tail = lastTail;
		setStar(board);
		snake->len += 1;
	}

	return isCol;

}




bool borderCollision(struct Snake* snake, char** board) {
	if (snake->head->x >= height - 1 || snake->head->x <= 0 || snake->head->y >= width - 1 || snake->head->y <= 0) {
		return false;
	}
	return true;
}

void printOpening(char** board) {
	printf(CLEAR);
	printScreen(board);
	printf("Are you ready?");
	Sleep(WAITING_OPENING);
	for (int i = 3; i >= 1; i--) {
		printf(CLEAR);
		printScreen(board);
		printf("%d", i);
		Sleep(WAITING_OPENING);
	}
	printf(CLEAR);
	printScreen(board);
	printf("Let's go!!!!\n");
	Sleep(WAITING_OPENING);
}

void freeGame(char** board, struct Snake* snake) {
	if (board != NULL) {
		for (int i = 0; i < height; i++) {
			free(board[i]);
		}
		free(board);
	}


	while (snake != NULL && snake->tail != NULL) {
	struct Node* cur = snake->tail;
		snake->tail = snake->tail->next;
		free(cur);
	}
	free(snake);
}


void game(char** board, struct Snake* snake) {
	printOpening(board);
	setStar(board);
	char dir = 'D';
	bool selfCollision = false;
	bool win = false;
	while (!win && !selfCollision && borderCollision(snake, board)) {
		printf("Direction: %c\n", dir);
		Sleep(WAITING_GAME);

		if (_kbhit()) {
			char lastDir = dir;
			dir = _getch();
			if (dir == -32) {
				dir = _getch();

				switch (dir) {
				case KEY_UP:
					dir = 'U';
					break;
				case KEY_DOWN:
					dir = 'D';
					break;
				case KEY_LEFT:
					dir = 'L';
					break;
				case KEY_RIGHT:
					dir = 'R';
					break;
				default:
					dir = lastDir;
					break;
				}
			}
			else if (dir == 27) { // ESC key
				return 0;
			}
			else {
				dir = lastDir;
			}
		}
		selfCollision = step(board, dir, snake);
		setSnakeOnBoard(snake, board);
		printf(CLEAR);
		printScreen(board);	
		printf("%d/%d\n", snake->len, LEN_WIN);
		if (snake->len >= LEN_WIN) {
			win = true;
		}

	}
	char* statusGame = (win)? "you win!" : "GameOver!";
	printf("%s\n", statusGame);
	return 0;
}

int main() {
	
	char** board = initGame();
	struct Snake* snake = initSnake(board);

	if (board != NULL && snake != NULL) {
		game(board, snake);
	}
	else {
		printf("init failed\n");
		freeGame(board, snake);
		return -1;
	}
	freeGame(board, snake);
	return 0;
}