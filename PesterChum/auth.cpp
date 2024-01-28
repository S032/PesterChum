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

}


void Auth::on_SignUpButton_clicked() // SIGN UP EVENT
{
    std::string username = ui->userLine->text().toStdString();
    std::string password = ui->passwordLine->text().toStdString();

    cl->Connect();
    if(!cl->reg_user(username, password)) {
        cl->err("Failed to registratre,\nuser name is taken!");
        return;
    }
    emit sendUser(username, password, cl);
    this->close();

}

