/**
 * @file MlpNetwork.cpp
 * @author Ron Shuvy
 * @brief This file implements MlpNetwork class
 */
// ------------------------------ includes ------------------------------
#include "MlpNetwork.h"
// ------------------------------ method definitions -----------------------------
/**
 * @brief This functions finds the top-rated digit and return its value and its probability
 */
Digit findDigit(const Matrix& result)
{
    Digit d = {0, result[0]};
    for (int i = 1; i < result.getCols() * result.getRows(); ++i)
    {
        if (result[i] > d.probability)
        {
            d.probability = result[i], d.value = i;
        }
    }
    return d;
}

/**
 * @fn operator ()
 * @param img - an image of a digit coded to matrix representation
 * @return the identified digit with its probability
 */
Digit MlpNetwork::operator()(const Matrix &img) const
{
    Matrix result = img;
    for (int i = 0; i < MLP_SIZE - 1; ++i)
    {
        result = Dense(_weights[i], _biases[i], Relu)(result);
    }
    result = Dense(_weights[MLP_SIZE-1], _biases[MLP_SIZE-1], Softmax)(result);
    return findDigit(result);
}
