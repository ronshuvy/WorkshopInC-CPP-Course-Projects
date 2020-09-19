/**
 * @file Dense.h
 * @author Ron Shuvy
 * @brief This header file defines Dense class in Mlp Network Model
 */

#ifndef DENSE_H
#define DENSE_H
// ------------------------------ includes ------------------------------
#include "Matrix.h"
#include "Activation.h"
// ------------------------------ method definitions -----------------------------

/**
 * @class Dense
 * @brief This class represents a Dense in a Mlp Network Model
 */
class Dense
{
private:
    const Activation _act; // activation function
    const Matrix _weights; // weights matrix
    const Matrix _bias; // bias matrix

public:
    /**
     * @fn Constructor
     */
    Dense(const Matrix& w, const Matrix& bias, ActivationType actType) : _act(actType), _weights(w), _bias(bias) {}

    /**
     * @fn Getters
     */
    const Matrix& getWeights() const { return _weights; }
    /**
    * @fn Getters
    */
    const Matrix& getBias() const { return _bias; }
    /**
    * @fn Getters
    */
    Activation getActivation() const { return _act; }

    /**
     * @fn This function calculates the result of the current dense
     */
    Matrix operator()(const Matrix &x);
};

#endif //DENSE_H
