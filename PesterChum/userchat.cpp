#include "userchat.h"
#include "ui_userchat.h"

UserChat::UserChat(QWidget *parent, chatPacket_t chatinfo, windowsSystemHandler *m_winHandler)
    : QDialog(parent)
    , ui(new Ui::UserChat)
    , winHandler(m_winHandler)
    , cl(chatinfo.cl)
    , username(chatinfo.sender_name)
    , fontFamily(chatinfo.fontFamily)
{
    ui->setupUi(this);
    model = new MessageModel(this, ui);
    recipient_name = "::  " + chatinfo.recipient_name + "  ::";
    ui->label_2->setText(recipient_name.c_str());
    recipient_name = chatinfo.recipient_name;
    setup_font();
    setup_listview();
}

UserChat::~UserChat()
{
    delete ui;
}

void UserChat::setup_listview()
{
    ui->listView->setModel(model);
    ui->listView->verticalScrollBar()->setSingleStep(20);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    delegate = new ColoredMessageDelegate(ui->listView, ui->listView, fontFamily, 30);
    ui->listView->setItemDelegate(delegate);
}

void UserChat::setup_font()
{
    QFont pixelFont(fontFamily);
    pixelFont.setPixelSize(40);
    pixelFont.setBold(true);
    ui->label_2->setFont(pixelFont);
    ui->label->setFont(pixelFont);
    ui->pushButton->setFont(pixelFont);
    pixelFont.setPixelSize(30);
    pixelFont.setBold(false);
    ui->lineEdit->setFont(pixelFont);
    ui->listView->setFont(pixelFont);
}

void UserChat::writeSenderMessage(std::string message)
{
    model->addMessage(message.c_str(), Qt::red);
}

void UserChat::pre_initBuffAppend(std::string message)
{
    pre_initBuffer.push_back(message);
}

void UserChat::pre_initBuffLoadout()
{
    if (!pre_initBuffer.empty()) {
        for (std::string message : pre_initBuffer) {
            model->addMessage(message.c_str(), Qt::red);
        }
        pre_initBuffer.clear();
    }
}

bool UserChat::wasItOpened()
{
    return wasOpened;
}

void UserChat::setItOpened()
{
    wasOpened = true;
}

void UserChat::writeSystemMessage(QString message)
{
    std::string recp_name = recipient_name + " ";
    message = recp_name.c_str() + message + " " + winHandler->getTime();
    model->addMessage(message, Qt::gray);
}

void UserChat::throw_message() {
    std::string message = ui->lineEdit->text().toStdString();
    std::string messageToSend = message;
    if (!message.empty()) {
        message = username + ": " + message;
        model->addMessage(message.c_str(), Qt::blue);
        messageToSend = "/sendto/" + recipient_name + "/" + messageToSend;
        cl->writeMessage(messageToSend);
        ui->lineEdit->clear();
    }
}

void UserChat::on_pushButton_clicked() {
    throw_message();
}

void UserChat::on_lineEdit_returnPressed() {
    throw_message();
}
///////////////////////////////////////////////////////
UserchatsController::UserchatsController(QWidget *m_parent, windowsSystemHandler *m_winHandler)
    :
    parent(m_parent),
    winHandler(m_winHandler)
{}

bool UserchatsController::chatIsOpen(std::string login)
{
    return !userchats[login]->isHidden();
}

void UserchatsController::addChat(chatPacket_t chatinfo)
{
    UserChat *chat = new UserChat(parent, chatinfo);
    userchats.insert({chatinfo.recipient_name, chat});
}

void UserchatsController::openChat(std::string login)
{
    userchats[login]->show();
    userchats[login]->pre_initBuffLoadout();
    userchats[login]->setItOpened();
}

void UserchatsController::sendMessageToChat(std::string login, std::string message)
{
    if (userchats[login]->isHidden() && !userchats[login]->wasItOpened()) {
        if (parent->isMinimized()) emit throwNotification(NEW_MSG, message);
        userchats[login]->pre_initBuffAppend(message);
    }
    else {
        if (parent->isMinimized()) emit throwNotification(NEW_MSG, message);
        userchats[login]->writeSenderMessage(message);
    }
}

void UserchatsController::sendLeftMessage(std::string login)
{
    userchats[login]->writeSystemMessage("вышел в offline в");
}

void UserchatsController::sendJoinMessage(std::string login)
{
    userchats[login]->writeSystemMessage("снова online с");
}
