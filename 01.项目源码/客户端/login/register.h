#ifndef REGISTER_H
#define REGISTER_H

#include "head.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Register; }
QT_END_NAMESPACE

class Register : public QMainWindow
{
    Q_OBJECT

public:
    Register(QWidget *parent = nullptr);
    ~Register();
    int  CheckRegisterInput();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();
signals:
    void reshow();//向登录界面发送返回信息
private:
    Ui::Register *ui;
};
#endif // REGISTER_H
