/*
// sprvector class provides an efficient representation of vectors with a large number of zero elements.
// It uses an unordered_map to store only the non-zero elements, reducing memory usage and improving performance for sparse data.
// The class supports basic operations such as element access, addition, scalar multiplication, and conversion to a dense vector.

// Example usage of sprvector
// Create a sparse vector of size 10
sprvector v1(10);

// Set some elements
v1[2] = 3.5;
v1[5] = 7.2;

// Access elements
std::cout << "v1[2] = " << v1[2] << std::endl; // Output: 3.5
std::cout << "v1[3] = " << v1[3] << std::endl; // Output: 0.0 (default for unset elements)

// Perform scalar multiplication
sprvector v2 = v1 * 2.0;
std::cout << "v2[2] = " << v2[2] << std::endl; // Output: 7.0

// Add two sparse vectors
sprvector v3(10);
v3[5] = 1.8;
sprvector v4 = v1 + v3;
std::cout << "v4[5] = " << v4[5] << std::endl; // Output: 9.0

// Convert to dense vector
std::vector<double> dense = v4.toDense();
std::cout << "Dense vector: ";
for (double val : dense) {
    std::cout << val << " ";
}
std::cout << std::endl;

// Iterate over non-zero elements
std::cout << "Non-zero elements in v1: ";
for (auto& [index, value] : v1) {
    std::cout << "(" << index << ", " << value << ") ";
}
std::cout << std::endl;
*/


# pragma once
#include <unordered_map>
#include <vector>
#include <stdexcept>

class sprvector {
private:
    std::unordered_map<size_t, double> data;
    size_t size;

public:
    // Constructor
    explicit sprvector(size_t size) : size(size) {}

    // Copy constructor
    sprvector(const sprvector& other) : size(other.size), data(other.data) {}

    // Move constructor
    sprvector(sprvector&& other) noexcept : size(other.size), data(std::move(other.data)) {
        other.size = 0;
    }

    // Assignment operator
    sprvector& operator=(const sprvector& other) {
        if (this != &other) {
            size = other.size;
            data = other.data;
        }
        return *this;
    }

    // Move assignment operator
    sprvector& operator=(sprvector&& other) noexcept {
        if (this != &other) {
            size = other.size;
            data = std::move(other.data);
            other.size = 0;
        }
        return *this;
    }

    // Destructor
    ~sprvector() = default;

    // Get size of the vector
    size_t getSize() const {
        return size;
    }

    // Access element (read/write)
    double& operator[](size_t index) {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        return data[index];
    }

    // Access element (read-only)
    double operator[](size_t index) const {
        if (index >= size) {
            throw std::out_of_range("Index out of range");
        }
        auto it = data.find(index);
        return it != data.end() ? it->second : 0.0;
    }

    // Add two sparse vectors
    sprvector operator+(const sprvector& other) const {
        if (size != other.size) {
            throw std::invalid_argument("Vectors must be of the same size");
        }
        sprvector result(size);
        for (const auto& [index, value] : data) {
            result.data[index] = value + other[index];
        }
        for (const auto& [index, value] : other.data) {
            if (data.find(index) == data.end()) {
                result.data[index] = value;
            }
        }
        return result;
    }

    // Scalar multiplication
    sprvector operator*(double scalar) const {
        sprvector result(size);
        for (const auto& [index, value] : data) {
            result.data[index] = value * scalar;
        }
        return result;
    }

    // Iterator class for sprvector, support: for (auto &[i, val] : r)
    class iterator {
    private:
        std::unordered_map<size_t, double>::iterator it;

    public:
        using iterator_category = std::forward_iterator_tag;
        using value_type = std::pair<const size_t, double>;
        using difference_type = std::ptrdiff_t;
        using pointer = value_type*;
        using reference = value_type&;

        explicit iterator(std::unordered_map<size_t, double>::iterator it) : it(it) {}

        reference operator*() const {
            return *it;
        }

        pointer operator->() const {
            return &(*it);
        }

        iterator& operator++() {
            ++it;
            return *this;
        }

        iterator operator++(int) {
            iterator temp = *this;
            ++it;
            return temp;
        }

        bool operator==(const iterator& other) const {
            return it == other.it;
        }

        bool operator!=(const iterator& other) const {
            return it != other.it;
        }
    };

    // Begin iterator
    iterator begin() {
        return iterator(data.begin());
    }

    // End iterator
    iterator end() {
        return iterator(data.end());
    }

    // Convert to dense vector
    std::vector<double> toDense() const {
        std::vector<double> dense(size, 0.0);
        for (const auto& [index, value] : data) {
            dense[index] = value;
        }
        return dense;
    }
};
