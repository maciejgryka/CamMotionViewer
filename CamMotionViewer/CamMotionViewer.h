#ifndef CAMMOTIONVIEWER_H
#define CAMMOTIONVIEWER_H

#include <QtGui/QMainWindow>
#include "NodeView.h"
#include "GLWidget.h"

class CamMotionViewer : public QMainWindow
{
	Q_OBJECT

public:
	CamMotionViewer(QWidget *parent = 0, Qt::WFlags flags = 0);
	~CamMotionViewer();

private:
	NodeView *nodeView;
	GLWidget *glWidget;

	QAction* loadCamerasAct;
	QAction* loadFramesAct;
	QAction* loadThumbsAct;

	QAction* exitAct;

	void createActions();
	void initWidgets();

private slots:
	void quit();
	void loadCameras();
};

#endif // CAMMOTIONVIEWER_H
