// COMP2521 25T3 - Assignment 1
// Implementation of the Multiset ADT
// Written by Xingyu Tang on 09/10/2025

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Mset.h"
#include "MsetStructs.h"

#define NULL_HEIGHT    -1
#define NULL_INDEX     -1
#define NULL_COUNT     0
#define NULL_NUM_NODES 0

#define NEW_HEIGHT    0
#define NEW_NUM_NODES 1

////////////////////////////////////////////////////////////////////////
// Helper Function Prototypes
// Helper functions created in Part 1
static void delete_bst(struct node *t);

static struct node *insert_elem(Mset s, struct node *t, int elem, int amount);
static struct node *new_node(int elem, int count);
static void update_min_max(Mset s, struct node *t);
static int num_nodes(struct node *t);
static int total_count(struct node *t);

static struct node *delete_elem(Mset s, struct node *t, int elem, int amount);
static struct node *join_bst(struct node *left, struct node *right);
static struct node *update_node_properties(struct node *t);

static int get_count(struct node *t, int elem);

static void print_bst(struct node *t, FILE *file, bool *printed);

// Helper functions created in Part 2
static void copy_bst(Mset dest, struct node *src);
static void bst_union(Mset ms_union, struct node *t);

static void bst_intersection(Mset intersection, struct node *t, Mset s2);

static bool bst_inclusion(struct node *t1, Mset s2);

// Helper functions created in Part 3
static int max(int num1, int num2);
static struct node *balance_bst(struct node *t);
static struct node *rotate_left(struct node *root);
static struct node *rotate_right(struct node *root);
static int balance(struct node *t);
static int height(struct node *t);

// Helper functions created in Part 4
static struct node *find_elem(struct node *t, int index);

static int find_index(struct node *t, int elem);

// Acknowledgements:
// - Implementation of insert_elem, delete_elem, join_bst, print_bst, balance
// 	 and balance_bst adapted from
//   https://cgi.cse.unsw.edu.au/~cs2521/25T3/lectures/slides/week04lec01-avl-trees.pdf
// - Implementation of rotate_left and rotate_right adapted from
//	 https://cgi.cse.unsw.edu.au/~cs2521/25T3/lectures/slides/week04lec01-balancing-bsts.pdf
// - Implementation of find_elem and find_index was inspired by
//   the partition function from
//	 https://cgi.cse.unsw.edu.au/~cs2521/25T3/lectures/slides/week04lec01-balancing-bsts.pdf

////////////////////////////////////////////////////////////////////////
// Basic Operations

/**
 * Creates a new empty multiset.
 */
Mset MsetNew(void) {
	// TODO
	Mset multiset = malloc(sizeof(struct mset));
	if (multiset == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	multiset->tree = NULL;
	multiset->start = new_node(UNDEFINED, 0);
	multiset->end = new_node(UNDEFINED, 0);
	multiset->start->next = multiset->end;
	multiset->end->prev = multiset->start;
	return multiset;
}

/**
 * Frees all memory allocated to the multiset.
 */
void MsetFree(Mset s) {
	// TODO
	if (s == NULL) {
		return;
	}
	struct node *t = s->tree;
	delete_bst(t);
	free(s->start);
	free(s->end);
	free(s);
}

/**
 * Frees all the nodes in the multiset.
 */
static void delete_bst(struct node *t) {
	if (t == NULL) {
		return;
	}
	delete_bst(t->left);
	delete_bst(t->right);
	if (t->prev != NULL) {
		t->prev->next = t->next;
	}
	if (t->next != NULL) {
		t->next->prev = t->prev;
	}
	free(t);
}

/**
 * Inserts one of an element into the multiset. Does nothing if the
 * element is equal to UNDEFINED.
 */
void MsetInsert(Mset s, int elem) {
	// TODO
	if (elem == UNDEFINED) {
		return;
	}
	s->tree = insert_elem(s, s->tree, elem, 1);
}

/**
 * Inserts an element into the bst, rebalances it, and returns the node. 
 * This code was adapted from 
 * https://cgi.cse.unsw.edu.au/~cs2521/25T3/lectures/slides/week04lec01-avl-trees.pdf
 */
static struct node *insert_elem(Mset s, struct node *t, int elem, int amount) {
	if (t == NULL) {
		t = new_node(elem, amount);
		update_min_max(s, t);
		return t;
	}

	if (elem < t->elem) {
		struct node *left = insert_elem(s, t->left, elem, amount);
		t->left = left;

		if (left->prev == NULL && left->next == NULL) {
			left->prev = t->prev;
			left->next = t;
			if (t->prev != NULL) {				
				t->prev->next = left;
			}
			t->prev = left;
		}
	} else if (elem > t->elem) {
		struct node *right = insert_elem(s, t->right, elem, amount);
		t->right = right;

		if (right->prev == NULL && right->next == NULL) {
			right->next = t->next;
			right->prev = t;
			if (t->next != NULL) {
				t->next->prev = right;
			}
			t->next = right;
		}
	} else {
		t->count += amount;
	}

	t = update_node_properties(t);
	return balance_bst(t);
}

/**
 * Creates and returns a new node
 */
static struct node *new_node(int elem, int amount) {
	struct node *new = malloc(sizeof(struct node));
	if (new == NULL) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	new->elem = elem;
	new->count = amount;
	new->left = NULL;
	new->right = NULL;

	new->num_nodes = NEW_NUM_NODES;
	new->total_count = amount;
	new->height = NEW_HEIGHT;

	new->prev = NULL;
	new->next = NULL;
	return new;
}

/**
 * Adds the first inserted node to the Mset's doubly linked list
*/
static void update_min_max(Mset s, struct node *t) {
	if (s->start->next == s->end) {
		s->start->next = t;
		t->prev = s->start;
		t->next = s->end;
		s->end->prev = t;
	}
}

/**
 * Updates the node's values for num_nodes, total_count and height
*/
static struct node *update_node_properties(struct node *t) {
	if (t == NULL) {
		return NULL;
	}
	t->num_nodes = 1 + num_nodes(t->left) + num_nodes(t->right);
	t->total_count = t->count + total_count(t->left) + total_count(t->right);
	t->height = 1 + max(height(t->left), height(t->right));
	return t;
}

/**
 * Calculates and returns the number of nodes in the subtree 
 * (including the root node itself)
 */
static int num_nodes(struct node *t) {
	if (t == NULL) {
		return NULL_NUM_NODES;
	}
	return t->num_nodes;
}

/**
 * Calculates and returns the sum of counts in the subtree 
 * (including the root node itself)
 */
static int total_count(struct node *t) {
	if (t == NULL) {
		return NULL_COUNT;
	}
	return t->total_count;
}

/**
 * Returns the integer with the bigger value
*/
static int max(int num1, int num2) {
	if (num1 >= num2) {
		return num1;
	}
	return num2;
}

/**
 * Returns the height of the subtree
*/
static int height(struct node *t) {
	if (t == NULL) {
		return NULL_HEIGHT;
	}
	return t->height;
}

/**
 * Rotates the AVL Mset such that it is height balanced
 * Returns the root of the subtree being balanced
 * This code was adapted from 
 * https://cgi.cse.unsw.edu.au/~cs2521/25T3/lectures/slides/week04lec01-avl-trees.pdf
*/
static struct node *balance_bst(struct node *t) {
	if (t == NULL) {
		return NULL;
	}

	t = update_node_properties(t);

	int bal = balance(t);
	if (bal > 1) {
		if (balance(t->left) < 0) {
			t->left = rotate_left(t->left);
		}
		t = rotate_right(t);
	} else if (bal < -1) {
		if (balance(t->right) > 0) {
			t->right = rotate_right(t->right);
		}
		t = rotate_left(t);
	}

	return t;
}

/**
 * Calculates and returns the height difference between the left and right
 * branches
 * This code was adapted from 
 * https://cgi.cse.unsw.edu.au/~cs2521/25T3/lectures/slides/week04lec01-avl-trees.pdf
*/
static int balance(struct node *t) {
	return height(t->left) - height(t->right);
}

/**
 * Rotates the subtree left and returns its new root
 * This code was adapted from 
 * https://cgi.cse.unsw.edu.au/~cs2521/25T3/lectures/slides/week04lec01-balancing-bsts.pdf
*/
static struct node *rotate_left(struct node *root) {
	if (root == NULL || root->right == NULL) {
		return root;
	}

	struct node *new_root = root->right;
	root->right = new_root->left;
	new_root->left = root;

	root = update_node_properties(root);
	new_root = update_node_properties(new_root);

	return balance_bst(new_root);
}

/**
 * Rotates the subtree right and returns its new root
 * This code was adapted from 
 * https://cgi.cse.unsw.edu.au/~cs2521/25T3/lectures/slides/week04lec01-balancing-bsts.pdf
*/
static struct node *rotate_right(struct node *root) {
	if (root == NULL || root->left == NULL) {
		return root;
	}

	struct node *new_root = root->left;
	root->left = new_root->right;
	new_root->right = root;

	root = update_node_properties(root);
	new_root = update_node_properties(new_root);

	return balance_bst(new_root);
}

/**
 * Inserts the given amount of an element into the multiset. Does
 * nothing if the element is equal to UNDEFINED or the given amount is 0
 * or less.
 */
void MsetInsertMany(Mset s, int elem, int amount) {
	// TODO
	if (elem == UNDEFINED || amount <= 0) {
		return;
	}
	s->tree = insert_elem(s, s->tree, elem, amount);
}

/**
 * Deletes one of an element from the multiset.
 */
void MsetDelete(Mset s, int elem) {
	// TODO
	if (elem == UNDEFINED) {
		return;
	}
	s->tree = delete_elem(s, s->tree, elem, 1);
}

/**
 * Deletes an element from the multiset, balances it, and 
 * returns the root of the new subtree 
 * This code was adapted from 
 * https://cgi.cse.unsw.edu.au/~cs2521/25T3/lectures/slides/week04lec01-avl-trees.pdf 
 */
static struct node *delete_elem(Mset s, struct node *t, int elem, int amount) {
	if (t == NULL) {
		return NULL;
	}
	if (elem < t->elem) {
		t->left = delete_elem(s, t->left, elem, amount);
	} else if (elem > t->elem) {
		t->right = delete_elem(s, t->right, elem, amount);
	} else {
		if (t->count > amount) {
			t->count -= amount;
		} else {
			struct node *removed = t;
			if (t->prev != NULL) {
				t->prev->next = t->next;
			}
			if (t->next != NULL) {
				t->next->prev = t->prev;
			}
			t = join_bst(t->left, t->right);
			free(removed);
		}
	}

	if (t != NULL) {
		t = update_node_properties(t);
		t = balance_bst(t);
	}

	return t;
}

/**
 * Joins 2 subtrees into a single bst
 * This code was adapted from 
 * https://cgi.cse.unsw.edu.au/~cs2521/25T3/lectures/slides/week04lec01-avl-trees.pdf
 */
static struct node *join_bst(struct node *left, struct node *right) {
	if (left == NULL) {
		return right;
	}
	if (right == NULL) {
		return left;
	}
	if (right->left == NULL) {
		right->left = left;
		right = update_node_properties(right);
		return balance_bst(right);
	}

	struct node *curr = right;
	struct node *prev = NULL;
	while (curr->left != NULL) {
		prev = curr;
		curr = curr->left;
	}

	if (prev != NULL) {
		prev->left = curr->right;
	} else {
		right = curr->right;
	}

	curr->left = left;
	curr->right = right;
	curr = update_node_properties(curr);

	return balance_bst(curr);
}

/**
 * Deletes the given amount of an element from the multiset.
 */
void MsetDeleteMany(Mset s, int elem, int amount) {
	// TODO
	if (elem == UNDEFINED || amount <= 0) {
		return;
	}
	s->tree = delete_elem(s, s->tree, elem, amount);
}

/**
 * Returns the number of distinct elements in the multiset.
 */
int MsetSize(Mset s) {
	// TODO
	if (s == NULL) {
		return NULL_NUM_NODES;
	}
	return num_nodes(s->tree);
}

/**
 * Returns the sum of counts of all elements in the multiset.
 */
int MsetTotalCount(Mset s) {
	// TODO
	if (s == NULL) {
		return NULL_COUNT;
	}
	return total_count(s->tree);
}

/**
 * Returns the count of an element in the multiset, or 0 if it doesn't
 * occur in the multiset.
 */
int MsetGetCount(Mset s, int elem) {
	// TODO
	if (s == NULL) {
		return NULL_COUNT;
	}
	return get_count(s->tree, elem);
}

/**
 * Returns the count of an element in the multiset, or 0 if it doesn't
 * occur in the multiset.
 */
static int get_count(struct node *t, int elem) {
	if (t == NULL) {
		return NULL_COUNT;
	}
	int size = 0;
	if (elem < t->elem) {
		size = get_count(t->left, elem);
	} else if (elem > t->elem) {
		size = get_count(t->right, elem);
	} else {
		size = t->count;
	}
	return size;
}

/**
 * Prints the multiset to a file.
 * The elements of the multiset should be printed in ascending order
 * inside a pair of curly braces, with elements separated by a comma
 * and space. Each element should be printed inside a pair of
 * parentheses with its count, separated by a comma and space.
 * This code was adapted from 
 * https://cgi.cse.unsw.edu.au/~cs2521/25T3/lectures/slides/week04lec01-avl-trees.pdf
 */
void MsetPrint(Mset s, FILE *file) {
	// TODO
	fprintf(file, "{");
	if (s != NULL && s->tree != NULL) {
		bool printed = true;
		print_bst(s->tree, file, &printed);
	}
	fprintf(file, "}");
}

/**
 * Prints the multiset to a file according to spec
 * Function uses in-order traversal, with code implementation of the traversal
 * section being adapted from pseudocode from Week 3 Lectures
 */
static void print_bst(struct node *t, FILE *file, bool *printed) {
	if (t == NULL) {
		return;
	}
	print_bst(t->left, file, printed);

	if (!*printed) {
		fprintf(file, ", ");
	} else {
		*printed = false;
	}
	fprintf(file, "(%d, %d)", t->elem, t->count);

	print_bst(t->right, file, printed);
}

////////////////////////////////////////////////////////////////////////
// Advanced Operations

/**
 * Returns a new multiset representing the union of the two given
 * multisets. 
 */
Mset MsetUnion(Mset s1, Mset s2) {
	// TODO
	Mset ms_union = MsetNew();

	if (s1 == NULL || s1->tree == NULL) {
		copy_bst(ms_union, s2->tree);
		return ms_union;
	}
	if (s2 == NULL || s2->tree == NULL) {
		copy_bst(ms_union, s1->tree);
		return ms_union;
	}

	copy_bst(ms_union, s1->tree);
	bst_union(ms_union, s2->tree);
	return ms_union;
}

/**
 * Copies all the nodes of the source multiset into the destination multiset
 */
static void copy_bst(Mset dest, struct node *src) {
	if (src == NULL) {
		return;
	}
	MsetInsertMany(dest, src->elem, src->count);
	copy_bst(dest, src->left);
	copy_bst(dest, src->right);
}

/**
 * Adds nodes in s2 but not in s1 into the union multiset
 */
static void bst_union(Mset ms_union, struct node *t) {
	if (t == NULL) {
		return;
	}

	int count = MsetGetCount(ms_union, t->elem);          // O(log n)
	MsetInsertMany(ms_union, t->elem, t->count - count);  // O(log n)

	bst_union(ms_union, t->left);
	bst_union(ms_union, t->right);
}

/**
 * Returns a new multiset representing the intersection of the two
 * given multisets.
 */
Mset MsetIntersection(Mset s1, Mset s2) {
	// TODO
	if (s1 == NULL || s2 == NULL || s1->tree == NULL || s2->tree == NULL) {
		return MsetNew();
	}

	Mset intersection = MsetNew();
	copy_bst(intersection, s1->tree);
	bst_intersection(intersection, s1->tree, s2);
	return intersection;
}

/**
 * Deletes nodes that are exclusively in s1 or s2 from the intersection Mset
*/
static void bst_intersection(Mset intersection, struct node *t, Mset s2) {
	if (t == NULL) {
		return;
	}

	int count = MsetGetCount(s2, t->elem);  // O(log m)
	if (count == 0) {
		MsetDeleteMany(intersection, t->elem, t->count);  // O(log n)
	} else {
		MsetDeleteMany(intersection, t->elem, t->count - count);
	}

	bst_intersection(intersection, t->left, s2);
	bst_intersection(intersection, t->right, s2);
}

/**
 * Returns true if the multiset s1 is included in the multiset s2, and
 * false otherwise.
 */
bool MsetIncluded(Mset s1, Mset s2) {
	// TODO
	if (s1 == NULL || s1->tree == NULL) {
		return true;
	}
	if (s2 == NULL || s2->tree == NULL) {
		return false;
	}

	return bst_inclusion(s1->tree, s2);
}

/**
 * Returns true if the subtree from s1 is included in s2, and false otherwise
*/
static bool bst_inclusion(struct node *t, Mset s2) {
	if (t == NULL) return true;

	int count = MsetGetCount(s2, t->elem);  // O(log m)
	if (count < t->count) return false;

	if (!bst_inclusion(t->left, s2)) return false;
	if (!bst_inclusion(t->right, s2)) return false;

	return true;
}

/**
 * Returns true if the two given multisets are equal, and false
 * otherwise.
 */
bool MsetEquals(Mset s1, Mset s2) {
	// TODO
	return MsetIncluded(s1, s2) && MsetIncluded(s2, s1);
}

////////////////////////////////////////////////////////////////////////
// Index Operations

/**
 * Returns the element at the given index and its count, or
 * {UNDEFINED, 0} if the given index is outside the range [0, n - 1]
 * where n is the size of the multiset.
 */
struct item MsetAtIndex(Mset s, int index) {
	// TODO
	if (s == NULL || s->tree == NULL || index < 0 ||
	    index > s->tree->num_nodes - 1) {
		return (struct item){UNDEFINED, 0};
	}

	struct node *target = find_elem(s->tree, index);
	struct item info;
	info.elem = target->elem;
	info.count = target->count;

	return info;
}

/**
 * Finds and returns the node at the given index
 * This code was inspired by the implementation of partition from
 * https://cgi.cse.unsw.edu.au/~cs2521/25T3/lectures/slides/week04lec01-balancing-bsts.pdf
 * It uses the same index searching method as the partition function
*/
static struct node *find_elem(struct node *t, int index) {
	if (t == NULL) return NULL;

	int left_size = num_nodes(t->left);
	if (index < left_size) {
		return find_elem(t->left, index);
	}
	if (index > left_size) {
		return find_elem(t->right, index - left_size - 1);
	}
	return t;
}

/**
 * Returns the index of the given element if it exists, or -1 otherwise.
 */
int MsetIndexOf(Mset s, int elem) {
	// TODO
	if (s == NULL) return NULL_INDEX;
	return find_index(s->tree, elem);
}

/**
 * Finds and returns the index of the given element if it exists, or 
 * -1 otherwise
 * This code was inspired by the implementation of partition from
 * https://cgi.cse.unsw.edu.au/~cs2521/25T3/lectures/slides/week04lec01-balancing-bsts.pdf
 * It uses a similar index calculation method as the partition function
*/
static int find_index(struct node *t, int elem) {
	if (t == NULL) return NULL_INDEX;

	int left_size = num_nodes(t->left);
	if (elem < t->elem) {
		return find_index(t->left, elem);
	} else if (elem > t->elem) {
		int offset = find_index(t->right, elem);
		if (offset == -1) return offset;
		return 1 + left_size + offset;
	}
	return left_size;
}

////////////////////////////////////////////////////////////////////////
// Cursor Operations

/**
 * Creates a new cursor positioned at the start of the multiset.
 * (see spec for explanation of start and end)
 */
MsetCursor MsetCursorNew(Mset s) {
	// TODO
	MsetCursor cursor = malloc(sizeof(struct cursor));
	cursor->set = s;
	cursor->curr = s->start;
	return cursor;
}

/**
 * Frees all memory allocated to the given cursor.
 */
void MsetCursorFree(MsetCursor cur) {
	// TODO
	if (cur != NULL) free(cur);
}

/**
 * Returns the element at the cursor's position and its count, or
 * {UNDEFINED, 0} if the cursor is positioned at the start or end of
 * the multiset.
 */
struct item MsetCursorGet(MsetCursor cur) {
	// TODO
	if (cur->curr == cur->set->start || cur->curr == cur->set->end) {
		return (struct item){UNDEFINED, 0};
	}
	struct item info;
	info.elem = cur->curr->elem;
	info.count = cur->curr->count;
	return info;
}

/**
 * Moves the cursor to the next greatest element, or to the end of the
 * multiset if there is no next greatest element. Does not move the
 * cursor if it is already at the end. Returns false if the cursor is at
 * the end after this operation, and true otherwise.
 */
bool MsetCursorNext(MsetCursor cur) {
	// TODO
	if (cur->curr == cur->set->end) return false;
	cur->curr = cur->curr->next;
	if (cur->curr == cur->set->end) return false;
	return true;
}

/**
 * Moves the cursor to the next smallest element, or to the start of the
 * multiset if there is no next smallest element. Does not move the
 * cursor if it is already at the start. Returns false if the cursor is
 * at the start after this operation, and true otherwise.
 */
bool MsetCursorPrev(MsetCursor cur) {
	// TODO
	if (cur->curr == cur->set->start) return false;
	cur->curr = cur->curr->prev;
	if (cur->curr == cur->set->start) return false;
	return true;
}

////////////////////////////////////////////////////////////////////////
