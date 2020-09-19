
#include "Matrix.h"
#include <iostream>
using namespace std;

int main(int argc, char **argv)
{
    Matrix m1(2,4);
    for (int i=0; i<m1.getCols()*m1.getRows(); i++)
        m1[i] = 1.0f + (float)i;

    m1.plainPrint();
    Matrix m2 = 10 * m1;
    //Matrix m2 = m1 * 10;
    m2.plainPrint();

    return 0;
}
