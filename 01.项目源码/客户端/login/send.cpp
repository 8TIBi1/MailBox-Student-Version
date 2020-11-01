#include "send.h"
#include "ui_send.h"
#include "head.h"

#define SEND "SEND"
#define RECEIVE "RECV"
#define CONN "CONN"
#define QUIT "QUIT"

struct st_mail{
    char operation[8];
    char userid[30];
    char title[64];
    char content[952];
};

send::send(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::send)
{
    ui->setupUi(this);
    ui->frame->setFrameShape(QFrame::Box);
    ui->frame->setAutoFillBackground(true);
    ui->lineEdit_4->setText(tr("welcome!"));
}

send::~send()
{
    delete ui;
}


void send::on_pushButton_2_clicked()//返回
{
    emit reshow();
    this->close();

}

void send::on_pushButton_clicked()//发送
{

    QTcpSocket *client; // socket 句柄
    char sip[64];   // IP 地址字符串
    struct st_mail mail; // mail结构体
    memset(&mail, 0, sizeof(mail));

    // 获取IP、端口号、收件人ID、邮件标题、邮件内容
    QString qip ("192.168.64.130");
    QString qport ("8888");
    QString qmailto = ui->lineEdit->text();
    QString qtitle = ui->lineEdit_2->text();
    QString qcontent = ui->textEdit->toPlainText();

    // 连接到服务器
    strcpy(sip, qip.toStdString().data());
    client = new QTcpSocket(this);
    client->connectToHost(QHostAddress(sip), qport.toInt());

    // 发送邮件到服务器
    //strncpy(mail.operation, SEND, 4);
   //strncpy(mail.operation, "SEND", 4);
    strncpy(mail.userid, qmailto.toStdString().data(), qmailto.length());
    strncpy(mail.title, qtitle.toStdString().data(),qtitle.length());
    strncpy(mail.content, qcontent.toStdString().data(),qcontent.length());

    qDebug()<<mail.userid<<endl;
    qDebug()<<mail.title<<endl;
    qDebug()<<mail.content<<endl;

    client->write("SEND");
    client->write("\n");
    client->write(mail.userid);
    client->write("\n");
    client->write(mail.title);
    client->write("\n");
    client->write(mail.content);
    client->write("\n");
    client->write(name.toStdString().data());
    client->write("\n");


    // 获取服务器处理结果
    connect(client, &QTcpSocket::readyRead, [=]()
    {
        // 获取对方发送的内容
        QByteArray array = client->readAll();
        QMessageBox::information(this, "Server Message", array);
        // 关闭连接
        client->write(QUIT);
    });

    QMessageBox::information(this, "send", "发送成功");
 //g   QMessageBox::information(this, "send", "发送失败，请重试");
}

void send::on_pushButton_3_clicked()
{
    bool ok;
    QFont f = QFontDialog::getFont(&ok);
    if(ok){
        ui->lineEdit_4->setFont(f);
        ui->textEdit->setFont(f);
    }
}

void send::on_pushButton_4_clicked()
{
    QString s = QFileDialog::getOpenFileName(this, "open file dialog", "/", "(*.*)");//c++ files(*.cpp);;c files(*.c);;Head files(*.h)
    ui->lineEdit_3->setText(s);
}

void send::on_pushButton_5_clicked()
{
    QPalette palette = ui->textEdit->palette();
    const QColor & color =QColorDialog::getColor(palette.color(QPalette::Base),this);
    if(color.isValid()){
        ui->frame->setPalette(QPalette(color));
        palette.setColor(QPalette::Text,color);
        ui->textEdit->setPalette(palette);
    }
}
