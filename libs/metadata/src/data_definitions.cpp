#include "data_definitions.h"

#include <cmath>

namespace AlgorithmicEditor
{
Point::Point(double x, double y, uint8_t color)
    : x(x), y(y), z(0.0), r(color), g(color), b(color)
{
}
Point::Point(double x, double y, uint8_t r, uint8_t g, uint8_t b)
    : x(x), y(y), z(0.0), r(r), g(g), b(b)
{
}
Point::Point(double x, double y, double z, uint8_t color)
    : x(x), y(y), z(z), r(color), g(color), b(color)
{
}
Point::Point(double x, double y, double z, uint8_t r, uint8_t g, uint8_t b)
    : x(x), y(y), z(z), r(r), g(g), b(b)
{
}
int Point::x_int() const { return static_cast<int>(std::round(this->x)); }
int Point::y_int() const { return static_cast<int>(std::round(this->y)); }
int Point::z_int() const { return static_cast<int>(std::round(this->z)); }
bool Point::operator==(const Point &other) const
{
    return (this->x_int() == other.x_int()) &&
           (this->y_int() == other.y_int()) && (this->z_int() == other.z_int());
}
} // namespace AlgorithmicEditor