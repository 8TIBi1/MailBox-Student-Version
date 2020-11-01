#include "login.h"
#include "ui_login.h"
#include "head.h"
#include <unistd.h>
#include <QtNetwork>
#include <QMessageBox>
#include <string.h>

#define SEND "SEND"
#define RECEIVE "RECV"
#define CONN "CONN"
#define QUIT "QUIT"

struct loginJudge{
    char sendName[30];
    char sendPassword[30];
};

login::login(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::login)
{
    ui->setupUi(this);
    connect(&people,&users::reshow,[this]()
        {
            people.close();
            this->show();
        });
    connect(&reg,&Register::reshow,[this]()
        {
            reg.close();
            this->show();
        });
}

login::~login()
{
    delete ui;
}


void login::on_pushButton_clicked()
{

    name = ui->lineEdit->text();
    password = ui->lineEdit_2->text();
    struct loginJudge sendLogin;
    memset(&sendLogin, 0, sizeof(sendLogin));

    strcpy(sendLogin.sendName,name.toStdString().data());
    strcpy(sendLogin.sendPassword,password.toStdString().data());

    QTcpSocket *client;
    char sip[64];

    QString qip ("192.168.64.130");
    QString qport ("8888");
    QString qmessage;

    strcpy(sip, qip.toStdString().data());
//    ui->textMessage->setText(qip.append(":").append(qport).append("\n"));

    client = new QTcpSocket(this);
    client->connectToHost(QHostAddress(sip), qport.toInt());
  //  client->write("Hello, I am the client\n");

   // strncpy(sendName, SEND, name.length());
   // strncpy(sendPassword, SEND, password.length() );
    strncpy(sendLogin.sendName, name.toStdString().data(), name.length());
    strncpy(sendLogin.sendPassword, password.toStdString().data(), password.length());

    client->write("LOGI");
    client->write("\n");
    client->write(sendLogin.sendName);
    client->write("\n");
    client->write(sendLogin.sendPassword);
    client->write("\n");

    connect(client, &QTcpSocket::readyRead, [=]()
    {
        //获取对方发送的内容
        QByteArray array = client->readAll();
        //追加到编辑区中
  //      ui->textMessage->append(array + "\n");
        if (array[0]=='C')  //lai ge han shu
        {
            qDebug() << "登录成功";
            QMessageBox::information(this, "Login", "登录成功");
            ui->lineEdit_2->clear();
            this->hide();
            people.show();
        }
        else if(array[0]=='W')
        {
            qDebug() <<"账号或密码错误,请重试";
            ui->lineEdit_2->clear();
            ui->lineEdit->clear();
            QMessageBox::information(this, "Login", "账号或密码错误,请重试");
        }else;
        QMessageBox::information(this, "Server Message", array);
        client->write(QUIT);
    });
    qDebug() << qip;
    qDebug() << qport;

}

void login::on_pushButton_2_clicked()
{
    ui->lineEdit_2->clear();
    ui->lineEdit->clear();
    this->hide();
    reg.show();

}
