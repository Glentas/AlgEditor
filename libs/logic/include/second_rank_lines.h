#pragma once
#include "data_definitions.h"
#include <vector>

using AlgorithmicEditor::Point;

std::vector<Point> draw_circle(Point center, int radius);
std::vector<Point> draw_ellipse(Point center, int a, int b);
std::vector<Point> draw_parabola(Point center, int p);
std::vector<Point> draw_hyperbola(Point center, int a, int b);