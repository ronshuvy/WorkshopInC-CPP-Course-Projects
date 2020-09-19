//
// Created by dorro on 18/05/2020.
//

#ifndef EX3_RBUTILITIES_H
#define EX3_RBUTILITIES_H

#include "RBTree.h"

#define BASE_PATH "./"
#define PYTHON "python3"
#define JSON_FILE "tree.json"

// tree correctness validation
int isValidRBTree(RBTree *tree);

// tree visualizations
int viewTree(RBTree *tree, char* (*toString)(void*));

// tree print to console
void printRBTree(Node *tree);

#endif //EX3_RBUTILITIES_H
