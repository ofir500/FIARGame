#ifndef SPFIARGAME_H_
#define SPFIARGAME_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "SPArrayList.h"

/**
 * SPFIARGame Summary:
 *
 * A container that represents a classic connect-4 game, a two players 6 by 7
 * board game (rows X columns). The container supports the following functions.
 *
 * spFiarGameCreate           - Creates a new game board
 * spFiarGameCopy             - Copies a game board
 * spFiarGameDestroy          - Frees all memory resources associated with a game
 * spFiarGameSetMove          - Sets a move on a game board
 * spFiarGameIsValidMove      - Checks if a move is valid
 * spFiarGameUndoPrevMove     - Undoes previous move made by the last player
 * spFiarGamePrintBoard       - Prints the current board
 * spFiarGameGetCurrentPlayer - Returns the current player
 *
 */

//Definitions
#define SP_FIAR_GAME_SPAN 4
#define SP_FIAR_GAME_N_ROWS 6
#define SP_FIAR_GAME_N_COLUMNS 7
#define SP_FIAR_GAME_PLAYER_1_SYMBOL 'X'
#define SP_FIAR_GAME_PLAYER_2_SYMBOL 'O'
#define SP_FIAR_GAME_TIE_SYMBOL '-'
#define SP_FIAR_GAME_EMPTY_ENTRY ' '


typedef struct sp_fiar_game_t {
	char gameBoard[SP_FIAR_GAME_N_ROWS][SP_FIAR_GAME_N_COLUMNS];
	int tops[SP_FIAR_GAME_N_COLUMNS];
	char currentPlayer;
	SPArrayList* history;
} SPFiarGame;

/**
 * Type used for returning error codes from game functions
 */
typedef enum sp_fiar_game_message_t {
	SP_FIAR_GAME_INVALID_MOVE,
	SP_FIAR_GAME_INVALID_ARGUMENT,
	SP_FIAR_GAME_NO_HISTORY,
	SP_FIAR_GAME_SUCCESS,
//You may add any message you like
} SP_FIAR_GAME_MESSAGE;

/**
 * Initializes all entries of a game board array with the SP_FIAR_GAME_EMPTY_ENTRY symbol.
 * Method assumes that the size of the array is SP_FIAR_GAME_N_ROWS X SP_FIAR_GAME_N_COLUMNS
 * @param board - a 6x7 array
 */
void initGameBoard(char board[][7]);

/**
 * Initializes all entries of an array to 5 (the lowest row of a game board)
 * Method assumes that the size of the array is SP_FIAR_GAME_N_COLUMNS
 * @param tops - an array of integers with size SP_FIAR_GAME_N_COLUMNS
 */
void initTops(int tops[]);

/**
 * Copies the entries of the src array to another array.
 * Method assumes that the size of both arrays is SP_FIAR_GAME_N_ROWS X SP_FIAR_GAME_N_COLUMNS
 * @param to - target 6x7 array
 * @param src - source 6x7 array
 */
void copyGameBoard(char to[][7], char src[][7]);

/**
 * Copies the entries of the src array to another array.
 * Method assumes that the size of both arrays is SP_FIAR_GAME_N_COLUMNS
 * @param to - target array
 * @param src - source array
 */
void copyTops(int to[], int src[]);

/**
 * Creates a new game with an history list set to NULL.
 *
 * @return
 * NULL if either a memory allocation failure occurs or historySize <= 0.
 * Otherwise, a new game instant is returned.
 */
SPFiarGame* spFiarGameCreateWithoutHistory();

/**
 * Creates a new game with a specified history size. The history size is a
 * parameter which specifies the number of previous moves to store. If the number
 * of moves played so far exceeds this parameter, then first moves stored will
 * be discarded in order for new moves to be stored.
 *
 * @historySize - The total number of moves to undo,
 *                a player can undo at most historySizeMoves turns.
 * @return
 * NULL if either a memory allocation failure occurs or historySize <= 0.
 * Otherwise, a new game instant is returned.
 */
SPFiarGame* spFiarGameCreate(int historySize);

/**
 *	Creates a copy of a given game.
 *	The new copy has the same status as the src game,
 *	except for the history list which is not copied and is set to NULL
 *
 *	@param src - the source game which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 *
 */
SPFiarGame* spFiarGameCopyWithoutHistory(SPFiarGame* src);

/**
 *	Creates a copy of a given game.
 *	The new copy has the same status as the src game.
 *
 *	@param src - the source game which will be copied
 *	@return
 *	NULL if either src is NULL or a memory allocation failure occurred.
 *	Otherwise, an new copy of the source game is returned.
 *
 */
SPFiarGame* spFiarGameCopy(SPFiarGame* src);

/**
 * Frees all memory allocation associated with a given game. If src==NULL
 * the function does nothing.
 *
 * @param src - the source game
 */
void spFiarGameDestroy(SPFiarGame* src);

/**
 * adds a move to a history list. If the number of moves played so far exceeds the history size,
 * then the first move stored will be removed.
 * @param history - an SPArrayList
 * @param col - index of a column to which a disc has been inserted
 */
void addToHistory(SPArrayList* history, int col);

/**
 * Sets the next move in a given game by specifying column index. The
 * columns are 0-based and in the range [0,SP_FIAR_GAME_N_COLUMNS -1].
 *
 * @param src - The target game
 * @param col - The target column, the columns are 0-based
 * @return
 * SP_FIAR_GAME_INVALID_ARGUMENT - if src is NULL or col is out-of-range
 * SP_FIAR_GAME_INVALID_MOVE - if the given column is full.
 * SP_FIAR_GAME_SUCCESS - otherwise
 */
SP_FIAR_GAME_MESSAGE spFiarGameSetMove(SPFiarGame* src, int col);

/**
 * Checks if a disk can be put in the specified column.
 *
 * @param src - The source game
 * @param col - The specified column
 * @return
 * true  - if the a disc can be put in the target column
 * false - otherwise.
 */
bool spFiarGameIsValidMove(SPFiarGame* src, int col);

/**
 * Removes a disc that was put in the previous move and changes the current
 * player's turn. If the user invoked this command more than historySize times
 * in a row then an error occurs.
 *
 * @param src - The source game
 * @return
 * SP_FIAR_GAME_INVALID_ARGUMENT - if src == NULL
 * SP_FIAR_GAME_NO_HISTORY       - if the user invoked this function more then
 *                                 historySize in a row.
 * SP_FIAR_GAME_SUCCESS          - on success. The last disc that was put on the
 *                                 board is removed and the current player is changed
 */
SP_FIAR_GAME_MESSAGE spFiarGameUndoPrevMove(SPFiarGame* src);

/**
 * On success, the function prints the board game. If an error occurs, then the
 * function does nothing. The characters 'X' and 'O' are used to represent
 * the discs of player 1 and player 2, respectively.
 *
 * @param src - the target game
 * @return
 * SP_FIAR_GAME_INVALID_ARGUMENT - if src==NULL
 * SP_FIAR_GAME_SUCCESS - otherwise
 *
 */
SP_FIAR_GAME_MESSAGE spFiarGamePrintBoard(SPFiarGame* src);

/**
 * Returns the current player of the specified game.
 * @param src - the source game
 * @return
 * SP_FIAR_GAME_PLAYER_1_SYMBOL - if it's player one's turn
 * SP_FIAR_GAME_PLAYER_2_SYMBOL - if it's player two's turn
 * SP_FIAR_GAME_EMPTY_ENTRY     - otherwise
 */
char spFiarGameGetCurrentPlayer(SPFiarGame* src);

/**
 * Checks if a span of 4 characters are all equal
 * @param c1 - first character
 * @param c2 - second character
 * @param c3 - third character
 * @param c4 - fourth character
 * @return
 * true - if all characters are equal
 * false - otherwise
 */
bool equals(char c1, char c2, char c3, char c4);

/**
* Checks if there's a winner in the specified game status. The function returns either
* SP_FIAR_GAME_PLAYER_1_SYMBOL or SP_FIAR_GAME_PLAYER_2_SYMBOL in case there's a winner, where
* the value returned is the symbol of the winner. If the game is over and there's a tie
* then the value SP_FIAR_GAME_TIE_SYMBOL is returned. in any other case the null characters
* is returned.
* @param src - the source game
* @return
* SP_FIAR_GAME_PLAYER_1_SYMBOL - if player 1 won
* SP_FIAR_GAME_PLAYER_2_SYMBOL - if player 2 won
* SP_FIAR_GAME_TIE_SYMBOL - If the game is over and there's a tie
* null character - otherwise
*/
char spFiarCheckWinner(SPFiarGame* src);

/**
 * Restarts a game state by initializing both the game board and the tops array to their default values
 * and by clearing the history.
 * @param src - a game instance
 */
void restartGame(SPFiarGame* src);

#endif
