#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "auth.h"
#include "client.h"
#include "messagethread.h"
#include "messagemodel.h"
#include <QMessageBox>
#include <QListView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QThread>

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
private:
    void setup_listview();
    void throw_message();
    void startReadInThread(client *client);
private:
    Ui::MainWindow  *ui;
    MessageModel    *model;
    MessageThread   *ReadThread;
    Auth            *reg;
    client          *cl;
    std::string     username;
public slots:
    void proccesFatalError(QString error);
    void proccesError(QString error);
    void startChat(std::string S_user, client *cl);
    void handleResult(std::string result);
    void FinishThread();
private slots:
    void on_pushButton_clicked();
    void on_lineEdit_returnPressed();
};

#endif // MAINWINDOW_H
