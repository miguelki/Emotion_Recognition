#include "stdafx.h"
#include "KinectAccess.h"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "Hello World !" << endl;

	DWORD flags = NUI_INITIALIZE_FLAG_USES_COLOR|NUI_INITIALIZE_FLAG_USES_DEPTH;
	KinectAccess* kA = new KinectAccess(flags);
	kA->init();

	system("pause");

	return 0;
}

