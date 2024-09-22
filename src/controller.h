#ifndef CONTROLLER_H_
#define CONTROLLER_H_

#include "model/affine_transformation.h"
#include "model/affine_transformation_facade.h"
#include "model/matrix_oop.h"
#include "model/parser.h"

namespace proj {

// Класс выполнен в соответствии с шаблонами проектирования "facade" и
// "singleton". Он служит интерфейсом для классов афинных преобразований и
// парсера. В нём отсутствует бизнес-логика.
class Controller {
 public:
  Controller(const Controller&) = delete;
  Controller& operator=(const Controller&) = delete;
  static Controller& getInstance(proj::Parser* other1,
                                 proj::AffineTransformationFacade* other2,
                                 proj::Caretaker* other3) {
    static Controller instance(
        other1, other2, other3);  // Создается при первом вызове и уничтожается
                                  // при завершении программы
    return instance;
  }

  void pars_obj_file(std::string file_name) {
    parser_obj->Clean();
    parser_obj->OpenFile(file_name);
    transformation_obj->normalize(parser_obj);
    caretaker->AddMemento(parser_obj->saveState());
  }

  void restore_state() { parser_obj->RestoreState(caretaker->GetMemento()); }

  void scale_matrix(double scale_x, double scale_y, double scale_z) {
    transformation_obj->scale(parser_obj, scale_x, scale_y, scale_z);
  }

  void move_matrix(double move_x, double move_y, double move_z) {
    transformation_obj->move(parser_obj, move_x, move_y, move_z);
  }

  void rotate_matrix(int axis0, double angle) {
    proj::Axes axis = AxisX;
    if (axis0 == 1) axis = AxisY;
    if (axis0 == 2) axis = AxisZ;
    transformation_obj->rotate(parser_obj, axis, angle);
  }

  const std::vector<proj::projMatrix>& GetMatrix() {
    return parser_obj->GetMatrixPoints();
  }

  const std::vector<Polygon>& GetPoly() {
    return parser_obj->GetArrayPolygons();
  }

  int CountVertexes() { return parser_obj->GetVertexes(); }

  int CountPolygons() { return parser_obj->GetPolygons(); }

  void remove_matrix() { parser_obj->Clean(); }

  int Get_Error() { return parser_obj->GetError(); }

  proj::Parser* parser_obj;
  proj::AffineTransformationFacade* transformation_obj;
  proj::Caretaker* caretaker;

 private:
  explicit Controller(proj::Parser* other1,
                      proj::AffineTransformationFacade* other2,
                      proj::Caretaker* other3)
      : parser_obj(other1), transformation_obj(other2), caretaker(other3) {}
  ~Controller(){};
};

}  // namespace proj

#endif  // CONTROLLER_H_
