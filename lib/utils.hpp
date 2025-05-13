/**
 * @file utils.hpp
 * @brief Utility functions and helpers for Spectral Algorithms.
 *
 * This header file is intended to serve as a collection of utility functions
 * and commonly used helpers that can be shared across the project. Developers
 * are encouraged to contribute reusable and efficient functions here to
 * promote code modularity and maintainability.
 *
 * Please ensure that any additions are well-documented and tested to maintain
 * the quality and reliability of this utility library.
 */
# pragma once

#include <iostream>
#include <filesystem>
#include <vector>

#include "convenientPrint.hpp"


// Ensures the existence of a directory.
void ensure_dir(std::string dir){
    if (!std::filesystem::exists(dir)){
        std::filesystem::create_directory(dir);
    }
}

