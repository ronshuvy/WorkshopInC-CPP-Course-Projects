/**
 * @file MlpNetwork.h
 * @author Ron Shuvy
 * @brief This header file defines MLP Network class for digit recognition
 * input - image which contains a single digit
 * output - the identified digit with its correctness probability
 */
#ifndef MLPNETWORK_H
#define MLPNETWORK_H
// ------------------------------ includes ------------------------------
#include "Matrix.h"
#include "Digit.h"
#include "Dense.h"
// -------------------------- const definitions -------------------------
#define MLP_SIZE 4
const MatrixDims imgDims = {28, 28};
const MatrixDims weightsDims[] = {{128, 784}, {64, 128}, {20, 64}, {10, 20}};
const MatrixDims biasDims[]    = {{128, 1}, {64, 1}, {20, 1},  {10, 1}};
// ------------------------------ class declaration -----------------------------

/**
 * @class MlpNetwork
 * @brief This class contains the complete mlp network
 */
class MlpNetwork
{
private:
    const Matrix* _weights; // array of weights
    const Matrix* _biases; // array of biases

public:
    /**
     * @fn Constructor
     */
    MlpNetwork(const Matrix weights[], const Matrix biases[]) : _weights(weights), _biases(biases) {};

    /**
     * @fn operator ()
     * @param img - an image of a digit coded to matrix representation
     * @return the identified digit with its probability
     */
    Digit operator()(const Matrix &img) const;

};
#endif // MLPNETWORK_H
