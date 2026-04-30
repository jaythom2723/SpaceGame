#include "engine_utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

EngineLinkedList* engineCreateLinkedList(char* lista, char* listb, size_t arrsize, size_t typesize)
{
	EngineLinkedList* list = malloc(sizeof(EngineLinkedList));
	if (list == NULL)
		return NULL;

	list->lista = malloc(arrsize * typesize);
	if (list->lista == NULL)
	{
		free(list);
		return NULL;
	}

	list->listb = malloc(arrsize * typesize);
	if (list->listb == NULL)
	{
		free(list->lista);
		free(list);
		return NULL;
	}


	list->arrsize = arrsize;
	list->typesize = typesize;

	memset(list->lista, 0, arrsize * typesize);
	memset(list->listb, 0, arrsize * typesize);

	if (lista != NULL)
		memcpy(list->lista, lista, arrsize * typesize);

	if (listb != NULL)
		memcpy(list->listb, listb, arrsize * typesize);

	return list;
}

void engineDestroyLinkedList(EngineLinkedList* list)
{
	free(list->lista);
	list->lista = NULL;

	free(list->listb);
	list->listb = NULL;

	free(list);
	list = NULL;
}

const EBOOL _checkisoutbounds(EngineLinkedList* list, int index)
{
	return (list->lista + index > list->lista + list->arrsize
			|| list->listb + index > list->listb + list->arrsize
		);
}

const EBOOL engineSetValue(EngineLinkedList* list, int index, int value)
{
	if (_checkisoutbounds(list, index) == ETRUE)
		return EFALSE;

	memset(list->lista + index, value, list->typesize);
	memset(list->listb + index, value, list->typesize);

	return ETRUE;
}

void engineGetValues(EngineLinkedList* list, char* desta, char* destb)
{
	if (desta != NULL)
		memcpy(desta, list->lista, list->arrsize * list->typesize);

	if (destb != NULL)
		memcpy(destb, list->listb, list->arrsize * list->typesize);
}

char* engineReadFile(const char* path)
{
	char* source;
	size_t size;
	FILE* fp;

	fp = fopen(path, "rb");
	if (fp == NULL)
	{
		fprintf(stderr, "Could not find file specified!\n\t%s\n", path);
		return NULL;
	}

	fseek(fp, 0l, SEEK_END);
	size = ftell(fp);
	rewind(fp);

	source = calloc(size + 1, sizeof(char));
	if (source == NULL)
	{
		fprintf(stderr, "Could not read the file!\n\t%s\n", path);
		fclose(fp);
		return NULL;
	}
	fread(source, sizeof(char), size + 1, fp);

	fclose(fp);

	return source;
}

int engineGetRandomRangeI(int min, int max)
{
	return (rand() % (max - min + 1) + min);
}

double engineGetRandomRangeD(double min, double max)
{
	return min + ((double)rand() / (double)RAND_MAX) * (max - min);
}
