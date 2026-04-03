#pragma once

#include <cstdint>
#include <vector>

namespace AlgorithmicEditor
{
using std::uint8_t;
using std::uint16_t;

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

inline constexpr int CONTROL_POINTS = 8192;
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
	// Area fill
	FillOrderedEdges,
	FillActiveEdges,
	FillSeedSimple,
	FillSeedScanline,
	Dot,
	// Polygonalisation
	Delaunay,
	Voronoi
};

class Point {
    public:
	Point() = delete;
	Point(double x, double y, uint8_t color);
	Point(double x, double y, uint8_t r, uint8_t g, uint8_t b);
	Point(double x, double y, double z, uint8_t color);
	Point(double x, double y, double z, uint8_t r, uint8_t g, uint8_t b);
	int x_int() const;
	int y_int() const;
	int z_int() const;

	double x;
	double y;
	double z;
	uint8_t r;
	uint8_t g;
	uint8_t b;
};

class Figure {
    public:
	Figure() = delete;
	Figure(GType figure_type);
	virtual ~Figure() = default;

	void set_id(uint16_t id);
	void set_figure_type(GType figure_type);
	void set_points(const std::vector<Point> &points);

	uint16_t get_id() const;
	GType get_figure_type() const;
	const std::vector<Point> &get_points() const;
	const std::vector<Point> &get_connected_points() const;

	void invalidate_cache() const;
	void add_point(const Point pt);

    protected:
	virtual std::vector<Point>
	connect_figure_points(std::vector<Point> &points) const = 0;

    private:
	uint16_t id;
	GType figure_type;
	std::vector<Point> points;
	mutable bool is_cache_valid;
	mutable std::vector<Point> connected_points_cache;
};
} // AlgorithmicEditor