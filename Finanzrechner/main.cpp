#include "stdafx.h"
#include "finanzrechner.h"

int main(int argc, char *argv[]) {
	QApplication a(argc, argv);
	Finanzrechner w;
	w.show();
	return a.exec();
}
