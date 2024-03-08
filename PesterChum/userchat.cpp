#include "userchat.h"
#include "ui_userchat.h"

UserChat::UserChat(QWidget *parent, std::string r_name, std::string s_name, client *auth_cl, QString m_fontFamily)
    : QDialog(parent)
    , ui(new Ui::UserChat)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    cl = auth_cl;
    fontFamily = m_fontFamily;
    model = new MessageModel(this, ui);
    recipient_name = "::  " + r_name + "  ::";
    ui->label_2->setText(recipient_name.c_str());
    recipient_name = r_name;
    username = s_name;
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
    ui->listView->verticalScrollBar()->setSingleStep(20);;
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
void UserchatsController::addChat(std::string login, UserChat *chat)
{
    userchats.insert({login, chat});
}

void UserchatsController::openChat(std::string login)
{
    userchats[login]->show();
}

void UserchatsController::sendMessageToChat(std::string login, std::string message)
{
    userchats[login]->writeSenderMessage(message);
}
