#ifndef USERS_H
#define USERS_H

#include "head.h"
#include "send.h"

QT_BEGIN_NAMESPACE
namespace Ui { class users; }
QT_END_NAMESPACE

class users : public QMainWindow
{
    Q_OBJECT

public:
    users(QWidget *parent = nullptr);
    ~users();
signals:
    void reShow();
private slots:
    void on_pushButton_clicked();//编写邮件
    void on_pushButton_3_clicked();

    void on_pushButton_2_clicked();

    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_4_clicked();
public slots:
    void timerUpdate(void);
signals:
    void reshow();//向登录界面发送返回信息
private:
    send s;
    QStringList L;
private:
    Ui::users *ui;
};
#endif // USERS_H
