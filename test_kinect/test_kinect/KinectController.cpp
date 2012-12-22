//
//
//				Class KinectController, controller of the MVC pattern used
//				Manage the two other class
//
//
//
//
//
//
#include "StdAfx.h"
#include "KinectController.h"


//Constructor
KinectController::KinectController(DWORD f) {
	model = new KinectAccess(f); //Init a model
	window = new FrameWindow(); // Init a view
	title = "Untitled Window";
	//iplimg = NULL;
	//iplimg = cvCreateImage(cvSize(640, 480), 8, 4);
}

// Overloaded constructor  --NOT USED--
KinectController::KinectController(KinectAccess* m, FrameWindow* w, char* t) : model(m), window(w), img(NULL) {
	title = new char[100];
	strcpy(title, t);
}

// Destructor
KinectController::~KinectController(void){

}

// Initialisation
void KinectController::init() {

	model->init(); // run init function from the model
	cascade = ( CvHaarClassifierCascade* )cvLoad("haarcascade_frontalface_alt.xml", 0, 0, 0 ); // load model for viola-jones classifier
	processImg.init("model_kdef_poly"); // init and load model for SVM classifier
	window->createWindow(title); // create a window through frameWindow class (view part of MVC)
	iplimg = cvCreateImage(cvSize(640, 480), 8, 4);
}


void KinectController::display() {

	int key = 'a'; //key init

	while( key != 'q' ) {
		// If there is already an image in img, delete it 
		if (img != NULL)
			delete img;

		faceok = false;// bool init

		model->fetchImg();

		//Create a buffer and get data from the image recovered from a kinect capture
		BYTE* buffer = model->getBuffer();

		//If the image isn't empty
		if (buffer != NULL) {
		//iplimg = NULL;
		//iplimg = cvCreateImage(cvSize(640, 480), 8, 4);
			
		//Put data from the buffer into the new image
		cvSetData(iplimg,(BYTE*) buffer, iplimg->widthStep);
		Mat m(iplimg);
		img = new IplImage(m);

		// Run face detection
		detectFaces(img);
		//processImg.findFace(img);

		if(cvWaitKey(5)== 'r') // If we press 'r' we run lbp and classification of face features
		{
			if(faceok == true) // if the face is in a good interval
			{
				std::cout<<"Process Recognition"<<std::endl;
				// We convert the current image into a grayscale version and stock it into a new image
				IplImage *img2 = cvCreateImage(cvGetSize(img), img->depth, 1);
				cvCvtColor( img, img2, CV_RGB2GRAY );
				// We run lbp and classification and show the result on the console
				std::cout<<processImg.processImg(img2)<<std::endl;
				// Show the image processed
				cvShowImage("image processed",img2);
				
			}
		}
		
		// Reset the region of interest which have been made into the detectfaces function
		cvResetImageROI(img);

		// Display the current image
		window->displayImg(img, title);


		key = cvWaitKey( 10 ); 
		}
		else
			exit(-2);
	}

	// Release models
	cvReleaseHaarClassifierCascade( &cascade );
	cvReleaseMemStorage( &storage );
}

// Run shutdown functions
void KinectController::shutdown() {
	model->shutdown();
	window->destroyWindow(title);
	delete[] title;
	delete img;
	
}


void KinectController::detectFaces( IplImage *img ){

	int i;
	
	storage = cvCreateMemStorage( 0 );

	// Detect faces and stock them into a CvSeq, 
	faces = cvHaarDetectObjects(img, cascade, storage, 1.2, 2 , CV_HAAR_DO_CANNY_PRUNING,  cvSize( 200, 200 ),cvSize(270,270) );

	for( i = 0 ; i < ( faces ? faces->total : 0 ) ; i++ ) {        
		CvRect *r = ( CvRect* )cvGetSeqElem( faces, i );// TO IMPROVE create a vector of Cvrect* because it can be severals faces
		
		//If face is in a good size interval
		if((r->width <= 240 && r->width >=225)&&(r->height <= 240 && r->height >=225)) 
		{
				faceok =true;// the face is enough good to be processed by lbp
				//draw a green rectangle around the face
				cvRectangle( img, cvPoint( r->x, r->y ), cvPoint( r->x + r->width, r->y + r->height ), CV_RGB( 50, 255, 0 ), 2, 8, 0 );
		}
		else cvRectangle( img, cvPoint( r->x, r->y ), cvPoint( r->x + r->width, r->y + r->height ), CV_RGB( 255, 0, 0 ), 2, 8, 0 ); // if not draw a red rectangle
		// Set the region of interest
		cvSetImageROI( img, *r);
		
	}
}

