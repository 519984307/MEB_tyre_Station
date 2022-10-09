#ifndef INPUTCODE_H
#define INPUTCODE_H

#include <QDialog>

namespace Ui {
class InputCode;
}

class InputCode : public QDialog
{
    Q_OBJECT

signals:
    void sendClose();
    void sendCode(QString);

public:
    explicit InputCode(QWidget *parent = 0);
    ~InputCode();

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_clicked();

private:
    Ui::InputCode *ui;
};

#endif // INPUTCODE_H
