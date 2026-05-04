#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include "engine_defs.h"

#include <stdlib.h>

#define ENGINE_DEF_LINK_LIST_ALLOC_SIZE 0xFF

/**
 * \brief Creates a linked list.
 * 
 * \param lista The first list to copy values from (can be NULL)
 * \param listb The second list to copy values from (can be NULL)
 * \param arrsize The number of elements in the array
 * \param typesze The size of the datatype in bytes
 * 
 * \return The created EngineLinkedList
 */
EngineLinkedList* engineCreateLinkedList(char* lista, char* listb, size_t arrsize, size_t typesize);

/**
 * \brief Destroys a linked list.
 * 
 * \param list The list to destroy
 */
void engineDestroyLinkedList(EngineLinkedList* list);

// TODO: come up with better names for lines 13-14

/**
 * \brief Set a value to a EngineLinkedList.
 * 
 * \param list The list to set the value to
 * \param index The index to set the value of
 * \param value The the value to set
 * 
 * \return Returns EFALSE if the operation was unsuccessful
 */
const EBOOL engineSetValue(EngineLinkedList* list, int index, int value);

/**
 * \brief Get values from a EngineLinkedList.
 * 
 * \param list The list to get values from
 * \param desta The first destination (can be NULL)
 * \param destb The second destination (can be NULL)
 */
void engineGetValues(EngineLinkedList* list, char* desta, char* destb);

/**
 * \brief Reads a file from the disk.
 * 
 * \param path The path of the file to read.
 * 
 * \return Returns the contents of the read file. (must free)
 */
char* engineReadFile(const char* path);

/**
 * \brief Generate a random number inclusively (int).
 * 
 * \param min The minimum value
 * \param max The maximum value
 * 
 * \return Returns the number generated
 */
int engineGetRandomRangeI(int min, int max);

/**
 * \brief Generate a random number inclusively (double).
 * 
 * \param min The minimum value
 * \param max The maximum value
 * 
 * \return Returns the number generated
 */
double engineGetRandomRangeD(double min, double max);

/**
 * \brief Sort an array using quick sort.
 * 
 * \param src The array to sort
 * \param sorted The sorted array
 * \param arrSize The array's size in elements
 * \param tsize The size of a single element in bytes
 */
void engineQuickSort(int* arr, size_t arrSize, size_t tsize);

void engineQuickSortF(float* arr, size_t arrSize, size_t tsize);

#endif // ENGINE_H


