#include "interpolation.h"
#include <cmath>
#include "data_definitions.h"
#include <vector>

using AlgorithmicEditor::Point;

std::vector<Point> draw_hermite(Point p1, Point force1, Point p2, Point force2,
				int accuracy)
{
	std::vector<Point> points;
	double dt = 1.0 / accuracy;

	for (int i = 0; i <= accuracy; i++) {
		double t = i * dt;
		double t2 = t * t;
		double t3 = t2 * t;

		double h00 = 2 * t3 - 3 * t2 + 1;
		double h10 = t3 - 2 * t2 + t;
		double h01 = -2 * t3 + 3 * t2;
		double h11 = t3 - t2;

		double x = h00 * p1.x_int() + h10 * force1.x_int() +
			   h01 * p2.x_int() + h11 * force2.x_int();
		double y = h00 * p1.y_int() + h10 * force1.y_int() +
			   h01 * p2.y_int() + h11 * force2.y_int();

		points.push_back({ x, y, 0 });
	}
	return points;
}

std::vector<Point> draw_bezier(Point p1, Point p2, Point p3, Point p4,
			       int accuracy)
{
	return draw_hermite(p1, { 3 * (p2.x - p1.x), 3 * (p2.y - p1.y), 0 }, p4,
			    { 3 * (p4.x - p3.x), 3 * (p4.y - p3.x), 0 },
			    accuracy);
}

std::vector<Point> draw_b_spline_segment(Point P_im1, Point P_i, Point P_ip1,
					 Point P_ip2, int accuracy)
{
	std::vector<Point> segment;
	double dt = 1.0 / accuracy;

	for (int k = 0; k <= accuracy; k++) {
		double t = k * dt;
		double t2 = t * t;
		double t3 = t2 * t;

		double b0 = (-t3 + 3 * t2 - 3 * t + 1) / 6.0;
		double b1 = (3 * t3 - 6 * t2 + 4) / 6.0;
		double b2 = (-3 * t3 + 3 * t2 + 3 * t + 1) / 6.0;
		double b3 = (t3) / 6.0;

		double x =
			b0 * P_im1.x + b1 * P_i.x + b2 * P_ip1.x + b3 * P_ip2.x;
		double y =
			b0 * P_im1.y + b1 * P_i.y + b2 * P_ip1.y + b3 * P_ip2.y;

		segment.push_back({ x, y, 0 });
	}
	return segment;
}

std::vector<Point> draw_spline(std::vector<Point> controlPoints, int accuracy)
{
	std::vector<Point> curve;
	if (controlPoints.size() < 4)
		return curve;

	for (size_t i = 1; i <= controlPoints.size() - 3; i++) {
		std::vector<Point> seg = draw_b_spline_segment(
			controlPoints[i - 1], controlPoints[i],
			controlPoints[i + 1], controlPoints[i + 2], accuracy);
		curve.insert(curve.end(), seg.begin(), seg.end());
	}
	return curve;
}