# AVL Tree Implementation

## Overview
Exercise in implementation of BST and AVL tree operations, completed as part of a university computing course. Utilises both basic and advanced operations, such as insertion, search, union, intersection and tree balancing operations. Focuses on program/algorithmic efficiency

## Technologies Used
- C

## Key Features
- Performs basic tree operations such as insertion, deletion and search operations
- Performs advanced tree operations such as union, intersection and AVL tree rebalancing operations
- Uses a 'cursor' to allow the user to efficiently conduct in-order traversal of the tree
- The in-order list has 'imaginary' start and end nodes. When the cursor is created, it will point to the start node
- Multiple cursors can be created, and each cursor can be at a different node of the tree
- Upon insertion and/or deletion of nodes, the list of in-order nodes will be updated accordingly
- If the value the cursor is pointing to gets deleted, the only operation the cursor can call is MsetCursorFree

## How to Run
1. Clone the repository
2. Compile the program using the 'make' command
3. Run the program

## Project Structure
Brief explanation of how the code is organised:
- `Mset.h` - header file for function definitions in Mset.c
- `Mset.c` - implementations of function prototypes found in Mset.h, along with implementations of any helper functions used
- `MsetStructs.h` - contains struct definitions for any structs used in the program
- `testMset.c` - contains all tests used throughout the implementation and testing process
- `Makefile` - makefile for the program

## Design Decisions
The nodes formed both part of an AVL BST, as well as a doubly linked list
- The doubly linked list was created to implement the cursor and efficient in-order traversal
    - The prev and next values in the doubly linked list would be updated after every insertion and deletion operation
- num_nodes, total_count and height was included as attributes in the node struct to ensure these frequently accessed values can be found quickly and efficiently
- The cursor also contains an 'imaginary' start and end node, which was implemented through the start and end nodes in the mset struct, which represents these 'imaginary' nodes

## Testing
The tests include the following test types:
1. Basic functionality tests: Check if the function works as intended. Edge cases will be tested in later sections
2. Edge case tests: Edge cases where the function is expected to work
3. Error tests: Tests where the function is expected to fail

Basic core functionalities are tested in the earlier sections of the test, followed by more advanced functionalities.
