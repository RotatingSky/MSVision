#include "stdafx.h"
#include "MSVisionQt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MSVisionQt w;
	w.show();
	return a.exec();
}
