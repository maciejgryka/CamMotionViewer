#ifndef EDGE
#define EDGE

class ImageNode;

class Edge {

public:
	Edge(ImageNode* nodeA, ImageNode* nodeB);
	~Edge(){};

	ImageNode* nodeA() { return a; };
	ImageNode* nodeB() { return b; };

	ImageNode *a, *b;

};

#endif // EDGE