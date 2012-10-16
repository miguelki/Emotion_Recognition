#include "StdAfx.h"
#include "KinectController.h"


KinectController::KinectController(DWORD f) {
	model = new KinectAccess(f);
	window = new FrameWindow();
	title = "Untitled Window";
}


KinectController::KinectController(KinectAccess* m, FrameWindow* w, char* t) : model(m), window(w) {
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
		model->fetchImg();
		BYTE* buffer = model->getBuffer();
		if (buffer != NULL) {
		IplImage * img = NULL;
		img = cvCreateImage(cvSize(640, 480), 8, 4);

		cvSetData(img,(BYTE*) buffer, img->widthStep);
		Mat m(img);
		IplImage iplimg(m);
		window->displayImg(&iplimg, title);

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
	//cvReleaseHaarClassifierCascade( &cascade );
	//cvReleaseMemStorage( &storage );

}