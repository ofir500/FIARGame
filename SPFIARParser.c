/*
 * SPFiarParser.c
 *
 *  Created on: 10/06/2017
 *      Author: Shawn Molga & Ofir Feffer
 */

#include "SPFIARParser.h"

bool spParserIsInt(const char* str) {
	int i = 0;
	if (str[0] == '-') {
		i++;
	}
	while (str[i] != '\0') {
		if (str[i] < '0' || str[i] > '9') {
			return false;
		}
		i++;
	}
	return true;
}

SP_COMMAND strToCMD(const char* str) {
	if (str == NULL) {
		return SP_INVALID_LINE;
	} else if (strcmp(str, "add_disc") == 0) {
		return SP_ADD_DISC;
	} else if (strcmp(str, "undo_move") == 0) {
		return SP_UNDO_MOVE;
	} else if (strcmp(str, "suggest_move") == 0) {
		return SP_SUGGEST_MOVE;
	} else if (strcmp(str, "quit") == 0) {
		return SP_QUIT;
	} else if (strcmp(str, "restart_game") == 0) {
		return SP_RESTART;
	} else {
		return SP_INVALID_LINE;
	}
}

SPCommand spParserPraseLine(const char* str) {
	SPCommand command;
	char strcopy[SP_MAX_LINE_LENGTH];
	strcpy(strcopy, str);
	const char *delim = " \t\r\n";
	char *token;

	/* get the first token */
	token = strtok(strcopy, delim);
	command.cmd = strToCMD(token);
	command.validArg = false;

	// get second token
	token = strtok(NULL, delim);

	if (token != NULL && command.cmd != SP_ADD_DISC) {
		command.cmd = SP_INVALID_LINE;
	} else if (token != NULL && command.cmd == SP_ADD_DISC && spParserIsInt(token) == true) {
	   command.arg = atoi(token);
	   command.validArg = true;
	}

	return command;
}
