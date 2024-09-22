#ifndef CPP4_3DViewer_V_2_0_1_MODEL_AFFINE_TRANSFORMATION_H
#define CPP4_3DViewer_V_2_0_1_MODEL_AFFINE_TRANSFORMATION_H

#include <cmath>
#include <iostream>
#include <mutex>

#include "matrix_oop.h"
#include "parser.h"

namespace proj {

typedef enum {
  AxisX = 0,
  AxisY = 1,
  AxisZ = 2,
} Axes;

class AffineTransformation {
 public:
  AffineTransformation(const AffineTransformation&) = delete;
  AffineTransformation& operator=(const AffineTransformation&) = delete;
  static AffineTransformation& getInstance() {
    static AffineTransformation instance;
    return instance;
  }
  void move_points(Parser* parser, double move_x, double move_y, double move_z);
  void rotation_points(Parser* parser, Axes axis, double angle);
  void scale_points(Parser* parser, double scale_x, double scale_y,
                    double scale_z);
  void normalize_figure(proj::Parser* parser);
  double get_delta_view() { return DELTA_VIEW; }

 private:
  AffineTransformation(){};
  ~AffineTransformation(){};
  projMatrix create_transfer_matrix(double* moving_coordinates);
  projMatrix create_rotation_X_matrix(double angle);
  projMatrix create_rotation_Y_matrix(double angle);
  projMatrix create_rotation_Z_matrix(double angle);
  projMatrix create_scale_matrix(double* scale_ratio);
  void make_unit_matrix(projMatrix& matrix);
  static constexpr int ORDER_TRANSFER_MATRIX = 4;
  static constexpr int DIMENSION_SPACE = 3;
  static constexpr double RAD_IN_DEGRE = 180.0;
  static constexpr double DELTA_VIEW = 25;
};

double max(int x, int y, int z);
}  // namespace proj

#endif
