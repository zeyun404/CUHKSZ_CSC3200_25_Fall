#ifndef STUDENT_PRIORITY_QUEUE_H
#define STUDENT_PRIORITY_QUEUE_H

#include <list>
#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>

namespace student_std {

template <typename T, typename Container = std::list<T>>
class priority_queue {
public:
    using container_type = Container;
    using value_type = typename Container::value_type;
    using size_type = typename Container::size_type;

    priority_queue() = default;
    priority_queue(const priority_queue&) = default;
    priority_queue& operator=(const priority_queue&) = default;

    const T& top() const { return container_.back(); }
    void pop() { container_.pop_back(); }

    size_type size() const noexcept { return container_.size(); }
    bool empty() const noexcept { return container_.empty(); }

    // push，保持升序排列，back() 为最大
    template <typename U>
    void push(U&& value) {
        if (container_.empty()) {
            container_.push_back(std::forward<U>(value));
            return;
        }

        auto it = container_.begin();
        for (; it != container_.end(); ++it) {
            if (value < *it) break;
        }
        container_.insert(it, std::forward<U>(value));
    }

    // 原地交换，不复制
    void swap(priority_queue& other) noexcept {
        container_.swap(other.container_);
    }

private:
    Container container_;
};

} // namespace student_std

#endif // STUDENT_PRIORITY_QUEUE_H
