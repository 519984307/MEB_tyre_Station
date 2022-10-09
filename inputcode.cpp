#include "inputcode.h"
#include "ui_inputcode.h"
#include <QDebug>

InputCode::InputCode(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InputCode)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);//去掉标题栏
    move((1366-this->width())/2,(768-this->height())/2);
}

InputCode::~InputCode()
{
    delete ui;
}

void InputCode::on_pushButton_cancel_clicked()
{
    emit sendClose();
}

//确定
void InputCode::on_pushButton_clicked()
{
    if(ui->lineEdit->text().isEmpty())
    {
        qDebug()<<"lineEditIsEmpty"<<ui->lineEdit->text();
    }
    else
    {
         emit sendCode(ui->lineEdit->text());
    }
    emit sendClose();
}
