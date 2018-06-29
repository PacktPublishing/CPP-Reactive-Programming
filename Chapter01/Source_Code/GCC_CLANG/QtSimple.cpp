/////////////////////////////////////
//
// The following program can be compiled using
// QtCreator or Qt Console.
//

#include <qapplication.h>
#include <qdialog.h>
#include <qmessagebox.h>
#include <qobject.h>
#include <qpushbutton.h>

class MyApp : public QDialog {
	Q_OBJECT
public:
	MyApp(QObject* /*parent*/ = 0):
		button(this)
	{
		button.setText("Hello world!"); 
		button.resize(100, 30);
		// When the button is clicked, run button_clicked
		connect(&button,
			&QPushButton::clicked, this, &MyApp::button_clicked);
	}

        public slots:
		void button_clicked() {
			QMessageBox box;
			box.setWindowTitle("Howdy");
			box.setText("You clicked the button");
			box.show();
			box.exec();
		}
	protected:
		QPushButton button;
};


int main(int argc, char** argv) {
	QApplication app(argc, argv);
	MyApp myapp;
	myapp.show();
	return app.exec();
}

// EOF  QSimple.cpp