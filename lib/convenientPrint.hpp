/*
    This header file provides convenient printing utilities for various data types.
    It includes overloaded operators for printing vectors, tuples, pairs, and unordered maps,
    as well as a generic print function for printing multiple arguments with high precision.

    Example usage:
    - Print a vector:
        std::vector<int> vec = {1, 2, 3};
        std::cout << vec << std::endl; // Output: [1, 2, 3]

    - Print a tuple:
        std::tuple<int, double, std::string> t = {1, 2.5, "hello"};
        std::cout << t << std::endl; // Output: (1, 2.5, hello)

    - Print a pair:
        std::pair<int, std::string> p = {1, "one"};
        std::cout << p << std::endl; // Output: (1, one)

    - Print an unordered_map:
        std::unordered_map<int, std::string> m = {{1, "one"}, {2, "two"}};
        std::cout << m << std::endl; // Output: {1:one, 2:two}

    - Use the print function:
        print(1, 2.5, "hello"); // Output: 1 2.5 hello
*/

#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <tuple>
#include <iomanip>

// reload vector operator<<
template<typename T>
std::ostream& operator<<(std::ostream& os, std::vector<T> vec){
    os << "[";
    for(auto it=vec.begin();it!=vec.end();++it){
        os << *it;
        if(it!=vec.end()-1){
            os << ", ";
        }
    }
    os << "]";
    return os;
}

template<std::size_t I = 0, typename... Types>
void print_tuple(std::ostream& os, const std::tuple<Types...>& t) {
    if constexpr (I < sizeof...(Types)) {
        if constexpr (I != 0)
            os << ", ";
        os << std::get<I>(t);
        print_tuple<I + 1>(os, t);
    }
}

// reload tuple operator<<
template<typename... Types>
std::ostream& operator<<(std::ostream& os, const std::tuple<Types...>& t) {
    os << "(";
    print_tuple(os, t);
    os << ")";
    return os;
}

// reload pair operator<<
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os, std::pair<T1,T2> p){
    os << "(" << p.first << ", " << p.second << ")";
    return os;
}

// reload unordered_map operator<<
template<typename T1, typename T2>
std::ostream& operator<<(std::ostream& os,const std::unordered_map<T1,T2> m){
    os << "{";
    for(auto it=m.begin();it!=m.end();++it){
        os << (*it).first << ":" << (*it).second;
        if (std::next(it) != m.end()) {
            std::cout << ", ";
        }
    }
    os << "}";
    return os;
}

// print function
template<typename... Args>
void print(Args... args) {
    auto print_arg = [](auto arg) {
        std::cout << std::setprecision(16) << arg << " ";
    };

    (void)std::initializer_list<int>{(print_arg(args), 0)...};
    std::cout << std::endl;
}


