/**
 * @file Matrix.cpp
 * @author Ron Shuvy
 * @brief This file implements a matrix class
 */
// ------------------------------ includes ------------------------------
#include "Matrix.h"
// -------------------------- const definitions -------------------------
#define EXIT_FAILURE 1
#define MIN_SIZE 1
#define ERROR_MAT_SIZE "Error: Matrix size should be positive integers."
#define ERROR_MAT_MULT "Error: Illegal matrix multiplication."
#define ERROR_MAT_ADD "Error: Illegal matrix addition."
#define MEMORY_FAILURE "Error: Memory allocation failed."
#define ERROR_READING "Error: Failed reading from input stream."
#define INDEX_OUT_OF_BOUNDS "Error: Index out of bounds."

// ------------------------------ method definitions -----------------------------

//--------- Constructor ---------

/**
 * @fn Constructor
 */
Matrix::Matrix(int rows, int cols)
{
    // Validate input
    if (rows < MIN_SIZE || cols < MIN_SIZE)
    {
        std::cerr << ERROR_MAT_SIZE << std::endl;
        exit(EXIT_FAILURE);
    }

    // Initialize matrix size
    _rows = rows, _cols = cols;
    _data = new (std::nothrow) float[rows * cols]();

    if (_data == nullptr)
    {
        std::cerr << MEMORY_FAILURE << std::endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * @fn Copy Constructor
 */
Matrix::Matrix(const Matrix& m) : Matrix(m.getRows(), m.getCols())
{
    for (int i = 0; i < _rows * _cols; ++i)
    {
        _data[i] = m[i];
    }
}

/**
 * @fn Destructor
 */
Matrix::~Matrix()
{
    delete[] _data;
}

//--------- Utilities ---------
/**
 * @fn vectorize
 * @brief converts multi-dim matrix to column vector
 */
Matrix& Matrix::vectorize()
{
    _rows *= _cols;
    _cols = 1;
    return *this;
}

/**
 * @fn plainPrint
 * @brief prints matrix elements to sdout
 */
void Matrix::plainPrint() const
{
    for(int i = 0; i < _rows; ++i)
    {
        for (int j = 0; j < _cols; ++j)
        {
            std::cout << (*this)(i, j) << " ";
        }
        std::cout << std::endl;
    }
}

//--------- Operators ---------

/**
 * @fn operator ()
 * @brief access entry (i,j) in matrix
 */
float& Matrix::operator()(int i, int j) const
{
    if (i < 0 || i >= _rows || j < 0 || j >= _cols)
    {
        std::cerr << INDEX_OUT_OF_BOUNDS << std::endl;
        exit(EXIT_FAILURE);
    }
    return _data[i * _cols + j];
}

/**
 * @fn operator []
 * @brief access i-th entry in matrix
 */
float& Matrix::operator[](int i) const
{
    if (i < 0 || i >= _rows * _cols)
    {
        std::cerr << INDEX_OUT_OF_BOUNDS << std::endl;
        exit(EXIT_FAILURE);
    }
    return _data[i];
}

/**
 * @fn operator =
 * @brief copy assigment
 */
Matrix& Matrix::operator=(const Matrix& b)
{
    if (this != &b)
    {
        Matrix &a = *this;
        if ((a._rows * a._cols) != (b._rows * b._cols))
        {
            delete[] a._data;
            a._data = new(std::nothrow) float[b._rows * b._cols];
            if (a._data == nullptr)
            {
                std::cerr << MEMORY_FAILURE << std::endl;
                exit(EXIT_FAILURE);
            }
        }
        a._rows = b._rows, a._cols = b._cols;
        for (int i = 0; i < a._rows * a._cols; ++i)
        {
            a[i] = b[i];
        }
    }
    return *this;
}

/**
 * @fn operator *
 * @brief Matrix multiplication
 */
Matrix operator*(const Matrix& a, const Matrix& b)
{
    if (a._cols != b._rows)
    {
        std::cerr << ERROR_MAT_MULT << std::endl;
        exit(EXIT_FAILURE);
    }
    Matrix mult(a._rows, b._cols);
    for(int i = 0; i < a._rows; ++i)
    {
        for(int j = 0; j < b._cols; ++j)
        {
            for (int k = 0; k < a._cols; ++k)
            {
                mult(i, j) += a(i, k) * b(k, j);
            }
        }
    }
    return mult;
}

/**
 * @fn operator *
 * @brief Multiplication between lhs=matrix and rhs=scalar
 */
Matrix operator*(const Matrix& m, const float c)
{
    Matrix res(m._rows, m._cols);
    for(int i = 0; i < m._rows; ++i)
    {
        for (int j = 0; j < m._cols; ++j)
        {
            res(i, j) = m(i, j) * c;
        }
    }
    return res;
}

/**
 * @fn operator +
 * @brief Matrix addition
 */
Matrix operator+(const Matrix& a, const Matrix& b)
{
    if (a._cols != b._cols || a._rows != b._rows)
    {
        std::cerr << ERROR_MAT_ADD << std::endl;
        exit(EXIT_FAILURE);
    }
    Matrix sum(a._rows, a._cols);
    for(int i = 0; i < a._rows; ++i)
    {
        for(int j = 0; j < a._cols; ++j)
        {
            sum(i, j) = a(i, j) + b(i, j);
        }
    }
    return sum;
}

/**
 * @fn operator +=
 * @brief Addition assignment
 */
Matrix& Matrix::operator+=(const Matrix& b)
{
    Matrix &a = *this;
    if (a._cols != b._cols || a._rows != b._rows)
    {
        std::cerr << ERROR_MAT_ADD << std::endl;
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < a._rows; ++i)
    {
        for(int j = 0; j < a._cols; ++j)
        {
            a(i, j) += b(i, j);
        }
    }
    return a;
}

//--------- I/O ---------
/**
 * @fn operator <<
 * @brief draw matrix as digit
 */
std::ostream& operator<<(std::ostream &out, const Matrix &m)
{
    for (int i = 0; i < m._rows; ++i)
    {
        for (int j = 0; j < m._cols; ++j)
        {
            out << ((m(i, j) <= 0.1f) ? "  " : "**");
        }
        out << std::endl;
    }
    return out;
}

/**
 * @fn operator >>
 * @brief loads data into matrix entries from an input
 */
void operator>>(std::istream &is, const Matrix &m)
{
    for (int i = 0; i < m.getRows()* m.getCols(); ++i)
    {
        is.read((char *) &(m[i]), sizeof(float));
        if (! is.good())
        {
            std::cerr << ERROR_READING << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    if (is.peek() != EOF)
    {
        std::cerr << ERROR_READING << std::endl;
        exit(EXIT_FAILURE);
    }
}





