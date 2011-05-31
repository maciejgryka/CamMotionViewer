#include "ImageNode.h"
#include "Edge.h"

const float ImageNode::r =  0.105f;		// NOTE: this should be coupled with sphere radius from GLWidget

ImageNode::ImageNode(int index, float theta, float fi, float* position3d) {
		coords[0] = theta;
		coords[1] = fi;

		normalize(position3d);
		coords3d[0] = position3d[0];
		coords3d[1] = position3d[1];
		coords3d[2] = position3d[2];

		float r_norm = r / sqrt(position3d[0]*position3d[0] + position3d[1]*position3d[1] + position3d[2]*position3d[2]);

		coords3d[0] = position3d[0] * r_norm;
		coords3d[1] = position3d[1] * r_norm;
		coords3d[2] = position3d[2] * r_norm;
}

float* ImageNode::normalize(float* position3d) {
	float r_norm = r / sqrt(position3d[0]*position3d[0] + position3d[1]*position3d[1] + position3d[2]*position3d[2]);

	coords3d[0] *= r_norm;
	coords3d[1] *= r_norm;
	coords3d[2] *= r_norm;

	return position3d;
}

void ImageNode::addEdge(Edge* edge) {
	edgeList << edge;
}

QList<Edge*> ImageNode::edges() {
	return edgeList;
}

bool ImageNode::isNeighbor(const ImageNode *otherNode) {
	for (int e = 0; e < edgeList.size(); e++) {
		if (edgeList.at(e)->nodeA() == otherNode || edgeList.at(e)->nodeB() == otherNode)
			return true;
	}
	return false;		
}

bool ImageNode::equals(const ImageNode *otherNode) {
	if (coords[0] != otherNode->coords[0] || coords[1] != otherNode->coords[1])
		return false;
	else
		return true;
}