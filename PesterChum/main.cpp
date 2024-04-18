#include "mainwindow.h"

#include <QApplication>
#include <windows.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QIcon icon(":/images/smile_28x23.png");
    a.setWindowIcon(icon);
    MainWindow mainWindow;
    //MainWindow mainWindow2;
    return a.exec();
}
