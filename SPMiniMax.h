#ifndef SPMINIMAX_H_
#define SPMINIMAX_H_

#include <stdio.h>
#include <stdlib.h>
#include "SPMiniMaxNode.h"

/**
 * Given a game state, this function evaluates the best move according to
 * the current player. The function initiates a MiniMax algorithm up to a
 * specified length given by maxDepth. The current game state doesn't change
 * by this function including the history of previous moves.
 *
 * @param currentGame - The current game state
 * @param maxDepth - The maximum depth of the miniMax algorithm
 * @return
 * -1 if either currentGame is NULL or maxDepth <= 0 or an allocation error occurs.
 * On success the function returns a number between [0,SP_FIAR_GAME_N_COLUMNS -1]
 * which is the best move for the current player.
 */
int spMinimaxSuggestMove(SPFiarGame* currentGame,
		unsigned int maxDepth);
 /**
  * A recursive function that evaluates the next best move using the MiniMax algorithm up to a
  * depth given by maxDepth.
  * Given a game state (a node) - if depth of the game state equals to maxDepth and\or the game state is a winning\losing state then its score will be calculated and returned.
  * 	- if depth of the game state is less than maxDepth then the method will calculate its children and call the method recursively with each of them (and a +1 depth)
  * When the recursive calls return, a node with an even depth returns the maximum score of its children,
  * 	and a node with an odd depth returns the minimum score of its children
  * Finally, when all recursive calls return to the first one (the root) - the method will return the index of the child (column) with the maximum score
  *
  * @param node - The current game state node
  * @param depth - The current depth of the game state node
  * @param maxDepth - The maximum depth of the miniMax algorithm
  * @param player - the character of the current player: 'X' or 'O'
  * @return
  * ALLOCATION_ERR_VALUE if an allocation error occurs (ALLOCATION_ERR_VALUE = INT_MIN + 1)
  * or an integer in range of 0-6 representing the index of a column which is best for the current player to put a disc into
  */
int spMinimaxSuggestMoveRec(SPMiniMaxNode* node, unsigned int depth, unsigned int maxDepth, char player);

#endif
