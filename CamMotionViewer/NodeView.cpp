#include "NodeView.h"
#include <math.h>
#include <QDir>
#include "Edge.h"
#include "VoodooParser.h"
#include "PolarParser.h"
#include "FileReader.h"

#include <QtDebug>

NodeView::NodeView(QWidget *parent)
	: QWidget(parent)
{
	coords[0] = 0.0f;
	coords[1] = 0.0f;
	currNode = 0;

	dirPath = new QString("C:\\Work\\research\\maya\\cam_motion\\");
	thumbsPath = new QString("frames\\");
	hiResPath = new QString("frames_big\\");
	QDir dir = QString(*dirPath + *thumbsPath);

	QStringList filter;
	filter << "*.jpg";
	dir.setNameFilters(filter);
	imageFiles = new QStringList(dir.entryList());

	//readInput(QString(*dirPath + "out100.txt"));

	calcCartesians();
}

NodeView::~NodeView() {
	for (int n = 0; n < nodeList.size(); n++) {
		delete nodeList[n];
	}
	delete dirPath, thumbsPath, hiResPath, imageFiles;
}

void NodeView::goToClosestNode() {
	goToNode(closestNode());
}

void NodeView::goToNode(int nodeIndex) {
	glWidget->currTex = nodeIndex;
}

int NodeView::closestNode() {
	float minDist = 3.40282e+038;	// maximum float value
	int minNode = 0;

	for (int n = 0; n < nodeList.size(); n++) {
		//// check if node n is connected to the current node
		//if (!nodeList.at(currNode)->isNeighbor(nodeList.at(n))) {
		//	continue;
		//}
		//if (abs(currNode - n) > 10) continue;

		//float tmpDist = euclideanDistance(cartesians, nodeList.at(n)->coords3d);
		float tmpDist = sphericalDistance(cartesians, nodeList.at(n)->coords3d);	// distance between current location and node n

		if (tmpDist < minDist) {
			minDist = tmpDist;
			minNode = n;
		}
	}

	this->currNode = minNode;
	qDebug() << minNode;
	return minNode;
}

void NodeView::setGLWidget(GLWidget* glw) {
	this->glWidget = glw;
};

void NodeView::calcCartesians() {
	float r = 0.105f;	//TODO: wtf?
	cartesians[2] = r * cos(coords[0]) * cos(coords[1]);
	cartesians[0] = -r * cos(coords[0]) * sin(coords[1]);
	cartesians[1] = r * sin(coords[0]);

	//normalize(cartesians);
}

float* NodeView::normalize(float* position3d) {
	float r = 0.105f;
	float r_norm = r / sqrt(position3d[0]*position3d[0] + position3d[1]*position3d[1] + position3d[2]*position3d[2]);

	position3d[0] *= r_norm;
	position3d[1] *= r_norm;
	position3d[2] *= r_norm;

	return position3d;
}

// Get the geodesic distance between two nodes (assumes r = 1, but that doesn't matter here)
float NodeView::sphericalDistance(const float *c0, const float *c1) {
	return acos(c0[0]*c1[0] + c0[1]*c1[1] + c0[2]*c1[2]);
	//return acos(cos(c0[0]) * cos(c1[0]) * cos(c0[1] - c1[1]) + sin(c0[0]) * sin(c1[0]));
}

// Get the euclidean distance between two nodes (assumes r = 1, but that doesn't matter here)
float NodeView::euclideanDistance(const float *c0, const float *c1) {
	float diff[3] = {c0[0] - c1[0], c0[1] - c1[1], c0[2] - c1[2]};
	return sqrt(diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2]);
}

void NodeView::elevate(float dTheta) {
	coords[0] += dTheta;
	calcCartesians();
	goToNode(closestNode());
}

void NodeView::rotate(float dFi) {
	coords[1] += dFi;
	calcCartesians();
	goToNode(closestNode());
}

void NodeView::setTheta(int theta) {
	coords[0] = PI*(float(theta)/16.0)/180;
	calcCartesians();
	goToNode(closestNode());
	qDebug() << "coords: " << coords[0]*180/PI << ", " << coords[1]*180/PI;
}

void NodeView::setFi(int fi) {
	coords[1] = PI*(float(fi)/16.0)/180;
	calcCartesians();
	goToNode(closestNode());
	qDebug() << "coords: " << coords[0]*180/PI << ", " << coords[1]*180/PI;
}

void NodeView::printNodeInfo(int nodeInd) {
	printNodeInfo(nodeList.at(nodeInd));
}

void NodeView::printNodeInfo(ImageNode* node) {
	qDebug() << QString("\nNode %1 with coords [%2, %3]").arg(node->index).arg(180*node->coords[0]/PI).arg(180*node->coords[1]/PI);
	qDebug() << QString("Current coords: [%1, %2]").arg(180*coords[0]/PI).arg(180*coords[1]/PI);
	qDebug() << QString("number of neighbors: %1").arg(node->edgeList.count());
	for (int n = 0; n < node->edgeList.count(); n++) {
		qDebug() << QString("neighbours: %1 - %2").arg(node->edgeList.at(n)->a->index).arg(node->edgeList.at(n)->b->index);
	}
}

void NodeView::readInput(QString filename) {
	FileReader fr(filename);
	if (fr.lines[0].contains("polar")) {
		parser = new PolarParser(filename);
	} else {
		parser = new VoodooParser(filename);
	}

	//VoodooParser vp(QString(*dirPath + "out.txt"));
	
	//// index of the first used camera
	//int firstFile = 0;

	//QStringList temp;
	//for (int f = 0; f < imageFiles->size(); f++) {
	//	if (f >= firstFile && f < firstFile + parser->cams.size()) {
	//		temp.append(imageFiles->at(f));
	//	}
	//}
	//*imageFiles = temp;

	// add the first node
	//parser->cams[0]->center(parser->center);
	nodeList << new ImageNode(nodeList.count(), parser->cams[0]->theta, parser->cams[0]->fi, parser->cams[0]->position);
	qDebug() << "node " << 0 << " at coords " << parser->cams[0]->theta * 180/PI << " " << parser->cams[0]->fi * 180/PI;

	// add all the other nodes and connections between them
	for (int c = 1; c < parser->cams.size(); c++) {
		//parser->cams.at(c)->center(parser->center);
		nodeList << new ImageNode(nodeList.count(), parser->cams[c]->theta, parser->cams[c]->fi, parser->cams[c]->position);
		Edge *e = new Edge(nodeList[c-1], nodeList[c]);
		qDebug() << "node " << c << " at coords " << parser->cams[c]->theta * 180/PI << " " << parser->cams[c]->fi * 180/PI;
	}

	coords[0] = nodeList.at(0)->coords[0];
	coords[1] = nodeList.at(0)->coords[1];

	delete parser;
}