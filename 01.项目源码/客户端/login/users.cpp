#include "users.h"
#include "ui_users.h"
#include "head.h"

#define SEND "SEND"
#define RECEIVE "RECV"
#define CONN "CONN"
#define QUIT "QUIT"

struct st_mail{
    char userid[30];
    char title[64];
    char content[952];
    char sendid[30];
};

struct st_opt{
    char operation[4];
    char userid[30];
};

users::users(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::users)
{
    ui->setupUi(this);
    QTimer *timer = new QTimer(this);//实时显示时间
    connect (timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(1000);
    connect(&s,&send::reshow,[this]()//用于退出登录
        {
            s.close();
            this->show();
        });
}

users::~users()
{
    delete ui;
}

void users::timerUpdate(void)//显示时间
{
    QDateTime time = QDateTime::currentDateTime();
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");
    ui->label_2->setText(str);
    ui->label->setText("用户名：" + name);
}

void users::on_pushButton_clicked()//发送按钮
{
    this->hide();
    s.show();
}

void users::on_pushButton_3_clicked()//退出登录按钮
{
    ui->label->clear();
    ui->listWidget->clear();
    L.clear();
    ui->textBrowser->clear();
    ui->label->clear();
    emit reshow();
}



void users::on_pushButton_2_clicked()//接收按钮
{
    QTcpSocket *client; // socket 句柄
    char sip[64];   // IP 地址字符串

    QString qip ("192.168.64.130");
    QString qport ("8888");

    // 连接到服务器
    strcpy(sip, qip.toStdString().data());
    client = new QTcpSocket(this);
    client->connectToHost(QHostAddress(sip), qport.toInt());

//    client->write("RECV");
//   // client->write("\n");
//    client->write(name.toStdString().data());
//    //client->write("\n");
    struct st_opt opt;
    memset(&opt,0,sizeof(struct st_opt));
    strcpy(opt.operation,"RECV");
    strncpy(opt.userid,name.toStdString().data(),30);

    client->write((char *)&opt,sizeof(st_opt));

    connect(client, &QTcpSocket::readyRead, [=]()
    {

        char buff[2024];
        memset(buff,0,2024);
        client->read(buff,2024);
        qDebug()<<buff<<endl;
        struct st_mail mail;
        memset(&mail,0,sizeof(st_mail));
        memcpy(&mail,buff,sizeof(st_mail));
        ui->listWidget->addItem(mail.title);
        ui->textBrowser->setText(mail.content);

        client->write(QUIT);
    });

  /*  QFile file("C:/Users/Lenovo/Desktop/qt/test.txt");
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"Can't open the file!"<<endl;
    }
    if (file.atEnd()){
        QMessageBox::information(this, "提示", "没有新邮件");
    }
    while (!file.atEnd()){
        QByteArray line = file.readLine();
        QString str(line);
        QStringList list= str.split(" ");
        QString sender = list.at(0);
        QString theme = list.at(1);
        QString send_mail = list.at(2);
        L << send_mail;

        QListWidgetItem *item = new QListWidgetItem;
        item->setText("发送者： " + sender + " 主题：" + theme);
        ui->listWidget->addItem(item);

        str.clear();
        sender.clear();
        theme.clear();
        send_mail.clear();

    }
    file.close();
    */
}

void users::on_listWidget_doubleClicked(const QModelIndex &index)//收件箱显示
{
    int line_count = ui->listWidget->currentRow();
    ui->textBrowser->setText(L.at(line_count));
/*    if(ui->listWidget->currentItem()==Q_NULLPTR)
        return;
    if(ui->listWidget->currentItem()!=Q_NULLPTR)
        {
            ui->listWidget->currentItem()->setBackgroundColor(Qt::transparent);
            ui->listWidget->currentItem()->setTextColor(Qt::black);
        }
        ui->listWidget->currentItem()->setBackgroundColor(Qt::red);
        ui->listWidget->currentItem()->setTextColor(Qt::blue);
*/
}



void users::on_pushButton_4_clicked()//删除按钮
{
    if (ui->listWidget->currentItem() != Q_NULLPTR){
        int line_count1 = ui->listWidget->currentRow();
        QListWidgetItem *item = ui->listWidget->takeItem(line_count1);
        //L.removeAt(line_count1);
        ui->textBrowser->clear();
        delete item;        
    }
}
