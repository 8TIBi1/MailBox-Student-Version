#include "register.h"
#include "ui_register.h"
#include "head.h"

#define SEND "SEND"
#define RECEIVE "RECV"
#define CONN "CONN"
#define QUIT "QUIT"

Register::Register(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Register)
{
    ui->setupUi(this);
    ui->lineEdit->setAlignment(Qt::AlignHCenter);
    ui->lineEdit->setPlaceholderText("邮箱地址");
    ui->lineEdit_2->setAlignment(Qt::AlignHCenter);
    ui->lineEdit_2->setPlaceholderText("密码");
    ui->lineEdit_3->setAlignment(Qt::AlignHCenter);
    ui->lineEdit_3->setPlaceholderText("确认密码");
    ui->lineEdit_4->setAlignment(Qt::AlignHCenter);
    ui->lineEdit_4->setPlaceholderText("手机号");
}

Register::~Register()
{
    delete ui;
}

int  Register::CheckRegisterInput()
{
    QString name = ui->lineEdit->text();
    QString pwd1 = ui->lineEdit_2->text();
    QString pwd2 = ui->lineEdit_3->text();
    if(pwd1==pwd2){
        int flag1=0;
        int flag2=0;
        int flag3=0;
        int flag4=0;
        int flag5=0;
        int flag6=0;
        int flag7=0;
        int len1=name.length();
        int len2=pwd1.length();
        if(len1>=6&&len1<=16&&len2>=6&&len2<=16){
            if(name[0]>='A'&&name[0]<='z') flag1=1;
            for(int i=0;i<len1;i++){
                if(name[i]>='0'&&name[i]<='9') flag2=1;
                if(name[i]>='A'&&name[i]<='z') flag3=1;
                if(name[i]=='_') flag4=1;
            }
            for(int i=0;i<len2;i++){
                if(pwd1[i]>='0'&&name[i]<='9') flag5=1;
                if(pwd1[i]>='A'&&name[i]<='z') flag6=1;
                if(pwd1[i]<'0'||(name[i]>'9'&&name[i]<'A')||name[i]>'z') flag7=1;
            }
            if(flag1==1&&flag2==1&&flag3==1&&flag4==1&&flag5==1&&flag6==1&&flag7==1){
                return 0;
            }else{
                return 1;
            }
        }else{
            return 1;
        }
    }else{
        return 1;
    }
}

void Register::on_pushButton_clicked()
{
    int flag=CheckRegisterInput();
    if(flag==0){

        QString name=ui->lineEdit->text();
        QString pwd1 = ui->lineEdit_2->text();
        QString telephone = ui->lineEdit_4->text();

        char sendName[30];
        strcpy(sendName,name.toStdString().data());

        QTcpSocket *client;
        char sip[64];

        QString qip ("192.168.64.130");
        QString qport ("8888");
        QString qmessage;

        strcpy(sip, qip.toStdString().data());
        client = new QTcpSocket(this);
        client->connectToHost(QHostAddress(sip), qport.toInt());

    //    client->write("Hello, I am the client\n");
        client->write("REGI\n");
        client->write(sendName);
        client->write("\n");
        client->write(pwd1.toStdString().data());
        client->write("\n");
        client->write(telephone.toStdString().data());
        client->write("\n");

        connect(client, &QTcpSocket::readyRead, [=]()
        {
            //获取对方发送的内容
            QString array = client->readAll();
            client->write(QUIT);
        }); 

        connect(this, SIGNAL(sendUsrname(QString)),this, SLOT(recUsrname(QString)));
        QMessageBox::information(this, "Login", "注册成功");

        ui->lineEdit->clear();
        ui->lineEdit_2->clear();
        ui->lineEdit_3->clear();
        ui->lineEdit_4->clear();
        emit reshow();
        this->close();

    }else{
        QMessageBox::information(this, "Login", "注册失败");
    }
}

void Register::on_pushButton_2_clicked()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    emit reshow();
    this->close();
}
