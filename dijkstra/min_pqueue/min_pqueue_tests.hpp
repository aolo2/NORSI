//
// Created by aolo2 on 7/11/17.
//

#ifndef NORSI_MIN_PQUEUE_TESTS_HPP
#define NORSI_MIN_PQUEUE_TESTS_HPP

#include <iostream>
#include <vector>

#include "min_pqueue.hpp"

template<typename T>
int load_unload(const std::vector<T> &data) {
    norsi::min_pqueue<T> queue(data.size());

    for (const auto &item : data) {
        queue.push(item);
    }

    try {

        if (queue.size() != data.size()) {
            throw -1;
        }

        while (!queue.empty()) {
            std::cout << queue.min_element() << std::endl;
            queue.pop();
        }
    } catch (...) {
        std::cerr << "load_unload pqueue test failed" << std::endl;
        return 1;
    }

    return 0;
}

template<typename T>
int check_if_contains(const std::vector<T> &data, const std::vector<T> &check_set) {
    norsi::min_pqueue<T> queue(data);

    for (const auto &item : check_set) {
        std::cout << item;
        if (!queue.contains(item)) {
            std::cout << " not";
        }

        std::cout << " in queue" << std::endl;
    }

    return 0;
}

template<typename T>
int load_array(const std::vector<T> &data) {

    norsi::min_pqueue<T> queue(data);

    try {

        if (queue.size() != data.size()) {
            throw -1;
        }

        while (!queue.empty()) {
            std::cout << queue.min_element() << std::endl;
            queue.pop();
        }
    } catch (...) {
        std::cerr << "load_array pqueue test failed" << std::endl;
        return 1;
    }

    return 0;
}


#endif //NORSI_MIN_PQUEUE_TESTS_HPP
