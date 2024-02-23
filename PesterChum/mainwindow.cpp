#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::setup_listview()
{
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    delegate = new ColoredMessageDelegate(ui->listView, ui->listView);
    ui->listView->setItemDelegate(delegate);
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
    cl->Close();
    delete ReadThread;
    delete ui;
}

void MainWindow::proccesFatalError(QString error)
{
    cl->Close();
    delete ReadThread;
    QMessageBox::critical(this, "FATAL ERROR", error);
    exit(EXIT_FAILURE);
}

void MainWindow::proccesError(QString error)
{
    QMessageBox::warning(this, "WARNING", error);
}

void MainWindow::startReadInThread(client *cl)
{
    ReadThread = new ThreadController(cl, model);
    QObject::connect(ReadThread, &ThreadController::throwFatalError, this, &MainWindow::proccesFatalError);
    ReadThread->emit startRead(cl);
}

void MainWindow::startChat(std::string S_user, client *auth_cl) {
    cl = auth_cl;
    auth_cl->giveNewParent(this);
    model = new MessageModel(this, ui);
    startReadInThread(auth_cl);
    username = S_user;
    setup_listview();
    ui->label->setText(username.c_str());
    this->show();
}

void MainWindow::throw_message() {
    std::string message = ui->lineEdit->text().toStdString();
    if (!message.empty()) {
        message = username + ": " +message;
        model->addMessage(message.c_str(), Qt::blue);
        cl->writeMessage(message);
        ui->lineEdit->clear();
    }
}

void MainWindow::on_pushButton_clicked() {
    throw_message();
}

void MainWindow::on_lineEdit_returnPressed() {
    throw_message();
}

