#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "auth.h"
#include "client.h"
#include <QMessageBox>
#include <QListView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <thread>
#include <iostream>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void GotMessageHandler();
    void startThread();

private:
    Ui::MainWindow  *ui;
    Auth            *reg;
    client          *cl;
    std::string     username;
    std::thread     *reader;
public slots:
    void startChat(std::string S_user, client *cl);
private slots:
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();
};

class MessageModel : public QStandardItemModel
{
public:
    MessageModel(QObject *parent = nullptr) : QStandardItemModel(parent) {}
    void addMessage(const QString &text);
};

#endif // MAINWINDOW_H
