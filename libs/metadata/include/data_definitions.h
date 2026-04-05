#pragma once
#include <cstdint>

namespace AlgorithmicEditor
{
using std::uint16_t;
using std::uint8_t;

namespace Config
{
inline constexpr int CODE_ERROR = -1;
inline constexpr int CELL_SIZE = 16;
inline constexpr int INIT_WIDTH = 48 * CELL_SIZE;
inline constexpr int INIT_HEIGHT = 48 * CELL_SIZE;
inline constexpr int OFFSET = CELL_SIZE / 2;
inline constexpr int CANVAS_SIZE = 128 * CELL_SIZE;

inline constexpr int MIN_WINDOW_WIDTH = (OFFSET + CANVAS_SIZE) / 4;
inline constexpr int MIN_WINDOW_HEIGHT = MIN_WINDOW_WIDTH;

inline constexpr int MAX_WINDOW_WIDTH =
    (CANVAS_SIZE <= MIN_WINDOW_WIDTH) ? MIN_WINDOW_WIDTH : CANVAS_SIZE;
inline constexpr int MAX_WINDOW_HEIGHT = MAX_WINDOW_WIDTH;

inline constexpr double E_FACTOR = 1e-6;
} // namespace Config

enum class GType : uint8_t {
    // Lines
    CDA,
    Bresenham,
    Wu,
    // Second rank lines
    Circle,
    Ellipse,
    Parabola,
    Hyperbola,
    // Interpolation
    Hermite,
    Bezier,
    BSpline,
    // 3D
    Cube,
    Tetrahedron,
    // Polygons
    Polygon,
    ConvexPolygon,
    // Polygonalisation
    Delaunay,
    Voronoi
};

class Point
{
  public:
    Point() = delete;
    Point(double x, double y, uint8_t color);
    Point(double x, double y, uint8_t r, uint8_t g, uint8_t b);
    Point(double x, double y, double z, uint8_t color);
    Point(double x, double y, double z, uint8_t r, uint8_t g, uint8_t b);
    int x_int() const;
    int y_int() const;
    int z_int() const;
    bool operator==(const Point &other) const;
    double x;
    double y;
    double z;
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

} // namespace AlgorithmicEditor