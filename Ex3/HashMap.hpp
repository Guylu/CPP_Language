//
// Created by guylu on 10/09/2019.
//
#include <cmath>
#include "vector"
#include "cassert"
#include <exception>
#include <iostream>

#ifndef EX3_HASHMAP_HPP
#define EX3_HASHMAP_HPP

using std::vector;
using std::pair;
using std::string;

static const double LOWER = 0.25;
static const double UPPER = 0.75;
static const int DEFSIZE = 16;
static const int TABLEBASE = 2;
static const double RESIZEUP = 2;
static const double RESIZEDOWN = 0.5;


/**
 * generic exception for input problems
 */
class genException : public std::exception
{
private:
public:
/**
 * Ctor
 */
    genException() : exception()
    {};

/**
 * deleted
 * @return
 */
    virtual const char *what() const throw() = 0;
};

/**
 * usage exception
 */
class usageException : public genException
{
private:
public:
/**
 * Ctor
 */
    usageException() : genException()
    {};

/**
 * hold message to user
 * @return
 */
    virtual const char *what() const throw()
    {
        return "Usage: SpamDetector <database path> <message path> <threshold>";
    }
};


/**
 * invalid input exception
 */
class invalidException : public genException
{
private:
public:
/**
 * Ctor
 */
    invalidException() : genException()
    {};

/**
 * hold message to user
 * @return
 */
    virtual const char *what() const throw()
    {
        return "Invalid input";
    }
};

/**
 * key not found in at method
 */
class keyNotFound : public std::exception
{
private:
public:
/**
 * Ctor
 */
    keyNotFound() : exception()
    {};

/**
 * hold message to user
 * @return
 */
    virtual const char *what() const throw()
    {
        return "Invalid input";
    };
};


/**
 * Ctor exception when vectors are different sizes
 */
class diffLenVectors : public std::exception
{
private:
public:
/**
 * Ctor
 */
    diffLenVectors() : exception()
    {};

/**
 * hold message to user
 * @return
 */
    virtual const char *what() const throw()
    {
        return "Invalid input";
    };
};


/**
 * key not dound in at method
 */
class invalidBucket : public std::exception
{
private:
public:
/**
 * Ctor
 */
    invalidBucket() : exception()
    {};

/**
 * hold message to user
 * @return
 */
    virtual const char *what() const throw()
    {
        return "Invalid bucket";
    };
};

/**
 * invalid lower or upper
 */
class invalidInitializors : public std::exception
{
private:
public:
/**
 * Ctor
 */
    invalidInitializors() : exception()
    {};

/**
 * hold message to user
 * @return
 */
    virtual const char *what() const throw()
    {
        return "Invalid initializors";
    };
};

/**
 * capacity is 0
 */
class noCapacity : public std::exception
{
private:
public:
/**
 * Ctor
 */
    noCapacity() : exception()
    {};

/**
 * hold message to user
 * @return
 */
    virtual const char *what() const throw()
    {
        return "no Capacity";
    };
};

/**
 * class to represent a hash map
 * @tparam KeyT type of key
 * @tparam ValueT type of val
 */
template<class KeyT, class ValueT>
class HashMap
{
    double _lowerLoad;
    double _upperrLoad;
    int _sizeOfMap;
    int _capacityOfMap;
    // the _map data structure
    vector<pair<KeyT, ValueT>> *_map;

/**
 * function that will calculate hash of the key
 * @param key key to hash
 * @return hash val of key
 */
    int _hashFunction(KeyT key) const;

/**
 * function to rehash the map to  increase the capacity by a factor
 * @param factor foctor to increase capacity by
 */
    void _reHash(double factor);

/**
 * iterator of the hashmap
 */
    class _Iterator
    {
    public:
/**
 * Ctor to the first element
 * @param myMap map to iterate
 * @param size size of map
 */
        _Iterator(vector<pair<KeyT, ValueT>> *myMap, int size) noexcept :
                _size(size), _arrayIndex(0), _vectorIndex(0)
        {
            _myMap = myMap;
            if (myMap[_arrayIndex].size() == 0)
            {
                ++(*this);
            }
        }

/**
 * Ctor for iterator pointing at acertain location in the map
 * @param myMap map to iterate
 * @param size size of map
 * @param indarr index in the array
 * @param indvec index in the vector
 */
        _Iterator(vector<pair<KeyT, ValueT>> *myMap, int size, int indarr, int indvec) noexcept :
                _myMap(myMap), _size(size), _arrayIndex(indarr), _vectorIndex(indvec)
        {}

/**
 * copy Ctor
 * @param iterator another iterator
 */
        _Iterator(const _Iterator &iterator) noexcept
        {
            _arrayIndex = iterator._arrayIndex;
            _vectorIndex = iterator._vectorIndex;
            _myMap = iterator._myMap;
            _size = iterator._size;
        }

/**
 * operator ++ pre
 * @return
 */
        _Iterator &operator++()
        {
            int arrLen = _size;
            int vecLen = _myMap[_arrayIndex].size();
            do
            {
                _vectorIndex++;
                if (_vectorIndex >= vecLen)
                {
                    _vectorIndex = 0;
                    _arrayIndex++;
                    if (_arrayIndex >= arrLen + 1)
                    {
                        _arrayIndex = arrLen;
                        return *this;
                    }
                }

            } while ((int) _myMap[_arrayIndex].size() <= _vectorIndex);
            return *this;
        }

/**
 * operator++ post
 * @return
 */
        const _Iterator operator++(int)
        {
            _Iterator iterator = *this;
            ++*this;
            return iterator;
        }

/**
 * operator =
 * @return
 */
        _Iterator &operator=(const _Iterator &iterator)
        {
            _arrayIndex = iterator._arrayIndex;
            _vectorIndex = iterator._vectorIndex;
            _myMap = iterator._myMap;
            _size = iterator._size;
            return *this;
        }


        /**
 * operator ==
 * @param iterator
 * @return
 */
        bool operator==(const _Iterator &iterator)
        {
            return _vectorIndex == iterator._vectorIndex && _arrayIndex == iterator._arrayIndex;
        }

/**
 * operator !=
 * @param iterator
 * @return
 */
        bool operator!=(const _Iterator &iterator)
        {
            return !(*this == iterator);
        }

/**
 * operator dereference
 * @return
 */
        const pair<KeyT, ValueT> &operator*()
        {
            return _myMap[_arrayIndex][_vectorIndex];
        }

/**
 * operator ->
 * @return
 */
        const pair<KeyT, ValueT> *operator->()
        {
            return &_myMap[_arrayIndex][_vectorIndex];
        }

    private:
        vector<pair<KeyT, ValueT>> *_myMap;
        int _size;
        int _arrayIndex;
        int _vectorIndex;
    };

public:
    // our typedef of iterator
    using iterator = _Iterator;
    using const_iterator = _Iterator;

    /**
     *
     */
    void print();

/**
 * default Ctor
 */
    HashMap() : _lowerLoad(LOWER), _upperrLoad(UPPER), _sizeOfMap(0),
                _capacityOfMap(DEFSIZE)
    { _map = new vector<pair<KeyT, ValueT>>[_capacityOfMap]; };

/**
 * Ctor
 * @param lower lower load
 * @param upper upper load
 */
    HashMap(double lower, double upper);

/**
 * Ctor given keys and vals
 * @param keys keys to initialize
 * @param vals vals to keys
 */
    HashMap(vector<KeyT> keys, vector<ValueT> vals);

/**
 * copy Ctor
 * @param hashMap
 */
    HashMap(const HashMap &hashMap) noexcept;

/**
 * move Ctor
 * @param hashMap
 */
    HashMap(HashMap && hashMap) noexcept;

/**
 * destructor
 */
    ~HashMap()
    { delete[] _map; };

/**
 * @return num of elements in map
 */
    int size() const
    { return _sizeOfMap; };

/**
 * @return capacity of map
 */
    int capacity() const
    { return _capacityOfMap; };

/**
 * @return load factor
 */
    double getLoadFactor() const
    { return (double) _sizeOfMap / (double) _capacityOfMap; };

/**
 * @return is the map empty
 */
    bool empty() const
    { return _sizeOfMap == 0; };

/**
 * will insert a new key and val
 * @param key key
 * @param val val
 * @return was the insert successful
 */
    bool insert(KeyT key, ValueT val);

/**
 * @param key key to find in map
 * @return whether the key found
 */
    bool containsKey(KeyT key) const;

/**
 * @param key key
 * @return val of key
 */
    const ValueT &at(KeyT key);

/**
 * erase a key from map
 * @param key key
 * @return whether the deletion was successful
 */
    bool erase(KeyT key);

/**
 * @param key key
 * @return length of vector the key is held in
 */
    int bucketSize(KeyT key) const;

/**
 * remove all elements from the map
 */
    void clear();

/**
 * operator= const
 * @param hashMap
 * @return
 */
    HashMap &operator=(const HashMap &hashMap);

/**
 * operator subscript
 * @param key
 * @return
 */
    ValueT &operator[](const KeyT &key) noexcept;

/**
 * operator subscript const
 * @param key
 * @return
 */
    ValueT operator[](const KeyT &key) const noexcept;

/**
 *operator ==
 * @param hashMap
 * @return
 */
    bool operator==(const HashMap &hashMap);

/**
 * operator== const
 * @param hashMap
 * @return
 */
    bool operator==(const HashMap &hashMap) const;

/**
 * operator!=
 * @param hashMap
 * @return
 */
    bool operator!=(const HashMap &hashMap)
    { return !(*this == hashMap); };

/**
 * operator !=
 * @param hashMap
 * @return
 */
    bool operator!=(const HashMap &hashMap) const
    { return !(*this == hashMap); };

/**
 * @return iterator to first element in map
 */
    _Iterator begin() const
    {
        return _Iterator(_map, _capacityOfMap);
    }

/**
 * @return iterator to last element in map
 */
    _Iterator end() const
    {
        return _Iterator(_map, _capacityOfMap, _capacityOfMap, 0);
    }

/**
 * @return iterator to first element in map
 */
    _Iterator cbegin() const
    {
        return begin();
    }

/**
 * @return iterator to last element in map
 */
    _Iterator cend() const
    {
        return end();
    }

};

template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap(double lower, double upper) : _sizeOfMap(0), _capacityOfMap(DEFSIZE)
{
    //checking boundaries
    if (lower > upper || lower < 0 || upper > 1)
    {
        throw invalidInitializors();
    }
    _lowerLoad = lower;
    _upperrLoad = upper;
    _map = new vector<pair<KeyT, ValueT>>[_capacityOfMap];
}

/**
 * Ctor given keys and vals
 * @param keys keys to initialize
 * @param vals vals to keys
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap(vector<KeyT> keys, vector<ValueT> vals)
{
    if (keys.size() != vals.size())
    {
        throw diffLenVectors();
    }
    // check for dups in the lists
    _lowerLoad = LOWER;
    _upperrLoad = UPPER;
    int tempSize = pow(TABLEBASE, ceil(log(keys.size())) + 1);
    if (keys.size() > tempSize * _upperrLoad)
    {
        _capacityOfMap = tempSize * TABLEBASE;
    }
    else
    {
        _capacityOfMap = pow(TABLEBASE, ceil(log(keys.size())));
    }
    _map = new vector<pair<KeyT, ValueT>>[_capacityOfMap];
    std::hash<KeyT> hash;
    _sizeOfMap = 0;
    for (int i = 0; i < keys.size(); ++i)
    {
        (*this)[keys[i]] = vals[i];
    }
}

/**
 * copy Ctor
 * @param hashMap
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap(const HashMap &hashMap) noexcept:_sizeOfMap(hashMap._sizeOfMap),
                                                                _capacityOfMap(hashMap.capacity()),
                                                                _upperrLoad(hashMap._upperrLoad),
                                                                _lowerLoad(hashMap._lowerLoad)
{
    _map = new vector<pair<KeyT, ValueT>>[_capacityOfMap];
    for (int i = 0; i < _capacityOfMap; ++i)
    {
        int len = hashMap._map[i].size();
        for (int j = 0; j < len; ++j)
        {
            _map[i].push_back(pair<KeyT, ValueT>(hashMap._map[i][j]));
        }
    }
}

/**
 * move Ctor
 * @param hashMap
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT>::HashMap(HashMap && hashMap) noexcept
{
    _map = hashMap._map;
    hashMap._map = nullptr;
    hashMap._capacityOfMap = 0;
    _sizeOfMap = hashMap._sizeOfMap;
    _capacityOfMap = hashMap._capacityOfMap;
    _upperrLoad = hashMap._upperrLoad;
    _lowerLoad = hashMap._lowerLoad;
}


/**
 * will insert a new key and val
 * @param key key
 * @param val val
 * @return was the insert successful
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::insert(KeyT key, ValueT val)
{
    if (containsKey(key))
    {
        return false;
    }
    if (_sizeOfMap + 1 > _capacityOfMap * _upperrLoad)
    {
        _reHash(RESIZEUP);
    }
    _map[_hashFunction(key)].push_back(pair<KeyT, ValueT>(key, val));
    _sizeOfMap++;
    return true;
}

/**
 * @param key key to find in map
 * @return whether the key found
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::containsKey(KeyT key) const
{
    int len = _map[_hashFunction(key)].size();
    for (int i = 0; i < len; ++i)
    {
        if (_map[_hashFunction(key)][i].first == key)
        {
            return true;
        }
    }
    return false;
}

/**
 * @param key key
 * @return val of key
 */
template<class KeyT, class ValueT>
const ValueT &HashMap<KeyT, ValueT>::at(KeyT key)
{
    int len = _map[_hashFunction(key)].size();
    for (int i = 0; i < len; ++i)
    {
        if (_map[_hashFunction(key)][i].first == key)
        {
            return _map[_hashFunction(key)][i].second;
        }
    }
    throw keyNotFound();
}

/**
 * erase a key from map
 * @param key key
 * @return whether the deletion was successful
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::erase(KeyT key)
{
    int len = _map[_hashFunction(key)].size();
    for (int i = 0; i < len; ++i)
    {
        if (_map[_hashFunction(key)][i].first == key)
        {
            if (_sizeOfMap - 1 < _capacityOfMap * _lowerLoad)
            {
                _reHash(RESIZEDOWN);
            }
            _map[_hashFunction(key)].erase(_map[_hashFunction(key)].begin() + i);
            _sizeOfMap--;
            return true;
        }
    }
    return false;
}

template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::bucketSize(KeyT key) const
{
    int temp = _map[_hashFunction(key)].size();
    if (temp == 0)
    {
        throw invalidBucket();
    }
    return temp;
}

/**
 * remove all elements from the map
 */
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::clear()
{
    for (int i = 0; i < _capacityOfMap; ++i)
    {
        _map[i].clear();
    }
    _sizeOfMap = 0;
}

/**
 * operator= const
 * @param hashMap
 * @return
 */
template<class KeyT, class ValueT>
HashMap<KeyT, ValueT> &HashMap<KeyT, ValueT>::operator=(const HashMap &hashMap)
{
    if (*this != hashMap)
    {
        _capacityOfMap = hashMap._capacityOfMap;
        _sizeOfMap = hashMap._sizeOfMap;
        delete[] _map;
        _map = new vector<pair<KeyT, ValueT>>[_capacityOfMap];
        for (int i = 0; i < _capacityOfMap; ++i)
        {
            _map[i] = hashMap[i];
        }
    }
    return *this;
}

/**
 * operator subscript
 * @param key
 * @return
 */
template<class KeyT, class ValueT>
ValueT &HashMap<KeyT, ValueT>::operator[](const KeyT &key) noexcept
{
    int len;
    try
    {
        len = _map[_hashFunction(key)].size();
    }
    catch (const noCapacity &e)
    {
        len = 0;
    }
    for (int i = 0; i < len; ++i)
    {
        if (_map[_hashFunction(key)][i].first == key)
        {
            return _map[_hashFunction(key)][i].second;
        }
    }
    if (_sizeOfMap + 1 > (double) _capacityOfMap * (double) _upperrLoad)
    {
        _reHash(RESIZEUP);
        len = _map[_hashFunction(key)].size();
    }
    _map[_hashFunction(key)].push_back(pair<KeyT, ValueT>(key, ValueT()));
    _sizeOfMap++;
    return _map[_hashFunction(key)][len].second;
}

/**
 * operator subscript const
 * @param key
 * @return
 */
template<class KeyT, class ValueT>
ValueT HashMap<KeyT, ValueT>::operator[](const KeyT &key) const noexcept
{
    int len;
    try
    {
        len = _map[_hashFunction(key)].size();
    }
    catch (const noCapacity &e)
    {
        len = 0;
    }
    for (int i = 0; i < len; ++i)
    {
        if (_map[_hashFunction(key)][i].first == key)
        {
            return _map[_hashFunction(key)][i].second;
        }
    }
    _map[_hashFunction(key)].push_back(pair<KeyT, ValueT>());
    return _map[_hashFunction(key)][len].second;
}

/**
 *operator ==
 * @param hashMap
 * @return
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator==(const HashMap &hashMap)
{
    if (_capacityOfMap != hashMap._capacityOfMap || _sizeOfMap != hashMap._sizeOfMap ||
        _upperrLoad != hashMap._upperrLoad || _lowerLoad != hashMap._lowerLoad)
    {
        return false;
    }
    for (int i = 0; i < _capacityOfMap; ++i)
    {
        int len = _map[i].size();
        if (len != hashMap._map[i].size())
        {
            return false;
        }
        for (int j = 0; j < len; ++j)
        {
            if (_map[i][j] != hashMap._map[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * operator== const
 * @param hashMap
 * @return
 */
template<class KeyT, class ValueT>
bool HashMap<KeyT, ValueT>::operator==(const HashMap &hashMap) const
{
    if (_capacityOfMap != hashMap._capacityOfMap || _sizeOfMap != hashMap._sizeOfMap ||
        _upperrLoad != hashMap._upperrLoad || _lowerLoad != hashMap._lowerLoad)
    {
        return false;
    }
    for (int i = 0; i < _capacityOfMap; ++i)
    {
        int len = _map[i].size();
        if (len != hashMap._map[i].size())
        {
            return false;
        }
        for (int j = 0; j < len; ++j)
        {
            if (_map[i][j] != hashMap._map[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * function that will calculate hash of the key
 * @param key key to hash
 * @return hash val of key
 */
template<class KeyT, class ValueT>
int HashMap<KeyT, ValueT>::_hashFunction(KeyT key) const
{
    if (capacity() < 1)
    {
        throw noCapacity();
    }
    std::hash<KeyT> hash;
    return hash(key) & (_capacityOfMap - 1);
}

/**
 * function to rehash the map to  increase the capacity by a factor
 * @param factor foctor to increase capacity by
 */
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::_reHash(double factor)
{
    pair<KeyT, ValueT> tempHolder[_sizeOfMap];
    int counter = 0;
    for (int i = 0; i < _capacityOfMap; ++i)
    {
        int len = _map[i].size();
        for (int j = 0; j < len; ++j)
        {
            tempHolder[counter] = _map[i][j];
            counter++;
        }
    }
    delete[] _map;
    _capacityOfMap *= factor;
    _map = new vector<pair<KeyT, ValueT>>[_capacityOfMap];
    for (int i = 0; i < _sizeOfMap; ++i)
    {
        _map[_hashFunction(tempHolder[i].first)].push_back(tempHolder[i]);
    }
}

/**
 *
 */
template<class KeyT, class ValueT>
void HashMap<KeyT, ValueT>::print()
{
    for (int j = 0; j < _capacityOfMap; ++j)
    {
        for (int i = 0; i < _map[j].size(); ++i)
        {
            std::cerr << _map[j].size() << "  : " << _map[j][i].first << " "
                      << _map[j][i].second;
            std::cerr << std::endl;
        }
    }
    std::cerr << std::endl;
}


#endif //EX3_HASHMAP_HPP
