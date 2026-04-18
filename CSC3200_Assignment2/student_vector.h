#ifndef STUDENT_VECTOR_H
#define STUDENT_VECTOR_H

#include <cstddef>
#include <cassert>
#include <stdexcept>
#include <iterator>
#include <algorithm> // for std::copy, std::move

namespace student_std {

template<typename T>
class vector {
public:
    // public typedefs
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using value_type = T;

private:
    T* data_;
    size_type size_;
    size_type capacity_;

    // allocate new buffer of capacity 'cap' and return pointer (may be nullptr)
    static T* allocate(size_type cap) {
        if (cap == 0) return nullptr;
        return new T[cap];
    }

    // deallocate buffer
    static void deallocate(T* p) {
        delete[] p;
    }

    // grow strategy: double capacity or set to 1 if zero
    void grow_if_needed() {
        if (size_ >= capacity_) {
            size_type newcap = capacity_ == 0 ? 1 : capacity_ * 2;
            reserve(newcap);
        }
    }

public:
    // Default constructor
    inline vector() noexcept
        : data_(nullptr), size_(0), capacity_(0) {}

    // Copy constructor
    inline vector(const vector& other)
        : data_(nullptr), size_(0), capacity_(0)
    {
        if (other.size_ > 0) {
            data_ = allocate(other.size_);
            capacity_ = other.size_;
            size_ = other.size_;
            // T is assumed trivially copyable per assignment statement: use copy
            for (size_type i = 0; i < size_; ++i) data_[i] = other.data_[i];
        }
    }

    // Assignment operator
    inline vector& operator=(const vector& other) {
        if (this == &other) return *this; // self-assignment
        // Simple copy-then-swap style could be used; here allocate then replace
        T* newdata = nullptr;
        if (other.size_ > 0) {
            newdata = allocate(other.size_);
            for (size_type i = 0; i < other.size_; ++i) newdata[i] = other.data_[i];
        }
        // free old
        deallocate(data_);
        data_ = newdata;
        size_ = other.size_;
        capacity_ = other.size_;
        return *this;
    }

    // Destructor
    inline ~vector() {
        deallocate(data_);
    }

    // Capacity & size
    inline size_type size() const noexcept { return size_; }
    inline size_type capacity() const noexcept { return capacity_; }
    inline bool empty() const noexcept { return size_ == 0; }

    // data pointers
    inline T* data() noexcept { return data_; }
    inline const T* data() const noexcept { return data_; }

    // Element access (const)
    inline const T& at(size_type pos) const {
        if (pos >= size_) throw std::out_of_range("vector::at: index out of range");
        return data_[pos];
    }
    inline const T& operator[](size_type pos) const { return data_[pos]; }
    inline const T& front() const { assert(size_ > 0); return data_[0]; }
    inline const T& back() const { assert(size_ > 0); return data_[size_ - 1]; }

    // Element access (non-const)
    inline T& at(size_type pos) {
        if (pos >= size_) throw std::out_of_range("vector::at: index out of range");
        return data_[pos];
    }
    inline T& operator[](size_type pos) { return data_[pos]; }
    inline T& front() { assert(size_ > 0); return data_[0]; }
    inline T& back() { assert(size_ > 0); return data_[size_ - 1]; }

    // Modifiers
    inline void push_back(const T& value) {
        grow_if_needed();
        data_[size_++] = value;
    }

    inline void pop_back() {
        assert(size_ > 0);
        --size_;
        // For trivially destructible T, nothing else required
    }

    inline void clear() noexcept {
        size_ = 0;
    }

    inline void swap(vector& other) noexcept {
        std::swap(data_, other.data_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    inline void reserve(size_type new_cap) {
        if (new_cap <= capacity_) return;
        T* newdata = allocate(new_cap);
        for (size_type i = 0; i < size_; ++i) newdata[i] = data_[i];
        deallocate(data_);
        data_ = newdata;
        capacity_ = new_cap;
    }

    inline void resize(size_type new_size) {
        if (new_size <= size_) {
            size_ = new_size;
            return;
        }
        if (new_size > capacity_) reserve(new_size);
        // Default-initialize new elements
        for (size_type i = size_; i < new_size; ++i) data_[i] = T();
        size_ = new_size;
    }

    inline void resize(size_type new_size, const T& value) {
        if (new_size <= size_) {
            size_ = new_size;
            return;
        }
        if (new_size > capacity_) reserve(new_size);
        for (size_type i = size_; i < new_size; ++i) data_[i] = value;
        size_ = new_size;
    }

    // --- Iterator nested class ---
    class iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::random_access_iterator_tag;

    private:
        T* ptr_;
    public:
        // constructors
        inline iterator() noexcept : ptr_(nullptr) {}
        inline explicit iterator(T* p) noexcept : ptr_(p) {}
        inline iterator(const iterator& other) noexcept = default;
        inline iterator& operator=(const iterator& other) noexcept = default;

        // dereference
        inline reference operator*() const { return *ptr_; }
        inline pointer operator->() const { return ptr_; }

        // ++ / --
        inline iterator& operator++() { ++ptr_; return *this; }
        inline iterator operator++(int) { iterator tmp(*this); ++ptr_; return tmp; }
        inline iterator& operator--() { --ptr_; return *this; }
        inline iterator operator--(int) { iterator tmp(*this); --ptr_; return tmp; }

        // arithmetic
        inline iterator& operator+=(difference_type n) { ptr_ += n; return *this; }
        inline iterator& operator-=(difference_type n) { ptr_ -= n; return *this; }
        inline iterator operator+(difference_type n) const { return iterator(ptr_ + n); }
        inline iterator operator-(difference_type n) const { return iterator(ptr_ - n); }

        // difference
        inline difference_type operator-(const iterator& other) const { return ptr_ - other.ptr_; }

        // comparisons
        inline bool operator==(const iterator& other) const { return ptr_ == other.ptr_; }
        inline bool operator!=(const iterator& other) const { return ptr_ != other.ptr_; }
        inline bool operator<(const iterator& other) const { return ptr_ < other.ptr_; }
        inline bool operator<=(const iterator& other) const { return ptr_ <= other.ptr_; }
        inline bool operator>(const iterator& other) const { return ptr_ > other.ptr_; }
        inline bool operator>=(const iterator& other) const { return ptr_ >= other.ptr_; }

        // allow access to raw pointer for vector internals
        inline T* get() const noexcept { return ptr_; }
    };

    // begin / end
    inline iterator begin() noexcept { return iterator(data_); }
    inline iterator end() noexcept { return iterator(data_ + size_); }

    // erase single element at position it, return iterator to element after erased
    inline iterator erase(iterator it) {
        T* p = it.get();
        assert(p >= data_ && p < data_ + size_);
        size_type idx = static_cast<size_type>(p - data_);
        // move elements left
        for (size_type i = idx; i + 1 < size_; ++i) data_[i] = data_[i + 1];
        --size_;
        return iterator(data_ + idx);
    }

    // erase range [it1, it2)
    inline iterator erase(iterator it1, iterator it2) {
        T* p1 = it1.get();
        T* p2 = it2.get();
        assert(p1 >= data_ && p1 <= data_ + size_);
        assert(p2 >= data_ && p2 <= data_ + size_);
        if (p1 >= p2) return it1; // nothing to erase
        size_type idx1 = static_cast<size_type>(p1 - data_);
        size_type idx2 = static_cast<size_type>(p2 - data_);
        size_type num_move = size_ - idx2;
        for (size_type i = 0; i < num_move; ++i) data_[idx1 + i] = data_[idx2 + i];
        size_ -= (idx2 - idx1);
        return iterator(data_ + idx1);
    }

    // insert value before position it, return iterator to new element
    inline iterator insert(iterator it, const T& value) {
        T* p = it.get();
        assert(p >= data_ && p <= data_ + size_);
        size_type idx = static_cast<size_type>(p - data_);
        if (size_ + 1 > capacity_) {
            // grow - choose new capacity (double or at least 1)
            size_type newcap = capacity_ == 0 ? 1 : capacity_ * 2;
            if (newcap < size_ + 1) newcap = size_ + 1;
            reserve(newcap);
            // data_ may have changed, recompute p
            p = data_ + idx;
        }
        // move elements right
        for (size_type i = size_; i > idx; --i) data_[i] = data_[i - 1];
        data_[idx] = value;
        ++size_;
        return iterator(data_ + idx);
    }
};

} // namespace student_std

#endif // STUDENT_VECTOR_H
