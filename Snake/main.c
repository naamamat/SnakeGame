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

unsigned int width = 0;
unsigned int height = 0;


struct Node {
	//char* placeinBoard;
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
	// Clears the screen
	//printf("\033[H\033[J");

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

	printf("x = %d, y = %d, count av = %d\n", x, realY,countAvailable);

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
	//here

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
		//snake->head->x 
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
		lastTail->x = xLastTail;
		lastTail->y = yLastTail;
		lastTail->next = snake->tail;
		snake->tail = lastTail;

		//snake->head->next = lastTail;
		//snake->head = lastHead;
		snake->len += 1;

	}

	return isCol;

}




bool statusGame(struct Snake* snake, char** board) {
	if (snake->head->x >= height - 1 || snake->head->x <= 0 || snake->head->y >= width - 1 || snake->head->y <= 0) {
		return false;
	}
	return true;
}




bool game() {
	char** board = initGame();
	if (!board) {
		printf("init game failed\n");
		return -1;
	}

	struct Snake* snake = initSnake(board);
	if (!snake) {
		printf("init snake failed\n");
		return -1;
	}
	printScreen(board);
	bool collision = false;
	board[height / 2 + 2][width / 2] = '*';
	char dir = 'D';
	while (!collision && statusGame(snake, board)) {
		printf("Last input: %c\n", dir);
		Sleep(3000);

		if (_kbhit()) {
			dir = _getch();
			dir = _getch();
			printf("_getch: c: %c d: %d\n", dir, dir);

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
				break;
			}

		}
		setStar(board);

		collision = step(board, dir, snake);
		//isEat = false;
		setSnakeOnBoard(snake, board);

		printScreen(board);
		printf("%d\n", (statusGame(snake, board) & !collision));
	}
}


/*
==============================================
 Name        : SwapValues
 Description : Swaps two integers using pointers
==============================================
*/
#include <stdio.h>

void swap(int* a, int* b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}


int main()
{

	//int x = 5, y = 10;
	//printf("Before swap: x = %d, y = %d\n", x, y);
	//swap(&x, &y);
	//printf("Before swap: x = %d, y = %d\n", x, y);
	
	game();



	return 0;
}