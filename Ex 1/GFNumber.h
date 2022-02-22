//
// Created by guylu on 26/08/2019.
//

#ifndef EX_1_GFNUMBER_H
#define EX_1_GFNUMBER_H


#include "GField.h"
#include <iostream>

/**
 * class to represent a number in a galua field;
 */
class GFNumber
{
private:
    // the num
    long _n;
    // the field
    GField _gField;

/**
 * simple Polynomial function of x^2+1
 * @param x param x
 * @return result of function
 */
    GFNumber _simplyPoly(const GFNumber &x) const
    { return (x * x) + 1; };

/**
 * Polard algorithm as described in the PDF
 * also called Rho
 * @param num number to divide into prime factors
 * @return a prime factor, or -1 if failed
 */
    long _polardsAlgo(const GFNumber &num) const;

/**
 * find a prime factor of a number with a brute force way
  * @param num number to divide into prime factors
 * @return a prime factor
 */
    long _bruteForce(const long &num) const;

/**
 * will preform abs func on 2 GFNumber objects difference
 * @param x GFNumber object 1
 * @param y GFNumber object 2
 * @return their abs diff
 */
    GFNumber _myAbs(const GFNumber &x, const GFNumber &y) const
    { return x > y ? (x - y) : (y - x); };

/**
 * will enter a new factor into the list of prime factors, will increase the index counter, and if
 * necessary will reallocate the array into a bigger array
 * @param factors list of prime factors
 * @param num the num to factor
 * @param index counter of array
 * @param factor the factor to add
 * @param len len of array
 */
    void _setNewFactor(GFNumber **factors, GFNumber &num, int &index, const long &factor, int &len)
    const;

/**
 * copy array of GFNumber objects into another one
 * @param newFactors new array
 * @param oldFactors old array
 * @param len len of old array
 */
    void _copyArrOfFactors(GFNumber *newFactors, const GFNumber *oldFactors, long len) const;


public:
/**
 * default constructor
 */
    GFNumber() : _n(0), _gField(2, 1)
    {};

/**
 * constructor of only giving number
 * (the formula is to mod the number such that it is compatible with the GField)
 * @param n long num to initialize thr object
 */
    GFNumber(const long &n) : _gField(2, 1)
    { _n = (_gField.getOrder() + (n % _gField.getOrder())) % _gField.getOrder(); };

/**
 * "full" constructor giving the object all the info - num+ field
 * @param n num
 * @param gField galua field
 */
    GFNumber(const long &n, const GField &gField) : _gField(gField)
    {
        _n = (_gField.getOrder() + (n % _gField.getOrder())) % _gField.getOrder();
    };

/**
 * copy constructor
 * @param old another gfnumber
 */
    GFNumber(const GFNumber &old);

/**
 * destructor
 */
    ~GFNumber()
    {};

/**
 * @return returns the _n
 */
    long getNumber() const
    { return _n; };

/**
 * @return the gfield
 */
    GField getField() const
    { return _gField; };

/**
 * will calculate a list of prime factor of a GFNumber
 * @param len len of list
 * @return array of GFNumbers which are prime factors of this
 */
    GFNumber *getPrimeFactors(int *len) const;

/**
 * will print the prime factors of this
 */
    void printFactors() const;

/**
 * @return whether this is prime
 */
    bool getIsPrime() const
    { return GField::isPrime(_n); };

/**
 * function to overload the operator =
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    GFNumber &operator=(const GFNumber &gfNumber);

/**
 * function to overload the operator +
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    GFNumber operator+(const GFNumber &gfNumber);

/**
 * function to overload the operator + - of const
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    GFNumber operator+(const GFNumber &gfNumber) const;

/**
 * function to overload the operator +=
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    GFNumber &operator+=(const GFNumber &gfNumber);

/**
 * function to overload the operator -
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    GFNumber operator-(const GFNumber &gfNumber);

/**
 * function to overload the operator =
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    GFNumber operator-(const GFNumber &gfNumber) const;

/**
 * function to overload the operator -=
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    GFNumber &operator-=(const GFNumber &gfNumber);

/**
 * function to overload the operator *
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    GFNumber operator*(const GFNumber &gfNumber);

/**
 * function to overload the operator * - of const
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    GFNumber operator*(const GFNumber &gfNumber) const;

/**
 * function to overload the operator *=
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    GFNumber &operator*=(const GFNumber &gfNumber);

/**
 * function to overload the operator %
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    GFNumber operator%(const GFNumber &gfNumber);


/**
 * function to overload the operator % - const
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    GFNumber operator%(const GFNumber &gfNumber) const;

/**
 * function to overload the operator %=
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    GFNumber &operator%=(const GFNumber &gfNumber);

/**
 * function to overload the operator !=
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    bool operator!=(const GFNumber &gfNumber)
    { return (this->_gField != gfNumber._gField || this->_n != gfNumber._n); };

/**
 * function to overload the operator ==
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    bool operator==(const GFNumber &gfNumber)
    { return (this->_gField == gfNumber._gField && this->_n == gfNumber._n); };

/**
 * function to overload the operator <
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    bool operator<(const GFNumber &gfNumber);

/**
 * function to overload the operator <=
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    bool operator<=(const GFNumber &gfNumber);

/**
 * function to overload the operator >
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    bool operator>(const GFNumber &gfNumber);

/**
 * function to overload the operator > - of const
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    bool operator>(const GFNumber &gfNumber) const;

/**
 * function to overload the operator >=
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
    bool operator>=(const GFNumber &gfNumber);

/**
 * function to overload the operator =
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
    GFNumber &operator=(const long &l);

/**
 * function to overload the operator +
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
    GFNumber operator+(const long &l);


/**
 * function to overload the operator + const
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
    GFNumber operator+(const long &l) const;

/**
 * function to overload the operator +=
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
    GFNumber &operator+=(const long &l);

/**
 * function to overload the operator -
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
    GFNumber operator-(const long &l);

/**
 * function to overload the operator -  const
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
    GFNumber operator-(const long &l) const;

/**
 * function to overload the operator -=
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
    GFNumber &operator-=(const long &l);

/**
 * function to overload the operator *
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
    GFNumber operator*(const long &l);


/**
 * function to overload the operator * - const
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
    GFNumber operator*(const long &l) const;

/**
 * function to overload the operator *=
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
    GFNumber &operator*=(const long &l);

/**
 * function to overload the operator %
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
    GFNumber operator%(const long &l);

/**
 * function to overload the operator % - const
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
    GFNumber operator%(const long &l) const;

/**
 * function to overload the operator %=
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
    GFNumber &operator%=(const long &l)
    { return *this %= _gField.createNumber(l); };

/**
 * will print the object according to the PDF
 * @param out output object
 * @param gfNumber the object to print
 * @return out
 */
    friend std::ostream &operator<<(std::ostream &out, const GFNumber &gfNumber);

/**
 * will take input of a GFNumber
 * @param in input object
 * @param gfNumber the gfNumber to take the input
 * @return in
 */
    friend std::istream &operator>>(std::istream &in, GFNumber &gfNumber);
};

#endif //EX_1_GFNUMBER_H
