//
// Created by guylu on 03/09/2019.
//

#include "Fractal.h"
#include "cassert"
#include "cmath"
#include <iostream>

using std::cout;
using std::endl;
using std::cerr;


static const char HASH = '#';
static const char EMPTY = ' ';

/**
 * cons of fractal
 * @param dim dim of fractal
 */
Fractal::Fractal(const int &dim1) : dim(dim1), baseSize(0)
{
    // create default arrays
    shape = new int *[baseSize];
    base = new int *[baseSize];
}

/**
 * copy ctor
 * @param fractal
 */
Fractal::Fractal(const Fractal &fractal)
{
    dim = fractal.dim;
    baseSize = fractal.baseSize;
    int calcDim = (int) pow(baseSize, dim);
    shape = new int *[calcDim];
    for (int i = 0; i < calcDim; ++i)
    {
        shape[i] = new int[calcDim];
    }

    _copy(shape, fractal.shape, calcDim);

    base = new int *[baseSize];
    for (int i = 0; i < baseSize; ++i)
    {
        base[i] = new int[baseSize];
    }

    _copy(base, fractal.base, baseSize);
}

/**
 * destructor
 */
Fractal::~Fractal()
{
    // free base:
    for (int i = 0; i < baseSize; ++i)
    {
        delete[]base[i];
    }
    delete[] base;
    //free shape
    for (int i = 0; i < (int) pow(baseSize, dim); ++i)
    {
        delete[]shape[i];
    }
    delete[] shape;
}

/**
 * operator =
 * @param fractal another fractal obj
 * @return new fractal obj
 */
Fractal &Fractal::operator=(const Fractal &fractal)
{
    dim = fractal.dim;
    baseSize = fractal.baseSize;
    _copy(base, fractal.base, baseSize);
    _copy(shape, fractal.shape, (int) pow(baseSize, dim));
    return *this;
}

/**
 * move ctor
 * @param fractal
 */

Fractal::Fractal(Fractal && fractal) noexcept
{
    dim = fractal.dim;
    baseSize = fractal.baseSize;
    shape = fractal.shape;
    fractal.shape = nullptr;
    base = fractal.base;
    fractal.base = nullptr;
}

/**
 * move assign
 * @param fractal
 * @return
 */
Fractal &Fractal::operator=(Fractal && fractal) noexcept
{
    //maybe should just be swap
    if (this != &fractal)
    {
        for (int i = 0; i < baseSize; ++i)
        {
            delete[]base[i];
        }
        delete[] base;
        //free shape
        for (int i = 0; i < (int) pow(baseSize, dim); ++i)
        {
            delete[]shape[i];
        }
        shape = fractal.shape;
        fractal.shape = nullptr;
        base = fractal.base;
        fractal.base = nullptr;
        baseSize = fractal.baseSize;
        dim = fractal.dim;
    }
    return *this;
}

/**
 * will create the shape
 */
void Fractal::createShape()
{
    // var to hold the dim of the finalized shape
    int calcDim = (int) pow(baseSize, dim);
    // initialize the array shape
    shape = new int *[calcDim];
    for (int i = 0; i < calcDim; ++i)
    {
        shape[i] = new int[calcDim]{0};
    }
    // initialize the array biggest yet - this array will hold the biggest recursive fractal we have
    //drawn yet such that in the next iteration we will copy paste the biggest yet array wherever is
    //necessary so the next iteration will be anew fractal, the the new fractal will be saved in
    // a new biggest array - handles by the reOrganize func
    int **biggestYet = new int *[1];
    biggestYet[0] = new int[1];
    biggestYet[0][0] = 1;

    // var to hold the dim of the shape as it grows
    int dynamicDim = 0;
    // main loop of the func - the one to generate the fractal shape:
    // it will copy paste biggestYet into shape wherever it should be(the if statement in the
    // middle of the loop handles this) and updates biggestYet after each iteration
    for (int k = 0; k < dim; ++k)
    {
        // update dynamicDim:
        dynamicDim = (int) pow(baseSize, k);
        for (int i = 0; i < dynamicDim * baseSize; i += dynamicDim)
        {
            for (int j = 0; j < dynamicDim * baseSize; j += dynamicDim)
            {
                // check the original base shape and check if it is filled.
                if (base[i / dynamicDim][j / dynamicDim] != 0)
                {
                    // copy paste biggestYet into shape in i,j
                    _translocate(shape, biggestYet, dynamicDim, i, j);
                }
            }
        }
        //update biggestYet
        reOrganize(biggestYet, dynamicDim);
    }

    for (int i = 0; i < calcDim; ++i)
    {
        delete[]biggestYet[i];
    }
    delete[] biggestYet;
}

/**
 * will create a larger biggestyet array that will hold the current state filled of shape
 * @param biggestYet biggestyet array
 * @param dynamicDim the dim of the array currently
 * @return pointer to new array
 */
void Fractal::reOrganize(int **&biggestYet, const int &dynamicDim)
{
    // delete biggestYet
    for (int l = 0; l < dynamicDim; ++l)
    {
        delete[]biggestYet[l];
    }
    delete[] biggestYet;
    // initialize new
    biggestYet = new int *[dynamicDim * baseSize];
    for (int l = 0; l < dynamicDim * baseSize; ++l)
    {
        biggestYet[l] = new int[dynamicDim * baseSize]{0};
    }
    // copy current state of shape into biggestYet
    _translocate(biggestYet, shape, dynamicDim * baseSize, 0, 0);
}

/**
 * func to copy array a2 into a1
 * @param a1 2d array
 * @param a2  2d array
 * @param size size of arrays - both dims
 */
void Fractal::_copy(int **&a1, int **const &a2, const int &size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            a1[i][j] = a2[i][j];
        }
    }
}

/**
 * function to fill the space in myshape with big such that we will the space recursivly
 * so we will get a fractal image
 * @param myshape shape
 * @param big biggest piece yet
 * @param size dim of big array
 * @param start0 where to fill index1
 * @param start1 where to fill index2
 */
void Fractal::_translocate(int **&myshape, int **const &big, const int &size, const int &start0,
                           const int &start1)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            myshape[start0 + i][start1 + j] = big[i][j];
        }
    }
}

/**
 * print the shape
 */
void Fractal::printShape()
{
    int size = (int) pow(baseSize, dim);
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            if (shape[i][j] == 1)
            {
                cout << HASH;
            }
            else
            {
                cout << EMPTY;
            }
        }
        cout << endl;
    }
}


/**
 * constructor
 * @param dim
 */
Carpet::Carpet(const int &dim) : Fractal(dim)
{
    // delete default fractals args to make new ones
    delete[] base;
    delete[] shape;
    // creating base shape
    baseSize = 3;
    base = new int *[baseSize];
    base[0] = new int[baseSize]{1, 1, 1};
    base[1] = new int[baseSize]{1, 0, 1};
    base[2] = new int[baseSize]{1, 1, 1};

    createShape();
}

/**
 * operator =
 * @param carpet another carpet obj
 * @return new carpet obj
 */
Carpet &Carpet::operator=(const Carpet &carpet)
{
    dim = carpet.dim;
    baseSize = carpet.baseSize;
    _copy(base, carpet.base, baseSize);
    _copy(shape, carpet.shape, (int) pow(baseSize, dim));
    return *this;
}

/**
 * move ctor
 * @param fractal
 */

Carpet::Carpet(Carpet && carpet) noexcept
{
    dim = carpet.dim;
    baseSize = carpet.baseSize;
    shape = carpet.shape;
    carpet.shape = nullptr;
    base = carpet.base;
    carpet.base = nullptr;
}

/**
 * move assign
 * @param fractal
 * @return
 */
Carpet &Carpet::operator=(Carpet && carpet) noexcept
{
    //maybe should just be swap
    if (this != &carpet)
    {
        for (int i = 0; i < baseSize; ++i)
        {
            delete[]base[i];
        }
        delete[] base;
        //free shape
        for (int i = 0; i < (int) pow(baseSize, dim); ++i)
        {
            delete[]shape[i];
        }
        shape = carpet.shape;
        carpet.shape = nullptr;
        base = carpet.base;
        carpet.base = nullptr;
        baseSize = carpet.baseSize;
        dim = carpet.dim;
    }
    return *this;
}

/**
 * constructor
 * @param dim
 */
Sieve::Sieve(const int &dim) : Fractal(dim)
{
    // delete default fractals args to make new ones
    delete[] base;
    delete[] shape;

    // creating base shape
    baseSize = 2;
    base = new int *[baseSize];
    base[0] = new int[baseSize]{1, 1};
    base[1] = new int[baseSize]{1, 0};

    createShape();
}

/**
 * operator =
 * @param carpet another carpet obj
 * @return new carpet obj
 */
Sieve &Sieve::operator=(const Sieve &sieve)
{
    dim = sieve.dim;
    baseSize = sieve.baseSize;
    _copy(base, sieve.base, baseSize);
    _copy(shape, sieve.shape, (int) pow(baseSize, dim));
    return *this;
}

/**
 * move ctor
 * @param fractal
 */

Sieve::Sieve(Sieve && sieve) noexcept
{
    dim = sieve.dim;
    baseSize = sieve.baseSize;
    shape = sieve.shape;
    sieve.shape = nullptr;
    base = sieve.base;
    sieve.base = nullptr;
}

/**
 * move assign
 * @param fractal
 * @return
 */
Sieve &Sieve::operator=(Sieve && sieve) noexcept
{
    //maybe should just be swap
    if (this != &sieve)
    {
        for (int i = 0; i < baseSize; ++i)
        {
            delete[]base[i];
        }
        delete[] base;
        //free shape
        for (int i = 0; i < (int) pow(baseSize, dim); ++i)
        {
            delete[]shape[i];
        }
        shape = sieve.shape;
        sieve.shape = nullptr;
        base = sieve.base;
        sieve.base = nullptr;
        baseSize = sieve.baseSize;
        dim = sieve.dim;
    }
    return *this;
}

/**
 * constructor
 * @param dim
 */
Dust::Dust(const int &dim) : Fractal(dim)
{
    // delete default fractals args to make new ones
    delete[] base;
    delete[] shape;

    // creating base shape
    baseSize = 3;
    base = new int *[baseSize];
    base[0] = new int[baseSize]{1, 0, 1};
    base[1] = new int[baseSize]{0, 0, 0};
    base[2] = new int[baseSize]{1, 0, 1};

    createShape();
}

/**
 * operator =
 * @param carpet another carpet obj
 * @return new carpet obj
 */
Dust &Dust::operator=(const Dust &dust)
{
    dim = dust.dim;
    baseSize = dust.baseSize;
    _copy(base, dust.base, baseSize);
    _copy(shape, dust.shape, (int) pow(baseSize, dim));
    return *this;
}

/**
 * move ctor
 * @param fractal
 */

Dust::Dust(Dust && dust) noexcept
{
    dim = dust.dim;
    baseSize = dust.baseSize;
    shape = dust.shape;
    dust.shape = nullptr;
    base = dust.base;
    dust.base = nullptr;
}

/**
 * move assign
 * @param fractal
 * @return
 */
Dust &Dust::operator=(Dust && dust) noexcept
{
    //maybe should just be swap
    if (this != &dust)
    {
        for (int i = 0; i < baseSize; ++i)
        {
            delete[]base[i];
        }
        delete[] base;
        //free shape
        for (int i = 0; i < (int) pow(baseSize, dim); ++i)
        {
            delete[]shape[i];
        }
        shape = dust.shape;
        dust.shape = nullptr;
        base = dust.base;
        dust.base = nullptr;
        baseSize = dust.baseSize;
        dim = dust.dim;
    }
    return *this;
}
