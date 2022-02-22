//
// Created by guylu on 26/08/2019.
//

#ifndef EX_1_GFIELD_H
#define EX_1_GFIELD_H


#include <iostream>
#include <iosfwd>
#include <cmath>

class GFNumber;

/**
 * class to represent a galua field
 */
class GField
{
private:
    long _p;
    long _l;

public:
/**
 * default constructor
 */
    explicit GField() : _p(2), _l(1)
    {};

/**
 * constrictor getting only p
 * @param p
 */
    explicit GField(long p);

/**
 * "full" constructor which gets p+l
 * @param p
 * @param l
 */
    GField(long p, long l);

/**
 * copy constructor
 * @param old old gfield to copy
 */
    GField(const GField &old);

/**
 * destructor
 */
    ~GField()
    {};

/**
 * @return _p
 */
    long getChar() const
    { return _p; };

/**
 * @return _l
 */
    long getDegree() const
    { return _l; };

/**
 * @return order of field - p^l
 */
    long getOrder() const
    { return (long) pow(_p, _l); };

/**
 * algorithm to find is a number is prime - kinda translated it from python from targil in intro ;)
 * @param p a number p
 * @return whether p is prime
 */
    static bool isPrime(long p);

/**
 *  took inspiration from http://www.borismod.net/2008/10/non-recursive-implementation-of.html
 * gcd algorithm to find the greatest common denominator
 * @param a first arg
 * @param b second arg
 * @return a and b -  greatest common denominator
 */
    GFNumber gcd(const GFNumber &a, const GFNumber &b) const;

/**
 * @param k long number
 * @return instance of GFNumber of this field with k as _n
 */
    GFNumber createNumber(long k) const;

/**
 * function to overload the operator =
 * will preform the operation on GField objects
 * @param gfNumber the GField object
 * @return result of operation
 */
    GField &operator=(GField &gField);

/**
 * function to overload the operator = of const
 * will preform the operation on GField objects
 * @param gfNumber the GField object
 * @return result of operation
 */
    GField &operator=(const GField &gField);

/**
 * function to overload the operator ==
 * will preform the operation on GField objects
 * @param gField the GField object
 * @return result of operation
 */
    bool operator==(const GField &gField) const
    { return (this->_l == gField._l && this->_p == gField._p); };

/**
 * function to overload the operator !=
 * will preform the operation on GField objects
 * @param gField the GField object
 * @return result of operation
 */
    bool operator!=(const GField &gField) const
    { return (this->_l != gField._l || this->_p != gField._p); };

/**
 * will print the object according to the PDF
 * @param out output object
 * @param gField the object to print
 * @return out
 */
    friend std::ostream &operator<<(std::ostream &out, const GField &gField);

/**
 * will take input of a GFNumber
 * @param in input object
 * @param gField the gField to take the input
 * @return in
 */
    friend std::istream &operator>>(std::istream &in, GField &gField);
};


#endif //EX_1_GFIELD_H
