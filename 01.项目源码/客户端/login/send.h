#ifndef SEND_H
#define SEND_H

#include "head.h"

QT_BEGIN_NAMESPACE
namespace Ui { class send; }
QT_END_NAMESPACE

class send : public QMainWindow
{
    Q_OBJECT

public:
    send(QWidget *parent = nullptr);
    ~send();
signals:
    void reshow();//向主界面发送返回信息
private slots:
    void on_pushButton_2_clicked();//返回按钮

    void on_pushButton_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::send *ui;

};
#endif // SEND_H
