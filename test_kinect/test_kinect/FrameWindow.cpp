#include "StdAfx.h"
#include "FrameWindow.h"

FrameWindow::FrameWindow(void) : img(NULL) {
}


FrameWindow::~FrameWindow(void) {
}

void FrameWindow::displayImg(IplImage* i, char* title) {
	img = i;
	cvNamedWindow(title,1);
	cvShowImage(title,img);
	waitKey(1);
}

void FrameWindow::destroyWindow(char* title) {
	cvDestroyWindow(title);
}