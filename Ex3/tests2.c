//
// Created by Maor on 21/05/2020.
//

#include "RBTree.h"
#include "RBUtilities.h"
#include "Structs.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>


#define LAST_NUMBER_OF_NODES_TO_CHECK 2000

#define MAX_STRING_LENGTH_CHECK 50
#define MAX_VECTOR_DATA_VALUE 1000
#define MAX_VECTOR_LENGTH_CHECK 50
#define MAX_CHAR_ASCII_VALUE 127
#define MIN_CHAR_ASCII_VALUE 33
#define MAX_INT_VALUE_CHECK 2000
#define MAX_INPUT_TO_SHOW_TREE 25
#define CHECK_DELETE true

int compInt(void* data1, void* data2)
{
    int a = *((int*) data1);
    int b = *((int*) data2);
    if(a == b)
        return 0;
    if(a > b)
        return 1;
    return -1;
}

int printS(const void *st, void *args)
{
    printf("<%s> ---> ", (char *)st);
    return 1;
}

int printV(const void *v1, void *args)
{
    Vector *v = (Vector*)v1;
    printf("vector [");
    for (int i=0; i < v->len; ++i)
    {
        printf(" %lf ,", (double)v->vector[i]);
    }
    printf("]\n");
    return 1;
}

void intFree(void* data)
{ // does nothing, just here so we will not get an error
    int* d = (int*) data;
    if(d == 0)
    {
        return;
    }
}

void insert(RBTree* t, void* data, const int i, const void* arr, const char* testName)
{
    insertToRBTree(t, data);
    if(!isValidRBTree(t))
    {
        if(!strcmp(testName, "int"))
            printf("ERROR - after the insertion of '%d', the tree is not valid\n", *((int*)data));
        else
        {
            if(!strcmp(testName, "string"))
                printf("ERROR - after the insertion of '%s', the tree is not valid\n", *((char**)data));
            else
                printf("ERROR - after the insertion of one of the nodes, the tree is not valid\n");
        }
        if(!strcmp(testName, "int"))
        {
            if(i <= MAX_INPUT_TO_SHOW_TREE)
            {
                int* arrInt = (int*) arr;
                printf("tried to insert in this order: \n");
                for(int j = 0; j < i; j++)
                {
                    printf("%d ", arrInt[j]);
                }
                printf("\ngot the following tree\n");
                printRBTree(t->root);
            }
        }
        if(!strcmp(testName, "string"))
        {
            if(i <= MAX_INPUT_TO_SHOW_TREE)
            {
                char** arrStrings = (char**) arr;
                printf("tried to insert in this order: \n");
                for(int j = 0; j < i; j++)
                {
                    printf("%s ", arrStrings[j]);
                }
            }
        }
        freeRBTree(&t);
        exit(EXIT_FAILURE);
    }
}

void delete(RBTree* t, void* data, const int i, const void* arr, const char* testName)
{
    if(deleteFromRBTree(t, data) == 0)
    { // if there was an error free the data so we will not get a memory leak error
        t->freeFunc(data);
    }
    if(!isValidRBTree(t))
    {
        if(!strcmp(testName, "int"))
            printf("ERROR - after the deletion of '%d', the tree is not valid\n", *((int*)data));
        else
        {
            if(!strcmp(testName, "string"))
                printf("ERROR - after the deletion of '%s', the tree is not valid\n", *((char**)data));
            else
                printf("ERROR - after the deletion of one of the nodes, the tree is not valid\n");
        }
        if(!strcmp(testName, "int"))
        {
            if(i <= MAX_INPUT_TO_SHOW_TREE)
            {
                int* arrInt = (int*) arr;
                printf("tried to delete in this order: \n");
                for(int j = 0; j < i; j++)
                {
                    printf("%d ", arrInt[j]);
                }
                printf("\ngot the following tree\n");
                printRBTree(t->root);
            }
        }
        if(!strcmp(testName, "string"))
        {
            if(i <= MAX_INPUT_TO_SHOW_TREE)
            {
                char** arrStrings = (char**) arr;
                printf("tried to delete in this order: \n");
                for(int j = 0; j < i; j++)
                {
                    printf("%s ", arrStrings[j]);
                }
            }
        }
        freeRBTree(&t);
        exit(EXIT_FAILURE);
    }
}

void intTree()
{
    for(int i = 0; i <= LAST_NUMBER_OF_NODES_TO_CHECK; i++)
    {
        // test a tree with i nodes
        int* a = (int*) malloc(i*sizeof(int));
        RBTree* t = newRBTree((CompareFunc) &compInt, (FreeFunc) &intFree);
        printf("Ints tree with %d nodes: ", i);
        for(int j = 0; j < i; j++)
        {
            a[j] = rand() % MAX_INT_VALUE_CHECK * (rand()%2? -1: 1);
        }
        for(int j = 0; j < i; j++)
        { // insert a[j]
            insert(t, &a[j], i, a, "int");
        }
        if(CHECK_DELETE)
        {
            for(int j = 0; j < i; j++)
            { // delete a[j]
                delete(t, &a[j], i, a, "int");
            }
            if(t->size != 0)
            {
                printf("ERROR - deleted all the nodes from the tree and yet the tree's size is not 0\n");
            }
        }
        freeRBTree(&t);
        printf("passed\n");
        free(a);
        a = NULL;
    }
    printf("\n\n*****passed the test of ints tree*****\n\n");
}

char* randomString(const int maxLength)
{
    int length = rand() % maxLength;
    char* toReturn = (char*) malloc(sizeof(char) * (length+1));
    for(int i = 0; i < length; i++)
    {
        int ch = rand() % MAX_CHAR_ASCII_VALUE;
        while(ch < MIN_CHAR_ASCII_VALUE)
        {
            ch = rand() % MAX_CHAR_ASCII_VALUE;
        }
        toReturn[i] = (char) ch;
    }
    toReturn[length] = '\0';
    return toReturn;
}

void stringTree()
{
    for(int i = 0; i <= LAST_NUMBER_OF_NODES_TO_CHECK; i++)
    {
        printf("%d\n", i);
        // test a tree with i nodes
        char** a = (char**) malloc(sizeof(char*) * i);
        RBTree* t = newRBTree((CompareFunc) &stringCompare, (FreeFunc) &freeString);
        printf("Strings tree with %d nodes:", i);
        for(int j = 0; j < i; j++)
        {
            a[j] = randomString(MAX_STRING_LENGTH_CHECK);
        }

        for(int j = 0; j < i; j++)
        { // insert a[j]
            //printf("inserting <%s>\n", a[j]);
            insert(t, a[j], i, a, "string");
        }
        //printf(" -------------Insertion successful --------------\n");
        //printf(" -------------Printing Tree --------------\n");
        //forEachRBTree(t, printS, NULL);
        //printf("\n------------- Finished Printing --------------\n");

        if(CHECK_DELETE)
        {
            for (int j = 0; j < i; j++)
            { // delete a[j]
                delete(t, a[j], i, a, "string");
            }
            if (t->size != 0)
            {
                printf("ERROR - deleted all the nodes from the tree and yet the tree's size is not 0\n");
            }
        }
        freeRBTree(&t);
        printf("passed\n");
        free(a);
        a = NULL;
    }
    printf("\n\n*****passed the test of strings tree*****\n\n");
}

Vector* randomVector(const int maxLength)
{
    int length = rand() % maxLength;
    Vector* toReturn = (Vector*) malloc(sizeof(Vector));
    toReturn->vector = (double*) malloc(sizeof(double) * length);
    toReturn->len = length;
    for(int i = 0; i < length; i++)
    {
        double vecCord = rand() % MAX_VECTOR_DATA_VALUE + ((double) rand()) / rand();
        toReturn->vector[i] = rand()%2? vecCord: -1*vecCord;
    }
    return toReturn;
}

void vectorTree()
{
    for(int i = 0; i <= LAST_NUMBER_OF_NODES_TO_CHECK; i++)
    {
        // test a tree with i nodes
        Vector** a = (Vector**) malloc(sizeof(Vector*) * i);
        RBTree* t = newRBTree((CompareFunc) &vectorCompare1By1, (FreeFunc) &freeVector);
        printf("Vectors tree with %d nodes: ", i);
        for(int j = 0; j < i; j++)
        {
            a[j] = randomVector(MAX_VECTOR_LENGTH_CHECK);
        }
        for(int j = 0; j < i; j++)
        { // insert a[j]
            insert(t, a[j], i, a, "vector");
        }

        //printf(" -------------Printing Tree After Build --------------\n");
        //forEachRBTree(t, printV, NULL);

        if(CHECK_DELETE)
        {
            for (int j = 0; j < i; j++)
            { // delete a[j]
                //printf("DELETING VECTOR %d LENGTH\n", (int)((Vector *)a[j])->len);
                //printf("Deleting ");
                //printV(a[j], NULL);
                delete(t, a[j], i, a, "vector");
                //printf(" -------------Printing Tree After Deletion --------------\n");
                //forEachRBTree(t, printV, NULL);
            }
            if (t->size != 0)
            {
                printf("ERROR - deleted all the nodes from the tree and yet the tree's size is not 0\n");
            }
        }

        //printf(" -------------Printing Tree At End --------------\n");
        //forEachRBTree(t, printV, NULL);

        freeRBTree(&t);
        printf("passed\n");
        free(a);
        a = NULL;
    }
    printf("\n\n*****passed the test of vectors tree*****\n\n");
}

int main()
{
    srand(time(0));
    //intTree();
    stringTree();
    vectorTree();
    printf("\nPassed All tests!!\n");
    return EXIT_SUCCESS;
}