#include "polygonalisation.h"
#include "first_rank_lines.h"
#include "polygon.h"
#include <algorithm>
#include <climits>
#include <cmath>
#include <utility>
#include <vector>

#include "data_definitions.h"

using AlgorithmicEditor::Point;

Edge get_start_edge(const std::vector<Point> &pts)
{
	int leftmost_idx = 0;
	for (size_t i = 1; i < pts.size(); i++) {
		if (pts[i].x_int() < pts[leftmost_idx].x_int() ||
		    (pts[i].x_int() == pts[leftmost_idx].x_int() &&
		     pts[i].y_int() < pts[leftmost_idx].y_int())) {
			leftmost_idx = i;
		}
	}

	int next_idx = -1;
	const Point &p0 = pts[leftmost_idx];

	for (size_t i = 0; i < pts.size(); i++) {
		if (i == leftmost_idx)
			continue;

		if (next_idx == -1) {
			next_idx = i;
			continue;
		}

		const Point &p1 = pts[next_idx];
		const Point &p2 = pts[i];

		int v1x = p1.x_int() - p0.x_int();
		int v1y = p1.y_int() - p0.y_int();
		int v2x = p2.x_int() - p0.x_int();
		int v2y = p2.y_int() - p0.y_int();

		int cross = v1x * v2y - v1y * v2x;

		if (cross > 0) {
			next_idx = i;
		} else if (cross == 0) {
			int dist1 = v1x * v1x + v1y * v1y;
			int dist2 = v2x * v2x + v2y * v2y;
			if (dist2 < dist1) {
				next_idx = i;
			}
		}
	}

	return { pts[leftmost_idx], pts[next_idx] };
}

bool circle_from_3_Points(const Point &p1, const Point &p2, const Point &p3,
			  Point &center, double &radius)
{
	double x1 = p1.x, y1 = p1.y;
	double x2 = p2.x, y2 = p2.y;
	double x3 = p3.x, y3 = p3.y;

	double d = 2.0 * (x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2));
	if (fabs(d) < 1e-10) {
		return false;
	}

	double ux = ((x1 * x1 + y1 * y1) * (y2 - y3) +
		     (x2 * x2 + y2 * y2) * (y3 - y1) +
		     (x3 * x3 + y3 * y3) * (y1 - y2)) /
		    d;

	double uy = ((x1 * x1 + y1 * y1) * (x3 - x2) +
		     (x2 * x2 + y2 * y2) * (x1 - x3) +
		     (x3 * x3 + y3 * y3) * (x2 - x1)) /
		    d;

	center.x = ux;
	center.y = uy;

	double dx = ux - x1;
	double dy = uy - y1;
	radius = std::sqrt(dx * dx + dy * dy);

	return true;
}

std::vector<Point> get_oncircle_Points(const Edge &e,
				       const std::vector<Point> &pts)
{
	std::vector<Point> candidates;

	for (const auto &p : pts) {
		if (p == e.first || p == e.second)
			continue;

		Point center = { 0.0, 0.0, 0.0, 0 };
		double radius;

		if (circle_from_3_Points(e.first, e.second, p, center,
					 radius)) {
			bool is_empty = true;

			for (const auto &p2 : pts) {
				if (p2 == p || p2 == e.first || p2 == e.second)
					continue;

				double dx = center.x - p2.x;
				double dy = center.y - p2.y;
				double dist2 = dx * dx + dy * dy;

				if (dist2 < radius * radius - 1e-8) {
					is_empty = false;
					break;
				}
			}

			if (is_empty) {
				candidates.push_back(p);
			}
		}
	}

	return candidates;
}

bool is_alive(const EdgeVector &alive, const Edge &e)
{
	Edge rev_e = { e.second, e.first };
	return (std::find(alive.begin(), alive.end(), e) != alive.end() ||
		std::find(alive.begin(), alive.end(), rev_e) != alive.end());
}

void update_alive_dead(EdgeVector &alive, EdgeVector &dead, const Edge &e)
{
	Edge rev_e = { e.second, e.first };

	auto it = std::find(alive.begin(), alive.end(), e);
	auto rev_it = std::find(alive.begin(), alive.end(), rev_e);

	if (it != alive.end()) {
		alive.erase(it);
		dead.push_back(e);
	} else if (rev_it != alive.end()) {
		alive.erase(rev_it);
		dead.push_back(e);
	} else {
		if (std::find(dead.begin(), dead.end(), e) == dead.end() &&
		    std::find(dead.begin(), dead.end(), rev_e) == dead.end()) {
			alive.push_back(e);
		}
	}
}

std::vector<Point> draw_delaunay(const std::vector<Point> &pts)
{
	std::vector<Point> result;

	if (pts.size() < 3) {
		return result;
	}

	EdgeVector alive_edges, dead_edges;

	Edge start_edge = get_start_edge(pts);
	alive_edges.push_back(start_edge);

	while (!alive_edges.empty()) {
		Edge current_edge = alive_edges.back();

		update_alive_dead(alive_edges, dead_edges, current_edge);

		std::vector<Point> candidates =
			get_oncircle_Points(current_edge, pts);

		for (const auto &p : candidates) {
			Edge edge1 = { current_edge.first, p };
			Edge edge2 = { current_edge.second, p };

			if (!is_alive(alive_edges, edge1) &&
			    !is_alive(dead_edges, edge1)) {
				update_alive_dead(alive_edges, dead_edges,
						  edge1);
			}

			if (!is_alive(alive_edges, edge2) &&
			    !is_alive(dead_edges, edge2)) {
				update_alive_dead(alive_edges, dead_edges,
						  edge2);
			}
		}
	}

	for (const auto &e : dead_edges) {
		std::vector<Point> line = draw_cda(e.first, e.second);

		result.insert(result.end(), line.begin(), line.end());
	}

	return result;
}

//////

std::vector<Point> get_bounding_box(const Point &min_xy, const Point &max_xy)
{
	return { min_xy,
		 { max_xy.x, min_xy.y, 0.0, 0 },
		 max_xy,
		 { min_xy.x, max_xy.y, 0.0, 0 } };
}

std::vector<Point>
intersect_half_plane(const std::vector<Point> &current_region, const Point &p0,
		     const Point &p1, const Point &min_xy, const Point &max_xy)
{
	std::vector<Point> new_region;

	if (current_region.empty()) {
		return new_region;
	}

	Point mid = { (p0.x + p1.x) / 2.0, (p0.y + p1.y) / 2.0, 0.0, 0 };

	double dx = p1.x - p0.x;
	double dy = p1.y - p0.y;

	Point normal = { p0.x - mid.x, p0.y - mid.y, 0.0, 0 };

	double norm_len = std::sqrt(normal.x * normal.x + normal.y * normal.y);
	if (norm_len > 1e-6) {
		normal.x /= norm_len;
		normal.y /= norm_len;
	}

	for (size_t i = 0; i < current_region.size(); ++i) {
		const Point &p_curr = current_region[i];
		const Point &p_next =
			current_region[(i + 1) % current_region.size()];

		double val_curr = (p_curr.x - mid.x) * normal.x +
				  (p_curr.y - mid.y) * normal.y;
		double val_next = (p_next.x - mid.x) * normal.x +
				  (p_next.y - mid.y) * normal.y;

		if (val_curr >= -1e-6) {
			new_region.push_back(p_curr);
		}

		if ((val_curr > 1e-6 && val_next < -1e-6) ||
		    (val_curr < -1e-6 && val_next > 1e-6)) {
			double t = -val_curr / (val_next - val_curr);
			Point intersection = {
				p_curr.x + t * (p_next.x - p_curr.x),
				p_curr.y + t * (p_next.y - p_curr.y), 0.0, 0
			};
			new_region.push_back(intersection);
		}
	}

	return new_region;
}

std::vector<Point> get_voronoi_cell_for_Point(const Point &p0,
					      const std::vector<Point> &pts,
					      const Point &min_xy,
					      const Point &max_xy)
{
	std::vector<Point> region = get_bounding_box(min_xy, max_xy);

	for (const auto &p1 : pts) {
		if (p0 == p1)
			continue;

		region = intersect_half_plane(region, p0, p1, min_xy, max_xy);

		if (region.size() < 3) {
			return {};
		}
	}

	return region;
}

std::vector<Point> get_voronoi_cell_vertexes(const std::vector<Point> &pts)
{
	std::vector<Point> all_vertices;

	if (pts.size() < 3) {
		return all_vertices;
	}

	double min_x = pts[0].x, min_y = pts[0].y;
	double max_x = pts[0].x, max_y = pts[0].y;

	for (const auto &p : pts) {
		min_x = std::min(min_x, p.x);
		min_y = std::min(min_y, p.y);
		max_x = std::max(max_x, p.x);
		max_y = std::max(max_y, p.y);
	}

	double padding = std::max((max_x - min_x) * 0.2, 5.0);
	Point min_xy = { min_x - padding, min_y - padding, 0.0, 0 };
	Point max_xy = { max_x + padding, max_y + padding, 0.0, 0 };

	for (const auto &p : pts) {
		std::vector<Point> cell =
			get_voronoi_cell_for_Point(p, pts, min_xy, max_xy);

		for (const auto &vertex : cell) {
			bool duplicate = false;
			for (const auto &existing : all_vertices) {
				if (std::abs(existing.x - vertex.x) < 1e-4f &&
				    std::abs(existing.y - vertex.y) < 1e-4f) {
					duplicate = true;
					break;
				}
			}
			if (!duplicate) {
				all_vertices.push_back(vertex);
			}
		}
	}

	return all_vertices;
}

std::vector<Point> draw_voronoi(const std::vector<Point> &pts)
{
	std::vector<Point> result;

	if (pts.size() < 3) {
		return result;
	}

	std::vector<Point> voronoi_vertices = get_voronoi_cell_vertexes(pts);

	if (voronoi_vertices.empty()) {
		return result;
	}

	double min_x = pts[0].x, min_y = pts[0].y;
	double max_x = pts[0].x, max_y = pts[0].y;

	for (const auto &p : pts) {
		min_x = std::min(min_x, p.x);
		min_y = std::min(min_y, p.y);
		max_x = std::max(max_x, p.x);
		max_y = std::max(max_y, p.y);
	}

	double padding = std::max((max_x - min_x) * 0.2, 5.0);
	Point min_xy = { min_x - padding, min_y - padding, 0.0, 0 };
	Point max_xy = { max_x + padding, max_y + padding, 0.0, 0 };

	EdgeVector all_edges;

	for (const auto &p : pts) {
		std::vector<Point> cell =
			get_voronoi_cell_for_Point(p, pts, min_xy, max_xy);

		if (cell.size() >= 3) {
			for (size_t i = 0; i < cell.size(); ++i) {
				Point p1 = cell[i];
				Point p2 = cell[(i + 1) % cell.size()];

				if (p1.x < p2.x - 1e-6 ||
				    (std::abs(p1.x - p2.x) < 1e-6 &&
				     p1.y < p2.y - 1e-6)) {
					all_edges.push_back({ p1, p2 });
				} else {
					all_edges.push_back({ p2, p1 });
				}
			}
		}
	}

	EdgeVector unique_edges;

	for (size_t i = 0; i < all_edges.size(); ++i) {
		bool duplicate = false;
		for (size_t j = i + 1; j < all_edges.size(); ++j) {
			if (std::abs(all_edges[i].first.x -
				     all_edges[j].first.x) < 1e-4 &&
			    std::abs(all_edges[i].first.y -
				     all_edges[j].first.y) < 1e-4 &&
			    std::abs(all_edges[i].second.x -
				     all_edges[j].second.x) < 1e-4 &&
			    std::abs(all_edges[i].second.y -
				     all_edges[j].second.y) < 1e-4) {
				duplicate = true;
				break;
			}
		}
		if (!duplicate) {
			unique_edges.push_back(all_edges[i]);
		}
	}

	for (const auto &edge : unique_edges) {
		std::vector<Point> line = draw_cda(edge.first, edge.second);

		result.insert(result.end(), line.begin(), line.end());
	}

	return result;
}