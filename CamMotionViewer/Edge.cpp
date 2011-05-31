#include "Edge.h"
#include "ImageNode.h"

Edge::Edge(ImageNode* nodeA, ImageNode* nodeB) {
	a = nodeA;
	b = nodeB;
	nodeA->addEdge(this);
	nodeB->addEdge(this);
}

