/**
 * @file VLVector.hpp
 * @author Ron Shuvy
 * @brief This class defines a generic dynamic sequence container ("Virtual Length Vector"),
 * making use of both static and dynamic storage.
 */

#ifndef VLVECTOR_HPP
#define VLVECTOR_HPP
// ------------------------------ includes ------------------------------
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <cstring>
// -------------------------- const definitions -------------------------
#define DEFAULT_STATIC_CAPACITY 16
#define OUT_OF_RANGE_MSG "Index out of range"
// ------------------------ class declaration ---------------------------
/**
 * @class VLVector
 * @brief sequence container in dynamic size
 *
 * @section Template parameters
 * @param T type of the elements.
 * @param StaticCapacity size of static storage
 *
 * @section Member functions
 *
 * @fn VLVector()
 * @brief Default constructor
 *
 * @fn VLVector(const VLVector& rhs)
 * @brief Copy constructor
 *
 * @fn VLVector(InputIterator first, InputIterator last)
 * @brief Constructor out of range of values
 *
 * @fn ~VLVector()
 * @brief Destructor
 *
 * @fn size_type size()
 * @brief Return container size
 *
 * @fn size_type capacity()
 * @brief Return storage capacity
 *
 * @fn bool empty()
 * @brief Return true if the container is empty, false otherwise
 *
 * @fn reference at(size_t i)
 * @brief Returns a reference to the element at position i (with bounds checking)
 *
 * @fn reference operator[](size_t i)
 * @brief Returns a reference to the element at position i (no bounds checking)
 *
 * @fn pointer data()
 * @brief Returns a pointer to the container which stores the elements
 *
 * @fn void push_back(T val)
 * @brief Add element at the end
 *
 * @fn iterator insert(const_iterator pos, T val)
 * @brief Insert element to a given position and returns an iterator to new element
 *
 * @fn iterator insert(const_iterator pos, InputIterator first, InputIterator last)
 * @brief Insert range of elements to a given position and returns an iterator to left-most new element
 *
 * @fn void pop_back()
 * @brief Delete last element
 *
 * @fn iterator erase(const_iterator pos)
 * @brief Delete the element in a given position and returns an iterator to the element next to 'pos'
 *
 * @fn iterator erase(const_iterator first, const_iterator last)
 * @brief Delete a range of elements and returns an iterator to the element next to 'last'
 *
 * @fn void clear()
 * @brief Clear content
 *
 * @fn VLVector& operator=(const VLVector& rhs)
 * @brief Assignment
 *
 * @fn bool operator==(const VLVector& rhs)
 * @brief Comparison (equal to)
 *
 * @fn bool operator!=(const VLVector& rhs)
 * @brief Comparison (not equal to)
 *
 */
template <class T, size_t StaticCapacity = DEFAULT_STATIC_CAPACITY>
class VLVector
{
public:
    // type definitions
    typedef T*              iterator;
    typedef const T*        const_iterator;
    typedef T&              reference;
    typedef const T&        const_reference;
    typedef T*              pointer;
    typedef const T*        const_pointer;

    // iterator support
    iterator begin() { return _elems; }
    const_iterator begin() const { return _elems; }
    const_iterator cbegin() const { return _elems; }
    iterator end() { return _elems + _size; }
    const_iterator end() const { return _elems + _size; }
    const_iterator cend() const { return _elems + _size; }

    // constructor & destructor
    VLVector() : _heapArr(nullptr), _elems(_stackArr), _capacity(StaticCapacity), _size(0){};
    VLVector(const VLVector& rhs) : VLVector() { *this = rhs; }
    template <class InputIterator> VLVector(InputIterator first, InputIterator last);
    ~VLVector() { delete[] _heapArr; }

    // capacity
    size_t size() const { return _size; }
    size_t capacity() const { return _capacity; }
    bool empty() const { return _size == 0; }

    // elements access
    reference at(size_t i);
    const_reference at(size_t i) const;
    reference operator[](size_t i) { return _elems[i]; }
    const_reference operator[](size_t i) const { return _elems[i]; }
    pointer data() { return _elems; }
    const_pointer data() const { return _elems; }

    // modifiers
    void push_back(T val);
    iterator insert(const_iterator pos, T val) { return insert(pos, &val, &val + 1); }
    template<class InputIterator> iterator insert(const_iterator pos, InputIterator first, InputIterator last);
    void pop_back();
    iterator erase(const_iterator pos) { return erase(pos, pos + 1); }
    iterator erase(const_iterator first, const_iterator last);
    void clear() { *this = VLVector(); }

    // operators
    VLVector& operator=(const VLVector& rhs);
    bool operator==(const VLVector& rhs) const;
    bool operator!=(const VLVector& rhs) const { return !(*this == rhs); }

private:
    T _stackArr[StaticCapacity]; // static fixed-sized array
    T* _heapArr; // dynamically allocated array

    T* _elems; // container elements (points to stackArr or heapArr)
    size_t _capacity; // container storage size
    size_t _size; // number of stored elements

    /** @brief return the expanded capacity when the container is full*/
    size_t _newCapacity() const { return std::floor(3 * (_size + 1) / 2); }
};

// ------------------------------ class definitions -----------------------------

template<class T, size_t StaticCapacity>
template<class InputIterator>
VLVector<T, StaticCapacity>::VLVector(InputIterator first, InputIterator last) : VLVector()
{
    for (; first != last; ++first)
    {
        push_back(*first);
    }
}

template<class T, size_t StaticCapacity>
T& VLVector<T, StaticCapacity>::at(size_t i)
{
    if (i >= _size || i < 0) // check boundaries
    {
        throw std::out_of_range(OUT_OF_RANGE_MSG);
    }
    return  _elems[i];
}

template<class T, size_t StaticCapacity>
const T& VLVector<T, StaticCapacity>::at(size_t i) const
{
    if (i >= _size || i < 0) // check boundaries
    {
        throw std::out_of_range(OUT_OF_RANGE_MSG);
    }
    return  _elems[i];
}

template<class T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::push_back(T val)
{
    if (_size == _capacity) // move to larger dynamic storage
    {
        _capacity = _newCapacity();
        T* newArr = new T[_capacity];
        std::memcpy(newArr, _elems, sizeof(T)*_size);
        delete[] _heapArr;
        _elems = _heapArr = newArr;
    }
    _elems[_size] = val;
    _size++;
}

template<class T, size_t StaticCapacity>
void VLVector<T, StaticCapacity>::pop_back()
{
    _size--;
    if (_size == StaticCapacity) // move to static storage
    {
        _capacity = StaticCapacity;
        std::memcpy(_heapArr, _stackArr, sizeof(T)*_size);
        delete[] _heapArr, _heapArr = nullptr;
        _elems = _stackArr;
    }
}

template<class T, size_t StaticCapacity>
template<class InputIterator>
T* VLVector<T, StaticCapacity>::insert(const VLVector::const_iterator pos, InputIterator first, InputIterator last)
{
    size_t relativePos = pos - this->begin();
    VLVector temp; // store changes in temporary object

    // add all values in order
    size_t i; // index
    for (i = 0; i < relativePos; ++i)
    {
        temp.push_back(_elems[i]);
    }
    for (; first != last; ++first)
    {
        temp.push_back(*first);
    }
    for (; i < _size; ++i)
    {
        temp.push_back(_elems[i]);
    }
    *this = temp;
    return _elems + relativePos;
}

template<class T, size_t StaticCapacity>
T* VLVector<T, StaticCapacity>::erase(VLVector::const_iterator first, VLVector::const_iterator last)
{
    VLVector temp; // store changes in temporary object
    iterator curr = this->begin();
    size_t relativePos = last - curr - (last - first);
    bool toAdd = true; // to add current value ?
    for (; curr != this->end(); ++curr)
    {
        if (curr == first)
        {
            toAdd = false;
        }
        if (curr == last)
        {
            toAdd = true;
        }
        if (toAdd)
        {
            temp.push_back(*curr);
        }
    }
    *this = temp;
    return _elems + relativePos;
}

template<class T, size_t StaticCapacity>
VLVector<T, StaticCapacity>& VLVector<T, StaticCapacity>::operator=(const VLVector &rhs)
{
    if (this != &rhs) // handle self-assignment
    {
        if (_capacity != rhs.capacity())
        {
            delete[] _heapArr, _heapArr = nullptr;
            _elems = _stackArr;
            if (rhs._size > StaticCapacity) // dynamic allocation required
            {
                _heapArr = new T[rhs._size];
                _elems = _heapArr;
            }
            _capacity = rhs._capacity;
        }
        _size = rhs.size();
        std::copy(rhs.begin(), rhs.end(), this->begin());
    }
    return *this;
}

template<class T, size_t StaticCapacity>
bool VLVector<T, StaticCapacity>::operator==(const VLVector &rhs) const
{
    return (_size == rhs.size()) && std::equal(this->begin(), this->end(), rhs.begin());
}

#endif //VLVECTOR_HPP