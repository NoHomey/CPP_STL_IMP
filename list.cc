#include <iostream>
#include <stdexcept>

template<class T>
class list {
protected:
    struct node{
        node(const T& data, struct node* prev = NULL, struct node* next = NULL)
        : data(data), prev(prev), next(next) {}

        T data;
        struct node* prev;
        struct node* next;
    };

    struct node* _head;
    struct node* _tail;
public:
    list(struct node* head = NULL, struct node* tail = NULL)
    : _head(head), _tail(tail) {}

    bool empty(void) const {
        return !_head;
    }

    void push_back(const T& data) {
        insert(end(), data);
    }

    void push_front(const T& data) {
        insert(begin(), data);
    }

    void pop_back(void) {
        erase(iterator(rbegin().node()));
    }

    void pop_front(void) {
        erase(begin());
    }

    T& front(void) {
        return _head->data;
    }

    const T& front(void) const {
        return _head->data;
    }

    T& back(void) {
        return _tail->data;
    }

    const T& back(void) const {
        return _tail->data;
    }

    unsigned size(void) const {
        if(empty()) {
            return 0;
        } else {
            unsigned i = 0;
            for(const_iterator it = cbegin(); it != cend(); ++it) {
                ++i;
            }
            return i;
        }
    }

    void swap(list& other) {
        list tmp = *this;
        *this = other;
        other = tmp;
    }

    void clear(void) {
        while(!empty()) {
            pop_front();
        }
    }

    void append(const list& other) {
        for(const_iterator it = other.cbegin(); it != other.cend(); ++it) {
            push_back(*it);
        }
    }

    list(const list& other): _head(NULL), _tail(NULL) {
        append(other);
    }

    list& operator=(const list& other) {
        if(this != &other) {
            clear();
            append(other);
        }
        return *this;
    }

    class iterator {
    protected:
        struct node* _node;
    public:
        iterator(struct node* node = NULL): _node(node) {}

        iterator operator++(void) {
            _node = _node->next;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp(_node);
            operator++();
            return tmp;
        }

        bool operator==(const iterator& other) const {
            return _node == other._node;
        }

        bool operator!=(const iterator& other) const {
            return !operator==(other);
        }

        T& operator*(void) {
            return _node->data;
        }

        T* operator->(void) {
            return &operator*();
        }

        struct node* node(void) {
            return _node;
        }
    };

    iterator begin(void) {
        return iterator(_head);
    }

    iterator end(void) {
        return iterator();
    }

    class reverse_iterator: public iterator {
    public:
        reverse_iterator(struct node* node = NULL): iterator(node) {}

        reverse_iterator operator++(void) {
            iterator::_node = iterator::_node->prev;
            return *this;
        }

        reverse_iterator operator++(int) {
            reverse_iterator tmp(iterator::_node);
            operator++();
            return tmp;
        }
    };

    reverse_iterator rbegin(void) {
        return reverse_iterator(_tail);
    }

    reverse_iterator rend(void) {
        return reverse_iterator();
    }

    class const_iterator: public iterator {
    public:
        const_iterator(struct node* node = NULL): iterator(node) {}

        const T& operator*(void) {
            return iterator::_node->data;
        }

        const T* operator->(void) {
            return &operator*();
        }
    };

    const_iterator cbegin() const {
        return const_iterator(_head);
    }

    const_iterator cend() const {
        return const_iterator();
    }

    class const_reverse_iterator: public const_iterator, public reverse_iterator {
        const_reverse_iterator(struct node* node = NULL): const_iterator(node), reverse_iterator(node) {}

        using const_iterator::operator*;
        using const_iterator::operator->;
        using reverse_iterator::operator++;
    };

    const_reverse_iterator crbegin(void) const {
        return const_reverse_iterator(_tail);
    }

    const_reverse_iterator crend(void) const {
        return const_reverse_iterator();
    }

    iterator insert(iterator position, const T& data) {
        struct node* current = position.node();
        if(current) {
            struct node* before = current->prev;
            struct node* node = new struct node(data, before, current);
            current->prev = node;
            if(before) {
                before->next = node;
            } else {
                _head = node;
            }
            return iterator(node);
        } else {
            if(empty()) {
                struct node* node = new struct node(data);
                _head = node;
                _tail = node;
                return begin();
            } else {
                struct node* node = new struct node(data, _tail);
                _tail->next = node;
                _tail = node;
                return iterator(node);
            }
        }
    }

    iterator erase(iterator position) {
        if(empty()) {
            throw std::length_error("list is empty!\n");
        } else {
            struct node* current = position.node();
            struct node* before = current->prev;
            struct node* after = current->next;
            if(current == _head) {
                _head = after;
            } else {
                before->next = after;
            }
            if(current == _tail) {
                _tail = before;
            } else {
                after->prev = before;
            }
            delete current;

            return iterator(after);
        }
    }

    iterator erase(iterator first, iterator last) {
        iterator it = first;
        while(it != last) {
            it = erase(it);
        }

        return it;
    }
};

template<class T>
std::ostream& operator<<(std::ostream& out, const list<T>& l) {
    out << '{';
    for(typename list<T>::const_iterator it = l.cbegin(); it != l.cend(); ++it) {
        out << *it << ',';
    }
    out << '}';
    return out;
}

list<int> construct(int first, int last) {
    list<int> l;
    for(int i = first; i < last; ++i) {
        l.push_back(i);
    }
    return l;
}

#include <cstdlib>

int main (int argc, char** argv) {
    list<int> l1 = construct(std::atoi(argv[1]), std::atoi(argv[2]));
    list<int> l2 = construct(std::atoi(argv[3]), std::atoi(argv[4]));
    std::cout << "l1: " << l1 << std::endl << "l2: " << l2 << std::endl;
    unsigned c = 0;
    for(list<int>::iterator it1 = l1.begin();it1 != l1.end();++it1) {
        for(list<int>::iterator it2 = l2.begin();it2 != l2.end();++it2) {
            if(*it1 == *it2) {
                c++;
                break;
            }
        }
    }
    std::cout << "equal element in l1 and l2: " << c << std::endl;
    l1.push_back(-100);
    l2.push_back(-100);
    std::cout << "l1: " << l1 << std::endl << "l2: " << l2 << std::endl;
    list<int> l(l2);
    std::cout << "l: " << l << std::endl;
    for(list<int>::reverse_iterator r = l1.rbegin();r != l1.rend();++r) {
        l.insert(l.begin(), *r);
    }
    std::cout << "l: " << l << std::endl;
    list<int>::iterator bit;
    for(bit = l.begin();bit != l.end();++bit) {
        if(*bit == -100) {
            break;
        }
    }
    l.erase(bit, l.end());
    std::cout << "l: " << l << std::endl;

    return 0;
}
