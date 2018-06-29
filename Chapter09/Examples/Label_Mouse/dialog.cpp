#include "dialog.h"
#include "my_qlabel.h"
#include <QVBoxLayout>
#include <QGroupBox>

Dialog::Dialog(QWidget *parent) : QDialog(parent)
{
    this->setWindowTitle("My Mouse-Event Handling App");
    initializeWidgets();

    connect(label_MouseArea, SIGNAL(Mouse_Position()), this, SLOT(Mouse_CurrentPosition()));
    connect(label_MouseArea, SIGNAL(Mouse_Pressed()), this, SLOT(Mouse_Pressed()));
    connect(label_MouseArea, SIGNAL(Mouse_Left()), this, SLOT(Mouse_Left()));
}

Dialog::~Dialog()
{
}

void Dialog::initializeWidgets()
{
    label_MouseArea = new my_QLabel(this);
    label_MouseArea->setText("Mouse Area");
    label_MouseArea->setMouseTracking(true);
    label_MouseArea->setAlignment(Qt::AlignCenter|Qt::AlignHCenter);
    label_MouseArea->setFrameStyle(2);

    label_Mouse_CurPos = new QLabel(this);
    label_Mouse_CurPos->setText("X = 0, Y = 0");
    label_Mouse_CurPos->setAlignment(Qt::AlignCenter|Qt::AlignHCenter);
    label_Mouse_CurPos->setFrameStyle(2);

    label_MouseEvents = new QLabel(this);
    label_MouseEvents->setText("Mouse current events!");
    label_MouseEvents->setAlignment(Qt::AlignCenter|Qt::AlignHCenter);
    label_MouseEvents->setFrameStyle(2);

    QGroupBox *groupBox = new QGroupBox(tr("Mouse Events"), this);
    QVBoxLayout *vbox = new QVBoxLayout;
    vbox->addWidget(label_Mouse_CurPos);
    vbox->addWidget(label_MouseEvents);
    vbox->addStretch(0);
    groupBox->setLayout(vbox);

    label_MouseArea->move(40, 40);
    label_MouseArea->resize(280,260);
    groupBox->move(330,40);
    groupBox->resize(200,150);
}

void Dialog::Mouse_CurrentPosition()
{
    label_Mouse_CurPos->setText(QString("X = %1, Y = %2")
                                    .arg(label_MouseArea->x)
                                    .arg(label_MouseArea->y));
    label_MouseEvents->setText("Mouse Moving!");
}

void Dialog::Mouse_Pressed()
{
    label_MouseEvents->setText("Mouse Pressed!");
}

void Dialog::Mouse_Left()
{
    label_MouseEvents->setText("Mouse Left!");
}
