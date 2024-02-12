#include "auth.h"
#include "ui_auth.h"
#include "mainwindow.h"
//#include "client.h"
#include <QLineEdit>
#include <string>

Auth::Auth(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Auth)
{
    ui->setupUi(this);
    cl = new client(this);
    ui->passwordLine->setEchoMode(QLineEdit::Password);
}

Auth::~Auth()
{
    delete ui;
}

void Auth::on_SignInButton_clicked() // SIGN IN EVENT
{
    std::string username = ui->userLine->text().toStdString();
    std::string password = ui->passwordLine->text().toStdString();

    cl->Connect();
    int res = cl->log_user(username, password);
    if (res == -1) {
        cl->err("Failed to sign in!");
        cl->Close();
        return;
    }
    if (res == -2) {
        cl->err("Check ur login or password!");
        cl->Close();
        return;
    }
    emit sendUser(username, cl);
    this->close();
}


void Auth::on_SignUpButton_clicked() // SIGN UP EVENT
{
    std::string username = ui->userLine->text().toStdString();
    std::string password = ui->passwordLine->text().toStdString();

    cl->Connect();
    int res = cl->reg_user(username, password);
    if (res == -1) {
        cl->err("Failed to registratre!");
        cl->Close();
        return;
    }
    if (res == -2) {
        cl->err("User name's taken!");
        cl->Close();
        return;
    }
    emit sendUser(username, cl);
    this->close();
}

