/**
 * @file RBTree.c
 * @author Ron Shuvy
 * @id 206330193
 *
 * @brief This file implements a generic Red-Black Tree DS
 *
 * @section DESCRIPTION
 * Node struct members : left, right, parent, data, color
 * RBTree struct members: root, size, compare function,
 * RBTree supported operations : building, insertion, deletion, contains, forEach, free memory
 */

#include <stdlib.h>
#include "RBTree.h"

#define SUCCESS 1
#define FAILURE 0

// ------------------------------ Functions -----------------------------

// ---------------- Utilities ----------------

/**
 * Switch between two nodes values
 * @param a: first node
 * @param b: second node
 */
static void switchValues(Node *a, Node *b)
{
    void *temp = a->data;
    a->data = b->data;
    b->data = temp;
}

/**
 *  Finds if n is a left child or right child
 * @param n: given node
 * @return: a pointer to the address in which the parent of n points to n
 */
static Node ** findPtrToChild(Node *n)
{
    if (n == NULL || n->parent == NULL)
    {
        return NULL;
    }
    Node *p = n->parent;
    if (p->left == n)
    {
        return &(p->left);
    }
    return &(p->right);
}

/**
 * Finds the sibling of a given node
 * @param n: a given node
 * @return: a pointer to n's sibling
 */
static Node * findSibling(Node *n)
{
    if (n == NULL || n->parent == NULL)
    {
        return NULL;
    }
    Node *p = n->parent;
    if (p->left != n)
    {
        return p->left;
    }
    return p->right;
}

/**
 * Finds the child of a given node (which suppose to has only one child)
 * @param n: a given node
 * @return: a pointer to n's child
 */
static Node * findChild(Node *n)
{
    if (n != NULL)
    {
        if (n->left != NULL)
        {
            return n->left;
        }
        return n->right;
    }
    return NULL;
}

/**
 * Finds the successor of a given node
 * @param x: a given node
 * @return: a pointer to x's successor
 */
static Node * successor(Node *x)
{
    if (x == NULL)
    {
        return NULL;
    }
    Node *s = x->right;
    while (s->left != NULL)
    {
        s = s->left;
    }
    return s;
}

/**
 * search a given data in a given RBTree
 * @param root: the root of a given tree
 * @param data: the data to search for
 * @param compFunc : comparison function
 * @return: the node which contains the data, NULL if no such exist
 */
static Node * search(Node *root, const void *data, CompareFunc compFunc)
{
    if (root == NULL)
    {
        return NULL;
    }
    int diff = compFunc(data, root->data);
    if (diff == 0)
    {
        return root;
    }
    // data is greater than root's data
    if (diff > 0)
    {
        return search(root->right, data, compFunc);
    }
    // data is smaller than root's data
    return search(root->left, data, compFunc);
}

/**
 * Traversing a tree In-Order and activates a given function with a given arguments on each node
 * @param root: the root of a given tree
 * @param func: a function to call on each node
 * @param args: an argument to pass to 'func'
 * @return: FAILURE if the given function with args input returns an error, SUCCESS otherwise.
 */
static int inOrder(Node *root, forEachFunc func, void *args)
{
    if (root == NULL)
    {
        return SUCCESS;
    }

    int success; // func activation feedback
    if (root->left != NULL)
    {
        success = inOrder(root->left, func, args);
        if (!success)
        {
            // func activation failure
            return FAILURE;
        }
    }

    success = func(root->data, args);
    if (!success)
    {
        // function activation failure
        return FAILURE;
    }

    if (root->right != NULL)
    {
        success = inOrder(root->right, func, args);
    }

    return success;
}

/**
 * De-allocated memory of a single node in a tree
 */
static void freeNode(Node *x, FreeFunc freeFunc)
{
    if (x != NULL)
    {
        freeFunc(x->data);
        x->data = NULL;
        free(x);
    }
}

/**
 * De-allocates memory of all the nodes in a given tree
 */
static void freeAllNodes(Node * root, FreeFunc freeFunc)
{
    if (root == NULL)
    {
        return;
    }

    freeAllNodes(root->left, freeFunc);
    freeAllNodes(root->right, freeFunc);

    freeFunc(root->data);
    root->data = NULL;
    free(root);
}

// ---------------- Insertion ----------------

/**
 * Performs left rotation on node 'n'
 * @param n: a node
 */
static void leftRotation(Node *n)
{
    Node *parent = n->parent;
    Node *rightChild = n->right;
    Node *grandson = rightChild->left;

    n->right = grandson;
    if (grandson != NULL)
    {
        grandson->parent = n;
    }

    n->parent = rightChild;
    rightChild->left = n;
    rightChild->parent = parent;

    if (parent != NULL)
    {
        if (parent->left == n)
        {
            parent->left = rightChild;
        }
        else
        {
            parent->right = rightChild;
        }
    }
}

/**
 * Performs right rotation on node 'n'
 * @param n: a node
 */
static void rightRotation(Node *n)
{
    Node *parent = n->parent; 
    Node *leftChild = n->left; 
    Node *grandson = leftChild->right;

    n->left = grandson;
    if (grandson != NULL)
    {
        grandson->parent = n;
    }

    n->parent = leftChild;
    leftChild->right = n;
    leftChild->parent = parent;

    if (parent != NULL)
    {
        if (parent->left == n)
        {
            parent->left = leftChild;
        }
        else
        {
            parent->right = leftChild;
        }
    }

}

/**
 * Rotates the parent of a given node in order to create a chain of nodes
 * @param n: a given node
 * @return: 1 if changes were made, 0 otherwise
 */
static int chain(Node *n)
{
    Node *p = n->parent;
    if (p == NULL || p->parent == NULL)
    {
        return 0;
    }
    Node *g = p->parent;

    if (g->left != NULL && g->left->right == n)
    {
        leftRotation(p);
        return 1;
    }

    if (g->right != NULL && g->right->left == n)
    {
        rightRotation(p);
        return 1;
    }
    return 0;

}

/**
 * Repair the tree structure (colors, rotations) after insertion
 * @param n: the newest node in the tree
 */
static void repairRBTree(Node *n)
{
    if (n->parent == NULL)
    {
        // case 1
        n->color = BLACK;
        return;
    }

    Node *p = n->parent; // N's parent
    if (p->color == BLACK)
    {
        // case 2
        return;
    }

    // define grandparent and uncle of N
    Node *g = p->parent; // N's Grandparent
    Node *u = g->right; // N's Uncle
    if (u == p)
    {
        u = g->left;
    }

    if (u != NULL && u->color == RED)
    {
        // case 3
        p->color = BLACK;
        u->color = BLACK;
        g->color = RED;
        repairRBTree(g);
        return;
    }

    // case 4a
    int swap = chain(n);
    if (swap)
    {
        Node *tmp = p;
        p = n;
        n = tmp;
    }

    // case 4b
    if (g->left != NULL && g->left->left == n)
    {
        rightRotation(g);
    }
    if (g->right != NULL && g->right->right == n)
    {

        leftRotation(g);
    }
    // case 4c
    p->color = BLACK;
    g->color = RED;
}

/**
 * Inserts a given data to a given tree
 * @param root: a pointer to the root of the tree
 * @param data: the data for insertion
 * @param compFunc : comparison function to compare between two node's data
 * @return: a pointer to the new node which stores the given data
 */
static Node * insertValue(Node **root, void *data, CompareFunc compFunc)
{
    Node *newNode = NULL;
    if (*root == NULL)
    {
        // initialize a new node with the data
        newNode = (Node*)malloc(sizeof(Node));
        newNode->data = data;
        newNode->color = RED;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->parent = NULL;
        *root = newNode;
        return newNode;
    }
    else
    {
        int diff = compFunc(data, (*root)->data);
        if (diff == 0)
        {
            // the tree already contains the given data
            return NULL;
        }
        else if (diff > 0)
        {
            // the data is "bigger" then the current node's data
            newNode = insertValue(&((*root)->right), data, compFunc);
        }
        else
        {
            // the data is "smaller" then the current node's data
            newNode = insertValue(&((*root)->left), data, compFunc);
        }
        if (newNode != NULL && newNode->parent == NULL)
        {
            newNode->parent = *root;
        }
        return newNode;
    }
}

// ---------------- Deletion ----------------

/**
 * Fix the tree structure (colors and rotations) after node deletion
 * @param p: the parent of the deleted node m
 * @param s: the sibling of the deleted node m
 */
static void fixTreeStructure(Node *p, Node *s)
{

    // ----------case 3a--------------
    if (p == NULL || s == NULL)
    {
        return; // C is the root
    }

    // ----------case 3b--------------
    if (s->color == BLACK && (s->left == NULL || s->left->color == BLACK) &&
        (s->right == NULL || s->right->color == BLACK))
    {
        // ----------case 3b-i-------------
        if (p->color == RED)
        {
            s->color = RED;
            p->color = BLACK;
        }
        // ----------case 3b-ii-------------
        else
        {
            s->color = RED;
            fixTreeStructure(p->parent, findSibling(p));
        }
    }
    else
    {
        // ----------case 3c--------------
        if (s->color == RED)
        {
            s->color = BLACK;
            p->color = RED;
            if (p->left != s)
            {
                leftRotation(p);
                fixTreeStructure(p, p->right);
            }
            else
            {
                rightRotation(p);
                fixTreeStructure(p, p->left);
            }
        }
        else
        {
            Node *sc = s->left;
            Node *sf = s->right;
            if (p->left == s)
            {
                sc = s->right;
                sf = s->left;
            }
            // ----------case 3d--------------
            if ((sc != NULL && sc->color == RED) && (sf == NULL || sf->color == BLACK))
            {
                sc->color = BLACK;
                s->color = RED;
                if (p->left != s)
                {
                    rightRotation(s);
                }
                else
                {
                    leftRotation(s);
                }
                fixTreeStructure(p, sc);
                return;
            }
            // ----------case 3e--------------
            if (sf->color == RED)
            {
                Color tempColor = s->color;
                s->color = p->color;
                p->color = tempColor;
                if (p->left != s)
                {
                    leftRotation(p);
                }
                else
                {
                    rightRotation(p);
                }
                sf->color = BLACK;
            }
        }
    }
}

/**
 * Deletes a single node from the tree
 * @param tree : the tree to delete from
 * @param m: node to delete
 * @param p: the parent of m
 * @param s: the sibling of m
 * @param c: the child of m
 */
static void deleteNode(RBTree *tree, Node *m, Node *p, Node *s, Node *c)
{
    Node ** ptrToM = findPtrToChild(m);
    Node ** ptrToC = findPtrToChild(c);

    // ----------case 1--------------
    if (m->color == RED)
    {
        *ptrToM = NULL;
        freeNode(m, tree->freeFunc);
        return;
    }

    // ----------case 2--------------
    if (c != NULL && c->color == RED)
    {
        *ptrToC = NULL;
        switchValues(m, c);
        freeNode(c, tree->freeFunc);
    }

    // ----------case 3--------------
    else
    {
        if (tree->root == m)
        {
            tree->root = NULL;
            freeNode(m, tree->freeFunc);
        }
        else
        {
            *ptrToM = NULL;
            freeNode(m, tree->freeFunc);
            fixTreeStructure(p, s);
        }
    }
}

// ---------------- Header ----------------

/**
 * remove an item from the tree
 * @param tree: the tree to remove an item from.
 * @param data: item to remove from the tree.
 * @return: 0 on failure, other on success. (if data is not in the tree - failure).
 */
int deleteFromRBTree(RBTree *tree, void *data)
{
    Node *m = search(tree->root, data, tree->compFunc);
    if (m == NULL)
    {
        return FAILURE;
    }
    if (m->left != NULL && m->right != NULL)
    {
        Node *s = successor(m);
        switchValues(m, s);
        m = s;
    }

    Node *p = m->parent;
    Node *s = findSibling(m);
    Node *c = findChild(m);
    deleteNode(tree, m, p, s, c);
    tree->size -= 1;

    while (tree->root != NULL && tree->root->parent != NULL)
    {
        tree->root = tree->root->parent;
    }

    return SUCCESS;
}

/**
 * add an item to the tree
 * @param tree: the tree to add an item to.
 * @param data: item to add to the tree.
 * @return: 0 on failure, other on success. (if the item is already in the tree - failure).
 */
int insertToRBTree(RBTree *tree, void *data)
{
    if (tree == NULL || data == NULL)
    {
        return FAILURE;
    }
    // Insert to tree
    Node *newNode = insertValue(&(tree->root), data, tree->compFunc);

    if (newNode == NULL)
    {
        return FAILURE;
    }
    tree->size += 1;

    // Repair the tree
    repairRBTree(newNode);
    // Update root pointer if needed
    while (tree->root != NULL && tree->root->parent != NULL)
    {
        tree->root = tree->root->parent;
    }
    return SUCCESS;
}

/**
 * check whether the tree RBTreeContains this item.
 * @param tree: RBTree
 * @param data: item to search for.
 * @return: 0 if the item is not in the tree, other if it is.
 */
int RBTreeContains(const RBTree *tree, const void *data)
{
    if (search(tree->root, data, tree->compFunc))
    {
        // RBTree contains the item
        return SUCCESS;
    }
    // RBTree does not contain the item
    return FAILURE;
}

/**
 * Activate a function on each item of the tree. the order is an ascending order. if one of the activations of the
 * function returns 0, the process stops.
 * @param tree: the tree with all the items.
 * @param func: the function to activate on all items.
 * @param args: more optional arguments to the function (may be null if the given function support it).
 * @return: 0 on failure, other on success.
 */
int forEachRBTree(const RBTree *tree, forEachFunc func, void *args)
{
    return inOrder(tree->root, func, args);
}

/**
 * free all memory of the data structure.
 * @param tree: pointer to the tree to free.
 */
void freeRBTree(RBTree **tree)
{
    if (tree == NULL || *tree == NULL)
    {
        return;
    }
    freeAllNodes((*tree)->root, (*tree)->freeFunc);
    free(*tree);
    *tree = NULL;
}

/**
 * constructs a new RBTree with the given CompareFunc.
 * comp: a function two compare two variables.
 */
RBTree * newRBTree(CompareFunc compFunc, FreeFunc freeFunc)
{
    RBTree *newTree = (RBTree *)malloc(sizeof(RBTree));
    if (newTree == NULL)
    {
        return NULL;
    }
    newTree->root = NULL;
    newTree->compFunc = compFunc;
    newTree->freeFunc = freeFunc;
    newTree->size = 0;
    return newTree;
}