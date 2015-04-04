﻿#include "../ui/mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QCoreApplication::setApplicationName("Viqo");
  QCoreApplication::setApplicationVersion("2.0");

  QCommandLineParser parser;
  parser.setApplicationDescription(
        QStringLiteral("Qt で作成されたマルチプラットフォームコメビュです"));
  parser.addHelpOption();
  parser.addVersionOption();

  // Process the actual command line arguments given by the user
  parser.process(a);

  MainWindow w;
  w.show();

  return a.exec();
}
