#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define SPACE	0
#define RED		1
#define BLUE	2
#define GREEN	3
#define PADDING	6
#define MAX_DEPTH 2

// Macros
#define getPiece(r,c) board[skipPadding + r + c * rows]
#define boardSize(x) (x - PADDING);

// Function Prototypes

void readboard(void);
void freeboard(void);
void printboard(void);
int getTop(int column);
void addPiece(int col, int colour);
void remPiece(int col);
int burninate(int player, int depth, int origDepth);
int isWin(int lastColumn);

// Global variables
int *board;
int columns;
int rows;
int last_move;
int total_time;
int player_1_time;
int last_move_time;
int skipPadding;
int *columnHeight;
char pieces[4] = { 's', 'r', 'b', 'g' };
int almostPoints[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 256, 0, 0, 256, 512, 0, 0, 0, 0, 0, 2, 0, 1, 0, 0, 512, 256, 0, 1,
		256, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 2, 0, 0, 0, 2, 0,
		0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 256, 0, 0, 256, 512, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 512, 0, 0, 0, 0, 0, 0, 0, 0, 512, 0, 0, 0, 0,
		0, 256, 256, 0, 0, 0, 0, 256, 0, 0, 0, 256, 0, 0, 0, 0, 0, 0, 0, 0, 2,
		0, 1, 0, 0, 512, 256, 0, 1, 256, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0,
		1, 0, 0, 0, 0, 0, 256, 256, 0, 0, 0, 0, 256, 0, 0, 0, 256, 0, 0, 0, 0,
		2, 512, 0, 2, 0, 0, 0, 512, 0, 0, 0, 0, 0, 0, 0 };
int points[256] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 3, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1280, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		768, 0, 0, 1024, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 1024, 0, 0, 768, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 1280, 0, 0, 0,
		0, 0 };

/**
 * Reads in the board and other variables from standard in.
 */
void readboard(void) {
	int i, j;
	char tmpChar = 0;

	scanf("(%d,%d,%d,%d,%d,%d", &columns, &rows, &last_move, &total_time,
			&player_1_time, &last_move_time);
	columnHeight = (int *) calloc(sizeof(int), columns);
	rows += PADDING; // Make board bigger for padding
	columns += PADDING;
	skipPadding = 3 * rows + 3;
	board = (int *) calloc(sizeof(int), rows * columns); // Need to pad the board with spaces
	for (i = 0; i < columns; i++)
		for (j = 0; j < rows; j++) {
			if (i < 3 || i > columns - 4 || j < 3 || j > rows - 4) {
				board[i * rows + j] = SPACE;
			} else {
				scanf(",%c", &tmpChar);

				switch (tmpChar) {
				case 's':
					board[i * rows + j] = SPACE;
					break;
				case 'r':
					columnHeight[i - 3] = j - 2;
					board[i * rows + j] = RED;
					break;
				case 'b':
					columnHeight[i - 3] = j - 2;
					board[i * rows + j] = BLUE;
					break;
				case 'g':
					columnHeight[i - 3] = j - 2;
					board[i * rows + j] = GREEN;
				}
			}
		}
	if (last_move)
		last_move--; // First column is zero, not one
}

/**
 * Frees the board
 */
void freeboard(void) {
	free(board);
	free(columnHeight);
}

/**
 * Prints out the board to standard out.
 * For testing purposes.
 */
void printBoard() {
	int i, j;
	//	fprintf(stderr, "From input:\n");
	//
	//	for (i = 0; i < columns - PADDING; i++) {
	//		for (j = 0; j < rows - PADDING; j++) {
	//			printf("%c", pieces[getPiece(j,i)]);
	//		}
	//	}
	//	fprintf(stderr, "\nTop Pieces:\n");
	//	for (i = 0; i < columns - PADDING; i++) {
	//		printf("%d ", columnHeight[i]);
	//	}
	//	fprintf(stderr, "\n\n");
	//	for (i = rows - 1; i >= 0; i--) {
	//	 for (j = 0; j < columns; j++) {
	//	 printf("%c ", pieces[board[i + j * rows]]);
	//	 }
	//	 printf("\n");
	//	 }
	//	 printf("\n\n");
	for (i = rows - 7; i >= 0; i--) {
		for (j = 0; j < columns - PADDING; j++) {
			fprintf(stderr, "%c ", pieces[getPiece(i,j)]);
		}
		fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n\n");
}

/**
 * Takes in the last column played
 * Returns an int value of points scored from the board.
 * Negative values are points for the other team.
 */
int isWin(int lastColumn) {
	int i, a, b, c, d;
	int lastRow;
	int left, right, top, bot;
	unsigned int redPoints = 0, bluePoints = 0, best = 0;
	//possible wins, values from -5 to 5
	unsigned int possible[13], winPoints;
	// Find height of last piece played, may hit padding
	for (i = 0;; i++) {
		//printf("%d ",board[i+lastColumn*rows]);
		if (getPiece(i,lastColumn) == SPACE) {
			lastRow = i - 1;
			break;
		}
	}
	//printf("last piece played at (%d,%d)\n", lastColumn, lastRow);
	//printf("Total of %d columns, %d rows\n", columns, rows);
	// Find boundaries to search, row 0 is bottom row, column 0 is left column
	top = lastRow + 3;
	bot = lastRow - 3;
	left = lastColumn - 3;
	right = lastColumn + 3;

	//printf("Top: %d, Bottom: %d, Right: %d, Left: %d\n", top, bot, right, left);

	// Search vertical
	a = getPiece(lastRow,lastColumn);
	b = getPiece(lastRow-1,lastColumn) << 2;
	c = getPiece(lastRow-2,lastColumn) << 4;
	d = getPiece(lastRow-3,lastColumn) << 6;
	possible[0] = points[a + b + c + d];

	// Search horizontal
	a = getPiece(lastRow,(left));
	b = getPiece(lastRow,(left + 1)) << 2;
	c = getPiece(lastRow,(left + 2)) << 4;
	d = getPiece(lastRow,(left + 3)) << 6;
	possible[1] = points[a + b + c + d];

	a = getPiece(lastRow,(left + 1));
	b = getPiece(lastRow,(left + 2)) << 2;
	c = getPiece(lastRow,(left + 3)) << 4;
	d = getPiece(lastRow,(left + 4)) << 6;
	possible[2] = points[a + b + c + d];

	a = getPiece(lastRow,(left + 2));
	b = getPiece(lastRow,(left + 3)) << 2;
	c = getPiece(lastRow,(left + 4)) << 4;
	d = getPiece(lastRow,(left + 5)) << 6;
	possible[3] = points[a + b + c + d];

	a = getPiece(lastRow,(left + 3));
	b = getPiece(lastRow,(left + 4)) << 2;
	c = getPiece(lastRow,(left + 5)) << 4;
	d = getPiece(lastRow,(left + 6)) << 6;
	possible[4] = points[a + b + c + d];

	// Search diagonal from top left to bottom right
	a = getPiece((lastRow + 0),(lastColumn - 0));
	b = getPiece((lastRow + 1),(lastColumn - 1)) << 2;
	c = getPiece((lastRow + 2),(lastColumn - 2)) << 4;
	d = getPiece((lastRow + 3),(lastColumn - 3)) << 6;
	possible[5] = points[a + b + c + d];

	a = getPiece((lastRow - 1),(lastColumn + 1));
	b = getPiece((lastRow + 0),(lastColumn - 0)) << 2;
	c = getPiece((lastRow + 1),(lastColumn - 1)) << 4;
	d = getPiece((lastRow + 2),(lastColumn - 2)) << 6;
	possible[6] = points[a + b + c + d];

	a = getPiece((lastRow - 2),(lastColumn + 2));
	b = getPiece((lastRow - 1),(lastColumn + 1)) << 2;
	c = getPiece((lastRow + 0),(lastColumn - 0)) << 4;
	d = getPiece((lastRow + 1),(lastColumn - 1)) << 6;
	possible[7] = points[a + b + c + d];

	a = getPiece((lastRow - 3),(lastColumn + 3));
	b = getPiece((lastRow - 2),(lastColumn + 2)) << 2;
	c = getPiece((lastRow - 1),(lastColumn + 1)) << 4;
	d = getPiece((lastRow + 0),(lastColumn - 0)) << 6;
	possible[8] = points[a + b + c + d];

	// Search diagonal from top right to bottom left
	a = getPiece((lastRow + 0),(lastColumn + 0));
	b = getPiece((lastRow + 1),(lastColumn + 1)) << 2;
	c = getPiece((lastRow + 2),(lastColumn + 2)) << 4;
	d = getPiece((lastRow + 3),(lastColumn + 3)) << 6;
	possible[9] = points[a + b + c + d];

	a = getPiece((lastRow - 1),(lastColumn - 1));
	b = getPiece((lastRow + 0),(lastColumn + 0)) << 2;
	c = getPiece((lastRow + 1),(lastColumn + 1)) << 4;
	d = getPiece((lastRow + 2),(lastColumn + 2)) << 6;
	possible[10] = points[a + b + c + d];

	a = getPiece((lastRow - 2),(lastColumn - 2));
	b = getPiece((lastRow - 1),(lastColumn - 1)) << 2;
	c = getPiece((lastRow + 0),(lastColumn + 0)) << 4;
	d = getPiece((lastRow + 1),(lastColumn + 1)) << 6;
	possible[11] = points[a + b + c + d];

	a = getPiece((lastRow - 3),(lastColumn - 3));
	b = getPiece((lastRow - 2),(lastColumn - 2)) << 2;
	c = getPiece((lastRow - 1),(lastColumn - 1)) << 4;
	d = getPiece((lastRow + 0),(lastColumn + 0)) << 6;
	possible[12] = points[a + b + c + d];

	// Add up wins
	winPoints = possible[0] + possible[1] + possible[2] + possible[3]
			+ possible[4] + possible[5] + possible[6] + possible[7]
			+ possible[8] + possible[9] + possible[10] + possible[11]
			+ possible[12];

	if (winPoints) {
		redPoints = winPoints & 255;
		bluePoints = (winPoints & 65280) >> 8;
		//printf ("winpoints: %d, win & number: %d, red: %d, blue: %d", winPoints, (winPoints & 65280) >> 8, redPoints, bluePoints);
		if (redPoints > bluePoints) {
			if (redPoints <= 5)
				return 0 - redPoints;
			for (i = 0; i < 13; i++) {
				if ((possible[i] & 255) > best) {
					best = possible[i] & 255;
				}
			}
			return 0 - best;
		} else if (redPoints < bluePoints) {
			if (bluePoints <= 5)
				return bluePoints;
			for (i = 0; i < 13; i++) {
				if ((possible[i] & 65280) >> 8 > best) {
					best = (possible[i] & 65280) >> 8;
				}
			}
			return best;
		} else {
			return 1;
		}
	}

	return 0;
}

/**
 * Takes in the last column played
 * Returns an int value of near wins.
 * Negative values are points for the other team.
 */
int isAlmostWin(int lastColumn) {
	int i, a, b, c, d;
	int lastRow;
	int left, right, top, bot;
	unsigned int redPoints = 0, bluePoints = 0;
	//possible wins, values from -5 to 5
	unsigned int possible[13], winPoints;
	// Find height of last piece played, may hit padding
	for (i = 0;; i++) {
		//printf("%d ",board[i+lastColumn*rows]);
		if (getPiece(i,lastColumn) == SPACE) {
			lastRow = i - 1;
			break;
		}
	}
	//printf("last piece played at (%d,%d)\n", lastColumn, lastRow);
	//printf("Total of %d columns, %d rows\n", columns, rows);
	// Find boundaries to search, row 0 is bottom row, column 0 is left column
	top = lastRow + 3;
	bot = lastRow - 3;
	left = lastColumn - 3;
	right = lastColumn + 3;

	//printf("Top: %d, Bottom: %d, Right: %d, Left: %d\n", top, bot, right, left);

	// Search vertical
	a = getPiece(lastRow,lastColumn);
	b = getPiece(lastRow-1,lastColumn) << 2;
	c = getPiece(lastRow-2,lastColumn) << 4;
	d = getPiece(lastRow-3,lastColumn) << 6;
	possible[0] = almostPoints[a + b + c + d];

	// Search horizontal
	a = getPiece(lastRow,(left));
	b = getPiece(lastRow,(left + 1)) << 2;
	c = getPiece(lastRow,(left + 2)) << 4;
	d = getPiece(lastRow,(left + 3)) << 6;
	possible[1] = almostPoints[a + b + c + d];

	a = getPiece(lastRow,(left + 1));
	b = getPiece(lastRow,(left + 2)) << 2;
	c = getPiece(lastRow,(left + 3)) << 4;
	d = getPiece(lastRow,(left + 4)) << 6;
	possible[2] = almostPoints[a + b + c + d];

	a = getPiece(lastRow,(left + 2));
	b = getPiece(lastRow,(left + 3)) << 2;
	c = getPiece(lastRow,(left + 4)) << 4;
	d = getPiece(lastRow,(left + 5)) << 6;
	possible[3] = almostPoints[a + b + c + d];

	a = getPiece(lastRow,(left + 3));
	b = getPiece(lastRow,(left + 4)) << 2;
	c = getPiece(lastRow,(left + 5)) << 4;
	d = getPiece(lastRow,(left + 6)) << 6;
	possible[4] = almostPoints[a + b + c + d];

	// Search diagonal from top left to bottom right
	a = getPiece((lastRow + 0),(lastColumn - 0));
	b = getPiece((lastRow + 1),(lastColumn - 1)) << 2;
	c = getPiece((lastRow + 2),(lastColumn - 2)) << 4;
	d = getPiece((lastRow + 3),(lastColumn - 3)) << 6;
	possible[5] = almostPoints[a + b + c + d];

	a = getPiece((lastRow - 1),(lastColumn + 1));
	b = getPiece((lastRow + 0),(lastColumn - 0)) << 2;
	c = getPiece((lastRow + 1),(lastColumn - 1)) << 4;
	d = getPiece((lastRow + 2),(lastColumn - 2)) << 6;
	possible[6] = almostPoints[a + b + c + d];

	a = getPiece((lastRow - 2),(lastColumn + 2));
	b = getPiece((lastRow - 1),(lastColumn + 1)) << 2;
	c = getPiece((lastRow + 0),(lastColumn - 0)) << 4;
	d = getPiece((lastRow + 1),(lastColumn - 1)) << 6;
	possible[7] = almostPoints[a + b + c + d];

	a = getPiece((lastRow - 3),(lastColumn + 3));
	b = getPiece((lastRow - 2),(lastColumn + 2)) << 2;
	c = getPiece((lastRow - 1),(lastColumn + 1)) << 4;
	d = getPiece((lastRow + 0),(lastColumn - 0)) << 6;
	possible[8] = almostPoints[a + b + c + d];

	// Search diagonal from top right to bottom left
	a = getPiece((lastRow + 0),(lastColumn + 0));
	b = getPiece((lastRow + 1),(lastColumn + 1)) << 2;
	c = getPiece((lastRow + 2),(lastColumn + 2)) << 4;
	d = getPiece((lastRow + 3),(lastColumn + 3)) << 6;
	possible[9] = almostPoints[a + b + c + d];

	a = getPiece((lastRow - 1),(lastColumn - 1));
	b = getPiece((lastRow + 0),(lastColumn + 0)) << 2;
	c = getPiece((lastRow + 1),(lastColumn + 1)) << 4;
	d = getPiece((lastRow + 2),(lastColumn + 2)) << 6;
	possible[10] = almostPoints[a + b + c + d];

	a = getPiece((lastRow - 2),(lastColumn - 2));
	b = getPiece((lastRow - 1),(lastColumn - 1)) << 2;
	c = getPiece((lastRow + 0),(lastColumn + 0)) << 4;
	d = getPiece((lastRow + 1),(lastColumn + 1)) << 6;
	possible[11] = almostPoints[a + b + c + d];

	a = getPiece((lastRow - 3),(lastColumn - 3));
	b = getPiece((lastRow - 2),(lastColumn - 2)) << 2;
	c = getPiece((lastRow - 1),(lastColumn - 1)) << 4;
	d = getPiece((lastRow + 0),(lastColumn + 0)) << 6;
	possible[12] = almostPoints[a + b + c + d];

	// Add up wins
	winPoints = possible[0] + possible[1] + possible[2] + possible[3]
			+ possible[4] + possible[5] + possible[6] + possible[7]
			+ possible[8] + possible[9] + possible[10] + possible[11]
			+ possible[12];

	if (winPoints) {
		redPoints = winPoints & 255;
		bluePoints = (winPoints & 65280) >> 8;
		return bluePoints - redPoints;
	}

	return 0;
}

int getTop(int column) {
	int top, i;
	for (i = 0;; i++) {
		if (getPiece(i,column) == SPACE) {
			top = i;
			return top;
		}
	}
}

int burninate(int player, int depth, int origDepth) {
	int i, points, colour;
	int fourMove;
	int threeMove;
	int neutralMoves[21];
	int enemy = 1;
	//printBoard();
	if (player == RED) {
		enemy = -1;
	}
	neutralMoves[0] = 0;
	int max = -5;
	colour = player;
	for (i = 0; i < columns - PADDING; i++) {
		if (columnHeight[i] >= rows - PADDING)
			continue;
		addPiece(i, colour);
		points = isWin(i);
		remPiece(i);
		if (points > max) {
			max = enemy * points;
			if (points == 5) {
				//fprintf(stderr, "Getting 5 points with %c!\n", pieces[colour]);
				return (5 << 6) + (i << 2) + colour;
			}
			if (points == 4) {
				//fprintf(stderr, "Getting 4 points with %c!\n", pieces[colour]);
				fourMove = (4 << 6) + (i << 2) + colour;
			}
			if (points == 3) {
				//fprintf(stderr, "Getting 3 points with %c!\n", pieces[colour]);
				threeMove = (3 << 6) + (i << 2) + colour;
			}
		}
		if (points == 0) {
			neutralMoves[neutralMoves[0] + 1] = (i << 2) + colour;
			neutralMoves[0]++;
		}
	}
	colour = GREEN;
	for (i = 0; i < columns - PADDING; i++) {
		if (columnHeight[i] >= rows - PADDING)
			continue;
		addPiece(i, colour);
		points = enemy * isWin(i);
		remPiece(i);
		if (points > max) {
			max = points;
			if (points == 5) {
				//fprintf(stderr, "Getting 5 points with %c!\n", pieces[colour]);
				return (5 << 6) + (i << 2) + colour;
			}
			if (points == 4) {
				//fprintf(stderr, "Getting 4 points with %c!\n", pieces[colour]);
				fourMove = (4 << 6) + (i << 2) + colour;
			}
			if (points == 3) {
				//fprintf(stderr, "Getting 3 points with %c!\n", pieces[colour]);
				threeMove = (3 << 6) + (i << 2) + colour;
			}
		}
		if (points == 0) {
			neutralMoves[neutralMoves[0] + 1] = (i << 2) + colour;
			neutralMoves[0]++;
		}
	}
	if (max == 4) {
		return fourMove;
	}
	if (max == 3) {
		return threeMove;
	}
	int original = 0;
	if (depth == origDepth)
		original = 1;
	if (max == 0) {
		int nonLosingMoves[21];
		nonLosingMoves[0] = 0;
		if (depth > 0) {
			for (i = 0; i < neutralMoves[0]; i++) {
				int col = (neutralMoves[i + 1] >> 2) & 15;
				int p = neutralMoves[i + 1] & 3;
				//fprintf(stderr,"Neutral move: %d, %c\n", col, pieces[p]);
				addPiece(col, p);
				int opponentTurn = (burninate(3 - player, depth - 1, origDepth)
						>> 6);
				if (opponentTurn < 3) {
//					if (original)
//						fprintf(stderr,
//								"Won't lose with %d, %c\nIs almost win: %d\n",
//								col, pieces[p], isAlmostWin(col));
					nonLosingMoves[nonLosingMoves[0] + 1] = neutralMoves[i + 1];
					nonLosingMoves[0]++;
				} else if (opponentTurn == 6) {
					//it's a win!
					remPiece(col);
//					if (original)
//						fprintf(stderr, "Forcing win with %d, %c\n", col,
//								pieces[p]);
					return (3 << 6) + neutralMoves[i + 1]; // might not be 3
				}
				remPiece(col);
			}
		} else {
			int centre;
			int closest = columns;
			int best = -5;
			for (i = 0; i < nonLosingMoves[0]; i++) {
				int col = (nonLosingMoves[i + 1] >> 2) & 15;
				int p = nonLosingMoves[i + 1] & 3;
				addPiece(col, p);
				int score = isAlmostWin(col);
				int current = abs((nonLosingMoves[i + 1] >> 2) - (columns
						- PADDING) / 2);

				if (score > best || (score == best && current < closest)) {
					closest = current;
					best = score;
					centre = nonLosingMoves[i + 1];
//					if (original)
//						fprintf(stderr, "Best score is %d with %d, %c\n", best, col,
//								pieces[p]);
				}
				remPiece(col);
			}
			return centre;
		}
		if (nonLosingMoves[0] > 0) {
			int centre;
			int closest = columns;
			int best = -5;
			for (i = 0; i < nonLosingMoves[0]; i++) {
				int col = (nonLosingMoves[i + 1] >> 2) & 15;
				int p = nonLosingMoves[i + 1] & 3;
				addPiece(col, p);
				int score = isAlmostWin(col);
				int current = abs((nonLosingMoves[i + 1] >> 2) - (columns
						- PADDING) / 2);

				if (score > best || (score == best && current < closest)) {
					closest = current;
					best = score;
					centre = nonLosingMoves[i + 1];
//					if (original)
//						fprintf(stderr, "Best score is %d with %d, %c\n", best, col,
//								pieces[p]);
				}
				remPiece(col);
			}
			return centre;
		} else { //we're gonna lose now...
			int centre;
			int closest = columns;
			for (i = 0; i < neutralMoves[0]; i++) {
				int current = abs((neutralMoves[i + 1] >> 2) - (columns
						- PADDING) / 2);
				if (current < closest) {
					closest = current;
					centre = (6 << 6) + neutralMoves[i + 1];
				}
			}
			return centre;
		}
	}
//	if (max < 0) {
//		fprintf(stderr, "GONNA LOSE!!! --- FIX THIS!!!");
//	}
	return 6 << 6;
}

void addPiece(int col, int colour) {
	getPiece(columnHeight[col],col) = colour;
	columnHeight[col]++;
}

void remPiece(int col) {
	getPiece(columnHeight[col]-1,col) = SPACE;
	columnHeight[col]--;
}

/**
 * Calls functions to read in board etc.
 */
int main(void) {
	int col, move;
	int totMoves = 0, i;
	char p;

	readboard();

	for (i = 0; i < columns - PADDING; i++) {
		totMoves += columnHeight[i];
	}
	if (totMoves < 3) {
		p = pieces[BLUE];
		col = 4;
	} else {
		move = burninate(BLUE, 5, 5);
		col = (move >> 2) & 15;
		p = pieces[move & 3];
	}

	//timeIswin(10000000);
	//printf("%d\n", isWin(last_move));
	//tempPrint();
	freeboard();

	printf("(%d,%c)", col + 1, p);

	return 0;
}

