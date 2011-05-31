#ifndef IMAGENODE
#define IMAGENODE

#include <QImage>

class Edge;

class ImageNode: public	QImage {

public:
	ImageNode() {};
	ImageNode(int index, float theta, float fi, float* position3d);
	~ImageNode(){};

	float* normalize(float* position3d);
	void addEdge(Edge* e);
	QList<Edge*> edges();
	bool isNeighbor(const ImageNode *otherNode);

	
	float index;
	float coords[2];
	float coords3d[3];

	bool equals(const ImageNode* otherNode);

	QList<Edge*> edgeList;

private:
	static const float r;
};
#endif // IMAGENODE