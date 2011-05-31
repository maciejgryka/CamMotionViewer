#ifndef CAMMOTIONVIEWER_H
#define CAMMOTIONVIEWER_H

#include <QtGui/QMainWindow>
#include "NodeView.h"
#include "GLWidget.h"
#include "ui_CamMotionViewer.h"

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
	QAction* loadCamerasButAct;
	QAction* loadFramesAct;
	QAction* loadThumbsAct;

	QAction* exitAct;

	void createActions();
	void initWidgets();

	Ui::CamMotionViewerClass ui;

private slots:
	void quit();
	void loadCameras();
	void loadThumbs();
	void loadHiRes();
	void showViewer();
};

#endif // CAMMOTIONVIEWER_H
