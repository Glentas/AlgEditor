#pragma once
#include "polygon.h"
#include <vector>
#include "data_definitions.h"

using AlgorithmicEditor::Point;

using Edge = std::pair<Point, Point>;
using EdgeVector = std::vector<Edge>;

Edge get_start_edge(const std::vector<Point> &pts);
bool circle_from_3_Points(const Point &p1, const Point &p2, const Point &p3,
			  Point &center, float &radius);
std::vector<Point> get_oncircle_Points(const Edge &e,
				       const std::vector<Point> &pts);
void update_alive_dead(EdgeVector &alive, EdgeVector &dead, const Edge &a);
std::vector<Point> draw_delaunay(const std::vector<Point> &pts);

std::vector<Point> get_bounding_box(const Point &min_xy, const Point &max_xy);
std::vector<Point>
intersect_half_plane(const std::vector<Point> &current_region, const Point &p0,
		     const Point &p1, const Point &min_xy, const Point &max_xy);
std::vector<Point> get_voronoi_cell_for_Point(const Point &p0,
					      const std::vector<Point> &pts,
					      const Point &min_xy,
					      const Point &max_xy);
std::vector<Point> get_voronoi_cell_vertexes(const std::vector<Point> &pts);
std::vector<Point> draw_voronoi(const std::vector<Point> &pts);
