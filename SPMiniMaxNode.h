#ifndef SPMINIMAXNODE_H_
#define SPMINIMAXNODE_H_

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "SPFIARGame.h"

// a new type that is used to encapsulate a game-state and its children
typedef struct sp_node_t {
	SPFiarGame* gameState;
	SPFiarGame** children;
} SPMiniMaxNode;

/**
 *	Creates a MiniMax node.
 *	@param gameState
 *	@return
 *	NULL if either an allocation error occurs or gameState == NULL.
 *	A node with null as the children attributes and the "gameState" input as the gameState attribute.
 */
SPMiniMaxNode* createMiniMaxNode(SPFiarGame* gameState);

/**
 *	Creates a MiniMax node's child, which is a copy of a given game (without history attribute).
 *	we set the chil's move using spFiarGameSetMove. We enter a disk at column number "col".
 *	@param MiniMax node
 *	@param col
 *	@return
 *	NULL if either spFiarGameCopyWithoutHistory returns NULL, or if an error occurs while setting move (after invoiking spFiarGameSetMove).
 *	otherwise, the created child is returned.
 */
SPFiarGame* createChild(SPMiniMaxNode* node, int col);

/**
 *	Creates an array of pointers to all of MiniMax node's child.
 *  each node has at most 7 children. For each of the possible columns in which the computer may choose from to enter the disk,
 *	we check to see if this is a valid move. If so, we create the relevant child (using the createChild method).
 * 	if the move is not valid, the child is set to be NULL.
 *	@param MiniMax node
 *	@return
 *	false - if an allocation error occurs
 *	true = otherwise
 */
bool createAllChildren(SPMiniMaxNode* node);

/**
 *	the method sets the first 7 cells of the array to zeros
 *	@param arr[] - an array of integers]
 *	@return void
 */
void initCounters(int arr[]);

/**
 *	the method recieves four characters representing a span of four disks on the board.
 *	the method adds (+1) to the sum if a disk belongs to the player, and (-1) if the disk belongs to the opponent
 *	(if there is no disk, meaning the character equals " ", we do not add to the sum)
 *	@param c1, c2, c3, c4 - 4 characters representing a series of four spaces on the game board
 *	@param char player - representing the player who's turn it is: either the computer or the user.
 *	@return sum, according to the abovementioned algorithm
 */
int calcSpanScore(char c1, char c2, char c3, char c4, char player);

/**
 *	if the spanScore=4, all four characters recieved in the spanScore method belong to "player", and therefore "player" won.
 *	we return INT_MAX in that case.
 *	if the spanScore=(-4), all four characters recieved in the spanScore method belong to "player's" opponent, and therefore "player" lost.
 *	we return INT_MIN in this case.
 *	Otherwise, we don't have a winner, and the method increments counters[spanScore + 3].
 *	@param counters[]
 *	@param spanScore - the score we calculate in calcSpanScore
 *	@return 0 if there is no winner
 *	INT_MAX if "player" won
 *	INT_MIN if "player" lost
 */
int updateCounters(int counters[], int spanScore);

/**
 *	the method computes all possible tuples of four consecutive gameboard slots.
 *	for each tuple, the method computes the spanScore (using the calcSpanScore method), and updates the array of counters (using the updateCounters method).
 *	if there is a winner in the game we return INT_MAX or INT_MIN (depending on the winner)
 *	otherwise, we calculate the score using the vector given in the algorithm
 *	@param node
 *	@param player
 *	@return score of board- INT_MAX if player won, INT_MIN if player lost. otherwise, the result of vector multiplication (as specified in algorithm).
 */
int calculateScore(SPMiniMaxNode* node, char player);

/**
 *	the method frees all memory used by node
 *	@param node
 *	@return void
 */
void destroyMiniMaxNode(SPMiniMaxNode* node);

/**
 * Frees all memory resources associated with the children of a SPMiniMaxNode,
 * starting at index 'from'
 * @param node - a MiniMaxNode
 * @param from - index from which to start destroying the children
 */
void destroyAllChildren(SPMiniMaxNode* node, int from);

#endif
