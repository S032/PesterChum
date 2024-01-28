#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    reg = new Auth;
    reg->show();
    connect(reg, &Auth::sendUser, this, &MainWindow::startChat);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::startChat(std::string S_user, std::string S_pass, client *cl)
{
    ui->label->setText(S_user.c_str());
    this->show();
    cl->reg_user(S_user, S_pass);
}
