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
	cascade = ( CvHaarClassifierCascade* )cvLoad("haarcascade_frontalface_alt.xml", 0, 0, 0 );
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


		detectFaces(img);
		//cout<< "il y a une image "<<endl;

		window->displayImg(img, title);

		key = cvWaitKey( 10 ); 
		}
		else
			exit(-2);
	}

	cvReleaseHaarClassifierCascade( &cascade );
	cvReleaseMemStorage( &storage );
}

void KinectController::shutdown() {
	model->shutdown();
	window->destroyWindow(title);
	delete[] title;
	delete img;
	//cvReleaseHaarClassifierCascade( &cascade );
	//cvReleaseMemStorage( &storage );

}

void KinectController::detectFaces( IplImage *img ){

	int i;
	
	storage = cvCreateMemStorage( 0 );
	faces = cvHaarDetectObjects(img, cascade, storage, 1.2, 2 , CV_HAAR_DO_CANNY_PRUNING,  cvSize( 100, 120 ),cvSize(400,300) );

	for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ ) {        
		CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );

		cvRectangle( img, cvPoint( r->x, r->y ), cvPoint( r->x + r->width, r->y + r->height ), CV_RGB( 255, 0, 0 ), 1, 8, 0 );        
	}

	//cvShowImage("Window-FT", img );   

	


}