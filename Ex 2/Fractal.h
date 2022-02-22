//
// Created by guylu on 03/09/2019.
//

#ifndef EX_2_FRACTAL_H
#define EX_2_FRACTAL_H

#include <exception>

/**
 * class to represent a fractal
 */
class Fractal
{
private:


/**
 * function to fill the space in myshape with big such that we will the space recursivly
 * so we will get a fractal image
 * @param myshape shape
 * @param big biggest piece yet
 * @param size dim of big array
 * @param start0 where to fill index1
 * @param start1 where to fill index2
 */
    void _translocate(int **&myshape, int **const &big, const int &size, const int &start0,
                      const int &start1);

protected:
    int dim; // dim of shape
    int **shape; // 2d array to hold the shape
    int **base; // base shape of the fractal
    int baseSize; // dim of base array
    /**
 * func to copy array a2 into a1
 * @param a1 2d array
 * @param a2  2d array
 * @param size size of arrays - both dims
 */
    void _copy(int **&a1, int **const &a2, const int &size);

public:

/**
 * cons of fractal
 * @param dim dim of fractal
 */
    Fractal(const int &dim = 0);

/**
 * copy ctor
 * @param fractal
 */
    Fractal(const Fractal &fractal);

/**
 * destructor
 */
    ~Fractal();

/**
 * operator =
 * @param fractal another fractal obj
 * @return new fractal obj
 */
    Fractal &operator=(const Fractal &fractal);

/**
 * move ctor
 * @param fractal
 */
    Fractal(Fractal && fractal) noexcept;

/**
 * move assign
 * @param fractal
 * @return
 */
    Fractal &operator=(Fractal && fractal) noexcept;

/**
 * will create the shape
 */
    void createShape();

/**
 * print the shape
 */
    void printShape();

/**
 * will create a larger biggestyet array that will hold the current state filled of shape
 * @param biggestYet biggestyet array
 * @param dynamicDim the dim of the array currently
 * @return pointer to new array
 */
    void reOrganize(int **&biggestYet, const int &dynamicDim);
};

/**
 * class to represent the fractal carpet
 */
class Carpet : public Fractal
{
public:
/**
 * constructor
 * @param dim
 */
    Carpet(const int &dim);

/**
 * copy ctor
 * @param carpet
 */
    Carpet(const Carpet &carpet) : Fractal(carpet)
    {};

/**
 * operator =
 * @param carpet another carpet obj
 * @return new carpet obj
 */
    Carpet &operator=(const Carpet &carpet);

/**
 * move ctor
 * @param carpet
 */
    Carpet(Carpet && carpet) noexcept;

/**
 * move assign
 * @param carpet
 * @return
 */
    Carpet &operator=(Carpet && carpet) noexcept;
    
    /**
 * destructor
 */
    ~Carpet()
    {};
};

/**
 * class to represent the fractal sieve
 */
class Sieve : public Fractal
{
public:
/**
 *constructor
 * @param dim
 */
    Sieve(const int &dim);

/**
 *destructor
 */
    ~Sieve()
    {};
    
/**
 * copy ctor
 * @param sieve
 */
    Sieve(const Sieve &sieve) : Fractal(sieve)
    {};
    
/**
 * operator =
 * @param sieve another sieve obj
 * @return new sieve obj
 */
    Sieve &operator=(const Sieve &sieve);

/**
 * move ctor
 * @param sieve
 */
    Sieve(Sieve && sieve) noexcept;

/**
 * move assign
 * @param sieve
 * @return
 */
    Sieve &operator=(Sieve && sieve) noexcept;

};

/**
 * class to represent the fractal cantors dust
 */
class Dust : public Fractal
{
public:
/**
 * constructor
 * @param dim
 */
    Dust(const int &dim);

/**
 *destructor
 */
    ~Dust()
    {};
    
/**
 * copy ctor
 * @param dust
 */
    Dust(const Dust &dust) : Fractal(dust)
    {};
    
/**
 * operator =
 * @param dust another dust obj
 * @return new dust obj
 */
    Dust &operator=(const Dust &dust);

/**
 * move ctor
 * @param dust
 */
    Dust(Dust && dust) noexcept;

/**
 * move assign
 * @param dust
 * @return
 */
    Dust &operator=(Dust && dust) noexcept;

};

/**
 *
 */
class genException : public std::exception
{
private:
public:
/**
 *
 */
    genException() : exception()
    {};

/**
 *
 * @return
 */
    virtual const char *what() const throw() = 0;
};

/**
 *
 */
class usageException : public genException
{
private:
public:
/**
 *
 */
    usageException() : genException()
    {};

/**
 *
 * @return
 */
    virtual const char *what() const throw()
    {
        return "Usage: FractalDrawer <file path>";
    }
};

/**
 *
 */
class invalidException : public genException
{
private:
public:
/**
 *
 */
    invalidException() : genException()
    {};

/**
 *
 * @return
 */
    virtual const char *what() const throw()
    {
        return "Invalid input";
    }
};

#endif //EX_2_FRACTAL_H
