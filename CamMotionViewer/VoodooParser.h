#ifndef VOODOOPARSER
#define VOODOOPARSER
#include "InputParser.h"

class VoodooParser: public InputParser {

public:
	VoodooParser(QString filename) {
		parseInput(filename);
	};
	~VoodooParser(){};

	void parseInput(QString filename) {
		FileReader fr(filename);
		QStringList valStr;

		for (int l = 0; l < fr.lines.size(); l++) {
			if (l == 1) {
				if (fr.lines[l].contains("voodoo",  Qt::CaseInsensitive)) {
					gen = voodoo;
				} else if (fr.lines[l].contains("VooCAT",  Qt::CaseInsensitive)) {
					gen = VooCAT;
				} else if (fr.lines[l].contains("Maya",  Qt::CaseInsensitive)) {
					gen = Maya;
				}
			}
			if (fr.lines.at(l).split(' ')[0] == "#timeindex") {
				l++;
				valStr = fr.lines.at(l).split('\t');
				// first camera position coord (Cx) in the CAHV camera string depends on what generated the output file
				int centerIndex;
				switch (gen) {
					case voodoo:
					case Maya:
						centerIndex = 0;
						break;
					case VooCAT:
						centerIndex = 9;
						break;
				}
				
				float vals[3];
				// invert Y and Z axes to mimic the Maya script
				vals[0] =  valStr.at(centerIndex).toFloat();
				vals[1] = -valStr.at(centerIndex + 1).toFloat();
				vals[2] = -valStr.at(centerIndex + 2).toFloat();

				cams.append(new CAHVCamera(vals));
			} else if (fr.lines.at(l).split(' ')[0] == "#center") {
				l++;
				valStr = fr.lines.at(l).split('\t');

				center[0] = valStr.at(0).toFloat();
				center[1] = -valStr.at(1).toFloat();
				center[2] = -valStr.at(2).toFloat();
			}
		}
	};
	enum GeneratorType {voodoo,
						VooCAT,
						Maya};
	GeneratorType gen;
};

#endif