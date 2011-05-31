/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtOpenGL>
#include <math.h>

#include "GLWidget.h"

#include <QtDebug>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

const float GLWidget::black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
const float GLWidget::gray[4]  = {0.5f, 0.5f, 0.5f, 1.0f};
const float GLWidget::red[4]   = {1.0f, 0.0f, 0.0f, 1.0f};
const float GLWidget::green[4] = {0.0f, 1.0f, 0.0f, 1.0f};
const float GLWidget::blue[4]  = {0.0f, 0.0f, 1.0f, 1.0f};
const float GLWidget::orange[4]  = {0.8f, 0.4f, 0.0f, 1.0f};

GLWidget::GLWidget(QWidget *parent)
	: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    xRot = 0;
    yRot = 0;
	common();
}

GLWidget::GLWidget(int xRot, int yRot, QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
    this->xRot = xRot;
    this->yRot = yRot;
	common();
}

void GLWidget::common() {
	makeCurrent();
	showHiRes = true;
}

void GLWidget::dumpDbgInfo() {
	dumpObjectInfo();
	qDebug() << QString("receivers: %1").arg(this->receivers(SIGNAL(xRotationChanged(int))));
}

static void qLimitAngle(int &angle, int minLimit, int maxLimit) {
	if (angle < minLimit)
		angle = minLimit;
	if (angle > maxLimit)
		angle = maxLimit;
}

static void qNormalizeAngle(int &angle) {
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
	qLimitAngle(angle, minXRot, maxXRot);
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
		coords2d[0] = xRot*PI/(16*180);
		calcCoords3d();
        emit xRotationChanged(xRot);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
		coords2d[1] = yRot*PI/(16*180);
		calcCoords3d();
        emit yRotationChanged(yRot);
        updateGL();
    }
}

void GLWidget::calcCoords3d() {
	float r = 0.11f;
	coords3d[2] = r * cos(coords2d[0]) * cos(coords2d[1]);
	coords3d[0] = -r * cos(coords2d[0]) * sin(coords2d[1]);
	coords3d[1] = r * sin(coords2d[0]);
}

void GLWidget::initializeGL()
{
	qglClearColor(QColor(0, 0, 0, 255));
	qobj = gluNewQuadric();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void GLWidget::setPaths(QString thumbsPath, QString hiResPath, QStringList* imageFiles) {
	//this->dirPath = dirPath;
	this->thumbsPath = thumbsPath;
	this->hiResPath = hiResPath;
	this->imageFiles = imageFiles;
}

void GLWidget::setThumbsPath(QString thumbsPath) {
	this->thumbsPath = thumbsPath;
}

void GLWidget::setHiResPath(QString hiResPath) {
	this->hiResPath = hiResPath;
}

void GLWidget::setImageFiles(QStringList* imageFiles) {
	this->imageFiles = imageFiles;
}

void GLWidget::loadTextures() {
	texNames = new GLuint[imageFiles->size()];
	glGenTextures(imageFiles->size(), texNames);

	QString imPath(thumbsPath + imageFiles->at(0));

	QImage b(imPath);
	windowSize = QSize(b.width(), b.height());
	hw = float(windowSize.height())/windowSize.width();

	for (int tf = 0; tf < imageFiles->size(); tf++) {
		imPath = thumbsPath + imageFiles->at(tf);
		qDebug() << "Loading texture " << tf << "from " << imPath;
		glBindTexture(GL_TEXTURE_2D, texNames[tf]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		QImage t = QGLWidget::convertToGLFormat(QImage(imPath));
		glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
	}

	//QImage *t = new QImage[imageFiles->size()];
	//for (int tf = 0; tf < imageFiles->size(); tf++) {
	//	t[tf] = QGLWidget::convertToGLFormat(QImage(*dirPath + *thumbsPath + imageFiles->at(tf)));
	//}
	//texNames = new GLuint[1];
	//glGenTextures(1, texNames);
	//glBindTexture(GL_TEXTURE_2D, texNames[0]);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//QImage t = QGLWidget::convertToGLFormat(QImage(*dirPath + *thumbsPath + imageFiles->at(tf)));
	//glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());

	loadHiRes();
}

void GLWidget::loadHiRes() {
	QImage t = QGLWidget::convertToGLFormat(QImage(hiResPath + imageFiles->at(currTex)));
	glBindTexture(GL_TEXTURE_2D, hiRes);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, t.width(), t.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, t.bits());
	//if (lit->isRunning()) {
	//	lit->terminate();
	//}
	//lit->run();
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glEnable(GL_TEXTURE_2D);
	if (showHiRes) {
		glBindTexture(GL_TEXTURE_2D, hiRes);
	} else {
		glBindTexture(GL_TEXTURE_2D, texNames[currTex]);
	}
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	float z = -20.0f;
	glBegin(GL_QUADS);
		glTexCoord2f(0.0, 0.0);
		glVertex3f(-1.0/2, -hw/2, z);
		glTexCoord2f(1.0, 0.0);
		glVertex3f( 1.0/2, -hw/2, z);
		glTexCoord2f(1.0, 1.0);
		glVertex3f( 1.0/2,  hw/2, z);
		glTexCoord2f(0.0, 1.0);
		glVertex3f(-1.0/2,  hw/2, z);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glLoadIdentity();
	float sphereCenter[3] = {hw/2, -hw/2, -10.0f};
    glTranslatef(sphereCenter[0], sphereCenter[1], sphereCenter[2]);
    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
	
	float sphereR = 0.1f;		// NOTE: this should be coupled with r from ImageNode
	paintAxes(2 * sphereR);
	
	// paint sphere
	glEnable(GL_LIGHTING);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, gray);
	gluSphere(qobj, sphereR, 10.0f, 10.0f);
	glDisable(GL_LIGHTING);

	// camera positions
	glPointSize(1.0f);
	glColor4fv(orange);
	glBegin(GL_POINTS);
		for (int c = 0; c < nodeList.length(); c++) {
			glVertex3f(nodeList[c]->coords3d[0], nodeList[c]->coords3d[1], nodeList[c]->coords3d[2]);
		}
	glEnd();
	
	glPointSize(5.0f);
	// current dislpayed node
	glColor4fv(green);
	glBegin(GL_POINTS);
		glVertex3f(nodeList[currTex]->coords3d[0], nodeList[currTex]->coords3d[1], nodeList[currTex]->coords3d[2]);
	glEnd();
	// current position
	glColor4fv(black);
	glBegin(GL_POINTS);
		glVertex3f(coords3d[0], coords3d[1], coords3d[2]);
	glEnd();
	// line between them
	glLineWidth(1.0f);
	glBegin(GL_LINES);
		glVertex3f(coords3d[0], coords3d[1], coords3d[2]);
		glVertex3f(nodeList[currTex]->coords3d[0], nodeList[currTex]->coords3d[1], nodeList[currTex]->coords3d[2]);
	glEnd();
}

void GLWidget::paintAxes(float size) {
	glDisable(GL_LIGHTING);
	glLineWidth(2.0f);

	// x axis
	glColor4fv(red);
	glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(size, 0.0f, 0.0f);
	glEnd();

	// y axis
	glColor4fv(green);
	glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, size, 0.0f);
	glEnd();

	// z axis
	glColor4fv(blue);
	glBegin(GL_LINES);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3f(0.0f, 0.0f, size);
	glEnd();

	glEnable(GL_LIGHTING);
}

void GLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho(-1/(2*hw), +1/(2*hw), -hw, +hw, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::keyPressEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_Escape) {
		emit quit();
	}
}

void GLWidget::mousePressEvent(QMouseEvent *event) {
	showHiRes = false;
    lastPos = event->pos();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e) {
	loadHiRes();
	showHiRes = true;
	updateGL();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    }
    lastPos = event->pos();
}

void GLWidget::addNodes(QList<ImageNode*> nodes) {
	nodeList = nodes;
};
