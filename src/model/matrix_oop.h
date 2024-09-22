#ifndef CPP4_3DViewer_V_2_0_1_MODEL_MATRIX_OOP_H
#define CPP4_3DViewer_V_2_0_1_MODEL_MATRIX_OOP_H

#include <algorithm>
#include <cmath>
#include <iostream>

#define defolt_rows 2
#define defolt_cols 2

// rows - строки
// cols - столбцы

namespace proj {
class projMatrix {
 private:
  int rows_, cols_;
  double** matrix_;
  // выделение памяти под матрицу
  void Allocate();
  // отчистка памяти
  void Deallocate();
  // функция осуществляющая сложение и вычитание матриц
  void help_add_or_sub(const projMatrix& other, int operation);

 public:
  // конструкторы и деструкторы

  // конструктор для создание дефолтной матрицы
  projMatrix();
  // конструктор для создание матрицы заданной размерности
  projMatrix(int rows, int cols);
  // конструктор копирования
  projMatrix(const projMatrix& other);
  // конструктор переноса
  projMatrix(projMatrix&& other);
  // деструктор
  ~projMatrix();

  // основные функции
  // проверяет матрицы на равенство между собой
  bool EqMatrix(const projMatrix& other) const;
  // суммирует две матрицы
  void SumMatrix(const projMatrix& other);
  // вычитает из текущей матрицы другую
  void SubMatrix(const projMatrix& other);
  // умножает текущую матрицу на число
  void MulNumber(const double num);
  // yмножает текущую матрицу на вторую
  void MulMatrix(const projMatrix& other);
  // cоздает новую транспонированную матрицу из текущей и возвращает ее.
  projMatrix Transpose();
  // вычисляет матрицу алгебраических дополнений текущей матрицы и возвращает
  // ее.
  projMatrix CalcComplements();
  // вычисляет определитель матрницы
  double Determinant();
  // вычисляет и возвращает обратную матрицу.
  projMatrix InverseMatrix();

  // числовые операторы
  //+
  projMatrix operator+(const projMatrix& other) const;
  //-
  projMatrix operator-(const projMatrix& other) const;
  //* матрица на число
  projMatrix operator*(const double num) const;
  //* на матрицу
  projMatrix operator*(const projMatrix& other) const;
  //==
  bool operator==(const projMatrix& other) const noexcept;
  //=
  projMatrix& operator=(const projMatrix& other);
  //=
  projMatrix& operator=(projMatrix&& other) noexcept;
  //+=
  projMatrix& operator+=(const projMatrix& other);
  //-=
  projMatrix& operator-=(const projMatrix& other);
  //*= матрица
  projMatrix& operator*=(const projMatrix& other);
  //*= число
  projMatrix& operator*=(const double num);
  // оператор для доступа к элементу матрицы
  double* operator[](int row) const;
  // оператор возвращает элемент матрицы
  double& operator()(int row, int col) const;

  // вспмогательные функции

  // проверяет текущую матрицу на содержание inf или nan
  int nan_or_inf();
  // удаляет текущую матрицу и создает новую в которую записывает передаваемую
  // матрицу
  void cpy_mat(const projMatrix& other);
  // определитель матрицы 2 на 2
  double help_minor(projMatrix A);
  // создает матрицу которая получается из исходной путем вычеркивания строки и
  // сталбца
  void help_matrix_formation(projMatrix* buf, int bani, int banj);
  // ставит нужные знаки для матрицы алгебраических дополнений
  void help_calc_complements();
  // проверяет текущую матрицу и переданную на содержание inf или nan
  int matrix_nan_or_inf(const projMatrix& other);

  // accessor и mutator

  // возвращает кол-во строк
  int get_rows() const;
  // возвращает кол-во столбцов
  int get_cols() const;
  // изменяет кол-во строк
  void set_rows(int rows);
  // изменяет кол-во столбцов
  void set_cols(int cols);
};
}  // namespace proj
#endif