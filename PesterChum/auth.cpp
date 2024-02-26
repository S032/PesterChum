#include "auth.h"
#include "ui_auth.h"
#include "mainwindow.h"
#include <QLineEdit>
#include <string>

void Auth::setup_font()
{
    int id = QFontDatabase::addApplicationFont(":/fonts/ChixaDemibold.ttf");
    QString fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont pixelFont(fontFamily);
    pixelFont.setPixelSize(62);
    pixelFont.setBold(true);

    ui->label_3->setFont(pixelFont);
    pixelFont.setPixelSize(40);
    ui->label->setFont(pixelFont);
    ui->label_2->setFont(pixelFont);
    ui->SignUpButton->setFont(pixelFont);
    ui->SignInButton->setFont(pixelFont);
}

Auth::Auth(QWidget *parent, client *m_cl)
    : QDialog(parent)
    , ui(new Ui::Auth)
    , cl(m_cl)
{
    ui->setupUi(this);
    ui->passwordLine->setEchoMode(QLineEdit::Password);
    setup_font();

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

