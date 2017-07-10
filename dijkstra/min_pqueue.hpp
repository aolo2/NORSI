//
// Created by aolo2 on 7/10/17.
//

#ifndef NORSI_MIN_PRIORITY_QUEUE_HPP
#define NORSI_MIN_PRIORITY_QUEUE_HPP

#include <unordered_set>

namespace norsi {

    template<typename T>
    class min_pqueue {
    public:
        min_pqueue(std::size_t n);

        void push(const T &item);

        T min_element();

        void pop();

        inline bool contains(const T &item) {
            return contents_set.find(item) != contents_set.end();
        }

        inline bool empty() { return heap.empty(); }

    private:
        void heapify(std::size_t i, std::size_t n);

        std::unordered_set<T> contents_set;
        std::vector<T> heap;
    };

    template<typename T>
    min_pqueue<T>::min_pqueue(std::size_t n) {
        heap.reserve(n);
    }

    template<typename T>
    T min_pqueue<T>::min_element() {
        if (heap.size() == 0) {
            std::cerr << "Queue is empty" << std::endl;
        }

        return heap[0];
    }

    template<typename T>
    void min_pqueue<T>::push(const T &item) {
        std::size_t i = heap.size();

        heap.push_back(item);

        while (i > 0 && (heap[(i - 1) / 2] > heap[i])) {
            std::swap(heap[(i - 1) / 2], heap[i]);
            i = (i - 1) / 2;
        }


        contents_set.insert(item);
    }

    template<typename T>
    void min_pqueue<T>::pop() {
        if (heap.empty()) {
            std::cerr << "Queue is empty" << std::endl;
            return;
        }

        contents_set.erase(heap[0]);

        if (heap.size() - 1 > 0) {
            heap[0] = heap[heap.size() - 1];
            heapify(0, heap.size() - 1);
        }

        heap.pop_back();
    }

    template<typename T>
    void min_pqueue<T>::heapify(std::size_t i, std::size_t n) {
        while (true) {
            std::size_t l = 2 * i + 1;
            std::size_t r = l + 1;
            std::size_t j = i;

            if (l < n && heap[i] > heap[l]) {
                i = l;
            }

            if (r < n && heap[i] > heap[r]) {
                i = r;
            }

            if (i == j) {
                break;
            }

            std::swap(heap[i], heap[j]);
        }
    }
}
#endif //NORSI_MIN_PRIORITY_QUEUE_HPP
