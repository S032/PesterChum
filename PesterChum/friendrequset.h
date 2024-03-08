#ifndef FRIENDREQUSET_H
#define FRIENDREQUSET_H

#include <QDialog>
#include <QFontDatabase>
#include "itemdelegate.h"
#include "customItemModel.h"

namespace Ui {
class friendrequset;
}

class friendrequset : public QDialog
{
    Q_OBJECT
public:
    explicit friendrequset(QWidget *parent = nullptr, QString m_fontFamily = nullptr);
    ~friendrequset();
private:
    void setup_icReqList();
    void setup_ogReqList();
    void set_font();
private:
    Ui::friendrequset *ui;
    QString           fontFamily;
    RequestModel      *icreqmodel;
    RequestModel      *ogreqmodel;
    UserchatsDelegate *delegate;
public slots:
    void updateListOfIcReq(std::string icreqlist);
    void updateListOfOgReq(std::string ogreqlist);
    void acceptRequest();
    void denieRequest();
private slots:
    void on_lineEdit_returnPressed();
    void on_pushButton_3_clicked();

signals:
    void sendRequest(std::string name);
    void sendRequestAnswer(std::string name, std::string answer);
};

#endif // FRIENDREQUSET_H
