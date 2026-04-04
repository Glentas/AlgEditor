#pragma once
#include <vector>
#include "data_definitions.h"

using AlgorithmicEditor::Point;

int sign(int a);
std::vector<Point> draw_cda(Point p1, Point p2);
std::vector<Point> draw_bresenham(Point p1, Point p2);
std::vector<Point> draw_wu(Point p1, Point p2);