#include <QApplication>

#include "mainwindow.h"
#include "model/affine_transformation.h"
#include "model/parser.h"
#include "controller.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  proj::Parser parser_obj;
  proj::Caretaker caretaker;

  proj::AffineTransformationFacade& transformation_obj =
      proj::AffineTransformationFacade::getInstance();
  proj::Controller& controller_obj = proj::Controller::getInstance(
      &parser_obj, &transformation_obj, &caretaker);
  MainWindow view(&controller_obj);
  view.show();
  return a.exec();
}
