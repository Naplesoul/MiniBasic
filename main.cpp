#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QObject::connect(&w, &MainWindow::quitSignal, &a, &QApplication::quit);
    w.show();
    return a.exec();
}
