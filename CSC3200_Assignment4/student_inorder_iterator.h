// student_inorder_iterator.h
#ifndef STUDENT_INORDER_ITERATOR_H
#define STUDENT_INORDER_ITERATOR_H

#include <cstddef>
#include <iterator>

namespace student_std {

template <typename BinaryTree>
class inorder_iterator {
public:
    using value_type = typename BinaryTree::value_type;
    using difference_type = std::ptrdiff_t;
    using iterator_category = std::forward_iterator_tag;
    using reference = value_type const&;
    using pointer = value_type const*;

    // default construct -> end iterator (nullptr)
    inorder_iterator() noexcept : m_node(nullptr) {}

    // construct from BinaryTree const*
    explicit inorder_iterator(BinaryTree const* root) noexcept {
        m_node = nullptr;
        if (root != nullptr) {
            // find leftmost node starting at root
            BinaryTree const* cur = root;
            while (cur->left() != nullptr) cur = cur->left();
            m_node = cur;
        }
    }

    // copy / assign default are fine
    inorder_iterator(inorder_iterator const&) = default;
    inorder_iterator& operator=(inorder_iterator const&) = default;

    // dereference
    reference operator*() const {
        return m_node->value();
    }

    pointer operator->() const {
        return std::addressof(m_node->value());
    }

    // pre-increment
    inorder_iterator& operator++() {
        if (m_node == nullptr) return *this;
        BinaryTree const* cur = m_node;
        if (cur->right() != nullptr) {
            // successor is leftmost of right subtree
            cur = cur->right();
            while (cur->left() != nullptr) cur = cur->left();
            m_node = cur;
            return *this;
        }
        // climb to parent until we come from left
        BinaryTree const* parent = cur->parent();
        while (parent != nullptr && cur == parent->right()) {
            cur = parent;
            parent = parent->parent();
        }
        m_node = parent; // possibly nullptr => end
        return *this;
    }

    // post-increment
    inorder_iterator operator++(int) {
        inorder_iterator tmp(*this);
        ++(*this);
        return tmp;
    }

    // equality
    friend bool operator==(inorder_iterator const& a, inorder_iterator const& b) noexcept {
        return a.m_node == b.m_node;
    }
    friend bool operator!=(inorder_iterator const& a, inorder_iterator const& b) noexcept {
        return a.m_node != b.m_node;
    }

private:
    BinaryTree const* m_node;
};

} // namespace student_std

#endif // STUDENT_INORDER_ITERATOR_H
