#include "tools.h"

#include "data_definitions.h"

#include <cmath>
#include <string>

namespace AlgorithmicEditor
{

std::string gtype_to_string(const GType &type)
{
    switch (type) {
    case GType::CDA:
        return "CDA";
    case GType::Bresenham:
        return "Bresenham";
    case GType::Wu:
        return "Wu";
    case GType::Circle:
        return "Circle";
    case GType::Ellipse:
        return "Ellipse";
    case GType::Parabola:
        return "Parabola";
    case GType::Hyperbola:
        return "Hyperbola";
    case GType::Hermite:
        return "Hermite";
    case GType::Bezier:
        return "Bezier";
    case GType::BSpline:
        return "BSpline";
    case GType::Cube:
        return "Cube";
    case GType::Tetrahedron:
        return "Tetrahedron";
    case GType::Polygon:
        return "Polygon";
    case GType::ConvexPolygon:
        return "ConvexPolygon";
    case GType::Delaunay:
        return "Delaunay";
    case GType::Voronoi:
        return "Voronoi";
    default:
        return "Unknown";
    }
}

bool f_eq(const double &a, const double &b)
{
    return std::abs(a - b) <= Config::E_FACTOR;
}

} // namespace AlgorithmicEditor