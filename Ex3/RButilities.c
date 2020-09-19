#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "RBUtilities.h"

/**
 * return a single (always left) path number of blacks.
 */
int getPathBlacksNum(Node *node, int blacks)
{
	if (node == NULL)
	{
		return blacks + 1;
	}

	if (node->color == BLACK)
	{
		return getPathBlacksNum(node->left, blacks+1);
	}
	return getPathBlacksNum(node->left, blacks);
}

/**
 * return true if all paths from node to a leaf have (shouldBe - blacks) blacks
 */
int validatePaths(Node *node, int blacks, int shouldBe)
{
	if (node == NULL)
	{
		return blacks + 1 == shouldBe;
	}

	blacks += (node->color == BLACK) ? 1 : 0;
	return validatePaths(node->left, blacks, shouldBe) &&
		   validatePaths(node->right, blacks, shouldBe);
}

/**
 * return true if all nodes follow the color code requirements for RB tree
 */
int validateColors(Node *node)
{
	if (node == NULL)
	{
		return 1;
	}
	if (node->color == RED && node->parent && node->parent->color == RED)
	{
		return 0;
	}
	return validateColors(node->left) && validateColors(node->right);
}

int validateBST(RBTree *tree, Node *node)
{
	if (node == NULL)
	{
		return 1;
	}
	if (node->left != NULL && 0 < tree->compFunc(node->left->data, node->data))
	{
		return 0;
	}
	if (node->right != NULL && 0 < tree->compFunc(node->data, node->right->data))
	{
		return 0;
	}
	return validateBST(tree, node->left) && validateBST(tree, node->right);
}

int validatePointers(Node *node)
{
	if (node == NULL)
	{
		return 1;
	}
	if (node->left != NULL)
	{
		if (node->left->parent != node)
		{
			return 0;
		}
	}
	if (node->right != NULL)
	{
		if (node->right->parent != node)
		{
			return 0;
		}
	}
	return validatePointers(node->left) && validatePointers(node->right);
}

int treeSize(Node *node, int sum)
{
	if (node == NULL)
	{
		return sum;
	}
	return treeSize(node->left, sum) + treeSize(node->right, sum) + 1;
}

/**
 * validate a tree structure according to the 4 RB tree invariants
 */
int isValidRBTree(RBTree *tree)
{
	Node *root = tree->root;

	if (root != NULL)
	{
		if (root->color != BLACK)
		{
			printf("Root must be black\n");
			return 0;
		}

		int pathBlacks = getPathBlacksNum(root, 0);
		if (!validatePaths(root, 0, pathBlacks))
		{
			fprintf(stderr, "Not all paths between the root and leafs have the same number of blacks (should be %d)\n", pathBlacks);
			return 0;
		}
		if (!validateColors(root))
		{
			fprintf(stderr, "All nodes must be BLACK or RED, and no consecutive RED nodes allowed.\n");
			return 0;
		}
		if (!validateBST(tree, root))
		{
			fprintf(stderr, "BST invariant isn't preserved.\n");
			return 0;
		}
		if (!validatePointers(tree->root))
		{
			fprintf(stderr, "Double pointers aren't matching\n");
			return 0;
		}
		if (treeSize(tree->root, 0) != (int)tree->size)
		{
			fprintf(stderr, "%d - %d \n", treeSize(tree->root, 0), (int)tree->size);
			fprintf(stderr, "Calculated tree size and tree.size property are different.\n");
			return 0;
		}
	}
	return 1;
}

// ------------------------------------


int _print_t(Node *tree, int isLeftBranch, int offset, int depth, char printBuffer[20][255])
{
	char nodeBuffer[20];
	int width = 7;

	if (!tree)
	{
		return 0;
	}

	char color = (tree->color == RED) ? 'r' : 'b';
	sprintf(nodeBuffer, "(%03d %c)", *(int*)(tree->data), color);

	int left  = _print_t(tree->left, 1, offset,depth + 1, printBuffer);
	int right = _print_t(tree->right, 0, offset + left + width,depth + 1, printBuffer);

	memcpy((printBuffer[2 * depth] + offset + left), nodeBuffer, 7 * sizeof(char));

	if (depth && isLeftBranch)
	{
		for (int i = 0; i < width + right; i++)
		{
			printBuffer[2 * depth - 1][offset + left + width / 2 + i] = '-';
		}
		printBuffer[2 * depth - 1][offset + left + width / 2] = '+';
		printBuffer[2 * depth - 1][offset + left + width + right + width / 2] = '+';

	}
	else if (depth && !isLeftBranch)
	{

		for (int i = 0; i < left + width; i++)
		{
			printBuffer[2 * depth - 1][offset - width / 2 + i] = '-';
		}
		printBuffer[2 * depth - 1][offset + left + width / 2] = '+';
		printBuffer[2 * depth - 1][offset - width / 2 - 1] = '+';
	}

	int levelSize = left + width + right;
	return levelSize;
}

void printRBTree(Node *tree)
{
    char printBuffer[20][255];
    for (int i = 0; i < 20; i++)
    {
        sprintf(printBuffer[i], "%190s", " ");
    }

    int totalWidth = _print_t(tree, 0, 0, 0, printBuffer);

    for (int i = 0; i < 20; i++)
    {
        printf("%.*s\n", totalWidth, printBuffer[i]);
    }
}
// ------------------------------------


void nodeToJSON(Node *node, char *buffer, char* (*toString)(void*))
{
	if (node == NULL)
	{
		strcat(buffer, "null");
		return;
	}

	char *tmp = (char*)calloc(255, sizeof(char));
	char color = (node->color == RED) ? 'r' : 'b';
	char *data = toString(node->data);
	sprintf(tmp, "{\n\"data\": \"%s\",\n\"color\": \"%c\",\n", data, color);
	strcat(buffer, tmp);
	free(data);
	free(tmp);

	strcat(buffer, "\"left\": ");
	nodeToJSON(node->left, buffer, toString);
	strcat(buffer, ",\n\"right\": ");
	nodeToJSON(node->right, buffer, toString);
	strcat(buffer, "}");
}

int RBTreeToJSON(RBTree *tree, char *filename, char* (*toString)(void*))
{
	if (!isValidRBTree(tree))
	{
		printf("printing invalid tree - visualizer behaviour might be undefined");
	}

	if (tree->root == NULL)
	{
		printf("tree is empty");
		return 1;
	}

	FILE *json = fopen(filename, "w");
	assert(json != NULL && "failed file open");

	size_t bufferSize = tree->size * (256 * sizeof(char));
	char *buffer = (char*)calloc(bufferSize, sizeof(char));
	assert(buffer != NULL && "failed alloc");

	nodeToJSON(tree->root, buffer, toString);

	fwrite(buffer, sizeof(char), strlen(buffer), json);
	fclose(json);
	return 1;
}

int viewTree(RBTree *tree, char* (*toString)(void*))
{
	if (!RBTreeToJSON(tree, JSON_FILE, toString))
		return 0;
	system(PYTHON " " BASE_PATH"utilities/visualizer.py " JSON_FILE);
	return 1;
}