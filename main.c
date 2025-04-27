#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int height = 10;
int width = 10;
int selectedTotal = 0;

enum gameState {
	PLAYING,
	LOSE,
	WIN
};

int min8(int8_t a, int8_t b) {
	return a < b ? a : b;
}

int max8(int8_t a, int8_t b) {
	return a > b ? a : b;
}

enum gameState show(int x, int y, int8_t board[height][width], int8_t selected[height][width]) {
	enum gameState result = PLAYING;
	if(selected[y][x] == 1)
		return PLAYING;

	selected[y][x] = 1;
	selectedTotal += 1;

	if (board[y][x] == 9) {
		result = LOSE;
	}
	if (board[y][x] == 0) {
		if (x > 0) {
			show(x - 1, y, board, selected);
			if (y > 0)
				show(x - 1, y - 1, board, selected);
			if (y < height - 1)
				show (x - 1, y + 1, board, selected);
		}
		if (x < width - 1){
			show(x + 1, y, board, selected);
			if (y > 0) 
				show(x + 1, y - 1, board, selected);
			if (y < height - 1)
				show(x + 1, y + 1, board, selected);
		}
		if (y > 0) 
			show(x, y - 1, board, selected);
		if (y < height - 1)
			show(x, y + 1, board, selected);
	}
	return result;
};

int main(void) {
	enum gameState state = PLAYING;
	int mines = 8;
	int8_t board[height][width];
	int8_t selected[height][width];
	// board doesnt initialize to all zeroes so whateva
	for (int i = 0; i < height; i++) {
		for (int j = 0;j < width; j++) {
			board[j][i] = 0;
		}
	}
	for (int i = 0; i < height; i++) {
		for (int j = 0;j < width; j++) {
			selected[j][i] = 0;
		}
	}
	// random number initialization
	int seed = (int)time(NULL);
	srand(seed);
	printf("%d\n", seed);

	// bomb generation with resampling for good randomness
	int bombX, bombY;
	for (int i = 0; i < mines; i++) {
		bombX = rand() % width;
		bombY = rand() % height;
		// resample if we hit an existing bomb
		if (board[bombY][bombX] == 9) {
			i--;
			continue;
		}

		// this is some horrible logic for getting all the boxes in a 1 radius
		board[bombY][bombX] = 9;
		if (bombY > 0 && bombX > 0) {
			board[bombY - 1][bombX - 1] = board[bombY - 1][bombX - 1] == 9 ? 9 : board[bombY - 1][bombX - 1] + 1;
		}
		if (bombY > 0) {
			board[bombY - 1][bombX] = board[bombY - 1][bombX] == 9 ? 9 : board[bombY - 1][bombX] + 1;
		}
		if (bombY > 0 && bombX < width - 1) {
			board[bombY - 1][bombX + 1] = board[bombY - 1][bombX + 1] == 9 ? 9 : board[bombY - 1][bombX + 1] + 1;
		}
		if (bombY < height - 1 && bombX > 0) {
			board[bombY + 1][bombX - 1] = board[bombY + 1][bombX - 1] == 9 ? 9 : board[bombY + 1][bombX - 1] + 1;
		}
		if (bombY < height - 1) {
			board[bombY + 1][bombX] = board[bombY + 1][bombX] == 9 ? 9 : board[bombY + 1][bombX] + 1;
		}
		if (bombY < height - 1 && bombX < width - 1) {
			board[bombY + 1][bombX + 1] = board[bombY + 1][bombX + 1] == 9 ? 9 : board[bombY + 1][bombX + 1] + 1;
		}
		if (bombX > 0) {
			board[bombY][bombX - 1] = board[bombY][bombX - 1] == 9 ? 9 : board[bombY][bombX - 1] + 1;
		}
		if (bombX < width - 1) {
			board[bombY][bombX + 1] = board[bombY][bombX + 1] == 9 ? 9 : board[bombY][bombX + 1] + 1;
		}
	}

	

	int selectedX, selectedY;
	while (state == PLAYING) {
		// show the board
		for (int i = 0; i < height; i++) {
			for (int j = 0;j < width; j++) {
				if (selected[j][i] == 1) {
					printf("%d ", board[j][i]);
				} else {
					printf("= ");
				}	
			}
			printf("\n");
		}
		printf("\n");
		// get the next input
		scanf("%d %d", &selectedY, &selectedX);
		state = show(selectedX, selectedY, board, selected);
		if (selectedTotal + mines == height * width) {
			state = WIN;
		}
	}

	if (state == WIN) {
		printf("Congrats! You won!\n");
	} else {
		printf("Aw! There was a mine on (%d, %d)\n", selectedY, selectedX);
	}
}
