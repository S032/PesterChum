#include "auth.h"
#include "ui_auth.h"
#include "mainwindow.h"
#include <QLineEdit>
#include <string>

Auth::Auth(QWidget *parent, client *m_cl)
    : QDialog(parent)
    , ui(new Ui::Auth)
    , cl(m_cl)
{
    ui->setupUi(this);
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
        cl->emit throwError("Failed to sign in!");
        cl->Close();
        return;
    }
    if (res == -2) {
        cl->emit throwError("Check ur login or password!");
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
        cl->emit throwError("Failed to registratre!");
        cl->Close();
        return;
    }
    if (res == -2) {
        cl->emit throwError("User name's taken!");
        cl->Close();
        return;
    }
    emit sendUser(username, cl);
    this->close();
}

