#include "polygon.h"
#include "first_rank_lines.h"
#include <algorithm>
#include <cmath>
#include "data_definitions.h"
#include <vector>

using AlgorithmicEditor::Point;

bool is_convex(const std::vector<Point> &points)
{
	if (points.size() < 3)
		return false;

	int n = points.size();
	int sign = 0;

	for (int i = 0; i < n; i++) {
		int x1 = points[i].x_int();
		int y1 = points[i].y_int();
		int x2 = points[(i + 1) % n].x_int();
		int y2 = points[(i + 1) % n].y_int();
		int x3 = points[(i + 2) % n].x_int();
		int y3 = points[(i + 2) % n].y_int();

		int v1x = x2 - x1;
		int v1y = y2 - y1;
		int v2x = x3 - x2;
		int v2y = y3 - y2;

		int cross_product = v1x * v2y - v1y * v2x;

		int current_sign = 0;
		if (cross_product > 0)
			current_sign = 1;
		else if (cross_product < 0)
			current_sign = -1;

		if (i == 0) {
			sign = current_sign;
		} else if (current_sign != 0 && current_sign != sign) {
			return false;
		}
	}

	return true;
}

double calculate_polar_angle(int base_x, int base_y, int x, int y)
{
	int dx = x - base_x;
	int dy = y - base_y;

	if (dx == 0 && dy == 0)
		return 0;

	double angle = atan2(dy, dx);
	if (angle < 0)
		angle += 2 * M_PI;

	return angle;
}

std::vector<Point> jarvis_march(const std::vector<Point> &points)
{
	if (points.size() < 3)
		return points;

	int leftmost_idx = 0;
	for (size_t i = 1; i < points.size(); i++) {
		if (points[i].x < points[leftmost_idx].x) {
			leftmost_idx = i;
		}
	}

	std::vector<Point> hull;
	int current_idx = leftmost_idx;

	do {
		hull.push_back(points[current_idx]);

		int next_idx = (current_idx + 1) % points.size();

		for (size_t i = 0; i < points.size(); i++) {
			const auto &p1 = points[current_idx];
			const auto &p2 = points[next_idx];
			const auto &p3 = points[i];

			int v1x = p2.x_int() - p1.x_int();
			int v1y = p2.y_int() - p1.y_int();
			int v2x = p3.x_int() - p1.x_int();
			int v2y = p3.y_int() - p1.y_int();

			int cross = v1x * v2y - v1y * v2x;

			if (cross < 0) {
				next_idx = i;
			} else if (cross == 0) {
				int dist_to_next =
					(p2.x_int() - p1.x_int()) *
						(p2.x_int() - p1.x_int()) +
					(p2.y_int() - p1.y_int()) *
						(p2.y_int() - p1.y_int());
				int dist_to_candidate =
					(p3.x_int() - p1.x_int()) *
						(p3.x_int() - p1.x_int()) +
					(p3.y_int() - p1.y_int()) *
						(p3.y_int() - p1.y_int());
				if (dist_to_candidate > dist_to_next) {
					next_idx = i;
				}
			}
		}

		current_idx = next_idx;

	} while (current_idx != leftmost_idx);

	return hull;
}

std::vector<Point> graham_scan(const std::vector<Point> &points)
{
	if (points.size() < 3)
		return points;

	int min_y_idx = 0;
	for (size_t i = 1; i < points.size(); i++) {
		if (points[i].y < points[min_y_idx].y ||
		    (points[i].y == points[min_y_idx].y &&
		     points[i].x < points[min_y_idx].x)) {
			min_y_idx = i;
		}
	}

	std::vector<Point> sorted_points;
	sorted_points.push_back(points[min_y_idx]);
	for (size_t i = 0; i < points.size(); i++) {
		if (i != min_y_idx) {
			sorted_points.push_back(points[i]);
		}
	}

	auto p0 = sorted_points[0];
	std::sort(sorted_points.begin() + 1, sorted_points.end(),
		  [&p0](const Point &a, const Point &b) {
			  double angle_a =
				  calculate_polar_angle(p0.x, p0.y, a.x, a.y);
			  double angle_b =
				  calculate_polar_angle(p0.x, p0.y, b.x, b.y);

			  if (angle_a == angle_b) {
				  double dist_a = (a.x - p0.x) * (a.x - p0.x) +
						  (a.y - p0.y) * (a.y - p0.y);
				  double dist_b = (b.x - p0.x) * (b.x - p0.x) +
						  (b.y - p0.y) * (b.y - p0.y);
				  return dist_a < dist_b;
			  }
			  return angle_a < angle_b;
		  });

	std::vector<Point> hull;
	hull.push_back(sorted_points[0]);
	hull.push_back(sorted_points[1]);

	for (size_t i = 2; i < sorted_points.size(); i++) {
		while (hull.size() >= 2) {
			const auto &p1 = hull[hull.size() - 2];
			const auto &p2 = hull[hull.size() - 1];
			const auto &p3 = sorted_points[i];

			int v1x = p2.x - p1.x;
			int v1y = p2.y - p1.y;
			int v2x = p3.x - p2.x;
			int v2y = p3.y - p2.y;

			int cross = v1x * v2y - v1y * v2x;

			if (cross <= 0) {
				hull.pop_back();
			} else {
				break;
			}
		}
		hull.push_back(sorted_points[i]);
	}

	return hull;
}

std::vector<Point> get_convex_hull_points(const std::vector<Point> &vertices)
{
	std::vector<Point> hull_points;

	if (vertices.size() < 3) {
		return hull_points;
	}

	std::vector<Point> hull = jarvis_march(vertices);

	for (const auto &p : hull) {
		hull_points.push_back({ p.x, p.y, 0.0, 0 });
	}

	return hull_points;
}

std::vector<Point> draw_convex_polygon(std::vector<Point> vertices)
{
	std::vector<Point> all_points;

	if (vertices.size() < 3) {
		return all_points;
	}

	if (!is_convex(vertices)) {
		vertices = graham_scan(vertices);
	}

	for (size_t i = 0; i < vertices.size(); i++) {
		const auto &current = vertices[i];
		const auto &next = vertices[(i + 1) % vertices.size()];

		std::vector<Point> line_points = draw_cda(current, next);

		all_points.insert(all_points.end(), line_points.begin(),
				  line_points.end());
	}

	return all_points;
}