#ifndef SPMAINAUX_H_
#define SPMAINAUX_H_

#include <stdio.h>
#include <stdlib.h>
#include "SPFIARParser.h"
#include "SPFIARGame.h"
#include "SPMiniMax.h"

#define MSG_DIFFIULTY "Please enter the difficulty level between [1-7]:\n"
#define MSG_ERR_LEVEL "Error: invalid level (should be between 1 to 7)\n"
#define MSG_NEXT_MOVE "Please make the next move:\n"
#define MSG_ERR_HISTORY "Error: cannot undo previous move!\n"
#define HISTORY_SIZE 20
#define ERR_INVALID "Error: invalid command\n"
#define ERR_RANGE "Error: column number must be in range 1-7\n"
#define ERR_FULL "Error: column %d is full\n"
#define MSG_SUGGEST "Suggested move: drop a disc to column %d\n"
#define MSG_REMOVE_USER_DISC "Remove disc: remove user's disc at column %d\n"
#define MSG_REMOVE_PC_DISC "Remove disc: remove computer's disc at column %d\n"
#define MSG_EXIT "Exiting...\n"
#define MSG_RESTART "Game restarted!\n"
#define MSG_PCMOVE "Computer move: add disc to column %d\n"
#define MSG_YOU_WIN "Game over: you win\nPlease enter 'quit' to exit or 'restart' to start a new game!\n"
#define MSG_PC_WIN "Game over: computer wins\nPlease enter 'quit' to exit or 'restart' to start a new game!\n"
#define MSG_TIE "Game over: it's a tie\nPlease enter 'quit' to exit or 'restart' to start a new game!\n"
#define MSG_GAME_OVER "Error: the game is over\n"
#define ERR_MALLOC "Error: malloc has failed\n"

/**
 * a new type that is used to encapsulate a result of a game move
 */
typedef enum move_result_t {
	RESTART,
	QUIT,
	CONTINUE,
	PC_WON,
	USER_WON,
	TIE,
	ALLOCATION_ERROR
} MoveResult;

/**
 * Gets an input string from the user.
 * if string ends with '\n' character then it is trimmed
 * @return
 * a char array (string) with user input
 */
char* getUserInput();

/**
 * Returns an integer representation of the difficulty level asked by the user
 * the user is allowed to enter the string 'quit' or a number in the range of 1-7,
 * otherwise the method will call itself recursively.
 * @return
 * -1 - if an allocation error occured
 * 0 - if user entered the string 'quit'
 * otherwise, an integer in the range of 1-7 representing a game difficulty
 */
int inputDifficulty();

/**
 * If possible, removes a disc that was put in the previous move and prints a status message to the user.
 * @param gameState - a game instance
 * @return
 * true - if a disc was removed
 * false - if the user reached this action limit (history is empty)
 */
bool undoMove(SPFiarGame* gameState);

/**
 * Gets an input string from the user.
 * If input is not a valid command a message would be printed and the method will call itself recursively.
 * Otherwise, the method will act according to the command the user entered ('add_disc x', 'suggest_move', 'undo_move', 'restart_game' or 'quit')
 * If the input is a valid command but the action is illegal at the current time
 * (i.e. adding a disc to a full column or reaching the limit of undo_move) then a message would be printed and the method will call itself recursively.
 * @param gameState - a game instance
 * @param difficulty - difficulty of the game
 * @param prevRes - result of the previous move played
 * @return
 * CONTINUE - if a move was played successfully but there's no winner
 * USER_WON - if a move was played successfully and the user won the game
 * RESTART - if user requested to restart the game
 * QUIT - if user requested to quit
 */
MoveResult userMove(SPFiarGame* gameState, int difficulty, MoveResult prevRes);

/**
 * Plays a move of the PC using the minimax algorithm with max depth equal to the game difficulty
 * @param gameState - a game instance
 * @param difficulty - difficulty of the game (max depth of minimax algorithm)
 * @return
 * CONTINUE - if a move was played successfully but there's no winner
 * PC_WON - CONTINUE - if a move was played successfully and the PC won the game
 * TIE - CONTINUE - if a move was played successfully and game is finished with a tie
 */
MoveResult pcMove(SPFiarGame* gameState, int difficulty);

#endif
