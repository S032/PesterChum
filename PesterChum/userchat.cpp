#include "userchat.h"
#include "ui_userchat.h"

UserChat::UserChat(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::UserChat)
{
    ui->setupUi(this);
}

UserChat::~UserChat()
{
    delete ui;
}

void UserChat::setup_listview()
{
    ui->listView->setModel(model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listView->setSelectionMode(QAbstractItemView::NoSelection);
    ui->listView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->listView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    delegate = new ColoredMessageDelegate(ui->listView, ui->listView);
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


}

void UserChat::startChat(std::string S_user, client *auth_cl, QString m_fontFamily) {
    setWindowFlags(windowFlags() | Qt::MSWindowsFixedSizeDialogHint);
    cl = auth_cl;
    fontFamily = m_fontFamily;
    model = new MessageModel(this, ui);
    username = "::  " + S_user + "  ::";
    setup_font();
    setup_listview();
    ui->label_2->setText(username.c_str());
    this->show();
}

void UserChat::throw_message() {
    username = "Chepush"; // убрать
    std::string message = ui->lineEdit->text().toStdString();
    if (!message.empty()) {
        message = username + ": " +message;
        model->addMessage(message.c_str(), Qt::blue);
        //cl->writeMessage(message);
        ui->lineEdit->clear();
    }
}

void UserChat::on_pushButton_clicked() {
    throw_message();
}

void UserChat::on_lineEdit_returnPressed() {
    throw_message();
}
