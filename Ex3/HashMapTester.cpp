#include "HashMap.hpp"
#include <cassert>
#include <map>
#include <cstdlib>
#include <random>

void testDefConstructor()
{
    HashMap<int, int> myMap;
    assert(myMap.getLoadFactor() == 0);
    assert(myMap.capacity() == 16);
    assert(myMap.size() == 0);
    assert(myMap.empty());
    // checking that an assertion is thrown
    try
    {
        myMap.at(4);
        assert(0);
    }
    catch (...)
    {
    }
    std::cout << "passed test DefConstructor\n";
}

void testConstructorWithFactor()
{
    HashMap<int, int> myMap(0.25, 0.5);
    assert(myMap.getLoadFactor() == 0);
    assert(myMap.capacity() == 16);
    assert(myMap.size() == 0);
    assert(myMap.empty());
    for (int i = 0; i < 8; ++i)
    {
        myMap.insert(i, i);
    }
    assert(myMap.getLoadFactor() == 0.5);
    assert(myMap.capacity() == 16);
    assert(myMap.size() == 8);
    myMap.insert(9, 9);
    assert(myMap.getLoadFactor() == 0.28125);
    assert(myMap.capacity() == 32);
    assert(myMap.size() == 9);
    myMap.erase(0);
    myMap.erase(1);
    assert(myMap.size() == 7);
    assert(myMap.capacity() == 16);
    std::cout << "passed test ConstructorWithFactor\n";
}

void testConstructorVectors()
{
    std::vector<char> keysVec{'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm'};
    std::vector<int> illegalValsVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    try
    {
        HashMap<char, int> myMap(keysVec, illegalValsVec);
        assert(0);
    }
    catch (...)
    {
    }
    std::vector<int> valsVec{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
    HashMap<char, int> myMap(keysVec, valsVec);
    // capacity should be increased because default upper load factor was crossed
    assert(myMap.capacity() == 32);
    assert(myMap.getLoadFactor() == 0.40625);
    assert(myMap.size() == 13);
    assert(!myMap.empty());

    // shouldn't throw an exception
    try
    {
        for (char c = 'a'; c <= 'm'; c++)
        {
            assert(myMap.at(c) == (int) c - 96);
        }
    }
    catch (...)
    {
    }
    std::cout << "passed testConstructorVectors\n";
}

void testCopyAndAssign()
{
    HashMap<int, int> myMap;
    HashMap<int, int> copy1 = myMap;
    HashMap<int, int> copy2(myMap);
    assert(copy1.getLoadFactor() == 0 && copy1.capacity() == 16 && copy1.size() == 0 &&
           copy1.empty());
    assert(copy2.getLoadFactor() == 0 && copy2.capacity() == 16 && copy2.size() == 0 &&
           copy2.empty());
    for (int i = 0; i < 193; ++i)
    {
        myMap.insert(i, i);
    }
    assert(myMap.getLoadFactor() == 0.376953125 && myMap.capacity() == 512 && myMap.size() == 193 &&
           !myMap.empty());
    assert(copy1.getLoadFactor() == 0 && copy1.capacity() == 16 && copy1.size() == 0 &&
           copy1.empty());
    assert(copy2.getLoadFactor() == 0 && copy2.capacity() == 16 && copy2.size() == 0 &&
           copy2.empty());
    HashMap<int, int> copy3 = myMap;
    HashMap<int, int> copy4(myMap);
    assert(myMap.getLoadFactor() == 0.376953125 && myMap.capacity() == 512 && myMap.size() == 193 &&
           !myMap.empty());
    assert(copy3.getLoadFactor() == 0.376953125 && copy3.capacity() == 512 && copy3.size() == 193 &&
           !copy3.empty());
    assert(copy4.getLoadFactor() == 0.376953125 && copy4.capacity() == 512 && copy4.size() == 193 &&
           !copy4.empty());
    for (int i = 0; i < 66; ++i)
    {
        copy3.erase(i);
    }
    assert(copy3.getLoadFactor() == 0.49609375 && copy3.capacity() == 256 && copy3.size() == 127 &&
           !copy3.empty());
    assert(myMap.getLoadFactor() == 0.376953125 && myMap.capacity() == 512 && myMap.size() == 193 &&
           !myMap.empty());
    assert(copy4.getLoadFactor() == 0.376953125 && copy4.capacity() == 512 && copy4.size() == 193 &&
           !copy4.empty());
    std::cout << "passed testCopyAndAssign\n";
}

/**
 * at the moment ignores const
 */
void testIterator()
{
    HashMap<int, int> myMap;
    for (int i = 0; i < 100; ++i)
    {
        myMap.insert(i, i + 1);
    }
    // checks the postfix
    int count = 0;
    for (HashMap<int, int>::const_iterator it = myMap.begin(); it != myMap.end(); it++)
    {
        count += 1;
        assert((*it).first + 1 == (*it).second);
    }
    assert(count == 100);
    // checks the prefix
    count = 0;
    for (HashMap<int, int>::const_iterator it = myMap.begin(); it != myMap.end(); ++it)
    {
        count += 1;
        assert((*it).first + 1 == (*it).second);
    }
    assert(count == 100);
    // checks the for-each
    count = 0;
    for (std::pair<int, int> val : myMap)
    {
        count += 1;
        assert(val.first + 1 == val.second);
    }
    assert(count == 100);
    std::cout << "passed testIterator\n";
}

// makes sure that the copy is deep and therefore clear doesn't hurt the copy
void testClearAndCopy()
{
    HashMap<int, int> myMap;

    for (int i = 0; i < 13; ++i)
    {
        myMap.insert(i, i);
    }
    HashMap<int, int> copyOfMap = myMap;
    assert(myMap.capacity() == 32);
    assert(myMap.size() == 13);
    assert(myMap.at(2) == 2);
    myMap.clear();
    //capacity should stay the same
    assert(myMap.capacity() == 32);
    assert(myMap.size() == 0);
    //should throw an exception
    try
    {
        std::cout << myMap.at(2) << std::endl;
        assert(0);
    }
    catch (...)
    {
    }
    //copy shouldn't be affected
    assert(copyOfMap.capacity() == 32);
    assert(copyOfMap.size() == 13);
    assert(copyOfMap.at(2) == 2);
    // shouldn't throw an exception
    try
    {
        assert(copyOfMap.at(2) == 2);
    }
    catch (...)
    {
        assert(0);
    }
    //should contain all
    for (int i = 0; i < 13; ++i)
    {
        assert(copyOfMap.containsKey(i));
    }
    std::cout << "passed testClearAndCopy\n";
}

void testBucketSize()
{
    HashMap<std::string, int> myMap;
    myMap.insert("aacagj", 1);
    myMap.insert("aacag", 2);
    myMap.insert("aacagjl1220", 2);
    assert(myMap.bucketSize("aacagj") == 3);
    std::cout << "passed testBucketSize\n";
}

void testEraseEasyDefaultFactors()
{
    HashMap<int, int> myMap;
    for (int i = 0; i < 13; ++i)
    {
        myMap.insert(i, i + 1);
    }
    assert(myMap.size() == 13);
    assert(myMap.capacity() == 32);
    // removing 5 elements, shouldn't resize the map (because 8 isn't < 0.25 * 32)
    for (int i = 0; i < 5; ++i)
    {
        myMap.erase(i);
    }
    assert(myMap.size() == 8);
    assert(myMap.capacity() == 32);
    for (int i = 0; i < 5; ++i)
    {
        assert(!myMap.containsKey(i));
    }
    // in the next removal there should be resizing
    myMap.erase(5);
    assert(myMap.size() == 7);
    assert(myMap.capacity() == 16);
    // removing 3 more elements, shouldn't resize the map (because 4 isn't < 0.25 * 16)
    for (int i = 6; i < 9; ++i)
    {
        myMap.erase(i);
    }
    assert(myMap.size() == 4);
    assert(myMap.capacity() == 16);
    for (int i = 6; i < 9; ++i)
    {
        assert(!myMap.containsKey(i));
    }
    // in the next removal there should be resizing
    myMap.erase(9);
    assert(myMap.size() == 3);
    assert(myMap.capacity() == 8);
    std::cout << "passed testEraseEasyDefaultFactors\n";
}

void testInsertAndEraseRandom()
{
    std::map<int, int> stdMap;
    HashMap<int, int> myMap;
    int r;
    for (int i = 0; i < 100; ++i)
    {
        r = rand() % 200;
        stdMap[r] = 2 * r;
        myMap[r] = 2 * r;
    }

    assert(myMap.size() == stdMap.size());
    int countStd = 0;
    int countMyMap = 0;
    // now checking that all elements are identical
    try
    {
        for (std::pair<int, int> val : stdMap)
        {
            countStd++;
            assert(myMap.at(val.first) == val.second);
        }

        for (std::pair<int, int> val : myMap)
        {
            countMyMap++;
            assert(stdMap.at(val.first) == val.second);
        }
    }
    catch (...)
    {
        assert(false);
    }
    assert(countMyMap == countStd);

    //now erasing some random
    for (int i = 0; i < 20; ++i)
    {
        r = rand() % 200;
        stdMap.erase(r);
        myMap.erase(r);
    }
    assert(myMap.size() == stdMap.size());
    // counters to make sure that the number of iterations is the same
    countStd = 0;
    countMyMap = 0;
    // and checking that all elements are identical
    try
    {
        for (std::pair<int, int> val : stdMap)
        {
            countStd++;
            assert(myMap.at(val.first) == val.second);
        }

        for (std::pair<int, int> val : myMap)
        {
            countMyMap++;
            assert(stdMap.at(val.first) == val.second);
        }
    }
    catch (...)
    {
        assert(false);
    }
    assert(countMyMap == countStd);
    std::cout << "passed testInsertAndEraseRandom\n";
}

void testBracketsOperator()
{
    HashMap<char, int> myMap;
    myMap.insert('a', 1);
    assert(myMap['a'] == 1);
    myMap['b'] = 2;
    assert(myMap['b'] == 2);
    myMap['b'] = 3;
    assert(myMap['b'] == 3);
    std::cout << "passed testBracketsOperator\n";
}

void testEqualityOperators()
{
    HashMap<char, int> myMap1;
    HashMap<char, int> myMap2;
    for (int i = 0; i < 100; ++i)
    {
        myMap1.insert(i, i);
        myMap2.insert(i, i);
    }
    assert(myMap1 == myMap2);
    myMap2.insert(100, 100);
    assert(myMap1 != myMap2);
    myMap2.erase(100);
    myMap1.insert(100, 100);
    assert(myMap1 != myMap2);
    myMap1.erase(100);

    // equal again, and now checking situation when keys equal and values not
    myMap1.insert(100, 101);
    myMap2.insert(100, 100);
    assert(myMap1 != myMap2);

    // according to the forum, they have to be equal according to all parameters
    // therefore checking maps with same elements but different parameters
    HashMap<char, int> myMap3;
    HashMap<char, int> myMap4(0.2, 0.8);
    myMap3.insert('a', 1);
    myMap4.insert('a', 1);
    assert(myMap1 != myMap2);
    // now checking different capacities
    HashMap<char, int> myMap5;
    HashMap<char, int> myMap6;
    myMap5.insert('a', 1);
    myMap5.erase('a');
    // not myMap5 capacity decreases to 8
    assert(myMap5 != myMap6);
    std::cout << "passed testEqualityOperators\n";
}

void insertionTest()
{
    HashMap<int, int> myMap;
    for (int i = 0; i < 300; ++i)
    {
        myMap[i] = i + 1;
    }

    for (int i = 0; i < 300; ++i)
    {
        myMap[i] = 2 * i;
    }
    assert(myMap.size() == 300);
    std::cout << "passed insertionTest\n";
}

void testEraseContent()
{
    HashMap<int, int> myMap;
    for (int i = 0; i < 100; ++i)
    {
        myMap[i] = 2 * i;
    }
    for (int i = 0; i < 30; ++i)
    {
        myMap.erase(i);
    }

    assert(myMap.size() == 70);
    int count = 0;
    for (std::pair<int, int> val : myMap)
    {
        count++;
    }
    assert(count == 70);
    std::cout << "passed testEraseContent\n";
}

int main()
{
    testDefConstructor();
    testConstructorWithFactor();
    testConstructorVectors();
    testCopyAndAssign();
    testIterator();
    testClearAndCopy();
    testBucketSize();
    testEraseContent();
    testEraseEasyDefaultFactors();
    testBracketsOperator();
    testEqualityOperators();
    insertionTest();
    testInsertAndEraseRandom();
    std::cout << '\n';
    std::cout << "good job!! you passed all tests!\n";
    return 0;
}