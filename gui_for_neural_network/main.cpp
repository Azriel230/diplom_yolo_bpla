#include "mainwindow.h"

#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.setWindowTitle("Распознавание воздушных объектов с помощью нейросети");
//    w.show();
    w.showMaximized();
    return a.exec();
}
