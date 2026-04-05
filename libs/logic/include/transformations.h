#pragma once

#include "data_definitions.h"

#include <vector>

using AlgorithmicEditor::Point;

#define ROTATE_ANGLE 6.0f

#define UPSCALE_F 1.1f
#define DOWNSCALE_F 0.9f

#define MOVE_STEP 2.0f

double to_radians(double degrees);

Point find_center(const std::vector<Point> &pts);

void multiply_matrices_row_major(double result[4][4], const double a[4][4],
                                 const double b[4][4]);
void apply_matrix_to_point_row_major(const double matrix[4][4], double &x,
                                     double &y, double &z);

void translation_matrix_row_major(double matrix[4][4], double dx, double dy,
                                  double dz);
void rotation_x_matrix_row_major(double matrix[4][4], double angle);
void rotation_y_matrix_row_major(double matrix[4][4], double angle);
void rotation_z_matrix_row_major(double matrix[4][4], double angle);

std::vector<Point> rotate_figure(std::vector<Point> pts, char axis,
                                 bool use_center = false,
                                 Point center = {0.0, 0.0, 0.0, 0});

std::vector<Point> rotate_around_center(std::vector<Point> pts, double center_x,
                                        double center_y, double center_z,
                                        char axis,
                                        double angle_deg = ROTATE_ANGLE);

void scale_matrix_row_major(double matrix[4][4], double sx, double sy,
                            double sz);

std::vector<Point> scale_figure(std::vector<Point> pts, char method,
                                bool use_center = false,
                                Point center = {0.0, 0.0, 0.0, 0});

std::vector<Point> scale_around_center(std::vector<Point> pts, double center_x,
                                       double center_y, double center_z,
                                       double sx, double sy, double sz);

std::vector<Point> move_figure(std::vector<Point> pts, char direction);
