/*
 * SPFiarGame.c
 *
 *  Created on: 10/06/2017
 *      Author: Shawn Molga & Ofir Feffer
 */

#include "SPFIARGame.h"

void initGameBoard(char board[][7]) {
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			board[i][j] = SP_FIAR_GAME_EMPTY_ENTRY;
		}
	}
}

void initTops(int tops[]) {
	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		tops[i] = 5;
	}
}

void copyGameBoard(char to[][7], char src[][7]) {
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			to[i][j] = src[i][j];
		}
	}
}

void copyTops(int to[], int src[]) {
	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		to[i] = src[i];
	}
}

SPFiarGame* spFiarGameCreateWithoutHistory() {
	SPFiarGame* game = (SPFiarGame*) malloc(sizeof(SPFiarGame));
	if (game == NULL) {
		return NULL;
	}
	initGameBoard(game->gameBoard);
	initTops(game->tops);
	game->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	game->history = NULL;
	return game;
}

SPFiarGame* spFiarGameCreate(int historySize) {
	if (historySize <= 0) {
		return NULL;
	}
	SPFiarGame* game = spFiarGameCreateWithoutHistory();
	if (game == NULL) {
		return NULL;
	}
	game->history = spArrayListCreate(historySize);
	if (game->history == NULL) {
		free(game);
		return NULL;
	}
	return game;
}

SPFiarGame* spFiarGameCopyWithoutHistory(SPFiarGame* src) {
	if (src == NULL) {
		return NULL;
	}
	SPFiarGame* new = spFiarGameCreateWithoutHistory();
	if (new == NULL) {
		return NULL;
	}
	copyGameBoard( new->gameBoard, src->gameBoard);
	copyTops(new->tops, src->tops );
	new->currentPlayer = src->currentPlayer;
	return new;
}

SPFiarGame* spFiarGameCopy(SPFiarGame* src){
	SPFiarGame* new = spFiarGameCopyWithoutHistory(src);
	if (new == NULL) {
		return NULL;
	}
	new->history = spArrayListCopy(src->history);
	if (new->history == NULL) {
		free(new);
		return NULL;
	}
	return new;
}

void spFiarGameDestroy(SPFiarGame* src){
	if (src == NULL) {
		return;
	}
	spArrayListDestroy(src->history);
	free(src);
}

void addToHistory(SPArrayList* history, int col) {
	if (spArrayListIsFull(history) == true) {
		spArrayListRemoveFirst(history);
	}
	spArrayListAddLast(history, col);
}

SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col){
	if (src == NULL || col < 0 || col > 6)
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	if (src->tops[col] == -1)
		return SP_FIAR_GAME_INVALID_MOVE;
	int row = src->tops[col];
	src->gameBoard[row][col] = src->currentPlayer;
	src->tops[col]--;
	addToHistory(src->history, col);
	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	} else {
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
	return SP_FIAR_GAME_SUCCESS;
}

bool spFiarGameIsValidMove(SPFiarGame* src, int col) {
	if (src == NULL || col < 0 || col > 6)
		return false;
	if (src->tops[col] == -1)
		return false;
	return true;
}

SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src) {
	if (src == NULL) {
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}
	if (spArrayListIsEmpty(src->history) == true) {
		return SP_FIAR_GAME_NO_HISTORY;
	}
	int col = spArrayListGetLast(src->history);
	int row = src->tops[col] + 1;
	src->gameBoard[row][col] = ' ';
	src->tops[col]++;
	spArrayListRemoveLast(src->history);
	if (src->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
		src->currentPlayer = SP_FIAR_GAME_PLAYER_2_SYMBOL;
	} else {
		src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	}
	return SP_FIAR_GAME_SUCCESS;
}

SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src){
	if (src == NULL) {
		return SP_FIAR_GAME_INVALID_ARGUMENT;
	}
	for (int i = 0; i < 6; i++) {
		printf("%s", "| ");
		for (int j = 0; j < 7; j++) {
			printf("%c", src->gameBoard[i][j]);
			printf("%s", " ");
		}
		printf("%s", "|\n");
	}
	printf("%s", "-----------------\n");
	printf("%s", "  1 2 3 4 5 6 7  \n");
	return SP_FIAR_GAME_SUCCESS;
}

char spFiarGameGetCurrentPlayer(SPFiarGame* src) {
	if (src == NULL) {
		return SP_FIAR_GAME_EMPTY_ENTRY;
	}
	return src->currentPlayer;
}

bool equals(char c1, char c2, char c3, char c4) {
	if (c1 == c2 && c2 == c3 && c3 == c4) {
		return true;
	}
	return false;
}

char spFiarCheckWinner(SPFiarGame* src) {
	if (src == NULL) {
		return '\0';
	}
	int counter = 0;
	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			if (src->gameBoard[i][j] == ' ') {
				continue;
			} else {
				counter++;
			}
			if (j < 4 && equals(src->gameBoard[i][j],
								src->gameBoard[i][j+1],
								src->gameBoard[i][j+2],
								src->gameBoard[i][j+3])) {
				return src->gameBoard[i][j];
			}
			if (i < 3 && equals(src->gameBoard[i][j],
								src->gameBoard[i+1][j],
								src->gameBoard[i+2][j],
								src->gameBoard[i+3][j])) {
				return src->gameBoard[i][j];
			}
			if (i < 3 && j < 4) {
				if (equals(src->gameBoard[i][j],
						   src->gameBoard[i+1][j+1],
						   src->gameBoard[i+2][j+2],
						   src->gameBoard[i+3][j+3])) {
					return src->gameBoard[i][j];
				}
			}
			if (i < 3 && j > 2) {
				if (equals(src->gameBoard[i][j],
						   src->gameBoard[i+1][j-1],
						   src->gameBoard[i+2][j-2],
						   src->gameBoard[i+3][j-3])) {
					return src->gameBoard[i][j];
				}
			}
		}
	}
	if (counter == 42) {
		return SP_FIAR_GAME_TIE_SYMBOL;
	}
	return '\0';
}

void restartGame(SPFiarGame* src) {
	initGameBoard(src->gameBoard);
	initTops(src->tops);
	src->currentPlayer = SP_FIAR_GAME_PLAYER_1_SYMBOL;
	spArrayListClear(src->history);
}

