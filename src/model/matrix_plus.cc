#include "matrix_oop.h"

// конструкторы и деструкторы
namespace proj {

projMatrix::projMatrix() : rows_(defolt_rows), cols_(defolt_cols) { Allocate(); }

projMatrix::projMatrix(int rows, int cols) : rows_(rows), cols_(cols) {
  if (rows_ > 0 && cols_ > 0) {
    Allocate();
  } else {
    throw std::length_error("Size array error");
  }
}

projMatrix::~projMatrix() { Deallocate(); }

projMatrix::projMatrix(const projMatrix &other)
    : rows_(other.rows_), cols_(other.cols_) {
  matrix_ = new double *[rows_];
  for (int i = 0; i < rows_; ++i) {
    matrix_[i] = new double[cols_];
  }
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = other[i][j];
    }
  }
}

projMatrix::projMatrix(projMatrix &&other) {
  this->rows_ = other.rows_;
  this->cols_ = other.cols_;
  this->matrix_ = other.matrix_;
  other.rows_ = 0;
  other.cols_ = 0;
  other.matrix_ = nullptr;
}

void projMatrix::Allocate() {
  matrix_ = new double *[rows_];
  for (int i = 0; i < rows_; i++) {
    matrix_[i] = new double[cols_];
  }
  for (int i = 0; i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      matrix_[i][j] = 0;
    }
  }
}

void projMatrix::Deallocate() {
  for (int i = 0; i < rows_; i++) {
    delete[] matrix_[i];
  }
  delete[] matrix_;
  matrix_ = nullptr;
}

// основные функции

bool projMatrix::EqMatrix(const projMatrix &other) const {
  bool res = true;
  if (this->rows_ != other.rows_ || this->cols_ != other.cols_) {
    res = false;
  }
  for (int i = 0; i < this->rows_ && res != false; i++) {
    for (int j = 0; j < this->cols_ && res != false; j++) {
      if (fabs(this->matrix_[i][j] - other.matrix_[i][j]) > 1e-7) {
        res = false;
      }
    }
  }
  return res;
}

void projMatrix::SumMatrix(const projMatrix &other) {
  if (this->rows_ != other.rows_ || this->cols_ != other.cols_) {
    throw std::logic_error("Matrix dimensions do not match");
  } else if (matrix_nan_or_inf(other)) {
    throw std::invalid_argument("matrix element cannot be nan or inf");
  }
  help_add_or_sub(other, 1);
}

void projMatrix::SubMatrix(const projMatrix &other) {
  if (this->rows_ != other.rows_ || this->cols_ != other.cols_) {
    throw std::logic_error("Matrix dimensions do not match");
  } else if (matrix_nan_or_inf(other)) {
    throw std::invalid_argument("matrix element cannot be nan or inf");
  }
  help_add_or_sub(other, -1);
}

void projMatrix::MulNumber(const double num) {
  if (nan_or_inf() || std::isnan(num) || std::isinf(num)) {
    throw std::invalid_argument("matrix element cannot be nan or inf");
  }
  for (int i = 0; i < this->rows_; i++) {
    for (int j = 0; j < this->cols_; j++) {
      matrix_[i][j] *= num;
    }
  }
}

void projMatrix::MulMatrix(const projMatrix &other) {
  if (matrix_nan_or_inf(other)) {
    throw std::invalid_argument("matrix element cannot be nan or inf");
  } else if (this->cols_ != other.rows_) {
    throw std::logic_error("Matrix dimensions do not match");
  }
  double res = 0.0;
  projMatrix buffer(rows_, other.get_cols());
  for (int i = 0; i < this->rows_; i++) {
    for (int j = 0; j < other.cols_; j++) {
      res = 0;
      for (int k = 0; k < this->cols_; k++) {
        res = res + this->matrix_[i][k] * other.matrix_[k][j];
      }
      buffer[i][j] = res;
    }
  }
  cpy_mat(buffer);
}

projMatrix projMatrix::Transpose() {
  projMatrix trans(this->cols_, this->rows_);
  for (int i = 0; i < this->rows_; i++) {
    for (int j = 0; j < this->cols_; j++) {
      trans[j][i] = (*this)[i][j];
    }
  }
  return trans;
}

projMatrix projMatrix::CalcComplements() {
  if (this->rows_ != this->cols_) {
    throw std::logic_error("Matrix dimensions do not match");
  } else if (nan_or_inf()) {
    throw std::invalid_argument("matrix element cannot be nan or inf");
  }
  projMatrix result(this->rows_, this->rows_);
  if (this->rows_ > 1) {
    projMatrix result(this->rows_, this->rows_);
    projMatrix bufmat(this->rows_ - 1, this->rows_ - 1);
    double buf_det = 0.0;
    for (int i = 0; i < this->rows_; i++) {
      for (int j = 0; j < this->rows_; j++) {
        help_matrix_formation(&bufmat, i, j);
        buf_det = bufmat.Determinant();
        result[i][j] = buf_det;
      }
    }
    result.help_calc_complements();
    return result;
  }
  result.set_cols(1);
  result.set_rows(1);
  result[0][0] = 1.0;
  return result;
}

double projMatrix::Determinant() {
  double res = 0;
  int sign = 1;
  if (this->rows_ != this->cols_) {
    throw std::logic_error("Invalid matrix");
  } else if (nan_or_inf()) {
    throw std::invalid_argument("matrix element cannot be nan or inf");
  }
  if (this->rows_ == 3) {
    for (int i = 0; i < this->rows_; i++) {
      projMatrix buf(this->rows_ - 1, this->cols_ - 1);
      if ((i + 2) % 2 == 0) {
        sign = 1;
      } else {
        sign = -1;
      }
      help_matrix_formation(&buf, 0, i);
      res = res + sign * (*this)[0][i] * help_minor(buf);
    }
  } else if (this->rows_ == 2) {
    res = help_minor(*this);
  } else if (this->rows_ == 1) {
    res = (*this)[0][0];
  } else if (this->rows_ > 3) {
    for (int i = 0; i < this->rows_; i++) {
      projMatrix buf(this->rows_ - 1, this->cols_ - 1);
      help_matrix_formation(&buf, 0, i);
      double minor = buf.Determinant();
      if ((i + 2) % 2 == 0) {
        sign = 1;
      } else {
        sign = -1;
      }
      res = res + sign * (*this)[0][i] * minor;
    }
  }

  return res;
}

projMatrix projMatrix::InverseMatrix() {
  if (this->nan_or_inf()) {
    throw std::invalid_argument("matrix element cannot be nan or inf");
  } else if (this->cols_ != this->rows_) {
    throw std::logic_error("Invalid matrix");
  }
  double det = this->Determinant();
  if (det == 0) {
    throw std::logic_error("The determinant cannot be zero");
  }
  projMatrix result(this->rows_, this->cols_);
  result = this->CalcComplements();
  result = result.Transpose();
  det = 1.0 / det;
  result.MulNumber(det);
  return result;
}

// числовые операторы

projMatrix projMatrix::operator+(const projMatrix &other) const {
  projMatrix res = (*this);
  res.SumMatrix(other);
  return res;
}

projMatrix projMatrix::operator-(const projMatrix &other) const {
  projMatrix res = (*this);
  res.SubMatrix(other);
  return res;
}

projMatrix projMatrix::operator*(const double num) const {
  projMatrix res = (*this);
  res.MulNumber(num);
  return res;
}

projMatrix projMatrix::operator*(const projMatrix &other) const {
  projMatrix res = (*this);
  res.MulMatrix(other);
  return res;
}

bool projMatrix::operator==(const projMatrix &other) const noexcept {
  return this->EqMatrix(other);
}

projMatrix &projMatrix::operator=(const projMatrix &other) {
  if (this != &other) {
    cpy_mat(other);
  }
  return *this;
}

projMatrix &projMatrix::operator=(projMatrix &&other) noexcept {
  if (this != &other) {
    std::swap(rows_, other.rows_);
    std::swap(cols_, other.cols_);
    std::swap(matrix_, other.matrix_);
  }

  return *this;
}

projMatrix &projMatrix::operator+=(const projMatrix &other) {
  this->SumMatrix(other);
  return *this;
}

projMatrix &projMatrix::operator-=(const projMatrix &other) {
  this->SubMatrix(other);
  return *this;
}

projMatrix &projMatrix::operator*=(const projMatrix &other) {
  this->MulMatrix(other);
  return *this;
}

projMatrix &projMatrix::operator*=(const double num) {
  this->MulNumber(num);
  return *this;
}

double *projMatrix::operator[](int row) const {
  if (this->rows_ <= row || this->rows_ < 0) {
    throw std::out_of_range("There is no element with such an index");
  }
  return matrix_[row];
}

double &projMatrix::operator()(int row, int col) const {
  if (this->rows_ <= row || this->cols_ <= col) {
    throw std::out_of_range("There is no element with such an index");
  } else if (row < 0 || col < 0) {
    throw std::length_error(
        "The dimension of the matrix must be greater than zero");
  }
  return matrix_[row][col];
}

// вспмогательные функции

void projMatrix::help_calc_complements() {
  int sign = -1;
  for (int i = 0; i < this->rows_; i++) {
    for (int j = 0; j < this->cols_; j++) {
      if ((i + j + 2) % 2 == 0) {
        sign = 1;
      } else {
        sign = -1;
      }
      (*this)[i][j] *= sign;
    }
  }
}

double projMatrix::help_minor(projMatrix A) {
  double res = 0;
  res = A[0][0] * A[1][1] - A[1][0] * A[0][1];
  return res;
}

void projMatrix::help_matrix_formation(projMatrix *buf, int bani, int banj) {
  int i1 = 0, j1 = 0;
  for (int i = 0; i < this->rows_; i++) {
    for (int j = 0; j < this->cols_; j++) {
      if (i != bani && j != banj) {
        (*buf)[j1][i1] = (*this)[i][j];
        i1++;
        if (i1 == buf->rows_) {
          j1++;
          i1 = 0;
        }
      }
    }
  }
}

int projMatrix::get_rows() const { return this->rows_; }

int projMatrix::get_cols() const { return this->cols_; }

void projMatrix::set_rows(int rows) {
  int trow = this->rows_;
  if (rows <= 0) {
    throw std::length_error("Size array error");
  }
  if (rows != trow) {
    projMatrix buf(rows, cols_);
    for (int i = 0; i < rows && i < rows_; i++) {
      for (int j = 0; j < cols_; j++) {
        if (i < trow) {
          buf[i][j] = (*this)[i][j];
        } else {
          buf[i][j] = 0;
        }
      }
    }
    cpy_mat(buf);
  }
}

void projMatrix::set_cols(int cols) {
  if (cols <= 0) {
    throw std::length_error("Size array error");
  }
  if (cols != this->cols_) {
    projMatrix buf(rows_, cols);
    for (int i = 0; i < rows_; i++) {
      for (int j = 0; j < cols_ && j < cols; j++) {
        if (j < this->cols_) {
          buf[i][j] = (*this)[i][j];
        } else {
          buf[i][j] = 0;
        }
      }
    }
    cpy_mat(buf);
  }
}

void projMatrix::cpy_mat(const projMatrix &other) {
  Deallocate();
  this->rows_ = other.rows_;
  this->cols_ = other.cols_;
  Allocate();
  for (int i = 0; i < rows_ && i < rows_; i++) {
    for (int j = 0; j < cols_; j++) {
      (*this)[i][j] = other[i][j];
    }
  }
}

void projMatrix::help_add_or_sub(const projMatrix &other, int operation) {
  for (int i = 0; i < this->rows_; i++) {
    for (int j = 0; j < this->cols_; j++) {
      this->matrix_[i][j] =
          this->matrix_[i][j] + operation * other.matrix_[i][j];
    }
  }
}

int projMatrix::nan_or_inf() {
  int res = 0;
  for (int i = 0; i < this->rows_ && res == 0; i++) {
    for (int j = 0; j < this->cols_ && res == 0; j++) {
      if (std::isnan((*this)[i][j]) || std::isinf((*this)[i][j])) {
        res = 1;
      }
    }
  }
  return res;
}

int projMatrix::matrix_nan_or_inf(const projMatrix &other) {
  int res = 0;
  for (int i = 0; i < this->rows_ && res == 0; i++) {
    for (int j = 0; j < this->cols_ && res == 0; j++) {
      if (std::isnan((*this)[i][j]) || std::isinf((*this)[i][j])) {
        res = 1;
      }
    }
  }
  for (int i = 0; i < other.rows_ && res == 0; i++) {
    for (int j = 0; j < other.cols_ && res == 0; j++) {
      if (std::isnan(other[i][j]) || std::isinf(other[i][j])) {
        res = 1;
      }
    }
  }
  return res;
}

}  // namespace proj