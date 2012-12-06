//#include "stdafx.h"
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

using namespace std;
using namespace cv;
#include "KinectController.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "Hello World !" << endl;

	DWORD flags = NUI_INITIALIZE_FLAG_USES_COLOR|NUI_INITIALIZE_FLAG_USES_DEPTH;


	KinectAccess* kA = new KinectAccess(flags);
	FrameWindow* wdw = new FrameWindow();
	KinectController* kC = new KinectController(kA, wdw, "Face Expression Recognition");
	kC->init();
	kC->display();
	kC->shutdown();

	system("pause");

	return 0;
}

