#pragma once

#include "FrameWindow.h"
#include "KinectAccess.h"
/** Contrôleur : va combiner l'UI (FrameWindow) et la récupération d'image (KinectAccess) **/

class KinectController
{
private:
	KinectAccess* model;
	FrameWindow* window;
	char* title;
	IplImage* img;
	CvHaarClassifierCascade * cascade; 
	CvMemStorage *storage;
	CvSeq *faces;
	ImgProcessing processImg;
	

public:
	KinectController(DWORD f);
	KinectController(KinectAccess* m, FrameWindow* w, char* t);
	~KinectController(void);

	

	void detectFaces( IplImage *img );

	void init();
	void display();
	void shutdown();	
};

