#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::setup_listview()
{
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new MessageModel(this, ui);
    reg = new Auth;
    reg->show();
    setup_listview();

    connect(reg, &Auth::sendUser, this, &MainWindow::startChat);
}

MainWindow::~MainWindow()
{
    reader->join();
    delete reader;
    delete ui;
}

/////////////////////////////////////////////////////////////////////
void MainWindow::GotMessageHandler() //получение сообщения от сервера
{
    while(true) {
        model->addMessage(cl->readMessage().c_str(), "blue");
    }
}

void MainWindow::startThread() //поток для отдельного чтения
{
    reader = new std::thread(&MainWindow::GotMessageHandler, this);
}
///////////////////////////////////////////////////////////////////

void MainWindow::startChat(std::string S_user, client *auth_cl)
{
    cl = auth_cl;
    cl->giveNewParent(this);
    username = S_user;
    ui->label->setText(username.c_str());
    this->show();
    startThread();
}

void MainWindow::throw_message()
{
    std::string message = ui->lineEdit->text().toStdString();

    if (!message.empty()) {
        message = username + ": " +message;
        model->addMessage(message.c_str(), "red");
        cl->writeMessage(message);
        ui->lineEdit->clear();
    }
}

void MainWindow::on_pushButton_clicked()
{
    throw_message();
}


void MainWindow::on_lineEdit_returnPressed()
{
    throw_message();
}

