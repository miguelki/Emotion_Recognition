#include "stdafx.h"
#include "KinectController.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "Hello World !" << endl;

	DWORD flags = NUI_INITIALIZE_FLAG_USES_COLOR|NUI_INITIALIZE_FLAG_USES_DEPTH;


	KinectAccess* kA = new KinectAccess(flags);
	kA->init();
	FrameWindow* wdw = new FrameWindow();
	KinectController* kC = new KinectController(kA, wdw);

	system("pause");

	return 0;
}

