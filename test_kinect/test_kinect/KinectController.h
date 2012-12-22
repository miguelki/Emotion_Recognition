#pragma once

#include "FrameWindow.h"
#include "KinectAccess.h"
/** Contrôleur : va combiner l'UI (FrameWindow) et la récupération d'image (KinectAccess) **/

class KinectController
{
private:

	// Elements to manage
	KinectAccess* model;
	FrameWindow* window;

	char* title;// Title of the window
	IplImage* img; // Current image
	CvHaarClassifierCascade * cascade;  // Contain Viola-Jones model
	CvMemStorage *storage; // Storage for face detection
	CvSeq *faces; // Structure to store faces
	ImgProcessing processImg;
	bool faceok; // bool which allow or not to run face expression recognition
	IplImage * iplimg;
	

public:
	// COnstructors and destructor
	KinectController(DWORD f);
	KinectController(KinectAccess* m, FrameWindow* w, char* t);
	~KinectController(void);

	void detectFaces( IplImage *img );

	void init();
	void display();
	void shutdown();	
	
};

