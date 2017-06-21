/*
 * SPMiniMaxNode.c
 *
 *  Created on: 10/06/2017
 *      Author: Shawn Molga & Ofir Feffer
 */


#include "SPMiniMaxNode.h"


SPMiniMaxNode* createMiniMaxNode(SPFiarGame* gameState) {
	if (gameState == NULL) {
		return NULL;
	}
	SPMiniMaxNode* node = (SPMiniMaxNode*) malloc(sizeof(SPMiniMaxNode));
	if (node == NULL) {
		return NULL;
	}
	node->gameState = gameState;
	node->children = NULL;
	return node;
}

SPFiarGame* createChild(SPMiniMaxNode* node, int col) {
	SPFiarGame* child = spFiarGameCopyWithoutHistory(node->gameState);
	if (child == NULL) {
		return NULL;
	}
	SP_FIAR_GAME_MESSAGE msg = spFiarGameSetMove(child, col);
	if (msg != SP_FIAR_GAME_SUCCESS) {
		free(child);
		return NULL;
	}
	return child;
}

bool createAllChildren(SPMiniMaxNode* node) {
	node->children = malloc(SP_FIAR_GAME_N_COLUMNS * sizeof(SPFiarGame*));
	if (node->children == NULL) {
		return false;
	}
	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		if (spFiarGameIsValidMove(node->gameState, i) == true) {
			SPFiarGame* child = createChild(node, i);
			if (child == NULL) {
				for (int j = 0; j < i; j++) { // allocation error - destroy the children we created so far
					spFiarGameDestroy(node->children[i]);
				}
				return false;
			}
			node->children[i] = child;
		} else {
			node->children[i] = NULL;
		}
	}
	return true;
}

void initCounters(int arr[]) {
	for (int i = 0; i < 7; i++) {
		arr[i] = 0;
	}
}

int calcSpanScore(char c1, char c2, char c3, char c4, char player) {
	int sum = 0;
	if (c1 == player) {
		sum += 1;
	} else if (c1 != SP_FIAR_GAME_EMPTY_ENTRY) {
		sum -=1;
	}
	if (c2 == player) {
		sum += 1;
	} else if (c2 != SP_FIAR_GAME_EMPTY_ENTRY) {
		sum -=1;
	}
	if (c3 == player) {
		sum += 1;
	} else if (c3 != SP_FIAR_GAME_EMPTY_ENTRY) {
		sum -=1;
	}
	if (c4 == player) {
		sum += 1;
	} else if (c4 != SP_FIAR_GAME_EMPTY_ENTRY) {
		sum -=1;
	}
	return sum;
}

int updateCounters(int counters[], int spanScore) {
	if (spanScore == 4) {
		return INT_MAX;
	} else if (spanScore == -4) {
		return INT_MIN;
	} else if (spanScore != 0) {
		counters[spanScore + 3]++;
	}
	return 0;
}

int calculateScore(SPMiniMaxNode* node, char player) {
	int counters[7];
	initCounters(counters);

	for (int i = 0; i < SP_FIAR_GAME_N_ROWS; i++) {
		for (int j = 0; j < SP_FIAR_GAME_N_COLUMNS; j++) {
			if (j < 4 ) {
				int spanScore = calcSpanScore(node->gameState->gameBoard[i][j],
											  node->gameState->gameBoard[i][j+1],
											  node->gameState->gameBoard[i][j+2],
											  node->gameState->gameBoard[i][j+3],
											  player);
				int res = updateCounters(counters, spanScore);
				if (res != 0) {
					return res;
				}
			}
			if (i < 3) {
				int spanScore = calcSpanScore(node->gameState->gameBoard[i][j],
											  node->gameState->gameBoard[i+1][j],
											  node->gameState->gameBoard[i+2][j],
											  node->gameState->gameBoard[i+3][j],
											  player);
				int res = updateCounters(counters, spanScore);
				if (res != 0) {
					return res;
				}
			}
			if (i < 3 && j < 4) {
				int spanScore = calcSpanScore(node->gameState->gameBoard[i][j],
											  node->gameState->gameBoard[i+1][j+1],
											  node->gameState->gameBoard[i+2][j+2],
											  node->gameState->gameBoard[i+3][j+3],
											  player);
				int res = updateCounters(counters, spanScore);
				if (res != 0) {
					return res;
				}
			}
			if (i < 3 && j > 2) {
				int spanScore = calcSpanScore(node->gameState->gameBoard[i][j],
											  node->gameState->gameBoard[i+1][j-1],
											  node->gameState->gameBoard[i+2][j-2],
											  node->gameState->gameBoard[i+3][j-3],
											  player);
				int res = updateCounters(counters, spanScore);
				if (res != 0) {
					return res;
				}
			}
		}
	}

	int score = counters[0]*-5 + counters[1]*-2 + counters[2]*-1 + counters[4] + counters[5]*2 + counters[6]*5;
	return score;
}

void destroyMiniMaxNode(SPMiniMaxNode* node) {
	if (node == NULL) {
		return;
	}
	spFiarGameDestroy(node->gameState);
	if (node->children != NULL) {
		free(node->children);
	}
	free(node);
}

void destroyAllChildren(SPMiniMaxNode* node, int from) {
	if (node == NULL || node->children == NULL) {
		return;
	}
	for (int i = from; i < 7; i++) {
		spFiarGameDestroy(node->children[i]);
	}
}
