#pragma once

#include "data_definitions.h"
#include <vector>

using AlgorithmicEditor::Point;

bool is_convex(const std::vector<Point> &points);
std::vector<Point> graham_scan(const std::vector<Point> &points);
std::vector<Point> draw_convex_polygon(std::vector<Point> vertices);
std::vector<Point> get_convex_hull_points(const std::vector<Point> &vertices);