#ifndef STUDENT_UNORDERED_MAP_H
#define STUDENT_UNORDERED_MAP_H

#include <vector>
#include <list>
#include <utility>
#include <cstddef>

namespace student_std {

template <typename Key, typename T, typename Hash>
class unordered_map {
public:
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using key_type = Key;
    using mapped_type = T;
    using value_type = std::pair<Key, T>;
    using hasher = Hash;
    using bucket_type = std::list<value_type>;

private:
    hasher m_hasher;
    std::vector<bucket_type> m_table;
    size_type m_size;
    size_type m_buckets;

    static constexpr size_type initial_buckets() noexcept { return 16u; }

public:
    unordered_map()
        : m_table(initial_buckets()), m_size(0), m_buckets(initial_buckets()) {}

    unordered_map(unordered_map const &other)
        : m_hasher(other.m_hasher), m_table(other.m_table),
          m_size(other.m_size), m_buckets(other.m_buckets) {}

    unordered_map &operator=(unordered_map const &other) {
        if (this != &other) {
            m_hasher = other.m_hasher;
            m_table = other.m_table;
            m_size = other.m_size;
            m_buckets = other.m_buckets;
        }
        return *this;
    }

    size_type size() const noexcept { return m_size; }
    size_type bucket_count() const noexcept { return m_buckets; }
    bool empty() const noexcept { return m_size == 0; }

private:
    double load_factor() const noexcept {
        return static_cast<double>(m_size) / static_cast<double>(m_buckets);
    }

    void rehash(size_type new_buckets) {
        std::vector<bucket_type> new_table(new_buckets);

        for (auto &bucket : m_table) {
            for (auto &kv : bucket) {
                size_type idx = m_hasher(kv.first) % new_buckets;

                // FIX: 正确复制整个 pair，避免 key 移动破坏比较计数
                new_table[idx].push_back(kv);
            }
        }

        m_table.swap(new_table);
        m_buckets = new_buckets;
    }

public:
    T &operator[](Key const &key) {
        size_type idx = m_hasher(key) % m_buckets;
        bucket_type &bucket = m_table[idx];

        for (auto &kv : bucket) {
            if (kv.first == key) {
                return kv.second;
            }
        }

        if (load_factor() >= 2.0) {
            rehash(m_buckets * 2);
            idx = m_hasher(key) % m_buckets;
        }

        m_table[idx].push_back(std::make_pair(key, T()));
        ++m_size;

        return m_table[idx].back().second;
    }

    bool contains(Key const &key) const {
        size_type idx = m_hasher(key) % m_buckets;
        bucket_type const &bucket = m_table[idx];

        for (auto const &kv : bucket) {
            if (kv.first == key)
                return true;
        }
        return false;
    }

    size_type erase(Key const &key) {
        size_type idx = m_hasher(key) % m_buckets;
        bucket_type &bucket = m_table[idx];

        for (auto it = bucket.begin(); it != bucket.end(); ++it) {
            if (it->first == key) {
                bucket.erase(it);
                --m_size;
                return 1;
            }
        }
        return 0;
    }

    void clear() {
        for (auto &bucket : m_table) {
            bucket.clear();
        }
        m_size = 0;
    }
};

} // namespace student_std

#endif
