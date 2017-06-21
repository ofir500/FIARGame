/*
 * SPMainAux.c
 *
 *  Created on: 10/06/2017
 *      Author: Shawn Molga & Ofir Feffer
 */

#include "SPMainAux.h"


char* getUserInput() {
	char* str = malloc(SP_MAX_LINE_LENGTH * sizeof(char));
	if (str == NULL) {
		return NULL;
	}
	fgets(str, SP_MAX_LINE_LENGTH, stdin);
	//remove trailing '\n'
	size_t len = strlen(str);
	if (len > 0 && str[len-1] == '\n') {
		str[--len] = '\0';
	}
	return str;
}

int inputDifficulty() {
	printf(MSG_DIFFIULTY);
	char* str = getUserInput();
	if (str == NULL) {
		return -1;
	}

	if (spParserIsInt(str)) {
		int res = atoi(str);
		free(str);
		if (res >= 1 && res <= 7) {
			return res;
		} else {
			printf(MSG_ERR_LEVEL);
			return inputDifficulty();
		}
	} else if (strcmp(str, "quit") == 0) {
		free(str);
		return 0;
	} else {
		printf(MSG_ERR_LEVEL);
		free(str);
		return inputDifficulty();
	}
}

bool undoMove(SPFiarGame* gameState) {
	int col = spArrayListGetLast(gameState->history);
	SP_FIAR_GAME_MESSAGE msg = spFiarGameUndoPrevMove(gameState);
	if (msg == SP_FIAR_GAME_NO_HISTORY) {
		printf(MSG_ERR_HISTORY);
		return false;
	} else {
		if (gameState->currentPlayer == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
			printf(MSG_REMOVE_USER_DISC, col + 1);
		} else {
			printf(MSG_REMOVE_PC_DISC, col + 1);
		}
	}
	return true;
}

MoveResult userMove(SPFiarGame* gameState, int difficulty, MoveResult prevRes) {
	char* str = getUserInput();
	if (str == NULL) {
		return ALLOCATION_ERROR;
	}
	SPCommand command = spParserPraseLine(str);
	free(str);
	bool undoRes = false;
	int col, length, i;
	char winner;
	switch(command.cmd) {
		case SP_UNDO_MOVE:
			length = (prevRes == USER_WON) ? 1 : 2;
			for (i = 0; i < length; i++) {
				undoRes = undoMove(gameState);
				if (!undoRes) {
					return userMove(gameState, difficulty, prevRes);
				}
			}
			return CONTINUE;

		case SP_ADD_DISC:
			if (prevRes == USER_WON || prevRes == TIE || prevRes == PC_WON){
				printf(MSG_GAME_OVER);
				return userMove(gameState, difficulty, prevRes);
			} else if (command.validArg == false || command.arg < 1 || command.arg > 7) {
				printf(ERR_RANGE);
				return userMove(gameState, difficulty, prevRes);
			} else if (spFiarGameIsValidMove(gameState, command.arg - 1) == false) {
				printf(ERR_FULL, command.arg);
				return userMove(gameState, difficulty, prevRes);
			} else {
				spFiarGameSetMove(gameState, command.arg - 1);
				winner = spFiarCheckWinner(gameState);
				if (winner == SP_FIAR_GAME_PLAYER_1_SYMBOL) {
					return USER_WON;
				} else if (winner == SP_FIAR_GAME_TIE_SYMBOL){
					return TIE;
				} else {
					return CONTINUE;
				}
			}

		case SP_SUGGEST_MOVE:
			if (prevRes == USER_WON || prevRes == TIE || prevRes == PC_WON){
				printf(MSG_GAME_OVER);
				return userMove(gameState, difficulty, prevRes);
			}
			col = spMinimaxSuggestMove(gameState, difficulty);
			if (col == -1) {
				return ALLOCATION_ERROR;
			}
			printf(MSG_SUGGEST, col + 1);
			return userMove(gameState, difficulty, prevRes);

		case SP_QUIT:
			return QUIT;

		case SP_RESTART:
			return RESTART;

		default:
			printf(ERR_INVALID);
			return userMove(gameState, difficulty, prevRes);
	}
}

MoveResult pcMove(SPFiarGame* gameState, int difficulty) {
	int col = spMinimaxSuggestMove(gameState, difficulty);
	if (col == -1) {
		return ALLOCATION_ERROR;
	}
	printf(MSG_PCMOVE, col + 1);
	spFiarGameSetMove(gameState, col);
	char winner = spFiarCheckWinner(gameState);
	if (winner == SP_FIAR_GAME_PLAYER_2_SYMBOL) {
		return PC_WON;
	} else if (winner == SP_FIAR_GAME_TIE_SYMBOL) {
		return TIE;
	} else {
		return CONTINUE;
	}
}
