#include "CamMotionViewer.h"
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QDir>
#include <QString>

CamMotionViewer::CamMotionViewer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	nodeView = new NodeView(this);
	
	createActions();

	QMenu* fileMenu = new QMenu("File", this);
	fileMenu->addAction(loadCamerasAct);
	fileMenu->addAction(loadFramesAct);
	fileMenu->addAction(loadThumbsAct);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);
	QMenuBar* mb = new QMenuBar(this);
	mb->addMenu(fileMenu);
	this->setMenuBar(mb);
	this->show();
}

CamMotionViewer::~CamMotionViewer() {
	delete nodeView;
	delete glWidget;
}

void CamMotionViewer::createActions() {
	loadCamerasAct = new QAction(tr("Load cameras..."), this);
	Q_ASSERT(connect(loadCamerasAct, SIGNAL(triggered()), this, SLOT(loadCameras())));

	loadFramesAct = new QAction(tr("Load frames..."), this);
	loadThumbsAct  = new QAction(tr("Load thumbs..."), this);

	exitAct = new QAction(tr("E&xit"), this);
	exitAct->setShortcut(Qt::Key_Escape);
	Q_ASSERT(connect(exitAct, SIGNAL(triggered()), this, SLOT(quit())));
}

void CamMotionViewer::quit() {
	nodeView->close();
	glWidget->close();
	close();
}

void CamMotionViewer::initWidgets() {
	Q_ASSERT(connect(glWidget, SIGNAL(xRotationChanged(int)), nodeView, SLOT(setTheta(int))));
	Q_ASSERT(connect(glWidget, SIGNAL(yRotationChanged(int)), nodeView, SLOT(setFi(int))));
	
	Q_ASSERT(connect(glWidget, SIGNAL(quit()), this, SLOT(quit())));
}

void CamMotionViewer::loadCameras() {
	nodeView->readInput(QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\Work\\research\\maya\\cam_motion", tr("Text files (*.txt);;All Files(*.*)")));
	
	glWidget = new GLWidget(16*180*nodeView->nodeList[0]->coords[0]/PI, 16*180*nodeView->nodeList[0]->coords[1]/PI);
	
	nodeView->setGLWidget(glWidget);
	nodeView->goToClosestNode();
	
	glWidget->addNodes(nodeView->nodeList);
	QString thumbsDir(QFileDialog::getExistingDirectory(this, tr("Open Thumbnail Directory"), "C:\\Work\\research\\maya\\cam_motion\\", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks) + "\\");
	glWidget->setThumbsPath(thumbsDir);

	QString hiResDir(QFileDialog::getExistingDirectory(this, tr("Open Hi Res Directory"), "C:\\Work\\research\\maya\\cam_motion\\", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks) + "\\");
	glWidget->setHiResPath(hiResDir);
	QDir dir(hiResDir);

	QStringList filter;
	filter << "*.jpg";
	dir.setNameFilters(filter);
	QStringList* imageFiles = new QStringList(dir.entryList());

	// if the number of cameras is different than the number of images, make than equal!
	int nCams = nodeView->nodeList.size();
	int nIms = imageFiles->size();
	if (nIms < nCams) {
		for (int c = nIms; c < nCams; c++) {
			nodeView->nodeList.removeAt(c);
		}
	} else if (nIms > nCams) {
		for (int i = nCams; i < nIms; i++) {
			imageFiles->removeAt(i);
		}
	}

	glWidget->setImageFiles(imageFiles);
	
	glWidget->loadTextures();
	glWidget->setGeometry(100, 100, glWidget->windowSize.width(), glWidget->windowSize.height());
	glWidget->show();

	initWidgets();
}