#include "second_rank_lines.h"
#include <cmath>
#include "data_definitions.h"
#include <vector>

using AlgorithmicEditor::Point;

std::vector<Point> draw_circle(Point center, int radius)
{
	using namespace std;

	std::vector<Point> points;

	if (radius < 1) {
		return points;
	}

	int c_x = center.x_int();
	int c_y = center.y_int();
	double lambda = 0;

	int x = 0;
	int y = radius;
	double err = 2.0 - 2.0 * radius;
	points.push_back({ 0.0, static_cast<double>(radius), 0 });

	while (y > 0) {
		if (err > 0) {
			lambda = 2.0 * err - 2 * x - 1;
			if (lambda <= 0) {
				x += 1;
				y -= 1;
				err = err + 2.0 * x - 2 * y + 2;
			} else {
				y -= 1;
				err = err - 2.0 * y + 1;
			}
		} else if (err < 0) {
			lambda = 2.0 * err + 2 * y - 1;
			if (lambda > 0) {
				x += 1;
				y -= 1;
				err = err + 2.0 * x - 2 * y + 2;
			} else {
				x += 1;
				err = err + 2.0 * x + 1;
			}
		} else if (err == 0) {
			x += 1;
			y -= 1;
			err = err + 2.0 * x - 2 * y + 2;
		}
		points.push_back(
			{ static_cast<double>(x), static_cast<double>(y), 0 });
	}

	int sz = points.size();
	for (int i = 0; i < sz; i++) {
		points.push_back({ static_cast<double>(points[i].y_int()),
				   static_cast<double>(-points[i].x_int()),
				   0 });
	}
	sz = points.size();
	for (int i = 0; i < sz; i++) {
		points.push_back({ static_cast<double>(-points[i].x_int()),
				   static_cast<double>(-points[i].y_int()),
				   0 });
	}
	sz = points.size();
	for (int i = 0; i < sz; i++) {
		points[i] = { static_cast<double>(points[i].x_int() + c_x),
			      static_cast<double>(points[i].y_int() + c_y), 0 };
	}

	return points;
}

std::vector<Point> draw_ellipse(Point center, int a, int b)
{
	using namespace std;

	std::vector<Point> points;

	if (a < 1 || b < 1) {
		return points;
	}

	int c_x = center.x_int();
	int c_y = center.y_int();
	double lambda = 0;

	int x = 0;
	int y = b;
	double err = double(pow(a, 2) + pow(b, 2) - 2 * b * pow(a, 2));
	points.emplace_back(0, b, 0.0);

	while (y > 0) {
		if (err > 0) {
			lambda = 2.0 * (err - pow(b, 2) * x) - 1;
			if (lambda <= 0) {
				x += 1;
				y -= 1;
				err = err + pow(b, 2) * (2.0 * x + 1) +
				      pow(a, 2) * (1 - 2.0 * y);
			} else {
				y -= 1;
				err = err + pow(a, 2) * (1 - 2.0 * y);
			}
		} else if (err < 0) {
			lambda = 2.0 * err + 2 * y - 1;
			if (lambda > 0) {
				x += 1;
				y -= 1;
				err = err + pow(b, 2) * (2.0 * x + 1) +
				      pow(a, 2) * (1 - 2.0 * y);
			} else {
				x += 1;
				err = err + pow(b, 2) * (2.0 * x + 1);
			}
		} else if (err == 0) {
			x += 1;
			y -= 1;
			err = err + pow(b, 2) * (2.0 * x + 1) +
			      pow(a, 2) * (1 - 2.0 * y);
		}
		points.push_back(
			{ static_cast<double>(x), static_cast<double>(y), 0 });
	}

	int sz = points.size();
	for (int i = 0; i < sz; i++) {
		points.push_back({ static_cast<double>(points[i].y_int()),
				   static_cast<double>(-points[i].x_int()),
				   0 });
	}
	sz = points.size();
	for (int i = 0; i < sz; i++) {
		points.push_back({ static_cast<double>(-points[i].x_int()),
				   static_cast<double>(points[i].y_int()), 0 });
	}
	sz = points.size();
	for (int i = 0; i < sz; i++) {
		points[i] = { static_cast<double>(points[i].x_int() + c_x),
			      static_cast<double>(points[i].y_int() + c_y), 0 };
	}

	return points;
}

std::vector<Point> draw_hyperbola(Point center, int a, int b)
{
	using namespace std;

	std::vector<Point> points;

	if (a < 1 || b < 1) {
		return points;
	}

	const int MAX_Y_OFFSET = 100;
	int c_x = center.x_int();
	int c_y = center.y_int();

	auto add_symmetric = [&](int dx, int dy) {
		points.push_back({ static_cast<double>(c_x + dx),
				   static_cast<double>(c_y + dy), 0 });
		if (dy != 0) {
			points.push_back({ static_cast<double>(c_x + dx),
					   static_cast<double>(c_y - dy), 0 });
		}
		points.push_back({ static_cast<double>(c_x - dx),
				   static_cast<double>(c_y + dy), 0 });
		if (dy != 0) {
			points.push_back({ static_cast<double>(c_x - dx),
					   static_cast<double>(c_y - dy), 0 });
		}
	};

	int prev_x = a;

	for (int y = 0; y <= MAX_Y_OFFSET; ++y) {
		double x_exact =
			a * sqrt(1.0 + (y * y) / static_cast<double>(b * b));
		int x_int = static_cast<int>(round(x_exact));

		if (x_int > prev_x) {
			for (int xi = prev_x + 1; xi <= x_int; ++xi) {
				add_symmetric(xi, y);
			}
		} else if (x_int < prev_x) {
			for (int xi = prev_x - 1; xi >= x_int; --xi) {
				add_symmetric(xi, y);
			}
		} else {
			add_symmetric(x_int, y);
		}

		prev_x = x_int;
	}

	return points;
}

std::vector<Point> draw_parabola(Point center, int p)
{
	std::vector<Point> points;
	if (p < 1) {
		return points;
	}

	const int MAX_Y_OFFSET = 100;
	int c_x = center.x_int();
	int c_y = center.y_int();

	auto add_symmetric = [&](int dx, int dy) {
		points.push_back({ static_cast<double>(c_x + dx),
				   static_cast<double>(c_y + dy), 0 });
		if (dy != 0) {
			points.push_back({ static_cast<double>(c_x + dx),
					   static_cast<double>(c_y - dy), 0 });
		}
	};

	int prev_x = 0;
	for (int y = 0; y <= MAX_Y_OFFSET; ++y) {
		double x_exact = (y * y) / (4.0 * p);
		int x_int = static_cast<int>(round(x_exact));

		if (x_int > prev_x) {
			for (int xi = prev_x + 1; xi <= x_int; ++xi) {
				add_symmetric(xi, y);
			}
		} else if (x_int < prev_x) {
			for (int xi = prev_x - 1; xi >= x_int; --xi) {
				add_symmetric(xi, y);
			}
		} else {
			add_symmetric(x_int, y);
		}
		prev_x = x_int;
	}

	return points;
}