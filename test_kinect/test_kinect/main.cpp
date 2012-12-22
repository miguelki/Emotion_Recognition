//
//
//				Entry point of the program
//				Initialize main elements and run the main loop
//
//
//
//
//
//

// Includes
#include <Windows.h>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>
#include "C:\Program Files\Microsoft SDKs\Kinect\v1.0 Beta2\inc\MSR_NuiApi.h"
#include <cv.h>
#include <cxcore.h>
#include <highgui.h>
#include "imgProcessing.h"
#include "svm\svm.h"
#include "KinectController.h"

using namespace std;
using namespace cv;


int main(int argc, char* argv[])
{
	
	// Initialize flags for Kinect connection
	DWORD flags = NUI_INITIALIZE_FLAG_USES_COLOR|NUI_INITIALIZE_FLAG_USES_DEPTH;

	// Init of MVC structure
	KinectAccess* kA = new KinectAccess(flags);
	FrameWindow* wdw = new FrameWindow();
	KinectController* kC = new KinectController(kA, wdw, "Face Expression Recognition");

	// Controller functions 
	kC->init();
	kC->display();
	kC->shutdown();

	system("pause");

	return 0;
}

