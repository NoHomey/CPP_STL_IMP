#include <stdexcept>
#include <iostream>

template <class T>
class vector {
public:
    vector(const unsigned capacity = 1): _capacity(capacity), _size(0), _buffer(new T[capacity]) {}

    ~vector(void) {
        delete[] _buffer;
    }

    unsigned size(void) const {
        return _size;
    }

    bool empty(void) const {
        return _size == 0;
    }

    bool full(void) const {
        return _size == _capacity;
    }

    T& operator[](const unsigned n) {
        if(n >= _size) {
            throw std::out_of_range("N must be in range from 0 to size()\n");
        } else {
            return _buffer[n];
        }
    }

    const T& operator[](const unsigned n) const {
        if(n >= _size) {
            throw std::out_of_range("N must be in range from 0 to size()\n");
        } else {
            return _buffer[n];
        }
    }

    void clear(void) {
        delete[] _buffer;
        _capacity = 1;
        _size = 0;
        _buffer = new T;
    }

    unsigned capacity(void) const {
        return _capacity;
    }

    vector(const vector& other): _capacity(other._capacity), _size(other._size), _buffer(new T[other._capacity]) {
        for(unsigned i = 0; i < _size; ++i) {
            _buffer[i] = other._buffer[i];
        }
    }

    vector& operator=(const vector& other) {
        if(this != &other) {
            _size = other._size;
            _capacity = other._capacity;
            delete[] _buffer;
            _buffer = new T[_capacity];
            for(unsigned i = 0; i < _size; ++i) {
                _buffer[i] = other._buffer[i];
            }
        }
        return *this;
    }

    class iterator {
    public:
        iterator(T* position): _position(position) {}

        iterator operator++(void) {
            ++_position;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp(_position);
            operator++();
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return _position == other._position;
        }

        bool operator!=(const iterator& other) const {
            return !operator==(other);
        }

        T& operator*(void) {
            return *_position;
        }

        T* operator->(void) {
            return  _position;
        }

    protected:
        T* _position;
    };

    iterator begin(void) {
        return iterator(_buffer);
    }

    iterator end(void) {
        return(_buffer + _size);
    }

    class const_iterator: public iterator {
    public:
        const_iterator(T* position): iterator(position) {}

        const T& operator*(void) {
            return *iterator::_position;
        }

        const T* operator->(void) {
            return iterator::_position;
        }
    };

    const_iterator cbegin(void) const {
        return const_iterator(_buffer);
    }

    const_iterator cend(void) const {
        return const_iterator(_buffer + _size);
    }

    class reverse_iterator: public iterator {
    public:
        reverse_iterator(T* position): iterator(position) {}

        reverse_iterator operator++(void) {
            --iterator::_position;
            return *this;
        }

        reverse_iterator operator++(int) {
            reverse_iterator tmp(iterator::_position);
            operator++();
            return tmp;
        }
    };

    reverse_iterator rbegin(void) {
        return reverse_iterator(_buffer + _size - 1);
    }

    reverse_iterator rend(void) {
        return reverse_iterator(_buffer - 1);
    }

    class const_reverse_iterator: public const_iterator, public reverse_iterator {
    public:
        const_reverse_iterator(T* position): const_iterator(position), reverse_iterator(position) {}

        using const_iterator::operator*;
        using const_iterator::operator->;
        using reverse_iterator::operator++;
    };

    const_reverse_iterator crbegin(void) const {
        return const_reverse_iterator(_buffer + _size - 1);
    }

    const_reverse_iterator crend(void) const {
        return const_reverse_iterator(_buffer - 1);
    }

    T& front(void) {
        return _buffer[0];
    }

    const T& front(void) const {
        return _buffer[0];
    }

    T& back(void) {
        return _buffer[_size - 1];
    }

    const T& back(void) const {
        return _buffer[_size - 1];
    }

    void push_back(const T& val) {
        if(full()) {
            _capacity++;
            T* tmp = new T[_capacity];
            for(unsigned i = 0; i < _size; ++i) {
                tmp[i] = _buffer[i];
            }
            delete[] _buffer;
            _buffer = tmp;
        }
        _buffer[_size++] = val;
    }

    void pop_back(void) {
        if(empty()) {
            throw std::length_error("vector is empty!\n");
        } else {
            --_size;
        }
    }

    iterator insert(iterator position, const T& val) {
        vector tmp(_capacity + full());
        iterator beg = begin();
        if(empty()) {
            push_back(val);
            return begin();
        } else {
            unsigned track = _size;
            for(iterator it = beg; it != end(); ++it) {
                if(it == position) {
                    track = it.operator->() - beg.operator->();
                    tmp.push_back(val);
                }
                tmp.push_back(*it);
            };
            operator=(tmp);
            return iterator(_buffer + track);
        }
    }

    iterator erase(iterator position) {
        vector tmp(_capacity);
        iterator beg = begin();
        unsigned track = _size;
        for(iterator it = beg; it != end(); ++it) {
            if(it == position) {
                track = it.operator->() - beg.operator->();
            } else {
                tmp.push_back(*it);
            }
        }
        operator=(tmp);
        return iterator(_buffer + track);
    }

    iterator erase(iterator first, iterator last) {
        vector tmp(_capacity);
        iterator beg = begin();
        unsigned track = _size;
        T* first_ptr = first.operator->();
        T* last_ptr = last.operator->();
        T* ptr;
        for(iterator it = beg; it != end(); ++it) {
            ptr = it.operator->();
            if(it == first) {
                track = ptr - beg.operator->();
            } else if((ptr < first_ptr) || (ptr >= last_ptr)) {
                tmp.push_back(*it);
            }
        }
        operator=(tmp);
        return iterator(_buffer + track);
    }

protected:
    unsigned _capacity;
    unsigned _size;
    T* _buffer;
};

template<class T>
std::ostream& operator<<(std::ostream& out, const vector<T>& vec) {
    out << '{';
    unsigned size = vec.size();
    for(unsigned i = 0; i < size; ++i) {
        out << vec[i] << ',';
    }
    out << '}';
    return out;
}

vector<int> construct(int first, int last) {
    vector<int> v(last - first + 1);
    for(int i = first; i < last; ++i) {
        v.push_back(i);
    }
    return v;
}


#include <cstdlib>

int main(int argc, char** argv) {
    vector<int> v1 = construct(std::atoi(argv[1]), std::atoi(argv[2]));
    vector<int> v2 = construct(std::atoi(argv[3]), std::atoi(argv[4]));
    std::cout << "v1: " << v1 << std::endl;
    std::cout << "v2: " << v2 << std::endl;
    unsigned count = 0;
    for(vector<int>::iterator it1 = v1.begin();it1 != v1.end();++it1) {
        for(vector<int>::iterator it2 = v2.begin();it2 != v2.end();++it2) {
            if(*it1 == *it2) {
                count++;
                break;
            }
        }
    }
    std::cout << "equal element in v1 and v2: " << count << std::endl;
    v1.push_back(-100);
    v2.push_back(-100);
    std::cout << "v1: " << v1 << std::endl;
    std::cout << "v2: " << v2 << std::endl;
    vector<int> v(v2);
    std::cout << "v: " << v << std::endl;
    for(vector<int>::reverse_iterator rit = v1.rbegin();rit != v1.rend();++rit) {
        v.insert(v.begin(), *rit);
    }
    std::cout << "v: " << v << std::endl;
    vector<int>::iterator bit = v.begin();
    for(;bit != v.end();++bit) {
        if(*bit == -100) {
            break;
        }
    }
    v.erase(bit, v.end());
    v.erase(++++v.begin());
    std::cout << "v: " << v << std::endl;
    return 0;
}
