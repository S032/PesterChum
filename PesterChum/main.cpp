#include "mainwindow.h"

#include <QApplication>
#include <windows.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow mainWindow;
    MainWindow mainWindow2;
    return a.exec();
}
