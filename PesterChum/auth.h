#ifndef AUTH_H
#define AUTH_H

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
};

#endif // AUTH_H
