// student_avl_tree.h

#ifndef STUDENT_AVL_TREE_H
#define STUDENT_AVL_TREE_H

#include <cstddef>
#include <functional>
#include <algorithm>
#include <iterator>
#include <utility>
#include <stdexcept>

namespace student_std {

template <typename Key, typename Comp = std::less<Key>>
class avl_tree {
public:
    class avl_node {
    public:
        using size_type = std::size_t;

        explicit avl_node(Key const& k)
            : m_key(k), m_parent(nullptr), m_left(nullptr), m_right(nullptr),
              m_size(1), m_height(0) {}

        Key const& value() const { return m_key; }
        avl_node const* parent() const { return m_parent; }
        avl_node const* left() const { return m_left; }
        avl_node const* right() const { return m_right; }

        size_type size() const { return m_size; }
        std::ptrdiff_t height() const { return m_height; }

    private:
        Key m_key;
        avl_node* m_parent;
        avl_node* m_left;
        avl_node* m_right;
        std::size_t m_size;
        std::ptrdiff_t m_height;

        friend class avl_tree<Key, Comp>;
    };

    class avl_iterator {
    public:
        using value_type = avl_node;
        using reference = value_type const&;
        using pointer = value_type const*;
        using difference_type = std::ptrdiff_t;
        using iterator_category = std::bidirectional_iterator_tag;

        // owner may be nullptr for a standalone iterator, but tree will pass 'this'
        avl_iterator(avl_tree const* owner = nullptr, pointer node = nullptr)
            : m_owner(const_cast<avl_tree*>(owner)), m_node(const_cast<avl_node*>(node)) {}

        avl_iterator& operator++() {
            if (!m_node) return *this; // ++end() stays end()
            m_node = next_node(m_node);
            return *this;
        }
        avl_iterator operator++(int) { avl_iterator tmp = *this; ++(*this); return tmp; }

        avl_iterator& operator--() {
            if (!m_node) {
                // --end() -> max element
                if (!m_owner) { m_node = nullptr; return *this; }
                m_node = m_owner->maximum_node(m_owner->m_root);
                return *this;
            }
            m_node = prev_node(m_node);
            return *this;
        }
        avl_iterator operator--(int) { avl_iterator tmp = *this; --(*this); return tmp; }

        bool operator==(avl_iterator const& o) const { return m_node == o.m_node; }
        bool operator!=(avl_iterator const& o) const { return m_node != o.m_node; }

        reference operator*() const { return *m_node; }
        pointer operator->() const { return m_node; }

    private:
        avl_tree* m_owner; // pointer to owning tree for --end()
        avl_node* m_node;

        static avl_node* minimum_node(avl_node* n) {
            while (n && n->m_left) n = n->m_left;
            return n;
        }

        static avl_node* maximum_node(avl_node* n) {
            while (n && n->m_right) n = n->m_right;
            return n;
        }

        static avl_node* next_node(avl_node* n) {
            if (!n) return nullptr;
            if (n->m_right) return minimum_node(n->m_right);
            avl_node* p = n->m_parent;
            while (p && n == p->m_right) { n = p; p = p->m_parent; }
            return p;
        }

        static avl_node* prev_node(avl_node* n) {
            if (!n) return nullptr;
            if (n->m_left) return maximum_node(n->m_left);
            avl_node* p = n->m_parent;
            while (p && n == p->m_left) { n = p; p = p->m_parent; }
            return p;
        }

        friend class avl_tree<Key, Comp>;
    };

    using key_type = Key;
    using node_type = avl_node;
    using size_type = std::size_t;
    using comparison = Comp;
    using const_iterator = avl_iterator;
    using iterator = avl_iterator;

    avl_tree() : m_root(nullptr), m_size(0) {}
    ~avl_tree() { clear(m_root); }

    iterator insert(Key const& k) {
        return iterator(this, insert_node(k));
    }

    iterator erase(Key const& k) {
        avl_node* n = find_node(k);
        if (!n) return end();
        avl_node* nxt = next_for_erase(n);
        erase_node(n);
        return iterator(this, nxt);
    }

    iterator find(Key const& k) const {
        return iterator(this, find_node(k));
    }

    bool contains(Key const& k) const {
        return find_node(k) != nullptr;
    }

    size_type size() const { return m_size; }
    std::ptrdiff_t height() const { return m_root ? m_root->m_height : -1; }

    iterator begin() const { return iterator(this, minimum_node(m_root)); }
    iterator end() const { return iterator(this, nullptr); }
    iterator root() const { return iterator(this, m_root); }

private:
    avl_node* m_root;
    std::size_t m_size;
    Comp m_comp;

    static std::ptrdiff_t h(avl_node* n) { return n ? n->m_height : -1; }
    static std::size_t s(avl_node* n) { return n ? n->m_size : 0; }

    static void update(avl_node* n) {
        if (!n) return;
        n->m_height = 1 + std::max(h(n->m_left), h(n->m_right));
        n->m_size = 1 + s(n->m_left) + s(n->m_right);
    }

    static int bf(avl_node* n) {
        return h(n->m_left) - h(n->m_right);
    }

    static avl_node* minimum_node(avl_node* n) {
        while (n && n->m_left) n = n->m_left;
        return n;
    }

    static avl_node* maximum_node(avl_node* n) {
        while (n && n->m_right) n = n->m_right;
        return n;
    }

    avl_node* rotate_right(avl_node* y) {
        avl_node* x = y->m_left;
        avl_node* T2 = x->m_right;

        x->m_right = y;
        x->m_parent = y->m_parent;
        y->m_parent = x;
        y->m_left = T2;
        if (T2) T2->m_parent = y;

        if (x->m_parent) {
            if (x->m_parent->m_left == y) x->m_parent->m_left = x;
            else x->m_parent->m_right = x;
        } else m_root = x;

        update(y);
        update(x);
        return x;
    }

    avl_node* rotate_left(avl_node* x) {
        avl_node* y = x->m_right;
        avl_node* T2 = y->m_left;

        y->m_left = x;
        y->m_parent = x->m_parent;
        x->m_parent = y;
        x->m_right = T2;
        if (T2) T2->m_parent = x;

        if (y->m_parent) {
            if (y->m_parent->m_left == x) y->m_parent->m_left = y;
            else y->m_parent->m_right = y;
        } else m_root = y;

        update(x);
        update(y);
        return y;
    }

    void rebalance_up(avl_node* n) {
        while (n) {
            update(n);
            int balance = bf(n);

            if (balance > 1) {
                if (bf(n->m_left) < 0) rotate_left(n->m_left);
                n = rotate_right(n);
            } else if (balance < -1) {
                if (bf(n->m_right) > 0) rotate_right(n->m_right);
                n = rotate_left(n);
            }
            n = n->m_parent;
        }
    }

    avl_node* find_node(Key const& k) const {
        avl_node* cur = m_root;
        while (cur) {
            if (m_comp(k, cur->m_key)) cur = cur->m_left;
            else if (m_comp(cur->m_key, k)) cur = cur->m_right;
            else return cur;
        }
        return nullptr;
    }

    avl_node* insert_node(Key const& k) {
        if (!m_root) {
            m_root = new avl_node(k);
            m_size = 1;
            return m_root;
        }

        avl_node* cur = m_root;
        avl_node* parent = nullptr;

        while (cur) {
            parent = cur;
            if (m_comp(k, cur->m_key)) cur = cur->m_left;
            else cur = cur->m_right;
        }

        avl_node* node = new avl_node(k);
        node->m_parent = parent;
        if (m_comp(k, parent->m_key)) parent->m_left = node;
        else parent->m_right = node;

        avl_node* p = node;
        while (p) { update(p); p = p->m_parent; }

        rebalance_up(node->m_parent);
        ++m_size;
        return node;
    }

    avl_node* next_for_erase(avl_node* n) {
        if (!n) return nullptr;
        if (n->m_right) return minimum_node(n->m_right);
        avl_node* p = n->m_parent;
        while (p && n == p->m_right) { n = p; p = p->m_parent; }
        return p;
    }

    void transplant(avl_node* u, avl_node* v) {
        if (!u->m_parent) m_root = v;
        else if (u == u->m_parent->m_left) u->m_parent->m_left = v;
        else u->m_parent->m_right = v;
        if (v) v->m_parent = u->m_parent;
    }

    void erase_node(avl_node* z) {
        avl_node* reb = nullptr;

        if (!z->m_left) {
            reb = z->m_parent;
            transplant(z, z->m_right);
        }
        else if (!z->m_right) {
            reb = z->m_parent;
            transplant(z, z->m_left);
        }
        else {
            avl_node* y = minimum_node(z->m_right);
            if (y->m_parent != z) {
                reb = y->m_parent;
                transplant(y, y->m_right);
                y->m_right = z->m_right;
                if (y->m_right) y->m_right->m_parent = y;
            } else {
                reb = y;
            }
            transplant(z, y);
            y->m_left = z->m_left;
            if (y->m_left) y->m_left->m_parent = y;
            update(y);
        }

        delete z;
        --m_size;

        while (reb) {
            update(reb);
            rebalance_up(reb);
            reb = reb->m_parent;
        }
    }

    void clear(avl_node* n) {
        if (!n) return;
        clear(n->m_left);
        clear(n->m_right);
        delete n;
    }

}; // class avl_tree

} // namespace student_std

#endif // STUDENT_AVL_TREE_H
