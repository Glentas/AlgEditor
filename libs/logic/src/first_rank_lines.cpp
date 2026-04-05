#include "first_rank_lines.h"
#include "data_definitions.h"
#include <cmath>
#include <cstdint>
#include <vector>

using AlgorithmicEditor::Point;
using std::uint8_t;

int sign(int a) { return (0 < a) - (a < 0); }

std::vector<Point> draw_cda(Point p1, Point p2) {
  using namespace std;
  std::vector<Point> points;
  int dx = p2.x_int() - p1.x_int();
  int dy = p2.y_int() - p1.y_int();
  int steps = max(abs(dx), abs(dy));

  if (steps == 0) {
    points.push_back(p1);
    return points;
  }

  for (int i = 0; i <= steps; ++i) {
    double t = static_cast<double>(i) / steps;
    double x = p1.x_int() + t * dx;
    double y = p1.y_int() + t * dy;
    points.push_back({x, y, 0});
  }

  points.back() = p2;

  return points;
}

std::vector<Point> draw_bresenham(Point p1, Point p2) {
  using namespace std;
  std::vector<Point> points;
  int x1 = p1.x_int();
  int x2 = p2.x_int();
  int y1 = p1.y_int();
  int y2 = p2.y_int();

  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = sign(x2 - x1);
  int sy = sign(y2 - y1);

  bool steep = (dy > dx);
  if (steep) {
    swap(x1, y1);
    swap(x2, y2);
    swap(dx, dy);
    swap(sx, sy);
  }

  double err = (static_cast<double>(dy) / dx) - 0.5;
  int x = x1, y = y1;

  for (int i = 0; i <= dx; i++) {
    if (steep)
      points.push_back({static_cast<double>(y), static_cast<double>(x), 0});
    else
      points.push_back({static_cast<double>(x), static_cast<double>(y), 0});

    x += sx;
    err -= dy;
    if (err < 0) {
      y += sy;
      err += dx;
    }
  }

  return points;
}

std::vector<Point> draw_wu(Point p1, Point p2) {
  using namespace std;

  std::vector<Point> points;
  int x1 = p1.x_int();
  int x2 = p2.x_int();
  int y1 = p1.y_int();
  int y2 = p2.y_int();

  int dx = x2 - x1;
  int dy = y2 - y1;

  if (dx == 0 && dy == 0) {
    points.push_back(p1);
    return points;
  }

  if (dx == 0 || dy == 0 || abs(dx) == abs(dy)) {
    return draw_bresenham(p1, p2);
  }

  bool steep = abs(dy) > abs(dx);

  if (steep) {
    swap(x1, y1);
    swap(x2, y2);
    swap(dx, dy);
  }

  if (x1 > x2) {
    swap(x1, x2);
    swap(y1, y2);
    dx = x2 - x1;
    dy = y2 - y1;
  }

  double gradient = static_cast<double>(dy) / dx;

  double y = y1;
  for (int x = x1; x <= x2; x++) {
    double y_floor = floor(y);
    double frac = y - y_floor;
    if (steep) {
      points.push_back(
          {y_floor, static_cast<double>(x), static_cast<uint8_t>(255 * frac)});
      points.push_back({y_floor + 1.0, static_cast<double>(x),
                        static_cast<uint8_t>(255 * (1.0 - frac))});
    } else {
      points.push_back(
          {static_cast<double>(x), y_floor, static_cast<uint8_t>(255 * frac)});
      points.push_back({static_cast<double>(x), y_floor + 1.0,
                        static_cast<uint8_t>(255 * (1.0 - frac))});
    }
    y += gradient;
  }

  points.back() = p2;

  return points;
}