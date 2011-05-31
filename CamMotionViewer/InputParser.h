#ifndef INPUTPARSER
#define INPUTPARSER

#include "CAHVCamera.h"
#include "FileReader.h"
#include <QVector>
#include <QString>
//#include <QRegExp>

class InputParser {
public:
	//InputParser(QString filename) {
	//	parseInput(filename);
	//};
	//~InputParser(){};

	virtual void parseInput(QString filename) = 0;
	
	float center[3];
	QVector<CAHVCamera*> cams;
};
#endif INPUTPARSER

