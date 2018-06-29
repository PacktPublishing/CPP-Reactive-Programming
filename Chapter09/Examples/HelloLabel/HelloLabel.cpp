
#include <QApplication>
#include <QLabel>
//#include <QPushButton>

int main(int argc, char **argv) {
    QApplication app(argc, argv);
    //QLabel label("Hello World, Qt!");
    QLabel label("<h2><i>Hello World,</i> <font color=green>Qt!</font><h2>");
    //QPushButton button("Hello, world!");
    
    //label.resize(200,60);
    label.show();
    //button.show();
    return app.exec();
}
