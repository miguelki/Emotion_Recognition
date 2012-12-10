#include "StdAfx.h"
#include "FrameWindow.h"
#include "highgui.h"

//void on_change(int state, void* userdata);
void on_change(int state, void* userdata)
{
	cout<<"coucou"<<endl;
	return;
}

FrameWindow::FrameWindow(void) : img(NULL) {
}


FrameWindow::~FrameWindow(void) {
}

void FrameWindow::displayImg(IplImage* i, char* title) {
	img = i;
	
	cvShowImage(title,img);
	//waitKey(1);
}

void FrameWindow::destroyWindow(char* title) {
	cvDestroyWindow(title);
}

void FrameWindow::createWindow(char* title){
	cvNamedWindow(title);
}

