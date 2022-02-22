#include <iostream>
#include "Fractal.h"
#include "cmath"
#include <fstream>
#include <string>
#include "vector"
#include <cstdlib>
#include "cassert"

using std::vector;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::pair;

static const int FIRSTVAR = 0;
static const int SECONDVAR = 1;
static const int THIRDVAR = 2;

static const int FBOUNDRY1 = 1;
static const int FBOUNDRY2 = 3;
static const int DIMBOUNDRY1 = 1;
static const int DIMBOUNDRY2 = 6;
static const int CARPET = 1;
static const int SIEVE = 2;
static const int DUST = 3;
static const int REQVARNUM = 2;


static const char COMA = ',';
static const char ZERO = '0';


/**
 * function to validate and parse the input
 * @param line the line to validate in the file
 * @param a will hold the first param in the line
 * @param b will hold the second param in the line
 * @return whether the line is valid
 */
void validateLine(string line, int &a, int &b)
{
    // because we know that the first argument and the second argument
    // can only be single digits long(such is instructed in the PDF)
    // we can assume that the template of the input is X,Y:

    if (!((line.length() == 4 && line[3] == '\r') || line.length() == 3))
    {
        throw invalidException();
    }
    // which is also why we can know the middle char is a ','
    // so we can just check that
    if (line[SECONDVAR] != COMA)
    {
        throw invalidException();
    }
    // update the vars:
    a = line[FIRSTVAR] - ZERO;
    b = line[THIRDVAR] - ZERO;
    // check they are within boundaries
    if (a < FBOUNDRY1 || a > FBOUNDRY2 || b < DIMBOUNDRY1 || b > DIMBOUNDRY2)
    {
        throw invalidException();
    }
}

/**
 * will manufacture a fractal object according to the number
 * @param type type of fractal
 * @param dim dim of fractal
 * @return the new fractal
 */
Fractal *factory(int type, int dim)
{
    switch (type)
    {
        case CARPET:
            return new Carpet(dim);
        case SIEVE:
            return new Sieve(dim);
        case DUST:
            return new Dust(dim);
        default:
            // can never happen
            return new Fractal(dim);
    }
}

/**
 * will generate the vector of info regarding which vectors to generate
 * @param argv input file
 * @param error whether the files info is valid
 * @return generated vector
 */
vector<pair<int, int>> getVector(int argc, char *const *argv)
{
    if (argc != REQVARNUM)
    {
        throw usageException();
    }
    vector<pair<int, int>> lines;
    string line;
    int a, b;
    std::ifstream myfile(argv[SECONDVAR]);
    if (myfile.is_open())
    {
        while (getline(myfile, line))
        {
            try
            {
                validateLine(line, a, b);
                lines.emplace_back(a, b);
            }
            catch (const invalidException &e)
            {
                myfile.close();
                throw;
            }
        }
        myfile.close();
        return lines;
    }
    throw invalidException();
}

/**
 * will print the fractals in reverse order
 * @param lines the vector of info
 */
void printFractals(const vector<pair<int, int>> &lines)
{
    for (int i = (int) lines.size() - 1; i >= 0; --i)
    {
        // create the fractal shape
        Fractal *f = factory(lines[i].first, lines[i].second);
        // print the shape
        f->printShape();
        // after we are done with it, delete the shape
        delete f;
        // print a line to divide
        cout << endl;
    }
}

/**
 * main function to run program
 * @param argc num of args
 * @param argv program args
 * @return fail/success
 */
int main(int argc, char *argv[])
{
    vector<pair<int, int>> lines;
    try
    {
        lines = getVector(argc, argv);
    }
    catch (const genException &e)
    {
        cerr << e.what() << endl;
        return EXIT_FAILURE;
    }
    printFractals(lines);
    return EXIT_SUCCESS;
}

