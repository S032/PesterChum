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
    cl = new client(this);
    QObject::connect(cl, &client::throwFatalError, this, &MainWindow::proccesFatalError);
    QObject::connect(cl, &client::throwError, this, &MainWindow::proccesError);
    reg = new Auth(nullptr, cl);
    reg->show();

    connect(reg, &Auth::sendUser, this, &MainWindow::startChat);
}

MainWindow::~MainWindow()
{
    ReadThread->stop();
    delete ReadThread;
    delete ui;
}

void MainWindow::proccesFatalError(QString error)
{
    //QMessageBox::critical(this, "FATAL ERROR", error);
    printf("bruh\n");
    exit(EXIT_FAILURE);
}

void MainWindow::proccesError(QString error)
{
    QMessageBox::warning(this, "WARNING", error);
}

void MainWindow::handleResult(std::string result)
{
    model->addMessage(result.c_str(), "blue");
}

void MainWindow::startReadInThread(client *cl)
{
    MessageThread *ReadThread = new MessageThread(this, cl);
    connect(ReadThread, &MessageThread::resultReady, this, &MainWindow::handleResult);
    connect(ReadThread, &MessageThread::finished, this, &QObject::deleteLater);
    ReadThread->start();
}

void MainWindow::startChat(std::string S_user, client *auth_cl)
{
    cl = auth_cl;
    auth_cl->giveNewParent(this);
    model = new MessageModel(this, ui);
    startReadInThread(auth_cl);
    username = S_user;
    setup_listview();
    ui->label->setText(username.c_str());
    this->show();
}

void MainWindow::FinishThread()
{
    ReadThread->wait();
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

