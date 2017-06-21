/*
 * SPArrayList.c
 *
 *  Created on: 10/06/2017
 *      Author: Shawn Molga & Ofir Feffer
 */

#include "SPArrayList.h"

SPArrayList* spArrayListCreate(int maxSize) {
	if (maxSize <= 0) {
		return NULL;
	}
	SPArrayList* list = (SPArrayList*) malloc(sizeof(SPArrayList));
	if (list == NULL) {
		return NULL;
	}
	list->elements = (int*) malloc(maxSize * sizeof(int));
	if (list->elements == NULL) {
		free(list);
		return NULL;
	}
	list->actualSize = 0;
	list->maxSize = maxSize;
	return list;
}

SPArrayList* spArrayListCopy(SPArrayList* src) {
	if (src == NULL) {
		return NULL;
	}
	SPArrayList* copy = spArrayListCreate(src->maxSize);
	if (copy == NULL) {
		return NULL;
	}

	for (int i = 0; i < src->actualSize; i++) {
		spArrayListAddLast(copy, spArrayListGetAt(src, i));
	}
	return copy;
}

void spArrayListDestroy(SPArrayList* src) {
	if (src == NULL) {
		return;
	}
	if (src->elements != NULL) {
		free(src->elements);
	}
	free(src);
}

SP_ARRAY_LIST_MESSAGE spArrayListClear(SPArrayList* src) {
	if (src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	src->actualSize = 0;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddAt(SPArrayList* src, int elem, int index) {
	if (src == NULL || index < 0 || index > src->actualSize) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	if (src->actualSize == src->maxSize) {
		return SP_ARRAY_LIST_FULL;
	}

	for (int i = src->actualSize; i > index; i--) {
		src->elements[i] = src->elements[i-1];
	}
	src->elements[index] = elem;
	src->actualSize++;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListAddFirst(SPArrayList* src, int elem) {
	return spArrayListAddAt(src, elem, 0);
}

SP_ARRAY_LIST_MESSAGE spArrayListAddLast(SPArrayList* src, int elem) {
	if (src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	}
	return spArrayListAddAt(src, elem, src->actualSize);
}

void shitLeft(SPArrayList *src, int from) {
	for (int i = from ; i < src->actualSize - 1; i++) {
		src->elements[i] = src->elements[i+1];
	}
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveAt(SPArrayList* src, int index){
	if (src == NULL || index < 0 || index >= src->actualSize) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	} else if (src->actualSize == 0) {
		return SP_ARRAY_LIST_EMPTY;
	}

	shitLeft(src, index);
	src->actualSize--;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveFirst(SPArrayList* src){
	if (src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	} else if (src->actualSize == 0) {
		return SP_ARRAY_LIST_EMPTY;
	}

	shitLeft(src, 0);
	src->actualSize--;
	return SP_ARRAY_LIST_SUCCESS;
}

SP_ARRAY_LIST_MESSAGE spArrayListRemoveLast(SPArrayList* src){
	if (src == NULL) {
		return SP_ARRAY_LIST_INVALID_ARGUMENT;
	} else if (src->actualSize == 0) {
		return SP_ARRAY_LIST_EMPTY;
	}
	src->actualSize--;
	return SP_ARRAY_LIST_SUCCESS;
}

int spArrayListGetAt(SPArrayList* src, int index){
	if (src == NULL || index < 0 || index >= src->actualSize) {
		return -1;
	}
	return src->elements[index];
}

int spArrayListGetFirst(SPArrayList* src) {
	return spArrayListGetAt(src, 0);
}

int spArrayListGetLast(SPArrayList* src) {
	if (src == NULL) {
		return -1;
	}
	return spArrayListGetAt(src, src->actualSize - 1);
}

int spArrayListMaxCapacity(SPArrayList* src) {
	if (src == NULL) {
		return -1;
	}
	return src->maxSize;
}

int spArrayListSize(SPArrayList* src) {
	if (src == NULL) {
		return -1;
	}
	return src->actualSize;
}

bool spArrayListIsFull(SPArrayList* src) {
	if (src == NULL || src->actualSize < src->maxSize) {
		return false;
	}

	return true;
}

bool spArrayListIsEmpty(SPArrayList* src) {
	if (src == NULL || src->actualSize > 0) {
		return false;
	}

	return true;
}
