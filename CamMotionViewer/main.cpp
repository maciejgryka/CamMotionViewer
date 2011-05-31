#include "CamMotionViewer.h"
#include <QtGui/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	CamMotionViewer w;
	return a.exec();
}
