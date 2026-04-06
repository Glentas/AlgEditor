#pragma once
#include "data_definitions.h"
#include "figure_class.h"

#include <memory>
#include <string>
#include <vector>

namespace AlgorithmicEditor
{

class Tools
{
  public:
    // Maps figure type to it's string representation
    static std::string gtype_to_string(const GType type);

    // Fuzzy equal
    static bool f_eq(const double a, const double b);

    static std::unique_ptr<Figure> get_figure_object_by_type(const GType type);

    // Appends all elements of the child to the end of the parent
    template <typename T>
    static inline void append(std::vector<T> &parent,
                              const std::vector<T> &child)
    {
        if (child.empty()) {
            return;
        }
        parent.reserve(parent.size() + child.size());
        parent.insert(parent.end(), child.begin(), child.end());
    }
};
} // namespace AlgorithmicEditor