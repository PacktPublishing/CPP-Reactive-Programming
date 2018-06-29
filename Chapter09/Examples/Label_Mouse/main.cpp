#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    Dialog dialog;
    dialog.resize(545, 337);
    dialog.show();

    return app.exec();
}
