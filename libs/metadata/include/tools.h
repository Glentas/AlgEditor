#pragma once
#include "data_definitions.h"
#include <string>

namespace AlgorithmicEditor
{

// Maps figure type to it's string representation
std::string gtype_to_string(const GType &type);
// Fuzzy equal
bool f_eq(const double &a, const double &b);
// Appends all elements of the child to the end of the parent
void append(std::vector<Point> &parent, const std::vector<Point> &child);

} // AlgorithmicEditor