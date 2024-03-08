#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "auth.h"
#include "client.h"
#include "userchat.h"
#include "friendrequset.h"
#include "messagethread.h"
#include "itemdelegate.h"
#include "customItemModel.h"
#include <QMessageBox>
#include <QListView>
#include <QMenu>
#include <QFontDatabase>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QThread>
#include <QDebug>
#include <windows.h>

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
    void setup_images();
    void sendQuery(std::string query);
    void throw_message();
    void startReadInThread();
private:
    Ui::MainWindow         *ui;
    ThreadController       *ReadThread;
    UserchatModel          *usermodel;
    UserchatsController    *usercontroller;
    UserchatsDelegate      *delegate;
    Auth                   *reg;
    friendrequset          *friendReq;
    client                 *cl;
    std::string            username;
    QString                fontFamily;
    QPixmap                *friends_pic;
    QPixmap                *friends_pic_alert;
    QPixmap                *smile_pic;
    QPixmap                *inactive_pic;
    QPixmap                *angry_pic;
public slots:
    void proccesFatalError(QString error);
    void proccesError(QString error);
    void start(std::string S_user, client *cl);
    void sendMessageByUsername(std::string sender_name, std::string message);
    void sendRequest(std::string name);
    void sendRequestAnswer(std::string name, std::string answer);
    void getUsers();
    void getIcRequests();
    void getOgRequests();
    void updateListOfUsers(std::string userlist);
    void PesterClicked();
    void deleteFriend(const QPoint &pos, const QModelIndex);
    void set_mood(std::string mood);
signals:
    void startChat(std::string S_user, client *cl, QString fontFamily);
private slots:
    void on_UserList_doubleClicked(const QModelIndex &index);
    void on_friends_button_clicked();
protected:
    void contextMenuEvent(QContextMenuEvent *event);
};

#endif // MAINWINDOW_H
