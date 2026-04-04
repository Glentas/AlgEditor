#include "data_definitions.h"
#include <cmath>
#include "polygonalisation.h"
#include "tools.h"
#include "first_rank_lines.h"
#include <vector>
#include "second_rank_lines.h"
#include "interpolation.h"

namespace AlgorithmicEditor
{
Point::Point(double x, double y, uint8_t color)
	: x(x)
	, y(y)
	, z(0.0)
	, r(color)
	, g(color)
	, b(color)
{
}
Point::Point(double x, double y, uint8_t r, uint8_t g, uint8_t b)
	: x(x)
	, y(y)
	, z(0.0)
	, r(r)
	, g(g)
	, b(b)
{
}
Point::Point(double x, double y, double z, uint8_t color)
	: x(x)
	, y(y)
	, z(z)
	, r(color)
	, g(color)
	, b(color)
{
}
Point::Point(double x, double y, double z, uint8_t r, uint8_t g, uint8_t b)
	: x(x)
	, y(y)
	, z(z)
	, r(r)
	, g(g)
	, b(b)
{
}
int Point::x_int() const
{
	return static_cast<int>(std::round(this->x));
}
int Point::y_int() const
{
	return static_cast<int>(std::round(this->y));
}
int Point::z_int() const
{
	return static_cast<int>(std::round(this->z));
}

Figure::Figure(GType figure_type)
	: figure_type(figure_type)
	, is_cache_valid(false)
{
}

void Figure::set_id(uint16_t id)
{
	this->id = id;
}
void Figure::set_figure_type(GType figure_type)
{
	this->figure_type = figure_type;
}
void Figure::set_points(const std::vector<Point> &points)
{
	this->points = points;
}

uint16_t Figure::get_id() const
{
	return this->id;
}
GType Figure::get_figure_type() const
{
	return this->figure_type;
}
const std::vector<Point> &Figure::get_points() const
{
	return this->points;
}
const std::vector<Point> &Figure::get_connected_points() const
{
	if (this->is_cache_valid == false) {
		this->connected_points_cache.clear();
		this->connect_figure_points(this->connected_points_cache);
		this->is_cache_valid = true;
	}
	return this->connected_points_cache;
}

void Figure::invalidate_cache() const
{
	this->is_cache_valid = false;
}

void Figure::add_point(const Point &pt)
{
	this->points.push_back(pt);
}

void CDAFigure::connect_figure_points(std::vector<Point> &cache_points) const
{
	if (this->get_points().size() == 2) {
		const std::vector<Point> &pts = this->get_points();
		cache_points = draw_cda(pts[0], pts[1]);
	}
}
bool CDAFigure::add_and_validate_point(const Point &pt)
{
	if (this->get_points().size() < 2) {
		this->add_point(pt);
	}
	if (this->get_points().size() == 2) {
		return true;
	}
	return false;
}

void BresenhamFigure::connect_figure_points(
	std::vector<Point> &cache_points) const
{
	if (this->get_points().size() == 2) {
		const std::vector<Point> &pts = this->get_points();
		cache_points = draw_bresenham(pts[0], pts[1]);
	}
}
bool BresenhamFigure::add_and_validate_point(const Point &pt)
{
	if (this->get_points().size() < 2) {
		this->add_point(pt);
	}
	if (this->get_points().size() == 2) {
		return true;
	}
	return false;
}

void WuFigure::connect_figure_points(std::vector<Point> &cache_points) const
{
	if (this->get_points().size() == 2) {
		const std::vector<Point> &pts = this->get_points();
		cache_points = draw_wu(pts[0], pts[1]);
	}
}
bool WuFigure::add_and_validate_point(const Point &pt)
{
	if (this->get_points().size() < 2) {
		this->add_point(pt);
	}
	if (this->get_points().size() == 2) {
		return true;
	}
	return false;
}

void CircleFigure::connect_figure_points(std::vector<Point> &cache_points) const
{
	if (this->get_points().size() == 2) {
		const std::vector<Point> &pts = this->get_points();

		int radius = std::round(
			(std::sqrt(std::pow(pts[0].x - pts[1].x, 2) +
				   std::pow(pts[0].y - pts[1].y, 2))));

		cache_points = draw_circle(pts[0], radius);
	}
}
bool CircleFigure::add_and_validate_point(const Point &pt)
{
	if (this->get_points().size() < 2) {
		this->add_point(pt);
	}
	if (this->get_points().size() == 2) {
		return true;
	}
	return false;
}

void EllipseFigure::connect_figure_points(std::vector<Point> &cache_points) const
{
	if (this->get_points().size() == 2) {
		const std::vector<Point> &pts = this->get_points();

		int a = std::round(std::abs(pts[0].x - pts[1].x));
		int b = std::round(std::abs(pts[0].y - pts[1].y));

		cache_points = draw_ellipse(pts[0], a, b);
	}
}
bool EllipseFigure::add_and_validate_point(const Point &pt)
{
	if (this->get_points().size() < 2) {
		this->add_point(pt);
	}
	if (this->get_points().size() == 2) {
		return true;
	}
	return false;
}

void ParabolaFigure::connect_figure_points(
	std::vector<Point> &cache_points) const
{
	if (this->get_points().size() == 2) {
		const std::vector<Point> &pts = this->get_points();

		int p = std::round(
			(std::sqrt(std::pow(pts[0].x - pts[1].x, 2) +
				   std::pow(pts[0].y - pts[1].y, 2))));

		cache_points = draw_parabola(pts[0], p);
	}
}
bool ParabolaFigure::add_and_validate_point(const Point &pt)
{
	if (this->get_points().size() < 2) {
		this->add_point(pt);
	}
	if (this->get_points().size() == 2) {
		return true;
	}
	return false;
}

void HyperbolaFigure::connect_figure_points(
	std::vector<Point> &cache_points) const
{
	if (this->get_points().size() == 2) {
		const std::vector<Point> &pts = this->get_points();

		int a = std::round(std::abs(pts[0].x - pts[1].x));
		int b = std::round(std::abs(pts[0].y - pts[1].y));

		cache_points = draw_hyperbola(pts[0], a, b);
	}
}
bool HyperbolaFigure::add_and_validate_point(const Point &pt)
{
	if (this->get_points().size() < 2) {
		this->add_point(pt);
	}
	if (this->get_points().size() == 2) {
		return true;
	}
	return false;
}

void HermiteFigure::connect_figure_points(std::vector<Point> &cache_points) const
{
	if (this->get_points().size() == 4) {
		const std::vector<Point> &pts = this->get_points();

		cache_points = draw_hermite(pts[0], pts[1], pts[2], pts[3]);
	}
}
bool HermiteFigure::add_and_validate_point(const Point &pt)
{
	if (this->get_points().size() < 4) {
		this->add_point(pt);
	}
	if (this->get_points().size() == 4) {
		return true;
	}
	return false;
}

void BezierFigure::connect_figure_points(std::vector<Point> &cache_points) const
{
	if (this->get_points().size() == 4) {
		const std::vector<Point> &pts = this->get_points();

		cache_points = draw_bezier(pts[0], pts[1], pts[2], pts[3]);
	}
}
bool BezierFigure::add_and_validate_point(const Point &pt)
{
	if (this->get_points().size() < 4) {
		this->add_point(pt);
	}
	if (this->get_points().size() == 4) {
		return true;
	}
	return false;
}

void BSplineFigure::connect_figure_points(std::vector<Point> &cache_points) const
{
	if (this->get_points().size() >= 4) {
		const std::vector<Point> &pts = this->get_points();

		cache_points = draw_spline(pts);
	}
}
bool BSplineFigure::add_and_validate_point(const Point &pt)
{
	this->add_point(pt);
	return false;
}

void CubeFigure::connect_figure_points(std::vector<Point> &cache_points) const
{
	if (this->get_points().size() == 8) {
		const std::vector<Point> &pts = this->get_points();

		append(cache_points, draw_cda(pts[0], pts[1]));
		append(cache_points, draw_cda(pts[1], pts[2]));
		append(cache_points, draw_cda(pts[2], pts[3]));
		append(cache_points, draw_cda(pts[3], pts[0]));

		append(cache_points, draw_cda(pts[4], pts[5]));
		append(cache_points, draw_cda(pts[5], pts[6]));
		append(cache_points, draw_cda(pts[6], pts[7]));
		append(cache_points, draw_cda(pts[7], pts[4]));

		append(cache_points, draw_cda(pts[0], pts[4]));
		append(cache_points, draw_cda(pts[1], pts[5]));
		append(cache_points, draw_cda(pts[2], pts[6]));
		append(cache_points, draw_cda(pts[3], pts[7]));
	}
}
bool CubeFigure::add_and_validate_point(const Point &pt)
{
	if (this->get_points().size() == 1) {
		const std::vector<Point> &pts = this->get_points();

		// Edge length
		double length = static_cast<double>(
			std::round(std::sqrt(std::pow(pts[0].x - pt.x, 2) +
					     std::pow(pts[0].y - pt.y, 2))));

		this->add_point({ pts[0].x, pts[0].y, length, 0 });

		this->add_point({ pts[0].x, pts[0].y + length, length, 0 });

		this->add_point({ pts[0].x, pts[0].y + length, 0, 0 });

		this->add_point({ pts[0].x + length, pts[0].y, 0, 0 });

		this->add_point({ pts[0].x + length, pts[0].y, length, 0 });

		this->add_point(
			{ pts[0].x + length, pts[0].y + length, length, 0 });

		this->add_point({ pts[0].x + length, pts[0].y + length, 0, 0 });

		return true;
	}

	if (this->get_points().size() < 1) {
		this->add_point({ pt.x, pt.y, 0.0, 0 });
	}

	return false;
}

void TetrahedronFigure::connect_figure_points(
	std::vector<Point> &cache_points) const
{
	if (this->get_points().size() == 4) {
		const std::vector<Point> &pts = this->get_points();

		append(cache_points, draw_cda(pts[0], pts[1]));
		append(cache_points, draw_cda(pts[1], pts[2]));
		append(cache_points, draw_cda(pts[2], pts[0]));

		append(cache_points, draw_cda(pts[0], pts[3]));
		append(cache_points, draw_cda(pts[1], pts[3]));
		append(cache_points, draw_cda(pts[2], pts[3]));
	}
}

bool TetrahedronFigure::add_and_validate_point(const Point &pt)
{
	if (this->get_points().size() == 1) {
		const std::vector<Point> &pts = this->get_points();

		// Edge length
		double length = static_cast<double>(
			std::round(std::sqrt(std::pow(pts[0].x - pt.x, 2) +
					     std::pow(pts[0].y - pt.y, 2))));

		double h = length * sqrt(2.0 / 3.0);
		double r_base = length / sqrt(3.0);
		double r_center = length / (2.0 * sqrt(3.0));

		this->add_point({ pts[0].x + length, pts[0].y, 0, 0 });

		this->add_point({ pts[0].x + length / 2.0,
				  pts[0].y + length * sqrt(3.0) / 2.0, 0, 0 });

		double center_x = (pts[0].x + (pts[0].x + length) +
				   (pts[0].x + length / 2.0)) /
				  3.0;
		double center_y = (pts[0].y + pts[0].y +
				   (pts[0].y + length * sqrt(3.0) / 2.0)) /
				  3.0;

		this->add_point({ center_x, center_y, h, 0 });

		return true;
	}

	if (this->get_points().size() < 1) {
		this->add_point({ pt.x, pt.y, 0.0, 0 });
	}

	return false;
}

void PolygonFigure::connect_figure_points(std::vector<Point> &cache_points) const
{
	if (this->get_points().size() >= 3) {
		const std::vector<Point> &pts = this->get_points();

		for (int i = 0; i < pts.size() - 1; i++) {
			append(cache_points, draw_cda(pts[i], pts[i + 1]));
		}
		append(cache_points, draw_cda(

					     pts[pts.size() - 1], pts[0]));
	}
}
bool PolygonFigure::add_and_validate_point(const Point &pt)
{
	this->add_point(pt);
	return false;
}

void ConvexPolygonFigure::connect_figure_points(
	std::vector<Point> &cache_points) const
{
	if (this->get_points().size() >= 3) {
		const std::vector<Point> &pts = this->get_points();

		append(cache_points, draw_convex_polygon(pts));
	}
}
bool ConvexPolygonFigure::add_and_validate_point(const Point &pt)
{
	this->add_point(pt);
	return false;
}

void DelaunayFigure::connect_figure_points(
	std::vector<Point> &cache_points) const
{
	if (this->get_points().size() >= 3) {
		const std::vector<Point> &pts = this->get_points();

		append(cache_points, draw_delaunay(pts));
	}
}
bool DelaunayFigure::add_and_validate_point(const Point &pt)
{
	this->add_point(pt);
	return false;
}

void VoronoiFigure::connect_figure_points(std::vector<Point> &cache_points) const
{
	if (this->get_points().size() >= 3) {
		const std::vector<Point> &pts = this->get_points();

		append(cache_points, draw_voronoi(pts));
	}
}
bool VoronoiFigure::add_and_validate_point(const Point &pt)
{
	this->add_point(pt);
	return false;
}
} // AlgorithmicEditor