#include "stdafx.h"
#include "finanzrechner.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Finanzrechner w;
	w.show();
	return a.exec();
}

//TODO: Add data functions (create month etc.)
//TODO: Add scripting support via qjs for functions like "calculate savings" etc
//TODO: Create GUI with input options and graphic output options
//TODO: Create scriptable UI to easily interact with script functions supplied to the program