#include "GFNumber.h"
#include "GField.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <random>
#include <string>

static const int SRTLEN = 2;
static const int EVEN = 2;
static const int FAIL = -1;
static const int INCSIZEFACTOR = 2;
static const std::string EQ = "=";
static const std::string ONE = "*1";
static const std::string AST = "*";

/**
 * copy constructor
 * @param old another gfnumber
 */
GFNumber::GFNumber(const GFNumber &old)
{
    _n = old._n;
    _gField = old._gField;
}

/**
 * will calculate a list of prime factor of a GFNumber
 * the responsibility of the caller to delete the array that has been returned!!!
 * @param len len of list
 * @return array of GFNumbers which are prime factors of this
 */
GFNumber *GFNumber::getPrimeFactors(int *index) const
{
    GFNumber num(*this);
    int len = SRTLEN;
    auto *factors = new GFNumber[len];
    *index = 0;
    // if one of those then the list of prime factors is empty
    if (num._n == 0 || num._n == 1 || getIsPrime())
    {
        return factors;
    }
    // take out all 2 as possible
    while (num._n % EVEN == 0)
    {
        _setNewFactor(&factors, num, *index, EVEN, len);
    }
    if (num._n == 1)
    {
        return factors;
    }
    long factor;
    while (true)
    {
        while ((factor = _polardsAlgo(num)) != FAIL)
        {
            _setNewFactor(&factors, num, *index, factor, len);
        }
        if (num._n == 1)
        {
            return factors;
        }
        long i = _bruteForce(num._n);
        _setNewFactor(&factors, num, *index, i, len);
        if (num._n == 1)
        {
            return factors;
        }
    }
}

/**
 * Polards algorithm as described in the PDF
 * also called Rho
 * @param num number to divide into prime factors
 * @return a prime factor, or -1 if failed
 */
long GFNumber::_polardsAlgo(const GFNumber &num) const
{
    if (num._n == 1)
    {
        return FAIL;
    }
    if (GField::isPrime(num._n))
    {
        return num._n;
    }
    // how to generate random link i followed:
    // https://stackoverflow.com/questions/5008804/generating-random-integer-from-a-range
    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<int> uni(1, num._n - 1);

    auto temp = uni(rng);
    GFNumber x(temp, num.getField());
    GFNumber y = x;
    GFNumber p(1, num.getField());
    while (p._n == 1)
    {
        x = _simplyPoly(x);
        y = _simplyPoly((_simplyPoly(y)));
        if (_myAbs(x, y).getNumber() == 0)
        {
            continue;
        }
        p = _gField.gcd(_myAbs(x, y), num);
    }
    // if polard gave us a non prime factor, we shall iterate once more
    if (!GField::isPrime(p._n))
    {
        return _polardsAlgo(p._n);
    }
    return p == num ? FAIL : p._n;
}

/**
 * will enter a new factor into the list of prime factors, will increase the index counter, and if
 * necessary will reallocate the array into a bigger array
 * @param factors list of prime factors
 * @param num the num to factor
 * @param index counter of array
 * @param factor the factor to add
 * @param len len of array
 */
void GFNumber::_setNewFactor(GFNumber **factors, GFNumber &num, int &index, const long &factor,
                             int &len) const
{
    num._n /= factor;
    (*factors)[index] = GFNumber(factor, _gField);
    index++;
    if (index == len)
    {
        auto *newFactors = new GFNumber[len * INCSIZEFACTOR];
        _copyArrOfFactors(newFactors, *factors, len);
        len *= INCSIZEFACTOR;
        delete[] *factors;
        *factors = newFactors;
    }
}

/**
 * copy array of GFNumber objects into another one
 * @param newFactors new array
 * @param oldFactors old array
 * @param len len of old array
 */
void GFNumber::_copyArrOfFactors(GFNumber *newFactors, const GFNumber *oldFactors, long len) const
{
    for (long i = 0; i < len; ++i)
    {
        newFactors[i] = oldFactors[i];
    }
}

/**
 * find a prime factor of a number with a brute force way
  * @param num number to divide into prime factors
 * @return a prime factor
 */
long GFNumber::_bruteForce(const long &num) const
{
    long i = 2;
    while (i <= (long) floor(sqrt(num)))
    {
        if (num % i == 0)
        {
            return i;
        }
        else
        {
            i++;
        }
    }
    if (num > 1)
    {
        return num;
    }
    return FAIL;
}

/**
 * will print the prime factors of this
 */
void GFNumber::printFactors() const
{
    int len = 0;
    GFNumber *factors = getPrimeFactors(&len);
    if (len == 0)
    {
        std::cout << _n << EQ << _n << ONE << std::endl;
        delete[] factors;
        return;
    }

    std::string s = std::to_string(_n) + EQ;
    for (int i = 0; i < len - 1; ++i)
    {
        s += std::to_string(factors[i]._n);
        s += AST;
    }
    std::cout << s << factors[len - 1]._n << std::endl;

    delete[] factors;
}

/**
 * function to overload the operator =
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
GFNumber &GFNumber::operator=(const GFNumber &gfNumber)
{
    _gField = gfNumber._gField;
    _n = gfNumber._n;
    return *this;
}

/**
 * function to overload the operator +
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
GFNumber GFNumber::operator+(const GFNumber &gfNumber)
{
    assert(_gField == gfNumber._gField);
    long n = (_gField.getOrder() + ((_n + gfNumber._n) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * function to overload the operator + - of const
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
GFNumber GFNumber::operator+(const GFNumber &gfNumber) const
{
    assert(_gField == gfNumber._gField);
    long n = (_gField.getOrder() + ((_n + gfNumber._n) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * function to overload the operator +=
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
GFNumber &GFNumber::operator+=(const GFNumber &gfNumber)
{
    assert(_gField == gfNumber._gField);
    _n = (_gField.getOrder() + ((_n + gfNumber._n) % _gField.getOrder())) % _gField.getOrder();
    return *this;
}

/**
 * function to overload the operator -
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
GFNumber GFNumber::operator-(const GFNumber &gfNumber)
{
    assert(_gField == gfNumber._gField);
    long n = (_gField.getOrder() + ((_n - gfNumber._n) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * function to overload the operator - of const
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
GFNumber GFNumber::operator-(const GFNumber &gfNumber) const
{
    assert(_gField == gfNumber._gField);
    long n = (_gField.getOrder() + ((_n - gfNumber._n) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * function to overload the operator -=
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
GFNumber &GFNumber::operator-=(const GFNumber &gfNumber)
{
    assert(_gField == gfNumber._gField);
    _n = (_gField.getOrder() + ((_n - gfNumber._n) % _gField.getOrder())) % _gField.getOrder();
    return *this;
}

/**
 * function to overload the operator *
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
GFNumber GFNumber::operator*(const GFNumber &gfNumber)
{
    assert(_gField == gfNumber._gField);
    long n = (_gField.getOrder() + ((_n * gfNumber._n) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * function to overload the operator * - of const
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
GFNumber GFNumber::operator*(const GFNumber &gfNumber) const
{
    assert(_gField == gfNumber._gField);
    long n = (_gField.getOrder() + ((_n * gfNumber._n) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * function to overload the operator *=
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
GFNumber &GFNumber::operator*=(const GFNumber &gfNumber)
{
    assert(_gField == gfNumber._gField);
    _n = (_gField.getOrder() + ((_n * gfNumber._n) % _gField.getOrder())) % _gField.getOrder();
    return *this;
}

/**
 * function to overload the operator %
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
GFNumber GFNumber::operator%(const GFNumber &gfNumber)
{
    assert(_gField == gfNumber._gField);
    assert(gfNumber._n != 0);
    long n = (_gField.getOrder() + ((_n % gfNumber._n) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * function to overload the operator % - const
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
GFNumber GFNumber::operator%(const GFNumber &gfNumber) const
{
    assert(_gField == gfNumber._gField);
    assert(gfNumber._n != 0);
    long n = (_gField.getOrder() + ((_n % gfNumber._n) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * function to overload the operator %=
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
GFNumber &GFNumber::operator%=(const GFNumber &gfNumber)
{
    assert(_gField == gfNumber._gField);
    assert(gfNumber._n != 0);
    _n = (_gField.getOrder() + ((_n % gfNumber._n) % _gField.getOrder())) % _gField.getOrder();
    return *this;
}

/**
 * function to overload the operator <
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
bool GFNumber::operator<(const GFNumber &gfNumber)
{
    assert(_gField == gfNumber._gField);
    return _n < gfNumber._n;
}

/**
 * function to overload the operator <=
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
bool GFNumber::operator<=(const GFNumber &gfNumber)
{
    assert(_gField == gfNumber._gField);
    return _n <= gfNumber._n;
}

/**
 * function to overload the operator >
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
bool GFNumber::operator>(const GFNumber &gfNumber)
{
    assert(_gField == gfNumber._gField);
    return _n > gfNumber._n;
}

/**
 * function to overload the operator > - of const
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
bool GFNumber::operator>(const GFNumber &gfNumber) const
{
    assert(_gField == gfNumber._gField);
    return _n > gfNumber._n;
}

/**
 * function to overload the operator >=
 * will preform the operation on GFNumber objects
 * @param gfNumber the GFNumber object
 * @return result of operation
 */
bool GFNumber::operator>=(const GFNumber &gfNumber)
{
    assert(_gField == gfNumber._gField);
    return _n >= gfNumber._n;

}

//with longs:

/**
 * function to overload the operator =
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
GFNumber &GFNumber::operator=(const long &l)
{
    _n = l;
    return *this;
}

/**
 * function to overload the operator +
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
GFNumber GFNumber::operator+(const long &l)
{
    long n = (_gField.getOrder() + ((_n + l) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * function to overload the operator +
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
GFNumber GFNumber::operator+(const long &l) const
{
    long n = (_gField.getOrder() + ((_n + l) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * function to overload the operator +=
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
GFNumber &GFNumber::operator+=(const long &l)
{
    _n = (_gField.getOrder() + ((_n + l) % _gField.getOrder())) % _gField.getOrder();
    return *this;
}

/**
 * function to overload the operator -
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
GFNumber GFNumber::operator-(const long &l)
{
    long n = (_gField.getOrder() + ((_n - l) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * function to overload the operator -  const
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
GFNumber GFNumber::operator-(const long &l) const
{
    long n = (_gField.getOrder() + ((_n - l) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * function to overload the operator -=
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
GFNumber &GFNumber::operator-=(const long &l)
{
    _n = (_gField.getOrder() + ((_n - l) % _gField.getOrder())) % _gField.getOrder();
    return *this;
}

/**
 * function to overload the operator *
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
GFNumber GFNumber::operator*(const long &l)
{
    long n = (_gField.getOrder() + ((_n * l) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}


/**
 * function to overload the operator * - const
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
GFNumber GFNumber::operator*(const long &l) const
{
    long n = (_gField.getOrder() + ((_n * l) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * function to overload the operator *=
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
GFNumber &GFNumber::operator*=(const long &l)
{
    _n = (_gField.getOrder() + ((_n * l) % _gField.getOrder())) % _gField.getOrder();
    return *this;
}

/**
 * function to overload the operator %
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
GFNumber GFNumber::operator%(const long &l)
{
    assert(l != 0);
    long n = (_gField.getOrder() + ((_n % l) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}


/**
 * function to overload the operator % - const
 * will preform the operation on GFNumber objects
 * @param l the long number to preform operation on
 * @return result of operation
 */
GFNumber GFNumber::operator%(const long &l) const
{
    assert(l != 0);
    long n = (_gField.getOrder() + ((_n % l) % _gField.getOrder())) % _gField.getOrder();
    return GFNumber(n, _gField);
}

/**
 * will print the object according to the PDF
 * @param out output object
 * @param gfNumber the object to print
 * @return out
 */
std::ostream &operator<<(std::ostream &out, const GFNumber &gfNumber)
{
    out << gfNumber.getNumber() << " GF(" << (gfNumber.getField()).getChar() << "**"
        << (gfNumber.getField()).getDegree() << ")";
    return out;
}

/**
 * will take input of a GFNumber
 * @param in input object
 * @param gfNumber the gfNumber to take the input
 * @return in
 */
std::istream &operator>>(std::istream &in, GFNumber &gfNumber)
{
    long n = 0;
    GField g;
    bool fail = !(in >> n >> g);
    assert(!fail);
    gfNumber = GFNumber(n, g);
    return in;
}