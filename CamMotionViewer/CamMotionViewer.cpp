#include "CamMotionViewer.h"
#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QDir>
#include <QString>

CamMotionViewer::CamMotionViewer(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);

	nodeView = new NodeView(this);
	glWidget = NULL;
	
	createActions();

	//QMenu* fileMenu = new QMenu("File", this);
	//fileMenu->addAction(loadCamerasAct);
	//fileMenu->addAction(loadFramesAct);
	//fileMenu->addAction(loadThumbsAct);
	//fileMenu->addSeparator();
	//fileMenu->addAction(exitAct);
	//QMenuBar* mb = new QMenuBar(this);
	//mb->addMenu(fileMenu);
	//this->setMenuBar(mb);
	this->show();

	//loadCameras();
	//loadThumbs();
}

CamMotionViewer::~CamMotionViewer() {
	delete nodeView;
	if (glWidget != NULL)
		delete glWidget;
}

void CamMotionViewer::createActions() {
	//loadCamerasAct = new QAction(tr("Load cameras..."), this);
	//Q_ASSERT(connect(loadCamerasAct, SIGNAL(triggered()), this, SLOT(loadCameras())));

	//loadFramesAct = new QAction(tr("Load frames..."), this);
	//loadThumbsAct  = new QAction(tr("Load thumbs..."), this);

	Q_ASSERT(connect(ui.loadCamerasButton, SIGNAL(clicked()), this, SLOT(loadCameras())));
	Q_ASSERT(connect(ui.loadThumbsButton, SIGNAL(clicked()), this, SLOT(loadThumbs())));
	Q_ASSERT(connect(ui.loadHiResButton, SIGNAL(clicked()), this, SLOT(loadHiRes())));
	Q_ASSERT(connect(ui.showViewerButton, SIGNAL(clicked()), this, SLOT(showViewer())));

	//exitAct = new QAction(tr("E&xit"), this);
	//exitAct->setShortcut(Qt::Key_Escape);
	//Q_ASSERT(connect(exitAct, SIGNAL(triggered()), this, SLOT(quit())));
}

void CamMotionViewer::quit() {
	nodeView->close();
	if (glWidget != NULL){
		glWidget->close();
	}
	close();
}

void CamMotionViewer::initWidgets() {
	Q_ASSERT(connect(glWidget, SIGNAL(xRotationChanged(int)), nodeView, SLOT(setTheta(int))));
	Q_ASSERT(connect(glWidget, SIGNAL(yRotationChanged(int)), nodeView, SLOT(setFi(int))));
	
	Q_ASSERT(connect(glWidget, SIGNAL(quit()), this, SLOT(quit())));
}

void CamMotionViewer::loadCameras() {
	if (!nodeView->readInput(QFileDialog::getOpenFileName(this, 
														  tr("Open File"), 
														  "", 
														  tr("Text files (*.txt);;All Files(*.*)")))) 
	{
		return;
	}

	//nodeView->readInput("C:\\Work\\VS2010\\CamMotionViewer\\CamMotionViewer\\basil\\positions.txt");
	glWidget = new GLWidget(16*180*nodeView->nodeList[0]->coords[0]/PI, 16*180*nodeView->nodeList[0]->coords[1]/PI);
	
	nodeView->setGLWidget(glWidget);
	glWidget->addNodes(nodeView->nodeList);
	
	nodeView->goToClosestNode();
	
	ui.loadThumbsButton->setEnabled(true);
}

void CamMotionViewer::loadThumbs() {
	QString thumbsDir(QFileDialog::getExistingDirectory(this, tr("Open Thumbnail Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks) + "\\");
	
	//QString thumbsDir("C:\\Work\\VS2010\\CamMotionViewer\\CamMotionViewer\\basil\\basil_small\\");

	if (thumbsDir.isEmpty()) return;

	glWidget->setThumbsPath(thumbsDir);
	QDir dir(thumbsDir);
	QStringList filter;
	filter << "*.jpg";
	dir.setNameFilters(filter);
	QStringList* imageFiles = new QStringList(dir.entryList());

	// if the number of cameras is different than the number of images, make than equal
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
	showViewer();

	ui.showViewerButton->setEnabled(true);
	ui.loadHiResButton->setEnabled(true);

	initWidgets();
}

void CamMotionViewer::loadHiRes() {
	QString hiResDir(QFileDialog::getExistingDirectory(this, tr("Open Hi Res Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks) + "\\");
	
	//QString hiResDir("C:\\Work\\VS2010\\CamMotionViewer\\CamMotionViewer\\basil\\");

	if (hiResDir.isEmpty()) return;

	glWidget->setHiResPath(hiResDir);
	glWidget->updateGL();
}

void CamMotionViewer::showViewer() {
	if (glWidget != NULL)
		glWidget->show();
}
