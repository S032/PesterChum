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
    explicit Auth(QWidget *parent = nullptr, client *m_cl = nullptr);
    ~Auth();
private slots:
    void on_SignInButton_clicked();
    void on_SignUpButton_clicked();
private:
    Ui::Auth *ui;
private:
    void setup_font();
public:
    client *cl;
signals:
    void sendUser(QString R_user, client *cl, QString lastStatus);
};

#endif // AUTH_H
