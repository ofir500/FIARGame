/*
 * SPMiniMax.c
 *
 *  Created on: 10/06/2017
 *      Author: Shawn Molga & Ofir Feffer
 */

#include "SPMiniMax.h"
#define ALLOCATION_ERR_VALUE INT_MIN + 1

int spMinimaxSuggestMove(SPFiarGame* currentGame, unsigned int maxDepth) {
	if (currentGame == NULL || maxDepth <= 0) {
		return -1;
	}
	SPFiarGame* copy = spFiarGameCopyWithoutHistory(currentGame);
	if (copy == NULL) {
		return -1;
	}
	SPMiniMaxNode* root = createMiniMaxNode(copy);
	if (root == NULL) {
		free(copy);
		return -1;
	}
	int col = spMinimaxSuggestMoveRec(root, 0, maxDepth, currentGame->currentPlayer);
	if (col == ALLOCATION_ERR_VALUE) {
		return -1;
	}
	destroyMiniMaxNode(root);
	return col;
}

int spMinimaxSuggestMoveRec(SPMiniMaxNode* node, unsigned int depth, unsigned int maxDepth, char player) {
	int score = calculateScore(node, player);
	if (depth == maxDepth || score == INT_MAX || score == INT_MIN) {
		return score;
	}
	bool creationRes = createAllChildren(node);
	if (creationRes == false) {
		return ALLOCATION_ERR_VALUE;
	}
	int min = INT_MAX;
	int max = INT_MIN;
	int maxCol = -1;
	for (int i = 0; i < SP_FIAR_GAME_N_COLUMNS; i++) {
		if (node->children[i] != NULL) {
			if (maxCol == -1) {
				maxCol = i;
			}
			SPMiniMaxNode* childNode = createMiniMaxNode(node->children[i]);
			if (childNode == NULL) {
				destroyAllChildren(node, i+1); // free memory of children we didn't use yet
				return ALLOCATION_ERR_VALUE;
			}
			int score = spMinimaxSuggestMoveRec(childNode, depth + 1, maxDepth, player);
			if (score == ALLOCATION_ERR_VALUE) {
				destroyMiniMaxNode(childNode); // free memory of current node
				destroyAllChildren(node, i+1); // free memory of children we didn't use yet
				return ALLOCATION_ERR_VALUE;
			}
			if (score > max) {
				max = score;
				maxCol = i;
			}
			if (score < min) {
				min = score;
			}
			destroyMiniMaxNode(childNode);
		}
	}
	if (depth == 0) {
		return maxCol;
	} else if (depth % 2 == 0) {
		return max;
	} else {
		return min;
	}
}

