#include "auth.h"
#include "ui_auth.h"
#include "client.h"
#include <QLineEdit>
#include <string>

client *cl;

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
    cl->reg_user(username, password);
    cl->Close();
}

