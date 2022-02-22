#include "GField.h"
#include "GFNumber.h"
#include <cassert>
#include <string>

static const int DEFAULTL = 1;
static const int TWO = 2;
static const std::string GF = "GF(";
static const std::string AST = "***";
static const std::string CLOSE = ")";

/**
 * constrictor getting only p
 * @param p
 */
GField::GField(long p)
{
    p = std::abs(p);
    assert(isPrime(p));
    _p = p;
    _l = DEFAULTL;
}

/**
 * "full" constructor which gets p+l
 * @param p
 * @param l
 */
GField::GField(long p, long l)
{
    p = std::abs(p);
    assert(isPrime(p));
    assert(l > 0);
    _p = p;
    _l = l;
}

/**
 * copy constructor
 * @param old old gfield to copy
 */
GField::GField(const GField &old)
{
    _p = old._p;
    _l = old._l;
}

/**
 * algorithm to find is a number is prime - kinda translated it from python from targil in intro ;)
 * @param p a number p
 * @return whether p is prime
 */
bool GField::isPrime(long p)
{
    if (p < TWO)
    {
        return false;
    }
    if (p == TWO)
    {
        return true;
    }
    if (p % TWO == 0)
    {
        return false;
    }
    for (int i = 3; (i * i) <= p; i += TWO)
    {
        if (p % i == 0)
        {
            return false;
        }
    }
    return true;
}

/**
 *  took inspiration from http://www.borismod.net/2008/10/non-recursive-implementation-of.html
 * gcd algorithm to find the greatest common denominator
 * @param a first arg
 * @param b second arg
 * @return a and b -  greatest common denominator
 */
GFNumber GField::gcd(const GFNumber &a1, const GFNumber &b1) const
{
    assert(a1.getField() == b1.getField());
    assert(b1.getNumber() != 0 && a1.getNumber() != 0);
    long a = a1.getNumber();
    long b = b1.getNumber();
    // prime numbers
    if (a == 0 && b != 0)
    {
        return GFNumber(b, b1.getField());
    }
    if (a != 0 && b == 0)
    {
        return GFNumber(a, a1.getField());
    }
    if (a < b)
    {
        a += b;
        b = a - b;
        a -= b;
    }
    if (b == 0)
    {
        return GFNumber(a, a1.getField());
    }
    while (a % b != 0)
    {
        a += b;
        b = a - b;
        a -= b;
        b %= a;
    }
    return GFNumber(b, a1.getField());
}

/**
 * @param k long number
 * @return instance of GFNumber of this field with k as _n
 */
GFNumber GField::createNumber(long k) const
{
    GFNumber temp(k, *this);
    return temp;
}

/**
 * function to overload the operator =
 * will preform the operation on GField objects
 * @param gField the GField object
 * @return result of operation
 */
GField &GField::operator=(GField &gField)
{
    _l = gField._l;
    _p = gField._p;
    return *this;
}

/**
 * function to overload the operator =
 * will preform the operation on GField objects
 * @param gField the GField object
 * @return result of operation
 */
GField &GField::operator=(const GField &gField)
{
    _l = gField._l;
    _p = gField._p;
    return *this;
}

/**
 * will print the object according to the PDF
 * @param out output object
 * @param gField the object to print
 * @return out
 */
std::ostream &operator<<(std::ostream &out, const GField &gField)
{
    out << GF << gField.getChar() << AST << gField.getDegree() << CLOSE;
    return out;
}

/**
 * will take input of a GFNumber
 * @param in input object
 * @param gField the gField to take the input
 * @return in
 */
std::istream &operator>>(std::istream &in, GField &gField)
{
    long p = 0, l = 0;
    bool fail = !(in >> p >> l);
    assert(!fail);
    gField = GField(p, l);
    return in;
}

