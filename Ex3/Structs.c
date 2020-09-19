/**
 * @file Structs.c
 * @author Ron Shuvy
 * @id 206330193
 *
 * @brief This file implements string and vector utilities functions
 */

#include "Structs.h"
#include <string.h>
#include <stdlib.h>

#define EMPTY_VEC_NORM -1
#define MIN_VEC_LEN 0
#define SUCCESS 1
#define FAILURE 0
#define PROXIMITY 0.01
#define V1_BIGGER 1
#define V2_BIGGER -1

// ------------------------------ Functions -----------------------------

// ---------------- Strings ----------------

/**
 * CompFunc for strings (assumes strings end with "\0")
 * @param a - char* pointer
 * @param b - char* pointer
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a. (lexicographic
 * order)
 */
int stringCompare(const void *a, const void *b)
{
    return strcmp((const char *)a, (const char *)b);
}

/**
 * ForEach function that concatenates the given word and \n to pConcatenated. pConcatenated is
 * already allocated with enough space.
 * @param word - char* to add to pConcatenated
 * @param pConcatenated - char*
 * @return 0 on failure, other on success
 */
int concatenate(const void *word, void *pConcatenated)
{
    char *dest = (char *)pConcatenated;
    if (pConcatenated == NULL || word == NULL)
    {
        return FAILURE;
    }

    char newline[] = "\n";
    strcat(dest, (char *)word);
    strcat(dest, newline);
    return SUCCESS;
}

/**
 * FreeFunc for strings
 */
void freeString(void *s)
{
    if (s != NULL)
    {
        free(s);
    }
}

// ---------------- Vectors ----------------

/**
 * CompFunc for Vectors, compares element by element, the vector that has the first larger
 * element is considered larger. If vectors are of different lengths and identify for the length
 * of the shorter vector, the shorter vector is considered smaller.
 * @param a - first vector
 * @param b - second vector
 * @return equal to 0 iff a == b. lower than 0 if a < b. Greater than 0 iff b < a.
 */
int vectorCompare1By1(const void *a, const void *b)
{
    const Vector *v1 = (const Vector*)a;
    const Vector *v2 = (const Vector*)b;
    int maxIter = (v1->len < v2->len) ? v1->len : v2->len;
    for (int i = 0; i < maxIter; ++i)
    {
        double diff = v1->vector[i] - v2->vector[i];
        double eps = PROXIMITY;
        if (diff > eps)
        {
            return V1_BIGGER;
        }
        if (diff < -eps)
        {
            return V2_BIGGER;
        }
    }
    return v1->len - v2->len;
}

/**
 * FreeFunc for vectors
 */
void freeVector(void *pVector)
{
    Vector *pV = (Vector *)pVector;
    if (pV != NULL)
    {
        double *v = (double *)pV->vector;
        if (v != NULL)
        {
            free(v);
        }
        free(pV);
    }
}
/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */

/**
 * Calculate the squared norm of a given vector
 * @param pV: vector
 * @return: the result, EMPTY_VEC_NORM if the vector is empty
 */
static double getNormSum(const Vector *pV)
{
    if (pV == NULL || pV->vector == NULL)
    {
        return EMPTY_VEC_NORM;
    }
    double square_sums = 0;
    for (int i = 0; i < pV->len; ++i)
    {
        square_sums += (pV->vector[i] * pV->vector[i]);
    }
    return square_sums;
}

/**
 * Deep copies the vector pointed to by s1 into the vector pointed to by s2
 * @param v1: first vector
 * @param v2: second vector
 */
static void copyVector(const Vector *v1, Vector *v2)
{
    if (v1->len != v2->len)
    {
        double *newV = (double *)realloc(v2->vector, v1->len * sizeof(double));
        v2->len = v1->len;
        v2->vector = newV;
    }

    // Copy the values from the first vector to the other
    for (int i = 0; i < v2->len; ++i)
    {
        v2->vector[i] = v1->vector[i];
    }
}

/**
 * copy pVector to pMaxVector if : 1. The norm of pVector is greater then the norm of pMaxVector.
 * 								   2. pMaxVector->vector == NULL.
 * @param pVector pointer to Vector
 * @param pMaxVector pointer to Vector
 * @return 1 on success, 0 on failure (if pVector == NULL: failure).
 */
int copyIfNormIsLarger(const void *pVector, void *pMaxVector)
{
    // Casting to Vector type
    const Vector *pV = (const Vector*)pVector;
    Vector *pMaxV = (Vector*)pMaxVector;

    if (pVector == NULL)
    {
        return FAILURE;
    }

    // Calculate each vector's norm
    double normV = getNormSum(pV);
    double normMaxV = getNormSum(pMaxV);

    if (normV > normMaxV)
    {
        copyVector(pV, pMaxV);
    }
    return SUCCESS;
}

/**
 * @param tree a pointer to a tree of Vectors
 * @return pointer to a *copy* of the vector that has the largest norm (L2 Norm).
 */
Vector *findMaxNormVectorInTree(RBTree *tree)
{
    Vector *pMaxVector = malloc(sizeof(Vector));
    pMaxVector->len = MIN_VEC_LEN;
    pMaxVector->vector = NULL;
    forEachRBTree(tree, copyIfNormIsLarger, pMaxVector);
    return pMaxVector;
}
