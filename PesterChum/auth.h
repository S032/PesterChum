#ifndef AUTH_H
#define AUTH_H

#include "client.h"
#include <QDialog>

namespace Ui {
class Auth;
}

class Auth : public QDialog
{
    Q_OBJECT

public:
    explicit Auth(QWidget *parent = nullptr);
    ~Auth();

private slots:
    void on_SignInButton_clicked();

    void on_SignUpButton_clicked();

private:
    Ui::Auth *ui;

public:
    client *cl;
signals:
    void sendUser(std::string R_user, std::string R_pass, client *cl);
};

#endif // AUTH_H
