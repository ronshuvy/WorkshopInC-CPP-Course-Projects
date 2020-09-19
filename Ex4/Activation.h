/**
 * @file Activation.h
 * @author Ron Shuvy
 * @brief This header file defines the activation function on a matrix
 */

#ifndef ACTIVATION_H
#define ACTIVATION_H

// ------------------------------ includes ------------------------------
#include "Matrix.h"
// ------------------------------ class declaration -----------------------------

/**
 * @enum ActivationType
 * @brief Indicator of activation function.
 */
enum ActivationType
{
    Relu,
    Softmax
};

/**
 * @class Matrix
 * @brief This class defines the activation function
 */
class Activation
{
private:
    ActivationType _type; // activation type
    Matrix _relu(const Matrix& mat) const; // Relu function
    Matrix _softmax(const Matrix& mat) const; // Softmax function

public:
    /**
    * @fn Default constructor
    */
    explicit Activation(ActivationType actType);

    /**
    * Getter
    */
    ActivationType getActivationType() const { return _type; }

    /**
    * @fn operator ()
    * @brief this operator calls the relevant activation function on a matrix input
    */
    Matrix operator()(const Matrix &mat) const { return (_type == Relu) ? _relu(mat) : _softmax(mat); }
};

#endif //ACTIVATION_H
