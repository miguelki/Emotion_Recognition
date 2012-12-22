//
//
//				Class FrameWindow, view part of the MVC pattern used
//				Display images, window, for user interactions
//
//
//
//
//
//
#include "StdAfx.h"
#include "FrameWindow.h"
#include "highgui.h"


FrameWindow::FrameWindow(void) : img(NULL) {
}


FrameWindow::~FrameWindow(void) {
}

// Display an image
void FrameWindow::displayImg(IplImage* i, char* title) {

	img = i;
	cvShowImage(title,img);
}

void FrameWindow::destroyWindow(char* title) {
	cvDestroyWindow(title);
}

void FrameWindow::createWindow(char* title){
	cvNamedWindow(title);
}

