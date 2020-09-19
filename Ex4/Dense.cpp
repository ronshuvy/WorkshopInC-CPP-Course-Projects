/**
 * @file Dense.cpp
 * @author Ron Shuvy
 * @brief This file implements Dense class
 */

// ------------------------------ includes ------------------------------
#include "Dense.h"
// ------------------------------ method definitions -----------------------------

/**
 * @fn This function calculates the result of the current Dense
 */
Matrix Dense::operator()(const Matrix &x)
{
    Matrix z = (_weights * x) + _bias;
    return _act(z);
}
