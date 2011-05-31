#ifndef LOADIMGTHREAD
#define LOADIMGTHREAD

#include <QThread>

class GLWidget;

class LoadImgThread : public QThread {
public:
	LoadImgThread(GLWidget *glw) {
		this->glWidget = glw;
	};

	void run();// {
		//QImage t = QGLWidget::convertToGLFormat(QImage(*(glWidget->dirPath) + *(glWidget->hiResPath) + glWidget->imageFiles->at(0)));
		//glBindTexture(GL_TEXTURE_2D, glWidget->hiRes);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
	//};

	GLWidget *glWidget;
};

#endif