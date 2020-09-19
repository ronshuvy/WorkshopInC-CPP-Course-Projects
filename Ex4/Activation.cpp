/**
 * @file Activation.cpp
 * @author Ron Shuvy
 * @brief This file implements the activation functions
 */

// ------------------------------ includes ------------------------------
#include "Activation.h"
#include <cmath>
// -------------------------- const definitions -------------------------
#define ERROR_ACT_TYPE "Error: Invalid activation type."
// ------------------------------ method definitions -----------------------------
/**
* @fn  Constructor
*/
Activation::Activation(ActivationType actType)
{
    if (actType != Relu && actType != Softmax)
    {
        std::cerr << ERROR_ACT_TYPE << std::endl;
        exit(EXIT_FAILURE);
    }
    _type = actType;
}

/**
 * @fn Relu activation function
 */
Matrix Activation::_relu(const Matrix& mat) const
{
    Matrix r(mat.getRows(), mat.getCols());
    for (int i = 0; i < mat.getRows() * mat.getCols(); ++i)
    {
        r[i] = (mat[i] >= 0) ? mat[i] : 0;
    }
    return r;
}

/**
 * @fn Softmax activation function
 */
Matrix Activation::_softmax(const Matrix& mat) const
{
    float sum = 0;
    for (int k = 0; k < mat.getRows() * mat.getCols(); ++k)
    {
        sum += (float)std::exp(mat[k]);
    }

    Matrix r(mat.getRows(), mat.getCols());
    for (int i = 0; i < mat.getRows() * mat.getCols(); ++i)
    {
        r[i] = (float)std::exp(mat[i]) * (1 / sum);
    }
    return r;
}





