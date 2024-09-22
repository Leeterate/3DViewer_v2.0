#ifndef CPP4_3DViewer_V_2_0_1_MODEL_PARSER_H
#define CPP4_3DViewer_V_2_0_1_MODEL_PARSER_H

#include <array>
#include <fstream>
#include <string>
#include <string_view>
#include <vector>

#include "matrix_oop.h"

namespace proj {

struct Polygon {
  int pol[2];
};

struct MinMax {
  double x_min, x_max;
  double y_min, y_max;
  double z_min, z_max;
};

class Parser {
 public:
  Parser(){};
  ~Parser(){};

  void OpenFile(const std::string& filename);

  int GetVertexes();
  int GetPolygons();
  int GetError();
  MinMax GetMinMax();

  const std::vector<proj::projMatrix>& GetMatrixPoints();
  const std::vector<Polygon>& GetArrayPolygons();
  std::vector<proj::projMatrix> matrix_points;

  std::vector<Polygon> polygons;
  void Clean();
  MinMax min_max;

 public:
  class Memento {
   private:
    std::vector<proj::projMatrix> Matrix_points;

   public:
    std::vector<proj::projMatrix> GetState() const { return Matrix_points; }
    Memento(const std::vector<proj::projMatrix>& matrix_points)
        : Matrix_points(matrix_points) {}
  };

  Memento* saveState() { return new Memento(matrix_points); }
  void RestoreState(Memento* memento) { matrix_points = memento->GetState(); }

 private:
  int error;
  int vert_cnt;
  int pol_cnt;

  std::ifstream file;

  proj::projMatrix vertex;  // временная переменная для работы программы
  Polygon facet;  // временное хранилище для 1 вершины 1 ребра

 private:
  void ReadFile();
  void TakeVertex(std::string_view str);
  void PushMatrixPoint(std::string_view str, size_t* pos, int number_cols);
  void TakeFacet(std::string_view str);
  int PushPolygonPoint(std::string_view str, size_t* pos);
  void FindMinMax(int number_cols, double value);
};

class Caretaker {
 private:
  proj::Parser::Memento* memento;

 public:
  void AddMemento(Parser::Memento* new_memento) { memento = new_memento; }
  Parser::Memento* GetMemento() { return memento; }
};

}  // namespace proj

#endif
