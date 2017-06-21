/*
 * main.c
 *
 *  Created on: 10/06/2017
 *      Author: Shawn Molga & Ofir Feffer
 */

#include <stdio.h>
#include <stdlib.h>
#include "SPFIARGame.h"
#include "SPMainAux.h"

void quitGame(SPFiarGame *gameState) {
    if (gameState != NULL) {
        spFiarGameDestroy(gameState);
    }
    printf(MSG_EXIT);
    exit(0);
}

void mallocError(SPFiarGame *gameState) {
	if (gameState != NULL) {
		spFiarGameDestroy(gameState);
	}
    printf(ERR_MALLOC);
	exit(0);
}

int main() {
	int difficulty = inputDifficulty();
	if (difficulty == 0) {
        quitGame(NULL);
	} else if (difficulty == -1) {
        mallocError(NULL);
    }

	SPFiarGame* gameState = spFiarGameCreate(HISTORY_SIZE);
    if (gameState == NULL) {
        mallocError(NULL);
    }

	MoveResult prevRes = CONTINUE, res;
	for (;;) {
		if (gameState->currentPlayer == 'X' || prevRes == USER_WON) {
			if (prevRes == CONTINUE || prevRes == RESTART) {
				spFiarGamePrintBoard(gameState);
				printf(MSG_NEXT_MOVE);
			}
			res = userMove(gameState, difficulty, prevRes);
			switch (res) {
				case CONTINUE:
					break;

				case USER_WON:
					spFiarGamePrintBoard(gameState);
					printf(MSG_YOU_WIN);
					break;

				case RESTART:
					restartGame(gameState);
					printf(MSG_RESTART);
					difficulty = inputDifficulty();
					if (difficulty == 0) {
                        quitGame(gameState);
					} else if (difficulty == -1) {
						mallocError(gameState);
					}
					break;

				case QUIT:
                    quitGame(gameState);
					return 0;

				case ALLOCATION_ERROR:
					mallocError(gameState);

				default:
					break;
			}
		} else {
			res = pcMove(gameState, difficulty);
			switch (res) {
				case CONTINUE:
					break;

				case PC_WON:
					spFiarGamePrintBoard(gameState);
					printf(MSG_PC_WIN);
					break;

				case TIE:
					spFiarGamePrintBoard(gameState);
					printf(MSG_TIE);
					break;

				case ALLOCATION_ERROR:
					mallocError(gameState);

				default:
					break;
			}
		}
		prevRes = res;
	}
}
