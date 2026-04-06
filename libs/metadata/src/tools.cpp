#include "tools.h"

#include "data_definitions.h"
#include "figure_class.h"

#include <cmath>
#include <memory>
#include <string>

namespace AlgorithmicEditor
{

std::string Tools::gtype_to_string(const GType type)
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

bool Tools::f_eq(const double a, const double b)
{
    return std::abs(a - b) <= Config::E_FACTOR;
}

std::unique_ptr<Figure> Tools::get_figure_object_by_type(const GType type)
{
    switch (type) {
    case GType::CDA:
        return std::make_unique<CDAFigure>();
    case GType::Bresenham:
        return std::make_unique<BresenhamFigure>();
    case GType::Wu:
        return std::make_unique<WuFigure>();
    case GType::Circle:
        return std::make_unique<CircleFigure>();
    case GType::Ellipse:
        return std::make_unique<EllipseFigure>();
    case GType::Parabola:
        return std::make_unique<ParabolaFigure>();
    case GType::Hyperbola:
        return std::make_unique<HyperbolaFigure>();
    case GType::Hermite:
        return std::make_unique<HermiteFigure>();
    case GType::Bezier:
        return std::make_unique<BezierFigure>();
    case GType::BSpline:
        return std::make_unique<BSplineFigure>();
    case GType::Cube:
        return std::make_unique<CubeFigure>();
    case GType::Tetrahedron:
        return std::make_unique<TetrahedronFigure>();
    case GType::Polygon:
        return std::make_unique<PolygonFigure>();
    case GType::ConvexPolygon:
        return std::make_unique<ConvexPolygonFigure>();
    case GType::Delaunay:
        return std::make_unique<DelaunayFigure>();
    case GType::Voronoi:
        return std::make_unique<VoronoiFigure>();
    default:
        return std::make_unique<CDAFigure>();
    }
}
} // namespace AlgorithmicEditor