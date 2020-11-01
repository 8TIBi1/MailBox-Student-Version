#ifndef LOGIN_H
#define LOGIN_H
#include <unistd.h>
#include <QtNetwork>
#include <QMessageBox>
#include <QMainWindow>
#include "users.h"
#include "register.h"
#include "head.h"

#define SEND "SEND"
#define RECEIVE "RECV"
#define CONN "CONN"
#define QUIT "QUIT"


QT_BEGIN_NAMESPACE
namespace Ui { class login; }
QT_END_NAMESPACE

class login : public QMainWindow
{
    Q_OBJECT

public:
    login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_pushButton_clicked();//登录按钮
    void on_pushButton_2_clicked();

private:
    users people;
    Register reg;

private:
    Ui::login *ui;
};
#endif // LOGIN_H
