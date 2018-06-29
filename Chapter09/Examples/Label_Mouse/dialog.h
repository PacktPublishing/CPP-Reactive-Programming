#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

class my_QLabel;
class QLabel;

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private slots:
    void Mouse_CurrentPosition();
    void Mouse_Pressed();
    void Mouse_Left();

private:
    void initializeWidgets();
    my_QLabel *label_MouseArea ;
    QLabel *label_Mouse_CurPos;
    QLabel *label_MouseEvents;
};

#endif // DIALOG_H
