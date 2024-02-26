#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "auth.h"
#include "client.h"
#include "userchat.h"
#include "messagethread.h"
#include "coloredmessagedelegate.h"
#include <QMessageBox>
#include <QListView>
#include <QFontDatabase>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QThread>
#include <QDebug>

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
    void setup_font();
    void throw_message();
    void startReadInThread(client *client);
private:
    Ui::MainWindow         *ui;
    ThreadController       *ReadThread;
    ColoredMessageDelegate *delegate;
    Auth                   *reg;
    client                 *cl;
    std::string            username;
    QString                fontFamily;
public slots:
    void proccesFatalError(QString error);
    void proccesError(QString error);
    void start(std::string S_user, client *cl);
signals:
    void startChat(std::string S_user, client *cl, QString fontFamily);
private slots:
    //void on_pushButton_clicked();
    //void on_lineEdit_returnPressed();
};

#endif // MAINWINDOW_H
