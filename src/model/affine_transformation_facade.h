#ifndef CPP4_3DVIEWER_V_2_0_1_MODEL_affine_transformation_FACADE_H
#define CPP4_3DVIEWER_V_2_0_1_MODEL_affine_transformation_FACADE_H

#include "affine_transformation.h"
#include "matrix_oop.h"
#include "parser.h"

namespace proj {

class AffineTransformationFacade {
 public:
  AffineTransformationFacade(const AffineTransformationFacade&) = delete;
  AffineTransformationFacade& operator=(const AffineTransformationFacade&) =
      delete;
  static AffineTransformationFacade& getInstance() {
    static AffineTransformationFacade
        instance;  // Создается при первом вызове и уничтожается при завершении
                   // программы
    return instance;
  }
  void move(Parser* parser, double move_x, double move_y, double move_z);
  void rotate(Parser* parser, Axes axis, double angle);
  void scale(Parser* parser, double scale_x, double scale_y, double scale_z);
  void normalize(Parser* parser);

 private:
  AffineTransformationFacade(){};
  ~AffineTransformationFacade(){};
};

}  // namespace proj

#endif