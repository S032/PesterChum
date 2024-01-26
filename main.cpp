#include "mainwindow.h"
#include "auth.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Auth reg;
    reg.show();
    return a.exec();
}
