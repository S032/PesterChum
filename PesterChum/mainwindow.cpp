#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::setup_listview()
{
    /*
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    delegate = new ColoredMessageDelegate(ui->listView, ui->listView);
    ui->listView->setItemDelegate(delegate);
    */
}

void MainWindow::setup_font()
{
    int id = QFontDatabase::addApplicationFont(":/fonts/ChixaDemibold.ttf");
    fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont pixelFont(fontFamily);
    pixelFont.setPixelSize(58);
    pixelFont.setBold(true);

    ui->label->setFont(pixelFont);
    ui->label_2->setFont(pixelFont);
    pixelFont.setPixelSize(40);
    ui->label_3->setFont(pixelFont);
    ui->MyUserName->setFont(pixelFont);
    ui->UserName->setFont(pixelFont);
    ui->Mood->setFont(pixelFont);
    ui->pushButton->setFont(pixelFont);
    ui->pushButton_2->setFont(pixelFont);
    ui->pushButton_3->setFont(pixelFont);
    ui->pushButton_4->setFont(pixelFont);
    ui->pushButton_5->setFont(pixelFont);
    ui->pushButton_6->setFont(pixelFont);
    ui->pushButton_7->setFont(pixelFont);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);
    setup_font();
    cl = new client(this);
    QObject::connect(cl, &client::throwFatalError, this, &MainWindow::proccesFatalError);
    QObject::connect(cl, &client::throwError, this, &MainWindow::proccesError);
    reg = new Auth(nullptr, cl);
    reg->show();
    UserChat *usr;
    usr = new UserChat(this);
    //this->show();

    connect(reg, &Auth::sendUser, this, &MainWindow::start);
    connect(this, &MainWindow::startChat, usr, &UserChat::startChat);
    //emit startChat("виниловыйДемиург", nullptr, fontFamily);
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
    ReadThread = new ThreadController(cl);
    QObject::connect(ReadThread, &ThreadController::throwFatalError, this, &MainWindow::proccesFatalError);
    ReadThread->emit startRead(cl);
}

void MainWindow::start(std::string S_user, client *auth_cl) {
    cl = auth_cl;
    auth_cl->giveNewParent(this);
    startReadInThread(auth_cl);
    username = S_user;
    setup_listview();
    ui->UserName->setText(username.c_str());
    this->show();
}

