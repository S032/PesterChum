#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::setup_listview()
{
    ui->UserList->setModel(usermodel);
    ui->UserList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->UserList->verticalScrollBar()->setSingleStep(8);
    ui->UserList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->UserList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    delegate = new UserchatsDelegate(this);
    ui->UserList->setItemDelegate(delegate);
}

void MainWindow::setup_font()
{
    int id = QFontDatabase::addApplicationFont(":/fonts/ChixaDemibold.ttf");
    fontFamily = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont pixelFont(fontFamily);
    pixelFont.setPixelSize(58);
    pixelFont.setBold(true);

    ui->label->setFont(pixelFont);
    pixelFont.setPixelSize(39);
    ui->label_2->setFont(pixelFont);
    pixelFont.setPixelSize(35);
    ui->label_3->setFont(pixelFont);
    ui->MyUserName->setFont(pixelFont);
    ui->UserName->setFont(pixelFont);
    ui->Mood->setFont(pixelFont);
    pixelFont.setPixelSize(35);
    ui->pushButton->setFont(pixelFont);
    ui->pushButton_2->setFont(pixelFont);
    ui->pushButton_3->setFont(pixelFont);
    ui->pushButton_4->setFont(pixelFont);
    ui->pushButton_5->setFont(pixelFont);
    ui->pushButton_6->setFont(pixelFont);
    ui->pushButton_7->setFont(pixelFont);
    pixelFont.setPixelSize(28);
    ui->UserList->setFont(pixelFont);
}

void MainWindow::setup_images()
{
    friends_pic = new QPixmap("://images/friends_41x41.png");
    friends_pic_alert = new QPixmap("://images/friends_alert_41x41.png");
    notifications_pic = new QPixmap("://images/notifications_41x41.png");
    notifications_pic_alert = new QPixmap("://images/notifications_alert_41x41.png");

    QIcon ButtonIcon(*(friends_pic));
    ui->friends_button->setIcon(ButtonIcon);
    ui->friends_button->setIconSize(friends_pic->rect().size());

    QIcon ButtonIcon2(*(notifications_pic));
    //ui->notificationsButton->setIcon(ButtonIcon2);
    //ui->notificationsButton->setIconSize(notifications_pic->rect().size());
    //Mood button
    smile_pic = new QPixmap("://images/smile_43x43.png");
    inactive_pic = new QPixmap("://images/inactive_43x43.png");
    angry_pic = new QPixmap("://images/angry_43x43.png");
    //Pester Chums
    PesterChums = new QPixmap(":/images/PesterChums.png");

    //buttons
    QIcon SmileIcon(*(smile_pic));
    QIcon AngryIcon(*(angry_pic));
    ui->pushButton_2->setIcon(SmileIcon);
    ui->pushButton_2->setIconSize(smile_pic->rect().size());
    ui->pushButton_3->setIcon(AngryIcon);
    ui->pushButton_3->setIconSize(angry_pic->rect().size());
    ui->pushButton_4->setIcon(SmileIcon);
    ui->pushButton_4->setIconSize(smile_pic->rect().size());
    ui->pushButton_5->setIcon(SmileIcon);
    ui->pushButton_5->setIconSize(smile_pic->rect().size());
    ui->pushButton_6->setIcon(SmileIcon);
    ui->pushButton_6->setIconSize(smile_pic->rect().size());
    ui->pushButton_7->setIcon(SmileIcon);
    ui->pushButton_7->setIconSize(smile_pic->rect().size());
    //big label
    ui->picture->setPixmap(*(PesterChums));
    ui->picture->setContentsMargins(0, 0, 0, 0);
}

void MainWindow::setup_buttons_mapper()
{
    signalMapper = new QSignalMapper(this);

    QObject::connect(ui->pushButton_2, SIGNAL(clicked()), signalMapper, SLOT(map()));
    QObject::connect(ui->pushButton_3, SIGNAL(clicked()), signalMapper, SLOT(map()));
    QObject::connect(ui->pushButton_4, SIGNAL(clicked()), signalMapper, SLOT(map()));
    QObject::connect(ui->pushButton_5, SIGNAL(clicked()), signalMapper, SLOT(map()));
    QObject::connect(ui->pushButton_6, SIGNAL(clicked()), signalMapper, SLOT(map()));
    QObject::connect(ui->pushButton_7, SIGNAL(clicked()), signalMapper, SLOT(map()));

    signalMapper->setMapping(ui->pushButton_2, "smile");
    signalMapper->setMapping(ui->pushButton_3, "angry");
    signalMapper->setMapping(ui->pushButton_4, "smile");
    signalMapper->setMapping(ui->pushButton_5, "smile");
    signalMapper->setMapping(ui->pushButton_6, "smile");
    signalMapper->setMapping(ui->pushButton_7, "smile");

    QObject::connect(signalMapper, &QSignalMapper::mappedString, this, &MainWindow::changeStatus);
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    //setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    ui->setupUi(this);
    setup_font();
    cl = new client(this);
    friendReq = new friendrequset(this, fontFamily);
    reg = new Auth(nullptr, cl);
    reg->show();
    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::PesterClicked);
    QObject::connect(ui->UserList, &QListView::clicked, this, [this](const QModelIndex &index){
        ui->UserList->setCurrentIndex(index);
    });
    QObject::connect(cl, &client::throwFatalError, this, &MainWindow::proccesFatalError);
    QObject::connect(cl, &client::throwError, this, &MainWindow::proccesError);
    QObject::connect(friendReq, &friendrequset::sendRequest, this, &MainWindow::sendRequest);
    QObject::connect(friendReq, &friendrequset::sendRequestAnswer, this, &MainWindow::sendRequestAnswer);
    QObject::connect(reg, &Auth::sendUser, this, &MainWindow::start);

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

void MainWindow::startReadInThread()
{
    ReadThread = new ThreadController(cl);
    QObject::connect(ReadThread, &ThreadController::throwFatalError, this, &MainWindow::proccesFatalError);
    QObject::connect(ReadThread, &ThreadController::throwError, this, &MainWindow::proccesError);
    QObject::connect(ReadThread, &ThreadController::messageReady, this, &MainWindow::sendMessageByUsername);
    QObject::connect(ReadThread, &ThreadController::listOfUsersReady, this, &MainWindow::updateListOfUsers);
    QObject::connect(ReadThread, &ThreadController::getUsers, this, &MainWindow::getUsers);
    QObject::connect(ReadThread, &ThreadController::getIcRequests, this, &MainWindow::getIcRequests);
    QObject::connect(ReadThread, &ThreadController::getOgRequests, this, &MainWindow::getOgRequests);
    QObject::connect(ReadThread, &ThreadController::listOfIcReqReady, friendReq, &friendrequset::updateListOfIcReq);
    QObject::connect(ReadThread, &ThreadController::listOfOgReqReady, friendReq, &friendrequset::updateListOfOgReq);
    QObject::connect(ReadThread, &ThreadController::gotNewMessage, this, &MainWindow::gotNewMessage);
    QObject::connect(ReadThread, &ThreadController::sendLeaveMessage, this, &MainWindow::friendLeft);
    QObject::connect(ReadThread, &ThreadController::sendJoinMessage, this, &MainWindow::friendJoin);
    ReadThread->emit startRead(cl);
}

void MainWindow::changeStatus(const QString &status)
{
    qDebug() << "chstatus worked";
    std::string std_status = status.toStdString();
    cl->writeMessage("/chstatus/" + std_status);
    if (status == "smile") {
        QString userlabel = "<img src=':/images/smile_28x23.png' style='vertical-align: middle;'> " + username;
        ui->UserName->setText(userlabel);
    }
    if (status == "angry") {
        QString userlabel = "<img src=':/images/angry_28x23.png' style='vertical-align: middle;'> " + username;
        ui->UserName->setText(userlabel);
    }
}

void MainWindow::deleteFriend(const QPoint& pos, const QModelIndex index)
{
    QPoint globalPos = ui->UserList->mapToGlobal(pos);

    QMenu menu;
    menu.addAction("удалить");

    QAction *selectedItem = menu.exec(globalPos);
    if (selectedItem) {
        //добавить окно спрашивающее уверены ли вы
        std::string friend_name = index.data().toString().toStdString();
        std::string query = "/delfriend/" + friend_name;
        cl->writeMessage(query);
        sendQuery("getlist");
    }
}

void MainWindow::gotNewMessage(QString name)
{
    QModelIndexList indexes = usermodel->match(usermodel->index(0, 0), Qt::DisplayRole, QVariant::fromValue(name), -1, Qt::MatchExactly);
    QModelIndex index = indexes.first();
    qDebug() << usercontroller->chatIsOpen(index.data(Qt::DisplayRole).toString().toStdString());
    if (usercontroller->chatIsOpen(index.data(Qt::DisplayRole).toString().toStdString())) {

    }
    else {
        usermodel->setData(index, "://images/new_message_alert.png", Qt::UserRole + 1);
    }
}

void MainWindow::friendLeft(std::string name)
{
    if (usercontroller->chatIsOpen(name))
        usercontroller->sendLeftMessage(name);
}

void MainWindow::friendJoin(std::string name)
{
    if (usercontroller->chatIsOpen(name))
        usercontroller->sendJoinMessage(name);
}

void MainWindow::sendQuery(std::string query)
{
    query = "/" + query + "/";
    cl->writeMessage(query);
}

void MainWindow::sendRequest(std::string name) {
    name = "/sendreq/" + name;
    cl->writeMessage(name);
    sendQuery("getogreq");
}

void MainWindow::sendRequestAnswer(std::string name, std::string answer) {
    name = "/reqanswer/" + name + "/" + answer;
    cl->writeMessage(name);
    emit getIcRequests();
}

void MainWindow::getUsers()
{
    sendQuery("getlist");
}

void MainWindow::getIcRequests()
{
    sendQuery("geticreq");
    if(friendReq->isHidden()) {
        QIcon ButtonIcon(*(friends_pic_alert));
        ui->friends_button->setIcon(ButtonIcon);
        ui->friends_button->setIconSize(friends_pic->rect().size());
    }
    sendQuery("getlist");
}

void MainWindow::getOgRequests()
{
    sendQuery("getogreq");
    sendQuery("getlist");
}

void MainWindow::start(QString S_user, client *auth_cl, QString lastStatus) {
    cl = auth_cl;
    auth_cl->giveNewParent(this);
    username = S_user;
    usermodel = new UserchatModel(this);
    usercontroller = new UserchatsController();
    startReadInThread();
    sendQuery("getlist");
    sendQuery("geticreq");
    sendQuery("getogreq");
    setup_listview();
    setup_images();
    QString userlabel = "<img src=':/images/"+lastStatus+"_28x23.png' style='vertical-align: middle;'> " + username;
    ui->UserName->setText(userlabel);
    setup_buttons_mapper();
    this->show();
}

void MainWindow::sendMessageByUsername(std::string sender_name, std::string message)
{
    usercontroller->sendMessageToChat(sender_name, message);
}

void MainWindow::updateListOfUsers(std::string userlist)
{
    // /usr1/status//usr2/.../usrN/status/
    ui->UserList->model()->removeRows(0, ui->UserList->model()->rowCount());
    size_t l_pos, r_pos;
    std::string recipient_name;
    std::string status;
    std::string avaible;
    l_pos = 1;
    if((r_pos = userlist.find("/", 1)) == std::string::npos) return;
    while(r_pos != std::string::npos) {
        // getting username
        recipient_name = {userlist.begin() + l_pos, userlist.begin() + r_pos};
        // getting status
        l_pos = r_pos + 1;
        r_pos = userlist.find("/", l_pos);
        status = {userlist.begin() + l_pos, userlist.begin() + r_pos};
        // doing stuff
        usermodel->addUserchat(recipient_name.c_str(), status.c_str());
        UserChat *chat = new UserChat(this, recipient_name, username.toStdString(), cl, fontFamily);
        usercontroller->addChat(recipient_name, chat);
        // next step
        l_pos = r_pos + 1;
        r_pos = userlist.find("/", l_pos);
    }
}

void MainWindow::PesterClicked()
{
    QModelIndexList selectedIndexes = ui->UserList->selectionModel()->selectedIndexes();
    if (!selectedIndexes.isEmpty()) {
        QModelIndex selectedIndex = selectedIndexes.first();
        QVariant data = selectedIndex.data(Qt::DisplayRole);
        QString text = data.toString();

        usercontroller->openChat(text.toStdString());
        usermodel->setData(selectedIndex, NULL, Qt::UserRole + 1);
    }
}


void MainWindow::on_UserList_doubleClicked(const QModelIndex &index)
{
    QVariant data = index.data(Qt::DisplayRole);
    QString text = data.toString();

    usercontroller->openChat(text.toStdString());
    usermodel->setData(index, NULL, Qt::UserRole + 1);
}

void MainWindow::on_friends_button_clicked()
{
    QIcon ButtonIcon(*(friends_pic));
    ui->friends_button->setIcon(ButtonIcon);
    ui->friends_button->setIconSize(friends_pic->rect().size());
    friendReq->show();
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QPoint pos = event->pos();
    QPoint itemPos = ui->UserList->viewport()->mapFrom(this, pos);
    QModelIndex index = ui->UserList->indexAt(itemPos);
    qDebug() << index.isValid();
    if (index.isValid()) {
        deleteFriend(itemPos, index);
    }
}



