#include "data_definitions.h"
#include <cmath>

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

void Figure::add_point(const Point pt)
{
	this->points.push_back(pt);
}

} // AlgorithmicEditor