//
//  main.cpp
//  OpenCV
//
//  Created by julia vigo on 24/11/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "stdafx.h"

using namespace std;

CvHaarClassifierCascade *cascade; 
CvMemStorage *storage;
int key;

void detectFaces( IplImage *img );

//Point d'entrée du programme
int main (int argc, char * const argv[]) {

	CvCapture *capture;
	IplImage *img;
	int key = 'a';

	cascade = ( CvHaarClassifierCascade* )cvLoad("haarcascade_frontalface_alt.xml", 0, 0, 0 );
	capture = cvCreateCameraCapture(CV_CAP_ANY);

	if (!capture) {

		cout << "Ouverture du flux vidéo impossible !" << endl;
		return -1;
	}

	storage = cvCreateMemStorage( 0 );
	cvNamedWindow("Window-FT", 1 );

	while( key != 'q' ) { 
		img= cvQueryFrame( capture );
		detectFaces( img );
		key = cvWaitKey( 10 ); 
	}
	
	cvReleaseCapture( &capture ); 
	cvDestroyWindow("Window-FT");
	cvReleaseHaarClassifierCascade( &cascade );
	cvReleaseMemStorage( &storage );

	return 0;
}

void detectFaces( IplImage *img ) {

	int i;
	CvSeq *faces = cvHaarDetectObjects(img, cascade, storage, 1.1, 3 , 0,  cvSize( 40, 40 ) );

	for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ ) {        
		CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );

		cvRectangle( img, cvPoint( r->x, r->y ), cvPoint( r->x + r->width, r->y + r->height ), CV_RGB( 255, 0, 0 ), 1, 8, 0 );        
	}

	cvShowImage("Window-FT", img );    
}



