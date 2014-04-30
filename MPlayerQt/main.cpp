#include "mplayerqt.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	MPlayerQt w;
	w.show();
	return a.exec();
}
