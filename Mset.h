// COMP2521 25T3 - Assignment 1
// Interface to the Multiset ADT

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef MSET_H
#define MSET_H

#include <limits.h>
#include <stdbool.h>

#define UNDEFINED INT_MIN

typedef struct mset *Mset;

// Used by MsetAtIndex and MsetCursorGet
struct item {
	int elem;
	int count;
};

////////////////////////////////////////////////////////////////////////
// Basic Operations

/**
 * Creates a new empty multiset.
 */
Mset MsetNew(void);

/**
 * Frees all memory allocated to the multiset.
 */
void MsetFree(Mset s);

/**
 * Inserts one of an element into the multiset. Does nothing if the
 * element is equal to UNDEFINED.
 */
void MsetInsert(Mset s, int elem);

/**
 * Inserts the given amount of an element into the multiset. Does
 * nothing if the element is equal to UNDEFINED or the given amount is 0
 * or less.
 */
void MsetInsertMany(Mset s, int elem, int amount);

/**
 * Deletes one of an element from the multiset.
 */
void MsetDelete(Mset s, int elem);

/**
 * Deletes the given amount of an element from the multiset.
 */
void MsetDeleteMany(Mset s, int elem, int amount);

/**
 * Returns the number of distinct elements in the multiset.
 */
int MsetSize(Mset s);

/**
 * Returns the sum of counts of all elements in the multiset.
 */
int MsetTotalCount(Mset s);

/**
 * Returns the count of an element in the multiset, or 0 if it doesn't
 * occur in the multiset.
 */
int MsetGetCount(Mset s, int elem);

/**
 * Prints the multiset to a file.
 * The elements of the multiset should be printed in ascending order
 * inside a pair of curly braces, with elements separated by a comma
 * and space. Each element should be printed inside a pair of
 * parentheses with its count, separated by a comma and space.
 */
void MsetPrint(Mset s, FILE *file);

////////////////////////////////////////////////////////////////////////
// Advanced Operations

/**
 * Returns a new multiset representing the union of the two given
 * multisets.
 */
Mset MsetUnion(Mset s1, Mset s2);

/**
 * Returns a new multiset representing the intersection of the two
 * given multisets.
 */
Mset MsetIntersection(Mset s1, Mset s2);

/**
 * Returns true if the multiset s1 is included in the multiset s2, and
 * false otherwise.
 */
bool MsetIncluded(Mset s1, Mset s2);

/**
 * Returns true if the two given multisets are equal, and false
 * otherwise.
 */
bool MsetEquals(Mset s1, Mset s2);

////////////////////////////////////////////////////////////////////////
// Index Operations

/**
 * Returns the element at the given index and its count, or
 * {UNDEFINED, 0} if the given index is outside the range [0, n - 1]
 * where n is the size of the multiset.
 */
struct item MsetAtIndex(Mset s, int index);

/**
 * Returns the index of the given element if it exists, or -1 otherwise.
 */
int MsetIndexOf(Mset s, int elem);

////////////////////////////////////////////////////////////////////////
// Cursor Operations

typedef struct cursor *MsetCursor;

/**
 * Creates a new cursor positioned at the start of the multiset.
 * (see spec for explanation of start and end)
 */
MsetCursor MsetCursorNew(Mset s);

/**
 * Frees all memory allocated to the given cursor.
 */
void MsetCursorFree(MsetCursor cur);

/**
 * Returns the element at the cursor's position and its count, or
 * {UNDEFINED, 0} if the cursor is positioned at the start or end of
 * the multiset.
 */
struct item MsetCursorGet(MsetCursor cur);

/**
 * Moves the cursor to the next greatest element, or to the end of the
 * multiset if there is no next greatest element. Does not move the
 * cursor if it is already at the end. Returns false if the cursor is at
 * the end after this operation, and true otherwise.
 */
bool MsetCursorNext(MsetCursor cur);

/**
 * Moves the cursor to the next smallest element, or to the start of the
 * multiset if there is no next smallest element. Does not move the
 * cursor if it is already at the start. Returns false if the cursor is
 * at the start after this operation, and true otherwise.
 */
bool MsetCursorPrev(MsetCursor cur);

////////////////////////////////////////////////////////////////////////

#endif

