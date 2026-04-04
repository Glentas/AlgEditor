#include "data_definitions.h"
#include <vector>

using AlgorithmicEditor::Point;

std::vector<Point> draw_hermite(Point p1, Point force1, Point p2, Point force2,
				int accuracy = 1000);

std::vector<Point> draw_bezier(Point p1, Point p2, Point p3, Point p4,
			       int accuracy = 1000);

std::vector<Point> draw_b_spline_segment(Point P_im1, Point P_i, Point P_ip1,
					 Point P_ip2, int accuracy);

std::vector<Point> draw_spline(std::vector<Point> controlPoints,
			       int accuracy = 1000);