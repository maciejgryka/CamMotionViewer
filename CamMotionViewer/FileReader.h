#ifndef FILEREADER
#define FILEREADER

#include <QString>
#include <QstringList>
#include <QFile>
#include <QTextStream>

class FileReader {

public:
	FileReader(QString filename) { 
		parse(filename); 
	};

	~FileReader(){};

	void parse(QString filename) {
		QFile file(filename);
		if (file.open(QIODevice::ReadOnly | QIODevice::Text))
		{
			// Read the file
			QTextStream stream(&file);
			QString text = stream.readAll();

			lines = text.split("\r\n");
			if (lines.size() == 1) {
				lines = text.split("\n");
			}
		}
	};

	QStringList lines;
};

#endif