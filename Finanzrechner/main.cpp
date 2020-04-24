#include <stdafx.h>

//#include "frontend/financecalc.h"
//#include "backend/financeBackend.h"

int main(int argc, char *argv[]) {
	//testBackend();
	
	QGuiApplication a(argc, argv);
	QQuickView view;
	view.setSource(QUrl("frontend/qml/main.qml"));
	view.show();
	return a.exec();
}

//TODO: Create GUI with input options and graphic output options