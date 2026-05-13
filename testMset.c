// COMP2521 25T3 - Assignment 1
// Main program for testing the Multiset ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Mset.h"
#include "MsetStructs.h"

// These functions are deliberately not static to make testing more
// convenient.

void testMsetInsert(void);
void testMsetInsertMany(void);
void testMsetDelete(void);
void testMsetDeleteMany(void);
void testMsetSize(void);
void testMsetTotalCount(void);
void testMsetGetCount(void);
void testMsetPrint(void);
void checkMsetPrint(Mset s, char *expectedPrint);
void checkFileContents(FILE *file, char *expectedContents);

void testMsetUnion(void);
void testMsetIntersection(void);
void testMsetIncluded(void);
void testMsetEquals(void);

void testBalance1(void);
void testBalance2(void);
bool isHeightBalanced(struct node *t);
bool doIsHeightBalanced(struct node *t, int *height);

void testMsetAtIndex(void);
void testMsetIndexOf(void);

void testMsetCursor1(void);
void testMsetCursor2(void);

int main(int argc, char *argv[]) {
	printf("Part 1\n");

	printf("MsetInsert\n");
	testMsetInsert();
	printf("\n");

	printf("Testing MsetInsertMany...\n");
	testMsetInsertMany();
	printf("\n");

	printf("Testing MsetDelete...\n");
	testMsetDelete();
	printf("\n");

	printf("Testing MsetDeleteMany...\n");
	testMsetDeleteMany();
	printf("\n");

	printf("Testing MsetSize...\n");
	testMsetSize();
	printf("\n");

	printf("Testing MsetTotalCount...\n");
	testMsetTotalCount();
	printf("\n");

	printf("Testing MsetGetCount...\n");
	testMsetGetCount();
	printf("\n");

	printf("Testing MsetPrint...\n");
	testMsetPrint();
	printf("\n\n");


	printf("Part 2\n");

	printf("Testing MsetUnion...\n");
	testMsetUnion();
	printf("\n");

	printf("Testing MsetIntersection...\n");
	testMsetIntersection();
	printf("\n");

	printf("Testing MsetIncluded...\n");
	testMsetIncluded();
	printf("\n");

	printf("Testing MsetEquals...\n");
	testMsetEquals();
	printf("\n\n");

	printf("Part 3\n");
	printf("Testing balance...\n");
	testBalance1();
	testBalance2();
	printf("\n\n");

	printf("Part 4\n");
	printf("Testing MsetAtIndex...\n");
	testMsetAtIndex();
	printf("\nTesting MsetIndexOf...\n");
	testMsetIndexOf();
	printf("\n\n");

	printf("Part 5\n");
	printf("Testing MsetCursor...\n");
	testMsetCursor1();
	testMsetCursor2();
}

void testMsetInsert(void) {
	// NOTE: MsetInsert can't be tested on its own unless we directly
	//       access the internal representation of the ADT
	printf("Test 1: Normal usage\n");
	Mset s = MsetNew();

	MsetInsert(s, 4);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetInsert(s, 3);
	MsetInsert(s, 7);
	MsetInsert(s, 3);
	MsetInsert(s, 7);

	assert(MsetSize(s) == 4);
	assert(MsetTotalCount(s) == 7);
	assert(MsetGetCount(s, 1) == 1);
	assert(MsetGetCount(s, 3) == 2);
	assert(MsetGetCount(s, 4) == 1);
	assert(MsetGetCount(s, 7) == 3);
	assert(MsetGetCount(s, 9) == 0);

	checkMsetPrint(s, "{(1, 1), (3, 2), (4, 1), (7, 3)}");

	MsetFree(s);

	printf("Test 2: 1 node\n");
	s = MsetNew();
	MsetInsert(s, 1);
	assert(MsetSize(s) == 1);
	assert(MsetTotalCount(s) == 1);
	assert(MsetGetCount(s, 1) == 1);

	MsetInsert(s, 1);
	assert(MsetSize(s) == 1);
	assert(MsetTotalCount(s) == 2);
	assert(MsetGetCount(s, 1) == 2);
	MsetFree(s);

	printf("Test 3: elem is undefined\n");
	s = MsetNew();
	MsetInsert(s, UNDEFINED);
	assert(MsetSize(s) == 0);
	assert(MsetTotalCount(s) == 0);
	MsetFree(s);
}

void testMsetInsertMany(void) {
	// NOTE: The note in testMsetInsert also applies to MsetInsertMany
	printf("Test 1: normal\n");
	Mset s = MsetNew();

	MsetInsertMany(s, 4, 2);  // insert two 4's
	MsetInsertMany(s, 7, 3);  // insert three 7's
	MsetInsertMany(s, 1, 5);  // insert five 1's
	MsetInsertMany(s, 3, 1);  // insert one 3

	checkMsetPrint(s, "{(1, 5), (3, 1), (4, 2), (7, 3)}");

	assert(MsetSize(s) == 4);
	assert(MsetTotalCount(s) == 11);
	assert(MsetGetCount(s, 1) == 5);
	assert(MsetGetCount(s, 7) == 3);
	assert(MsetGetCount(s, 4) == 2);
	assert(MsetGetCount(s, 3) == 1);
	assert(MsetGetCount(s, 9) == 0);

	MsetFree(s);

	printf("Test 2: One node\n");
	s = MsetNew();
	MsetInsertMany(s, 1, 1);
	assert(MsetSize(s) == 1);
	assert(MsetTotalCount(s) == 1);
	assert(MsetGetCount(s, 1) == 1);
	MsetInsertMany(s, 1, 13);
	assert(MsetSize(s) == 1);
	assert(MsetTotalCount(s) == 14);
	assert(MsetGetCount(s, 1) == 14);
	MsetFree(s);

	printf("Test 3: Undefined elem\n");
	s = MsetNew();
	MsetInsertMany(s, UNDEFINED, 123);
	assert(MsetSize(s) == 0);
	assert(MsetTotalCount(s) == 0);

	printf("Test 4: negative amount inserted\n");
	MsetInsertMany(s, 11, -1);
	assert(MsetSize(s) == 0);
	assert(MsetTotalCount(s) == 0);
	MsetFree(s);
}

void testMsetDelete(void) {
	// NOTE: MsetDelete can't be tested without either MsetInsert or
	//       MsetInsertMany
	printf("Test 1: removing a node\n");
	Mset s = MsetNew();

	MsetInsert(s, 4);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetInsert(s, 3);
	MsetInsert(s, 7);
	MsetInsert(s, 3);
	MsetInsert(s, 7);

	MsetDelete(s, 1);
	assert(MsetSize(s) == 3);
	assert(MsetTotalCount(s) == 6);
	assert(MsetGetCount(s, 1) == 0);

	printf("Test 2: deleting a count of an element\n");
	MsetDelete(s, 7);
	assert(MsetSize(s) == 3);
	assert(MsetTotalCount(s) == 5);
	assert(MsetGetCount(s, 7) == 2);

	printf("Test 3: deleting an element that's already deleted from bst\n");
	MsetDelete(s, 1);
	assert(MsetSize(s) == 3);
	assert(MsetTotalCount(s) == 5);
	assert(MsetGetCount(s, 1) == 0);

	printf("Test 4: deleting an element that doesn't exist\n");
	MsetDelete(s, 6);
	assert(MsetSize(s) == 3);
	assert(MsetTotalCount(s) == 5);
	assert(MsetGetCount(s, 6) == 0);

	MsetFree(s);
}

void testMsetDeleteMany(void) {
	Mset s = MsetNew();

	MsetInsertMany(s, 4, 2);
	MsetInsertMany(s, 7, 3);
	MsetInsertMany(s, 1, 5);
	MsetInsertMany(s, 3, 1);

	printf("Test 1: deleting multiple counts of an element\n");
	MsetDeleteMany(s, 1, 2);
	assert(MsetSize(s) == 4);
	assert(MsetTotalCount(s) == 9);
	assert(MsetGetCount(s, 1) == 3);

	printf("Test 2: deleting a node\n");
	MsetDeleteMany(s, 3, 1);
	assert(MsetSize(s) == 3);
	assert(MsetTotalCount(s) == 8);
	assert(MsetGetCount(s, 3) == 0);

	printf("Test 3: deleting excess counts of an element\n");
	MsetDeleteMany(s, 1, 4);
	assert(MsetSize(s) == 2);
	assert(MsetTotalCount(s) == 5);
	assert(MsetGetCount(s, 1) == 0);

	printf("Test 4: deleting a node that's already deleted\n");
	MsetDeleteMany(s, 1, 4);
	assert(MsetSize(s) == 2);
	assert(MsetTotalCount(s) == 5);
	assert(MsetGetCount(s, 1) == 0);

	printf("Test 5: deleting an elem that doesn't exist\n");
	MsetDeleteMany(s, 6, 3);
	assert(MsetSize(s) == 2);
	assert(MsetTotalCount(s) == 5);
	assert(MsetGetCount(s, 6) == 0);

	MsetFree(s);
}

void testMsetSize(void) {
	Mset s = MsetNew();
	assert(MsetSize(s) == 0);

	printf("Test 1: normal\n");
	MsetInsert(s, 4);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetInsert(s, 3);
	assert(MsetSize(s) == 4);

	MsetInsert(s, 8);
	MsetInsert(s, 1);
	assert(MsetSize(s) == 5);

	MsetInsertMany(s, 6, 5);
	MsetInsertMany(s, 4, 2);
	assert(MsetSize(s) == 6);

	MsetFree(s);

	printf("Test 2: empty\n");
	s = MsetNew();
	assert(MsetSize(s) == 0);
	MsetFree(s);

	printf("Test 3: Mset = NULL\n");
	s = NULL;
	assert(MsetSize(s) == 0);
}

void testMsetTotalCount(void) {
	Mset s = MsetNew();

	printf("Test 1: normal\n");
	MsetInsert(s, 4);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetInsert(s, 3);
	assert(MsetTotalCount(s) == 4);

	MsetInsert(s, 8);
	MsetInsert(s, 1);
	assert(MsetTotalCount(s) == 6);

	MsetInsertMany(s, 6, 5);
	MsetInsertMany(s, 4, 2);
	assert(MsetTotalCount(s) == 13);

	MsetFree(s);

	printf("Test 2: empty\n");
	s = MsetNew();
	assert(MsetTotalCount(s) == 0);
	MsetFree(s);

	printf("Test 3: s == NULL\n");
	s = NULL;
	assert(MsetTotalCount(s) == 0);
}

void testMsetGetCount(void) {
	Mset s = MsetNew();

	printf("Test 1: normal\n");
	MsetInsert(s, 4);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetInsert(s, 3);
	assert(MsetGetCount(s, 1) == 1);
	assert(MsetGetCount(s, 3) == 1);
	assert(MsetGetCount(s, 4) == 1);
	assert(MsetGetCount(s, 7) == 1);

	printf("Test 2: elem doesn't exist\n");
	assert(MsetGetCount(s, 8) == 0);

	printf("Test 3: adding new elems into bst\n");
	MsetInsert(s, 8);
	MsetInsert(s, 1);
	assert(MsetGetCount(s, 8) == 1);
	MsetInsertMany(s, 6, 5);
	MsetInsertMany(s, 4, 2);
	assert(MsetGetCount(s, 6) == 5);
	assert(MsetGetCount(s, 4) == 3);

	printf("Test 4: increasing count of pre-existing elem\n");
	assert(MsetGetCount(s, 1) == 2);

	printf("Test 5: count of deleted elem\n");
	MsetDelete(s, 7);
	assert(MsetGetCount(s, 7) == 0);

	printf("Test 6: count of excessively deleted elem\n");
	MsetDeleteMany(s, 1, 40);
	assert(MsetGetCount(s, 1) == 0);

	MsetFree(s);

	printf("Test 7: s == NULL\n");
	s = NULL;
	assert(MsetGetCount(s, 8) == 0);
}

void testMsetPrint(void) {
	Mset s = MsetNew();

	printf("Test 1: normal\n");
	MsetInsert(s, 4);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetInsert(s, 3);
	MsetInsert(s, 7);
	MsetInsert(s, 3);
	MsetInsert(s, 7);

	checkMsetPrint(s, "{(1, 1), (3, 2), (4, 1), (7, 3)}");

	MsetFree(s);

	s = MsetNew();
	printf("Test 2: empty\n");
	checkMsetPrint(s, "{}");

	printf("Test 3: 1 elem only\n");
	MsetInsert(s, 1);
	checkMsetPrint(s, "{(1, 1)}");

	MsetInsertMany(s, 1, 11);
	checkMsetPrint(s, "{(1, 12)}");
	MsetFree(s);

	printf("Test 3: s == NULL\n");
	s = NULL;
	checkMsetPrint(s, "{}");
}

void checkMsetPrint(Mset s, char *expectedPrint) {
	FILE *out = tmpfile();
	MsetPrint(s, out);
	checkFileContents(out, expectedPrint);
	fclose(out);
}

/**
 * This function checks if the contents of a file matches the expected
 * contents. If the contents do not match, the program will exit with an
 * assertion error.
 */
void checkFileContents(FILE *file, char *expectedContents) {
	fflush(file);
	fseek(file, 0, SEEK_SET);
	char *line = NULL;
	size_t n = 0;
	getline(&line, &n, file);
	if (strcmp(line, expectedContents) != 0) {
		printf("Test failed for MsetPrint: expected \"%s\", "
		       "saw \"%s\"\n",
		       expectedContents, line);

		assert(strcmp(line, expectedContents) == 0);
	}
	free(line);
}

////////////////////////////////////////////////////////////////////////

void testMsetUnion(void) {
	printf("Test 1: normal\n");
	Mset a = MsetNew();
	MsetInsertMany(a, 4, 2);
	MsetInsertMany(a, 7, 3);
	MsetInsertMany(a, 1, 5);
	MsetInsertMany(a, 3, 1);

	Mset b = MsetNew();
	MsetInsertMany(b, 4, 4);
	MsetInsertMany(b, 7, 1);
	MsetInsertMany(b, 3, 2);
	MsetInsertMany(b, 8, 3);

	Mset c = MsetUnion(a, b);
	assert(MsetGetCount(c, 1) == 5);
	assert(MsetGetCount(c, 3) == 2);
	assert(MsetGetCount(c, 4) == 4);
	assert(MsetGetCount(c, 7) == 3);
	assert(MsetGetCount(c, 8) == 3);
	assert(MsetSize(c) == 5);
	assert(MsetTotalCount(c) == 17);

	MsetFree(a);
	MsetFree(b);
	MsetFree(c);

	printf("Test 2: 1 empty set\n");
	a = MsetNew();

	b = MsetNew();
	MsetInsertMany(b, 4, 4);
	MsetInsertMany(b, 7, 1);
	MsetInsertMany(b, 3, 2);
	MsetInsertMany(b, 8, 3);

	c = MsetUnion(a, b);
	assert(MsetGetCount(c, 1) == 0);
	assert(MsetGetCount(c, 3) == 2);
	assert(MsetGetCount(c, 4) == 4);
	assert(MsetGetCount(c, 7) == 1);
	assert(MsetGetCount(c, 8) == 3);
	assert(MsetSize(c) == 4);
	assert(MsetTotalCount(c) == 10);

	MsetFree(b);
	MsetFree(c);

	printf("Test 3: both empty\n");
	b = MsetNew();
	c = MsetUnion(a, b);

	assert(MsetGetCount(c, 1) == 0);
	assert(MsetSize(c) == 0);
	assert(MsetTotalCount(c) == 0);

	MsetFree(a);
	MsetFree(b);
	MsetFree(c);
}

void testMsetIntersection(void) {
	printf("Test 1: normal\n");
	Mset a = MsetNew();
	MsetInsertMany(a, 4, 2);
	MsetInsertMany(a, 7, 3);
	MsetInsertMany(a, 1, 5);
	MsetInsertMany(a, 3, 1);

	Mset b = MsetNew();
	MsetInsertMany(b, 4, 4);
	MsetInsertMany(b, 7, 1);
	MsetInsertMany(b, 3, 2);
	MsetInsertMany(b, 8, 3);

	Mset c = MsetIntersection(a, b);
	assert(MsetGetCount(c, 1) == 0);
	assert(MsetGetCount(c, 3) == 1);
	assert(MsetGetCount(c, 4) == 2);
	assert(MsetGetCount(c, 7) == 1);
	assert(MsetGetCount(c, 8) == 0);
	assert(MsetSize(c) == 3);
	assert(MsetTotalCount(c) == 4);

	MsetFree(a);
	MsetFree(b);
	MsetFree(c);

	printf("Test 2: 1 empty set\n");
	a = MsetNew();
	MsetInsertMany(a, 4, 2);
	MsetInsertMany(a, 7, 3);
	MsetInsertMany(a, 1, 5);
	MsetInsertMany(a, 3, 1);

	b = MsetNew();
	c = MsetIntersection(a, b);
	
	assert(MsetGetCount(c, 5) == 0);
	assert(MsetSize(c) == 0);
	assert(MsetTotalCount(c) == 0);

	MsetFree(a);
	MsetFree(c);

	printf("Test 3: both empty\n");
	a = MsetNew();
	c = MsetIntersection(a, b);

	assert(MsetGetCount(c, 9) == 0);
	assert(MsetSize(c) == 0);
	assert(MsetTotalCount(c) == 0);

	MsetFree(a);
	MsetFree(b);
	MsetFree(c);
}

void testMsetIncluded(void) {
	printf("Test 1: a included in b but not in c\n");
	Mset a = MsetNew();
	MsetInsertMany(a, 4, 2);
	MsetInsertMany(a, 7, 3);
	MsetInsertMany(a, 1, 5);

	Mset b = MsetNew();
	MsetInsertMany(b, 4, 5);
	MsetInsertMany(b, 7, 3);
	MsetInsertMany(b, 1, 6);
	MsetInsertMany(b, 3, 1);

	Mset c = MsetNew();
	MsetInsertMany(c, 4, 5);
	MsetInsertMany(c, 7, 3);
	MsetInsertMany(c, 1, 4);
	MsetInsertMany(c, 3, 1);

	assert(MsetIncluded(a, b));
	assert(!MsetIncluded(a, c));

	MsetFree(a);
	MsetFree(b);
	MsetFree(c);

	printf("Test 2: empty set included in b\n");
	a = MsetNew();
	b = MsetNew();
	MsetInsertMany(b, 4, 5);
	MsetInsertMany(b, 7, 3);
	MsetInsertMany(b, 1, 6);
	MsetInsertMany(b, 3, 1);
	assert(MsetIncluded(a, b));

	printf("Test 3: b not included in empty set\n");
	assert(!MsetIncluded(b, a));

	printf("Test 4: empty set included in empty set\n");
	c = MsetNew();
	assert(MsetIncluded(a, c));
	assert(MsetIncluded(c, a));

	MsetFree(a);
	MsetFree(b);
	MsetFree(c);

	printf("Test 5: a = b\n");
	a = MsetNew();
	MsetInsertMany(a, 4, 2);
	MsetInsertMany(a, 7, 3);
	MsetInsertMany(a, 1, 5);

	b = MsetNew();
	MsetInsertMany(b, 4, 2);
	MsetInsertMany(b, 7, 3);
	MsetInsertMany(b, 1, 5);

	assert(MsetIncluded(a, b));
	assert(MsetIncluded(b, a));
	
	MsetFree(a);
	MsetFree(b);	
}

void testMsetEquals(void) {
	printf("Test 1: a = b but a =/= c\n");
	Mset a = MsetNew();
	MsetInsertMany(a, 4, 2);
	MsetInsertMany(a, 7, 3);
	MsetInsertMany(a, 1, 5);
	MsetInsertMany(a, 3, 1);

	Mset b = MsetNew();
	MsetInsertMany(b, 3, 1);
	MsetInsertMany(b, 1, 5);
	MsetInsertMany(b, 7, 3);
	MsetInsertMany(b, 4, 2);

	Mset c = MsetNew();
	MsetInsertMany(c, 4, 2);
	MsetInsertMany(c, 7, 3);
	MsetInsertMany(c, 1, 5);
	MsetInsertMany(c, 3, 4);

	assert(MsetEquals(a, b));
	assert(!MsetEquals(a, c));

	MsetFree(a);
	MsetFree(b);
	MsetFree(c);

	printf("Test 2: empty set =/= b\n");
	a = MsetNew();

	b = MsetNew();
	MsetInsertMany(b, 3, 1);
	MsetInsertMany(b, 1, 5);
	MsetInsertMany(b, 7, 3);
	MsetInsertMany(b, 4, 2);
	
	assert(!MsetEquals(a, b));

	printf("Test 3: empty set = empty set\n");
	c = MsetNew();
	assert(MsetEquals(a, c));

	MsetFree(a);
	MsetFree(b);
	MsetFree(c);
}

////////////////////////////////////////////////////////////////////////

void testBalance1(void) {
	printf("Test 1: rebalance afer insertion\n");
	Mset s = MsetNew();

	MsetInsert(s, 8);
	MsetInsert(s, 5);
	MsetInsert(s, 2);

	// The tree should have been rebalanced after inserting 2
	// NOTE: Normally, a user should not have access to the concrete
	//       representation of an ADT, but since we have #included
	//       MsetStructs.h, we have access for testing purposes.
	assert(isHeightBalanced(s->tree));

	MsetFree(s);
	
	printf("Test 2: empty Mset\n");
	s = MsetNew();
	assert(isHeightBalanced(s->tree));
	MsetFree(s);

	printf("Test 3: rebalance after deletion\n");
	s = MsetNew();

	MsetInsert(s, 4);
	MsetInsert(s, 2);
	MsetInsert(s, 7);
	MsetInsert(s, 1);
	MsetDelete(s, 7);

	// The tree should have been rebalanced after deleting 7
	assert(isHeightBalanced(s->tree));

	MsetFree(s);

}

void testBalance2(void) {
	printf("Test 4: rebalance after union\n");
	Mset a = MsetNew();
	MsetInsert(a, 4);
	MsetInsert(a, 2);
	MsetInsert(a, 7);
	MsetInsert(a, 1);

	Mset b = MsetNew();
	MsetInsert(b, 4);
	MsetInsert(b, 2);
	MsetInsert(b, 7);
	MsetInsert(b, 0);

	Mset c = MsetUnion(a, b);
	assert(isHeightBalanced(c->tree));

	MsetFree(a);
	MsetFree(c);

	printf("Test 5: union between Mset and empty Mset\n");
	a = MsetNew();
	c = MsetUnion(a, b);
	assert(isHeightBalanced(c->tree));
	MsetFree(b);
	MsetFree(c);

	printf("Test 6: union between 2 empty sets\n");
	b = MsetNew();
	c = MsetUnion(a, b);
	assert(isHeightBalanced(c->tree));
	MsetFree(c);

	printf("Test 7: rebalance after intersection\n");
	MsetInsert(a, 4);
	MsetInsert(a, 2);
	MsetInsert(a, 7);
	MsetInsert(a, 1);

	MsetInsert(b, 4);
	MsetInsert(b, 2);
	MsetInsert(b, 1);

	c = MsetIntersection(a, b);
	assert(isHeightBalanced(c->tree));
	MsetFree(a);
	MsetFree(c);

	printf("Test 8: intersection between Mset and empty set\n");
	a = MsetNew();
	c = MsetIntersection(a, b);
	assert(isHeightBalanced(c->tree));
	MsetFree(b);
	MsetFree(c);

	printf("Test 9: intersection between 2 empty sets\n");
	b = MsetNew();
	c = MsetIntersection(a, b);
	assert(isHeightBalanced(c->tree));
	MsetFree(a);
	MsetFree(b);
	MsetFree(c);
}

bool isHeightBalanced(struct node *t) {
	int height = -1;
	return doIsHeightBalanced(t, &height);
}

bool doIsHeightBalanced(struct node *t, int *height) {
	if (t == NULL) {
		*height = -1;
		return true;
	}

	int lHeight = -1;
	int rHeight = -1;
	if (doIsHeightBalanced(t->left, &lHeight) &&
	    doIsHeightBalanced(t->right, &rHeight) && abs(lHeight - rHeight) <= 1) {
		*height = (lHeight > rHeight ? lHeight : rHeight) + 1;
		return true;
	} else {
		return false;
	}
}

////////////////////////////////////////////////////////////////////////

void testMsetAtIndex(void) {
	printf("Test 1: normal\n");
	Mset s = MsetNew();

	MsetInsertMany(s, 5, 2);
	MsetInsertMany(s, 2, 3);
	MsetInsertMany(s, 8, 5);
	MsetInsertMany(s, 4, 1);

	struct item item;
	item = MsetAtIndex(s, 0);
	assert(item.elem == 2 && item.count == 3);
	item = MsetAtIndex(s, 1);
	assert(item.elem == 4 && item.count == 1);
	item = MsetAtIndex(s, 2);
	assert(item.elem == 5 && item.count == 2);
	item = MsetAtIndex(s, 3);
	assert(item.elem == 8 && item.count == 5);

	printf("Test 2: inserting elements\n");
	MsetInsertMany(s, 3, 7);

	item = MsetAtIndex(s, 0);
	assert(item.elem == 2 && item.count == 3);
	item = MsetAtIndex(s, 1);
	assert(item.elem == 3 && item.count == 7);
	item = MsetAtIndex(s, 2);
	assert(item.elem == 4 && item.count == 1);
	item = MsetAtIndex(s, 3);
	assert(item.elem == 5 && item.count == 2);
	item = MsetAtIndex(s, 4);
	assert(item.elem == 8 && item.count == 5);

	printf("Test 3: deleting elements\n");
	MsetDeleteMany(s, 5, 2);

	item = MsetAtIndex(s, 0);
	assert(item.elem == 2 && item.count == 3);
	item = MsetAtIndex(s, 1);
	assert(item.elem == 3 && item.count == 7);
	item = MsetAtIndex(s, 2);
	assert(item.elem == 4 && item.count == 1);
	item = MsetAtIndex(s, 3);
	assert(item.elem == 8 && item.count == 5);

	printf("Test 4: inserting smallest\n");
	MsetInsertMany(s, 1, 3);

	item = MsetAtIndex(s, 0);
	assert(item.elem == 1 && item.count == 3);
	item = MsetAtIndex(s, 1);
	assert(item.elem == 2 && item.count == 3);
	item = MsetAtIndex(s, 2);
	assert(item.elem == 3 && item.count == 7);
	item = MsetAtIndex(s, 3);
	assert(item.elem == 4 && item.count == 1);
	item = MsetAtIndex(s, 4);
	assert(item.elem == 8 && item.count == 5);

	printf("Test 5: deleting smallest\n");
	MsetDeleteMany(s, 1, 3);

	item = MsetAtIndex(s, 0);
	assert(item.elem == 2 && item.count == 3);
	item = MsetAtIndex(s, 1);
	assert(item.elem == 3 && item.count == 7);
	item = MsetAtIndex(s, 2);
	assert(item.elem == 4 && item.count == 1);
	item = MsetAtIndex(s, 3);
	assert(item.elem == 8 && item.count == 5);

	printf("Test 6: inserting largest\n");
	MsetInsertMany(s, 9, 1);

	item = MsetAtIndex(s, 0);
	assert(item.elem == 2 && item.count == 3);
	item = MsetAtIndex(s, 1);
	assert(item.elem == 3 && item.count == 7);
	item = MsetAtIndex(s, 2);
	assert(item.elem == 4 && item.count == 1);
	item = MsetAtIndex(s, 3);
	assert(item.elem == 8 && item.count == 5);
	item = MsetAtIndex(s, 4);
	assert(item.elem == 9 && item.count == 1);

	printf("Test 7: deleting largest\n");
	MsetDeleteMany(s, 9, 1);

	item = MsetAtIndex(s, 0);
	assert(item.elem == 2 && item.count == 3);
	item = MsetAtIndex(s, 1);
	assert(item.elem == 3 && item.count == 7);
	item = MsetAtIndex(s, 2);
	assert(item.elem == 4 && item.count == 1);
	item = MsetAtIndex(s, 3);
	assert(item.elem == 8 && item.count == 5);

	printf("Test 8: index < 0\n");
	item = MsetAtIndex(s, -1);
	assert(item.elem == UNDEFINED && item.count == 0);

	printf("Test 9: index > size - 1\n");
	item = MsetAtIndex(s, 4);
	assert(item.elem == UNDEFINED && item.count == 0);
	item = MsetAtIndex(s, 12);
	assert(item.elem == UNDEFINED && item.count == 0);

	MsetFree(s);
}

void testMsetIndexOf(void) {
	printf("Test 1: normal\n");
	Mset s = MsetNew();

	MsetInsert(s, 5);
	MsetInsert(s, 2);
	MsetInsert(s, 8);
	MsetInsert(s, 4);

	assert(MsetIndexOf(s, 2) == 0);
	assert(MsetIndexOf(s, 4) == 1);
	assert(MsetIndexOf(s, 5) == 2);
	assert(MsetIndexOf(s, 8) == 3);

	printf("Test 2: inserting element in middle\n");
	MsetInsert(s, 3);

	assert(MsetIndexOf(s, 2) == 0);
	assert(MsetIndexOf(s, 3) == 1);
	assert(MsetIndexOf(s, 4) == 2);
	assert(MsetIndexOf(s, 5) == 3);
	assert(MsetIndexOf(s, 8) == 4);

	printf("Test 3: insert smallest\n");
	MsetInsert(s, 1);
	assert(MsetIndexOf(s, 1) == 0);
	assert(MsetIndexOf(s, 2) == 1);
	assert(MsetIndexOf(s, 3) == 2);
	assert(MsetIndexOf(s, 4) == 3);
	assert(MsetIndexOf(s, 5) == 4);
	assert(MsetIndexOf(s, 8) == 5);

	printf("Test 4: insert largest\n");
	MsetInsert(s, 9);
	assert(MsetIndexOf(s, 1) == 0);
	assert(MsetIndexOf(s, 2) == 1);
	assert(MsetIndexOf(s, 3) == 2);
	assert(MsetIndexOf(s, 4) == 3);
	assert(MsetIndexOf(s, 5) == 4);
	assert(MsetIndexOf(s, 8) == 5);
	assert(MsetIndexOf(s, 9) == 6);

	printf("Test 5: delete smallest\n");
	MsetDelete(s, 1);
	assert(MsetIndexOf(s, 2) == 0);
	assert(MsetIndexOf(s, 3) == 1);
	assert(MsetIndexOf(s, 4) == 2);
	assert(MsetIndexOf(s, 5) == 3);
	assert(MsetIndexOf(s, 8) == 4);
	assert(MsetIndexOf(s, 9) == 5);

	printf("Test 6: delete largest\n");
	MsetDelete(s, 9);
	assert(MsetIndexOf(s, 2) == 0);
	assert(MsetIndexOf(s, 3) == 1);
	assert(MsetIndexOf(s, 4) == 2);
	assert(MsetIndexOf(s, 5) == 3);
	assert(MsetIndexOf(s, 8) == 4);

	printf("Test 7: delete middle\n");
	MsetDelete(s, 3);
	assert(MsetIndexOf(s, 2) == 0);
	assert(MsetIndexOf(s, 4) == 1);
	assert(MsetIndexOf(s, 5) == 2);
	assert(MsetIndexOf(s, 8) == 3);

	printf("Test 8: element doesn't exist\n");
	assert(MsetIndexOf(s, 1) == -1);
	assert(MsetIndexOf(s, 9) == -1);
	MsetFree(s);
}

////////////////////////////////////////////////////////////////////////

void testMsetCursor1(void) {
	Mset s = MsetNew();

	MsetInsertMany(s, 4, 2);
	MsetInsertMany(s, 7, 3);
	MsetInsertMany(s, 1, 5);
	MsetInsertMany(s, 3, 1);

	MsetCursor cur = MsetCursorNew(s);

	struct item item;

	// start  1  3  4  7  end
	//   ^
	item = MsetCursorGet(cur);
	assert(item.elem == UNDEFINED);
	assert(item.count == 0);

	assert(MsetCursorNext(cur));
	// start  1  3  4  7  end
	//        ^
	item = MsetCursorGet(cur);
	assert(item.elem == 1);
	assert(item.count == 5);

	assert(MsetCursorNext(cur));
	// start  1  3  4  7  end
	//           ^
	item = MsetCursorGet(cur);
	assert(item.elem == 3);
	assert(item.count == 1);

	assert(MsetCursorNext(cur));
	// start  1  3  4  7  end
	//              ^
	item = MsetCursorGet(cur);
	assert(item.elem == 4);
	assert(item.count == 2);

	assert(MsetCursorNext(cur));
	// start  1  3  4  7  end
	//                 ^
	item = MsetCursorGet(cur);
	assert(item.elem == 7);
	assert(item.count == 3);

	assert(!MsetCursorNext(cur));
	// start  1  3  4  7  end
	//                     ^
	item = MsetCursorGet(cur);
	assert(item.elem == UNDEFINED);
	assert(item.count == 0);

	assert(MsetCursorPrev(cur));
	// start  1  3  4  7  end
	//                 ^
	item = MsetCursorGet(cur);
	assert(item.elem == 7);
	assert(item.count == 3);

	assert(MsetCursorPrev(cur));
	// start  1  3  4  7  end
	//              ^
	item = MsetCursorGet(cur);
	assert(item.elem == 4);
	assert(item.count == 2);

	assert(MsetCursorPrev(cur));
	// start  1  3  4  7  end
	//           ^
	item = MsetCursorGet(cur);
	assert(item.elem == 3);
	assert(item.count == 1);

	assert(MsetCursorPrev(cur));
	// start  1  3  4  7  end
	//        ^
	item = MsetCursorGet(cur);
	assert(item.elem == 1);
	assert(item.count == 5);

	assert(!MsetCursorPrev(cur));
	// start  1  3  4  7  end
	//   ^
	item = MsetCursorGet(cur);
	assert(item.elem == UNDEFINED);
	assert(item.count == 0);

	MsetCursorFree(cur);
	MsetFree(s);
}

void testMsetCursor2(void) {
	Mset s = MsetNew();

	MsetInsertMany(s, 4, 2);
	MsetInsertMany(s, 7, 3);
	MsetInsertMany(s, 1, 5);
	MsetInsertMany(s, 6, 1);

	MsetCursor cur = MsetCursorNew(s);

	struct item item;

	// start  1  4  6  7  end
	//   ^
	item = MsetCursorGet(cur);
	assert(item.elem == UNDEFINED);
	assert(item.count == 0);

	assert(MsetCursorNext(cur));
	// start  1  4  6  7  end
	//        ^
	item = MsetCursorGet(cur);
	assert(item.elem == 1);
	assert(item.count == 5);

	MsetInsertMany(s, 2, 4);
	// start  1  2  4  6  7  end
	//        ^

	assert(MsetCursorNext(cur));
	// start  1  2  4  6  7  end
	//           ^
	item = MsetCursorGet(cur);
	assert(item.elem == 2);
	assert(item.count == 4);

	assert(MsetCursorNext(cur));
	// start  1  2  4  6  7  end
	//              ^
	item = MsetCursorGet(cur);
	assert(item.elem == 4);
	assert(item.count == 2);

	MsetDelete(s, 6);
	// start  1  2  4  7  end
	//              ^

	assert(MsetCursorNext(cur));
	// start  1  2  4  7  end
	//                 ^
	item = MsetCursorGet(cur);
	assert(item.elem == 7);
	assert(item.count == 3);

	assert(!MsetCursorNext(cur));
	// start  1  2  4  7  end
	// 					   ^
	item = MsetCursorGet(cur);
	assert(item.elem == UNDEFINED);
	assert(item.count == 0);

	MsetDeleteMany(s, 7, 3);
	assert(MsetCursorPrev(cur));
	item = MsetCursorGet(cur);
	assert(item.elem == 4 && item.count == 2);

	MsetCursorFree(cur);

	cur = MsetCursorNew(s);
	assert(MsetCursorNext(cur));
	assert(MsetCursorNext(cur));
	// start  1  2  4   end
	// 	 	     ^
	MsetDeleteMany(s, 1, 5);
	assert(!MsetCursorPrev(cur));
	MsetCursorFree(cur);
	MsetFree(s);
}

////////////////////////////////////////////////////////////////////////
