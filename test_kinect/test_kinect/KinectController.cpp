#include "StdAfx.h"
#include "KinectController.h"


KinectController::KinectController(DWORD f) {
	model = new KinectAccess(f);
	window = new FrameWindow();
	title = "Untitled Window";
}


KinectController::KinectController(KinectAccess* m, FrameWindow* w, char* t) : model(m), window(w), img(NULL) {
	title = new char[100];
	strcpy(title, t);
}

KinectController::~KinectController(void){

}

void KinectController::init() {

	model->init();
}

void KinectController::display() {

	int key = 'a';
	while( key != 'q' ) { 
		if (img != NULL)
			delete img;

		model->fetchImg();
		BYTE* buffer = model->getBuffer();
		if (buffer != NULL) {
		IplImage * iplimg = NULL;
		iplimg = cvCreateImage(cvSize(640, 480), 8, 4);

		cvSetData(iplimg,(BYTE*) buffer, iplimg->widthStep);
		Mat m(iplimg);
		img = new IplImage(m);
		window->displayImg(img, title);

		key = cvWaitKey( 10 ); 
		}
		else
			exit(-2);
	}
}

void KinectController::shutdown() {
	model->shutdown();
	window->destroyWindow(title);
	delete[] title;
	delete img;
	//cvReleaseHaarClassifierCascade( &cascade );
	//cvReleaseMemStorage( &storage );

}