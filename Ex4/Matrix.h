/**
 * @file Matrix.h
 * @author Ron Shuvy
 * @brief This header file defines a matrix class of type float
 */

#ifndef MATRIX_H
#define MATRIX_H
// ------------------------------ includes ------------------------------
#include <iostream>
// -------------------------- const definitions -------------------------
#define BASIC_MAT_SIZE 1
// ------------------------------ class declaration -----------------------------
/**
 * @struct MatrixDims
 * @brief Matrix dimensions container
 */
typedef struct MatrixDims
{
    int rows, cols;
} MatrixDims;

/**
 * @class Matrix
 * @brief This class represents a matrix of type 'float'
 */
class Matrix
{
private:
    float *_data; // Matrix elements
    int _rows, _cols; // Matrix size

public:
    /**
     * @fn Constructor
     */
    Matrix(int rows, int cols);

    /**
     * @fn Default Constructor
     */
    Matrix() : Matrix(BASIC_MAT_SIZE, BASIC_MAT_SIZE) {}

    /**
     * @fn Copy Constructor
     */
    Matrix(const Matrix &m);

    /**
     * @fn Destructor
     */
    ~Matrix();

    /**
    * @fn Getters
    */
    int getRows() const { return _rows; }
    /**
    * @fn Getters
    */
    int getCols() const { return _cols; }

    /**
     * @fn vectorize
     * @brief converts multi-dim matrix to column vector
     */
    Matrix& vectorize();
    /**
     * @fn plainPrint
     * @brief prints matrix elements to sdout
     */
    void plainPrint() const;

    /**
     * @fn operator =
     * @brief copy assigment
     */
    Matrix& operator=(const Matrix& b);
    /**
     * @fn operator +=
     * @brief Addition assignment
     */
    Matrix& operator+=(const Matrix& b);
    /**
     * @fn operator *
     * @brief Matrix multiplication
     */
    friend Matrix operator*(const Matrix& a, const Matrix& b);
    /**
     * @fn operator *
     * @brief Multiplication between lhs=matrix and rhs=scalar
     */
    friend Matrix operator*(const Matrix& m, float c);
    /**
     * @fn operator *
     * @brief Multiplication between lhs=scalar and rhs=matrix
     */
    friend Matrix operator*(float c, const Matrix& m) { return m * c; }
    /**
     * @fn operator +
     * @brief Matrix addition
     */
    friend Matrix operator+(const Matrix& a, const Matrix& b);
    /**
     * @fn operator ()
     * @brief access entry (i,j) in matrix
     */
    float& operator()(int i, int j) const;
    /**
     * @fn operator []
     * @brief access i-th entry in matrix
     */
    float& operator[](int i) const;

    /**
     * @fn operator <<
     * @brief draw matrix as digit
     */
    friend std::ostream& operator<<(std::ostream& out, const Matrix& m);
    /**
     * @fn operator >>
     * @brief loads data into matrix entries from an input
     */
    friend void operator>>(std::istream& is, const Matrix& m);

};
#endif //MATRIX_H
