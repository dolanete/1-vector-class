#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm> // std::random_access_iterator_tag
#include <cstddef> // size_t
#include <stdexcept> // std::range_error

//NOtes
//nodiscard - a warning message will be ussued if the return value of this function is not stored in a variable
//explicit - the object cannot be implicitly constructed using this constructor.
//noexcept - specifies that function will not throw an exception

template <class T> // Make a template so any data type will work with the vector class.
class Vector {
public:
    class iterator;
private:
    T* array;
    size_t _capacity, _size;

    // You may want to write a function that grows the vector
    void grow(size_t newCapacity) {

        if (_capacity == 0) { // Check if the capacity is zero. the doubling will not work if the capacity is 0
            newCapacity = 1;
        } 

        T* newArray = new T[newCapacity]; // Make a dynamic array copy
       
        for (size_t i = 0; i < _size; i++) {
            newArray[i] = std::move(array[i]); // Move all the values from the old array to the new array
        }

        delete[] array; // Delete the old dynamic array
        array = newArray; // Assign the location of the new dynamic array to the old array pointer variable
        _capacity = newCapacity; // CHange the capcaity

     }

public:
//===============Default  Constructor===============//
    Vector() noexcept : array(nullptr), _capacity(0), _size(0) {}  // Default Contructor

//===============Parameterized  Constructor===============//
    Vector(size_t count, const T& value) : _capacity(count), _size(count) { //member initialization. Set array to size count
        array = new T[count]; // Set array to a dynamic array set in dynamic memory with size count.
        for (size_t i = 0; i < count; i++) { // Fill the array with value.
            array[i] = value;
        }
    } 

//===============Parameterized  Constructor===============//
    explicit Vector(size_t count) : _capacity(count), _size(count) { //Point towards a new dynamic array with size count. Don't initialize
        array = new T[count];
        for (size_t i = 0; i < count; i++) { // Fill the array with value.
            array[i] = T();
        }
     }

//===============Copy Constructor===============//
    Vector(const Vector& other) : _capacity(other.capacity()), _size(other.size()) { //Set the capacity and size from "other" array
        T* newPtr = new T[_capacity]; // Create a filler array
        
        for (size_t i = 0; i < _capacity; i++) {
            newPtr[i] = other.at(i);
        }
        array = newPtr; // set original array to copy
     }

//===============Move Constructor===============//
    Vector(Vector&& other) noexcept : _capacity(other.capacity()), _size(other.size()){
        array = other.array;

        other.array = nullptr;
        other._size = 0;
        other._capacity = 0;
     }
//===============Destructor===============//
    ~Vector() { 
        delete array;

    }

//===============Copy Assignment Operator===============//
    Vector& operator=(const Vector& other) { 

        if (this != &other) { 

            _size = other.size();
            _capacity = other.capacity();
            delete[] this->array;
            this->array = new T[_capacity]; // Create a filler array
            
            for (size_t i = 0; i < _capacity; i++) {
                array[i] = other.at(i);
            }

        }
        return *this;
     }

//===============Move Assignment Operator===============//
    Vector& operator=(Vector&& other) noexcept { 

        if (this != &other) { //Test if the object it trying to move to itself 

            delete[] array;
            array = other.array;
            _size = other.size();
            _capacity = other.capacity();

            other.array = nullptr;
            other._size = 0;
            other._capacity = 0;
        }
        return *this;


     }

    iterator begin() noexcept { 
        //return array[0]; // Gist, finish iterator
        return iterator(array);
     }
    iterator end() noexcept { 
        //return array[_size];
        return iterator(array + _size);
     }

    [[nodiscard]] bool empty() const noexcept { 
        if (_size == 0) {
            return true;
        }
        return false;
     }

//===============Size Function===============//
    size_t size() const noexcept { 
        return _size;
    }

//===============Capacity Function===============//
    size_t capacity() const noexcept { 
        return _capacity;
     }

//===============At Function===============//
    T& at(size_t pos) {     
        if (pos >= _size || pos < 0) {
            throw std::out_of_range("d");
        }
    return array[pos];

    }
    const T& at(size_t pos) const { 
        return array[pos];
    }

//===============Grab value at index===============//
    T& operator[](size_t pos) { 
        return array[pos]; 
    }
    const T& operator[](size_t pos) const { 
        return array[pos]; 
    }
//===============Front===============//
    T& front() { 
        return array[0];
    }
    const T& front() const { 
        return array[0];
    }
//===============Back===============//
    T& back() { 
        return array[_size-1];
    }
    const T& back() const { 
        return array[_size-1];
    }


    void push_back(const T& value) {
        if (_size >= _capacity) {
            grow(_capacity*2);
        }
        array[_size] = std::move(value); // set the last value in array to value
        _size++;
     }

    void push_back(T&& value) { 
        if (_size >= _capacity) {
            grow(_capacity*2);
        }
        array[_size] = std::move(value);
        _size++; 

        //Unknown
     }    
    void pop_back() { 
        _size--;
    }

    iterator insert(iterator pos, const T& value) { 
        ptrdiff_t poschange = pos - begin();
        if (_size >= _capacity) {
            grow(_capacity*2);
        }
        pos = begin() + poschange;
        //Work with iterators
        // Pos is an iterator. Because of the contructor in the iterator class, pos represents and address.
        // iterator has a dereference operator that I can use to derefference pos
        for (iterator i = end(); i > pos; i--) {
            *i = std::move(*(i-1));
        }
        *pos = value;
        _size++;
        return pos;

    }
    iterator insert(iterator pos, T&& value) { 
        ptrdiff_t poschange = pos - begin();
        if (_size >= _capacity) {
            grow(_capacity*2);
        }
        pos = begin() + poschange;
        //Work with iterators
        // Pos is an iterator. Because of the contructor in the iterator class, pos represents and address.
        // iterator has a dereference operator that I can use to derefference pos
        for (iterator i = end(); i > pos; i--) {
            *i = std::move(*(i-1));
        }
        *pos = std::move(value);
        _size++;
        return pos;

    }
    iterator insert(iterator pos, size_t count, const T& value) { 
        ptrdiff_t poschange = pos - begin();
        while ((_size + count) >= _capacity) {
            grow(_capacity*2);
        }
        pos = begin() + poschange;
        //Work with iterators
        // Pos is an iterator. Because of the contructor in the iterator class, pos represents and address.
        // iterator has a dereference operator that I can use to derefference pos
        
        iterator it;
        for (it = end()-1; it >= pos; it--) {
            *(it+count) = std::move(*(it));
        }
        for (size_t i = poschange; i < poschange+count; i++) {
            array[i] = value;
        }
        _size += count;
        return pos;
    }

    iterator erase(iterator pos) { 
        //Work with iterators
        // Pos is an iterator. Because of the contructor in the iterator class, pos represents and address.
        // iterator has a dereference operator that I can use to derefference pos
        for (iterator i = pos; i < end()-1; i++) {
            *i = std::move(*(i+1));
        }
        _size--;
        return pos;
        
    }
    iterator erase(iterator first, iterator last) { 
        ptrdiff_t range = last - first;
        
        iterator i;
        for (i = first; i < end()-range; i++) {
            *i = std::move(*(i+range));
        }
        _size -= range;
        return first;
    }

    class iterator {
    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type        = T;
        using difference_type   = ptrdiff_t;
        using pointer           = T*; // This is an alias - This is an allias for T*
        using reference         = T&;
    private:
        // Add your own data members here
        pointer ptr;
        // HINT: For random_access_iterator, the data member is a pointer 99.9% of the time
    public:
        iterator() : ptr{nullptr}{} // Default Constructor
        iterator(pointer curptr) : ptr(curptr) {}


        // Add any constructors that you may need

        // This assignment operator is done for you, please do not add more
        iterator& operator=(const iterator&) noexcept = default;

        [[nodiscard]] reference operator*() const noexcept { return *ptr; } // Derefference
        [[nodiscard]] pointer operator->() const noexcept { return ptr; } // Returns the pointer

        // Prefix Increment: ++a
        iterator& operator++() noexcept { 
            ++ptr; // First increment the pointer
            return *this; //returning myself which is an iterator
         }
        // Postfix Increment: a++
        iterator operator++(int) noexcept { 
            iterator temp = *this; // Makes a temparary copy
            ++(*this); // increment current
            return temp; // return the one before
         }

        // Prefix Decrement: --a
        iterator& operator--() noexcept { 
            --ptr; // First increment the pointer
            return *this; //returning myself which is an iterator
        }
        // Postfix Decrement: a--
        iterator operator--(int) noexcept { 
            iterator temp = *this; // Makes a temparary copy
            --(*this); // increment current
            return temp; // return the one before
        }

        iterator& operator+=(difference_type offset) noexcept { 
            ptr += offset; // Increase the pointer by the offset
            return *this; // return iterator type
         }
        [[nodiscard]] iterator operator+(difference_type offset) const noexcept { 
            iterator copy = *this; // be sure not to change the acutal iterator
            return copy += offset; // ust gets value of location after offset
         }
        
        iterator& operator-=(difference_type offset) noexcept { 
            ptr -= offset; // Increase the pointer by the offset
            return *this; // return iterator type
        }
        [[nodiscard]] iterator operator-(difference_type offset) const noexcept { 
            iterator copy = *this; // be sure not to change the acutal iterator
            return copy -= offset; // ust gets value of location after offset
        }

        [[nodiscard]] difference_type operator-(const iterator& rhs) const noexcept {
            return ptr - rhs.ptr;
        }

        [[nodiscard]] reference operator[](difference_type offset) const noexcept { 
            return *(*this+offset); // Use Reference
        }

        [[nodiscard]] bool operator==(const iterator& rhs) const noexcept { 
            return ptr == rhs.ptr;
        }
        [[nodiscard]] bool operator!=(const iterator& rhs) const noexcept {
            return !(*this == rhs); // That that == is defined. I can use interator comparison now. rhs is an iterator and *this is an iterator
        }
        [[nodiscard]] bool operator<(const iterator& rhs) const noexcept {
            return ptr < rhs.ptr;
        }
        [[nodiscard]] bool operator>(const iterator& rhs) const noexcept { 
            return ptr > rhs.ptr;
        }
        [[nodiscard]] bool operator<=(const iterator& rhs) const noexcept { 
            return ptr <= rhs.ptr;
        }
        [[nodiscard]] bool operator>=(const iterator& rhs) const noexcept { 
            return ptr >= rhs.ptr;
        }
    };


    void clear() noexcept { 
        for (size_t i = 0; i < _size ; i++) {
            array[i].~T();
        }

        _size = 0;
     }
};

template <class T>
[[nodiscard]] typename Vector<T>::iterator operator+(typename Vector<T>::iterator::difference_type offset, typename Vector<T>::iterator iterator) noexcept { /* TODO */ }

#endif
