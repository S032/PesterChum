#include "friendrequset.h"
#include "ui_friendrequset.h"

void friendrequset::setup_icReqList()
{
    ui->incoming_req->setModel(icreqmodel);
    ui->incoming_req->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->incoming_req->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    delegate = new UserchatsDelegate(this);
    ui->incoming_req->setItemDelegate(delegate);
}

void friendrequset::setup_ogReqList()
{
    ui->outgoing_req->setModel(ogreqmodel);
    ui->outgoing_req->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->outgoing_req->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    delegate = new UserchatsDelegate(this);
    ui->outgoing_req->setItemDelegate(delegate);
}

void friendrequset::set_font()
{
    QFont pixelFont(fontFamily);
    pixelFont.setPixelSize(35);
    pixelFont.setBold(true);
    ui->label_2->setFont(pixelFont);
    ui->label->setFont(pixelFont);
    pixelFont.setPixelSize(40);
    ui->pushButton->setFont(pixelFont);
    ui->pushButton_2->setFont(pixelFont);
    ui->pushButton_3->setFont(pixelFont);
    pixelFont.setPixelSize(28);
    ui->incoming_req->setFont(pixelFont);
    ui->outgoing_req->setFont(pixelFont);
    pixelFont.setPixelSize(20);
    pixelFont.setBold(false);
    ui->lineEdit->setFont(pixelFont);

}

void friendrequset::updateListOfIcReq(std::string icreqlist)
{
    // /req1/req2/.../reqN/
    ui->incoming_req->model()->removeRows(0, ui->incoming_req->model()->rowCount());
    size_t l_pos, r_pos;
    std::string requester_name;
    l_pos = 1;
    if((r_pos = icreqlist.find("/", 1)) == std::string::npos) return;
    while(r_pos != std::string::npos) {
        requester_name = {icreqlist.begin() + l_pos, icreqlist.begin() + r_pos};
        requester_name += " хочет стать вашим другом!";
        icreqmodel->addRequest(requester_name.c_str());
        l_pos = r_pos + 1;
        r_pos = icreqlist.find("/", l_pos);
    }
}

void friendrequset::updateListOfOgReq(std::string ogreqlist)
{
    // /req1/req2/.../reqN/
    ui->outgoing_req->model()->removeRows(0, ui->outgoing_req->model()->rowCount());
    size_t l_pos, r_pos;
    std::string requester_name;
    l_pos = 1;
    if((r_pos = ogreqlist.find("/", 1)) == std::string::npos) return;
    while(r_pos != std::string::npos) {
        requester_name = {ogreqlist.begin() + l_pos, ogreqlist.begin() + r_pos};
        requester_name = "ожидаем ответа от " + requester_name;
        ogreqmodel->addRequest(requester_name.c_str());
        l_pos = r_pos + 1;
        r_pos = ogreqlist.find("/", l_pos);
    }
}

void friendrequset::acceptRequest()
{
    QModelIndexList selectedIndexes = ui->incoming_req->selectionModel()->selectedIndexes();
    if(!selectedIndexes.isEmpty()) {
        QModelIndex selectedIndex = selectedIndexes.first();
        QVariant data = selectedIndex.data(Qt::DisplayRole);
        std::string text = data.toString().toStdString();

        std::string requester_name = {text.begin(), text.begin() + text.find(" ")};
        emit sendRequestAnswer(requester_name, "accept");
    }
}

void friendrequset::denieRequest()
{
    QModelIndexList selectedIndexes = ui->incoming_req->selectionModel()->selectedIndexes();
    if(!selectedIndexes.isEmpty()) {
        QModelIndex selectedIndex = selectedIndexes.first();
        QVariant data = selectedIndex.data(Qt::DisplayRole);
        std::string text = data.toString().toStdString();

        std::string requester_name = {text.begin(), text.begin() + text.find(" ")};
        emit sendRequestAnswer(requester_name, "denied");
    }
}

friendrequset::friendrequset(QWidget *parent, QString m_fontFamily)
    : QDialog(parent)
    , ui(new Ui::friendrequset)
    , fontFamily(m_fontFamily)
{
    ui->setupUi(this);
    icreqmodel = new RequestModel(this);
    ogreqmodel = new RequestModel(this);
    set_font();
    setup_icReqList();
    setup_ogReqList();
    QObject::connect(ui->pushButton, &QPushButton::clicked, this, &friendrequset::acceptRequest);
    QObject::connect(ui->pushButton_2, &QPushButton::clicked, this, &friendrequset::denieRequest);
    QObject::connect(ui->incoming_req, &QListView::clicked, this, [this](const QModelIndex &index){
        ui->incoming_req->setCurrentIndex(index);
    });
}

friendrequset::~friendrequset()
{
    delete ui;
}

void friendrequset::on_lineEdit_returnPressed()
{
    std::string name = ui->lineEdit->text().toStdString();
    emit sendRequest(name);
    ui->lineEdit->clear();
}


void friendrequset::on_pushButton_3_clicked()
{
    std::string name = ui->lineEdit->text().toStdString();
    emit sendRequest(name);
    ui->lineEdit->clear();
}

