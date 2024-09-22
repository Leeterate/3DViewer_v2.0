#include <gtest/gtest.h>

#include "../model/matrix_oop.h"
#include "../model/parser.h"

TEST(Parser, test_1) {
  proj::Parser pars;
  std::string filename = "tests/obj/cube.obj";
  pars.OpenFile(filename);

  EXPECT_EQ(pars.GetVertexes(), 8);
  EXPECT_EQ(pars.GetPolygons(), 12);
}

TEST(Parser, test_2) {
  proj::Parser pars;
  std::string filename = "blabla";
  pars.OpenFile(filename);

  EXPECT_EQ(pars.GetError(), 1);
}

TEST(Parser, test_3) {
  proj::Parser pars;
  std::string filename = "tests/obj/parser.txt";
  pars.OpenFile(filename);
  std::vector<proj::projMatrix> matrix = pars.GetMatrixPoints();

  proj::projMatrix res_matrix(4, 1);
  for (int i = 0, k = 1; i < 3; i++, k += 2) res_matrix[i][0] = k;
  EXPECT_EQ(matrix.at(0), res_matrix);

  for (int i = 0, k = 7; i < 3; i++, k += 2) res_matrix[i][0] = k;
  EXPECT_EQ(matrix.at(1), res_matrix);

  for (int i = 0, k = 13; i < 3; i++, k += 2) res_matrix[i][0] = k;
  EXPECT_EQ(matrix.at(2), res_matrix);

  for (int i = 0, k = 19; i < 3; i++, k += 2) res_matrix[i][0] = k;
  EXPECT_EQ(matrix.at(3), res_matrix);
}

TEST(Parser, test_4) {
  std::vector<proj::Polygon> expect_polygon = {{0, 1}, {1, 2}, {2, 0},
                                              {1, 2}, {2, 0}, {0, 1}};

  proj::Parser pars;
  std::string filename = "tests/obj/parser.txt";
  pars.OpenFile(filename);

  std::vector<proj::Polygon> polygons = pars.GetArrayPolygons();
  for (size_t i = 0; i < polygons.size(); i++) {
    EXPECT_EQ(polygons.at(i).pol[0], expect_polygon.at(i).pol[0]);
    EXPECT_EQ(polygons.at(i).pol[1], expect_polygon.at(i).pol[1]);
  }
}