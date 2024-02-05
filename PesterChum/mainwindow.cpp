#include "mainwindow.h"
#include "ui_mainwindow.h"

void MessageModel::addMessage(const QString &text)
{
    QStandardItem *item = new QStandardItem(QString("%1").arg(text));
    appendRow(item);
}

MessageModel *model = new MessageModel;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    reg = new Auth;
    reg->show();

    ui->listView->setModel(model);

    connect(reg, &Auth::sendUser, this, &MainWindow::startChat);
}

MainWindow::~MainWindow()
{
    reader->join();
    delete reader;
    delete ui;
}

void MainWindow::GotMessageHandler()
{
    while(true) {
        model->addMessage(cl->readMessage().c_str());
    }
}

void MainWindow::startThread()
{
    reader = new std::thread(&MainWindow::GotMessageHandler, this);
}

void MainWindow::startChat(std::string S_user, client *auth_cl)
{
    cl = auth_cl;
    username = S_user;
    ui->label->setText(username.c_str());
    this->show();
    startThread();
}

void MainWindow::on_pushButton_clicked()
{
    std::string message = ui->lineEdit->text().toStdString();

    if (!message.empty()) {
        message = username + ": " +message;
        model->addMessage(message.c_str());
        cl->writeMessage(message);
        ui->lineEdit->clear();
    }
}


void MainWindow::on_lineEdit_returnPressed()
{
    std::string message = ui->lineEdit->text().toStdString();

    if (!message.empty()) {
        message = username + ": " +message;
        model->addMessage(message.c_str());
        cl->writeMessage(message);
        ui->lineEdit->clear();
    }
}

