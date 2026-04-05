#pragma once
#include "data_definitions.h"

#include <string>
#include <vector>

namespace AlgorithmicEditor
{

// Maps figure type to it's string representation
std::string gtype_to_string(const GType &type);

// Fuzzy equal
bool f_eq(const double &a, const double &b);

// Appends all elements of the child to the end of the parent
template <typename T>
inline void append(std::vector<T> &parent, const std::vector<T> &child)
{
    if (child.empty()) {
        return;
    }
    parent.reserve(parent.size() + child.size());
    parent.insert(parent.end(), child.begin(), child.end());
}
} // namespace AlgorithmicEditor