#ifndef NODEVIEW_H
#define NODEVIEW_H

#include "ImageNode.h"
#include "GLWidget.h"
#include "InputParser.h"

#define PI 3.14159f

class NodeView : public QWidget
{
	Q_OBJECT

public:
	NodeView(QWidget *parent);
	~NodeView();

	void goToClosestNode();
	void printNodeInfo(int nodeInd);
	void printNodeInfo(ImageNode* node);

	void setGLWidget(GLWidget* glw);
	int readInput(QString filename);

	QList<ImageNode*> nodeList;
	float coords[2];
	float cartesians[3];
	int currNode;

	QString *dirPath, *thumbsPath, *hiResPath;
	QStringList* imageFiles;

public slots:
	void setTheta(int theta);
	void setFi(int fi);

private:
	void goToNode(int node);
	int closestNode();
	float sphericalDistance(const float *c1, const float *c2);
	float euclideanDistance(const float *c1, const float *c2);
	void calcCartesians();
	float* normalize(float* positions3d);

	void elevate(float dTheta);
	void rotate(float dFi);

	QPoint lastPos;
	GLWidget* glWidget;
	InputParser* parser;
};

#endif // NODEVIEW_H
