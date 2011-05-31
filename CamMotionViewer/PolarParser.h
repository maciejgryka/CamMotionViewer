#ifndef POLARPARSER
#define POLARPARSER

#define PI 3.14159f

#include "InputParser.h"

class PolarParser: public InputParser {
public:
	PolarParser(QString filename) {
		parseInput(filename);
	};
	~PolarParser(){};

	void parseInput(QString filename){
		FileReader fr(filename);
		QStringList valStr;

		center[0] = 0.0f;
		center[1] = 0.0f;
		center[2] = 0.0f;

		r = 0.105f;

		for (int l = 0; l < fr.lines.size(); l++) {
			QStringList valStr = fr.lines[l].split(" ");
			cams.append(new CAHVCamera(r, valStr[0].toFloat()*PI/180, 2*PI-valStr[1].toFloat()*PI/180));
			//if (fr.lines[l].compare('#r') {
			//	l++;
			//	r = fr.lines[l].toFloat();
			//}
			//if (fr.lines[l].compare('#c') {
			//	l++;
			//	valStr = fr.lines[l].split('\t');
			//	cams.append(new CAHVCamera(r, valStr[0].toFloat(), valStr[1].toFloat()));
			//}
		}
	};

	float r;
};
#endif POLARPARSER

